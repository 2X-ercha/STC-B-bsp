#include <STC15F2K60S2.H>
#include "uart.h"
#include "bsp_global.h"
#include "callback.h"

/*
注意:对于STC15系列单片机，串口2只能使用定时器2作为波特率发生器，不能够选择其他定时器作为其波特率发生器;
而串口1默认选择定时器2作为其波特率发生器，也可以选择定时器1作为其波特率发生器;
串口3默认选择定时器2作为其波特率发生器，也可以选择定时器3作为其波特率发生器;
串口4默认选择定时器2作为其波特率发生器，也可以选择定时器4作为其波特率发生器。

定时器0 - 系统时钟
定时器1 - 蜂鸣器
定时器2 - 串口

USBcom
P30 rxd 高阻输入
P31 txd 推挽输出
*/

void USBcom_Init(unsigned long baudrate){
    unsigned int reload;

    // P30 10
    P3M1 |= 0x01;
    P3M0 &= 0xfe;
    // P31 01
    P3M1 &= 0xfd;
    P3M0 |= 0X02;

    // Configure baudrate timer
    reload = 65536 - (MAIN_Fosc / 4) / baudrate;
    AUXR |= 0x01;		//S1 BRT Use Timer2;
    AUXR &= ~(1<<4);	//Timer stop
    AUXR &= ~(1<<3);	//Timer2 set As Timer
    AUXR |=  (1<<2);	//Timer2 set as 1T mode
    IE2  &= ~(1<<2);	//Disable interrup

    T2H = (reload>>8) & 0xff;
    T2L = reload & 0xff;
    AUXR |=  (1<<4);	//Timer run enable

    // Initialize UART1 on P30 and P31
    PS = 0;	                                //low priority interrupt
    SCON = (SCON & 0x3f) | (1<<6);          //8bit mode
    REN = 1;                                //Enable Rx
    P_SW1 = (P_SW1 & 0x3f) | (0 & 0xc0);	//Select IO port P30/P31
    ES = 1;	                                //Enable interrupt (wait for incoming data)
}

xdata unsigned char usbcom_buf[128];
xdata unsigned char usbcom_rxcnt;
xdata unsigned int usbcom_timeout;
xdata unsigned char usbcom_receive = 0;

void USBcom_Write(unsigned char* buf){
    unsigned char i;

    ES = 0;
    //Disable interrupt (Tx uses polling mode)
    for(i=0; buf[i] != 0; i++){
		TI = 0; //clear TX end flag
        SBUF = buf[i];
        buf[i] = 0;
        while(TI == 0);
    }

    RI = 0; //Reset all flags
    TI = 0;
    ES = 1;
}

// USBcom interrupt
void UART1_Rountine() interrupt 4{
    //Tx uses polling mode, INT source can only be RX

    //Disable UART1 interrupt
    //(Rx uses interrupt to trigger, but still runs in polling mode)
    ES = 0;

    usbcom_rxcnt = 0;
    while(1) {
        RI = 0; //Remove RX end flag.
        usbcom_buf[usbcom_rxcnt++] = SBUF; //Retrieve byte.

        //Spin until RX end flag is set (by hardware)
        usbcom_timeout = 500;//You have to try out a proper value for specific baud and clock frequency settings
        while(RI==0) {
            if(--usbcom_timeout == 0)
                goto RX_END;
        }
    }
    RX_END:;

    TI = 0; //Reset all flags
    RI = 0;
    ES = 1; //Enable UART1 interrupt

    usbcom_receive = 1;
}