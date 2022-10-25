#include <STC15F2K60S2.H>
#include "callback.h"
#include "timer0.h"
#include "seg_led.h"
#include "button.h"

unsigned int timer_count = 0;
unsigned char k1_press_flag = 0;
unsigned char k1_press_timing = 0;
unsigned char k1_release_flag = 0;
unsigned char k1_release_timing = 0;
unsigned char k2_press_flag = 0;
unsigned char k2_press_timing = 0;
unsigned char k2_release_flag = 0;
unsigned char k2_release_timing = 0;

void Timer0_Init(){
    // p498
    timer_count = 0;
    TR0 = 0;        // 停止timer0
    TMOD &= 0xF0;   // timer0 mode设置为(xxxx0000b, .3:0 打开定时/计数器 .2:0 选择定时模式 .1-0:00 16位自动重装定时器)
    AUXR &= 0x7F;   // 定时器0用传统8051速度，12分频
    TH0 = (Fosc_1ms & 0xff00) >> 8;  TL0 = Fosc_1ms & 0xff;     // 设置定时器
    ET0 = EA = 1;   // 启用timer0中断
	PT0 = 1;		// 设置高优先级（在C51中timer0本身优先级为第二，设为1基本上是最高了）
    TF0 = 0;		// 清除TF0标志
	TR0 = 1;        // 启用timer0
}

void Timer0_Rountine() interrupt 1{
    if(P23 == 1)
        display_pos_next();
    segled_display();
    
    // 时间回调
    if(CallbackFor1ms != 0)CallbackFor1ms();
    if(nav_key3_getadc_flag == 0 && timer_count % KEY_ANTISHAKE_MS == 0)Nav_Key3_GetAdc();

    if(timer_count % 10 == 0){
        if(CallbackFor10ms != 0){CallbackFor10ms();}

        if(timer_count % 100 == 0){
            if(CallbackFor100ms != 0){CallbackFor100ms();}

            if(timer_count == 0){
                if(CallbackFor1s != 0){CallbackFor1s();}
            }
        }
    }

    // 时间回调计时flag
    timer_count ++;
    if(timer_count >= 1000)
        timer_count = 0;

    // 按键防抖
    if(k1_press_flag == 1){
        // k1按下
        if(k1_press_timing < KEY_ANTISHAKE_MS) k1_press_timing++;
        else if(CallbackForKey1Press != 0 && P32 == enumKeyStatus_Press){
            k1_press_flag = 0;
            CallbackForKey1Press();
        }
    }
    if(k1_release_flag == 1){
        // k1松开
        if(k1_release_timing < KEY_ANTISHAKE_MS) k1_release_timing++;
        else if(CallbackForKey1Release != 0 && P32 == enumKeyStatus_Release){
            k1_release_flag = 0;
            CallbackForKey1Release();
        }
    }
    if(k2_press_flag == 1){
        // k2按下
        if(k2_press_timing < KEY_ANTISHAKE_MS) k2_press_timing++;
        else if(CallbackForKey2Press != 0 && P33 == enumKeyStatus_Press){
            k2_press_flag = 0;
            CallbackForKey2Press();
        }
    }
    if(k2_release_flag == 1){
        // k2松开
        if(k2_release_timing < KEY_ANTISHAKE_MS) k2_release_timing++;
        else if(CallbackForKey2Release != 0 && P33 == enumKeyStatus_Release){
            k2_release_flag = 0;
            CallbackForKey2Release();
        }
    }

    // TH0 = (Fosc_1ms & 0xff00) >> 8;  TL0 = Fosc_1ms & 0xff;     // 重置定时器（16位自动重载了，不需要 文档P499
}