#include <STC15F2K60S2.H>
// #include "callback.h"
#include <stdlib.h>
#include "timer0.h"
#include "seg_led.h"

unsigned int timer_count = 0;

void Timer0_Init(){
    // p498
    timer_count = 0;
    TR0 = 0;    // 停止timer0
    TMOD &= 0xF0;   // timer0 mode设置为(xxxx0000b, .3:0 打开定时/计数器 .2:0 选择定时模式 .1-0:00 16位自动重装定时器)
    AUXR &= 0x7F;   // 定时器0用传统8051速度，12分频（？）
    TH0 = (Fosc_1ms & 0xff00) >> 8;  TL0 = Fosc_1ms & 0xff;     // 设置定时器
    ET0 = EA = 1;   // 启用timer0中断
	PT0 = 1;		// 设置高优先级（在C51中timer0本身优先级为第二，设为1基本上是最高了）
	TR0 = 1;        // 启用timer0
}

void Timer0_Rountine() interrupt 1{
    if(P23 == 1)
        display_pos_next();
    segled_display();
    
    /*
    if(CallbackFor1ms != NULL)CallbackFor1ms();   
    if(timer_count % 10 == 0 && CallbackFor10ms != NULL){CallbackFor10ms();}
    if(timer_count % 100 == 0 && CallbackFor100ms != NULL){CallbackFor100ms();}
    if(timer_count == 0 && CallbackFor1s != NULL){CallbackFor1s();}
    */

    timer_count ++;
    if(timer_count >= 1000)
        timer_count = 0;
    // TH0 = (Fosc_1ms & 0xff00) >> 8;  TL0 = Fosc_1ms & 0xff;     // 重置定时器（16位自动重载了，不需要 文档P499
}