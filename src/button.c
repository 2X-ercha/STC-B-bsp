#include <STC15F2K60S2.H>
#include "button.h"
#include "callback.h"

// 按键key与adc导航备注说明
/* key1 - P32 - INT0
 * key2 - P33 - INT1
 * 均设置为输入
 */

void Button_Init(){
    EX1 = EX0 = EA = 1;
    PX0 = PX1 = 0;
    IE0 = IE1 = 0;          // 清除中断标志位
    IT0 = IT1 = 0;          // 0上下降沿均触发，1下降沿触发
    P3M1 |= 0x0c;
    P3M0 &= 0xf3;
}

void Int0_Routine() interrupt 0
{   
    /*
    if(P32 == enumKeyStatus_Press && CallbackForKey1Press != 0){CallbackForKey1Press();}
    else if(P32 == enumKeyStatus_Release && CallbackForKey1Release != 0){CallbackForKey1Release();}
    */
    if(P32 == enumKeyStatus_Press && CallbackForKey1Press != 0){
        k1_press_flag = 1;
        k1_press_timing = 0;
    }
    else if(P32 == enumKeyStatus_Release && CallbackForKey1Release != 0){
        k1_release_flag = 1;
        k1_release_timing = 0;
    }
}

void Int1_Routine() interrupt 2{
    if(P33 == enumKeyStatus_Press && CallbackForKey2Press != 0){
        k2_press_flag = 1;
        k2_press_timing = 0;
    }
    else if(P33 == enumKeyStatus_Release && CallbackForKey2Release != 0){
        k2_release_flag = 1;
        k2_release_timing = 0;
    }
}
