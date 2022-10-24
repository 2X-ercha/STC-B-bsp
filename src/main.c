#include <STC15F2K60S2.H>

#include "bsp_global.h"
#include "callback.h"
#include "timer0.h"
#include "seg_led.h"
#include "button.h"
#include "sm.h"

#include <INTRINS.H>
// debug test fun
unsigned char ledtest = 0xf;
unsigned char smtest = 0x33;
unsigned char leftbit = 0;
void waterled(){
    // led & sm 流水灯测试
    leftbit = (ledtest >> 7) & 1;
    ledtest <<= 1;
    ledtest |= leftbit;
    Led_Print(ledtest);
    leftbit = (smtest >> 7) & 1;
    smtest <<= 1;
    smtest |= leftbit;
    SM_Print(smtest);
}
unsigned char hello[8] = {0x76, 0x79, 0x38, 0x38, 0x5c, 0, 0, 0};

void seg7print(){OneSeg_Print(7, 0);}
void seg7print1(){OneSeg_Print(7, 0x06);}
void seg7print2(){OneSeg_Print(7, 0x5b);}
void seg7print1_(){OneSeg_Print(7, 0x06|0X80);}
void seg7print2_(){OneSeg_Print(7, 0x5b|0X80);}

void Delay2000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 85;
	j = 12;
	k = 155;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

unsigned char led_count = 0;
void ledcount(){
    led_count ++;
    Led_Print(led_count);
}

// end debug

int main(){
    Seg_Led_Init();
    Timer0_Init();
    Button_Init();
    SM_Init();
    AllSeg_Print(hello);
    Led_Print(0);

    SetEventCallback(enumEvent_100ms, waterled);
    // SetEventCallback(enumEvent_Key1_Press, ledcount);
    // SetEventCallback(enumEvent_key1_Release, ledcount);
    // SetEventCallback(enumEvent_Key2_Press, Delay2000ms);
    // SetEventCallback(enumEvent_key2_Release, seg7print2_);

    while(1);
}