#ifndef _UART_H_
#define _UART_H_

extern xdata unsigned char usbcom_buf[128];
extern xdata unsigned char usbcom_rxcnt;
extern xdata unsigned int usbcom_timeout;
extern xdata unsigned char usbcom_receive;

void USBcom_Init(unsigned long baudrate);

void USBcom_Write(unsigned char* buf);

void UART1_Rountine();

#endif