#include <STC15F2K60S2.H>
#include "button.h"
#include "callback.h"

#define cstAdcPower     0X80        /*ADC电源开关*/
#define cstAdcFlag      0X10        /*当A/D转换完成后，cstAdcFlag要软件清零*/
#define cstAdcStart     0X08        /*当A/D转换完成后，cstAdcStart会自动清零，所以要开始下一次转换，则需要置位*/
#define cstAdcSpeed90   0X60        /*ADC转换速度 90个时钟周期转换一次*/
#define cstAdcChs17     0X07        /*选择P1.7作为A/D输入*/

// 按键key与adc导航备注说明
/* key1 - P32 - INT0
 * key2 - P33 - INT1
 * key3 and navigation key - P17 -ADC7
 * 均设置为输入
 * 
 * 按键key3与导航键操作互斥，同时只能操作其中一个
 */

unsigned char last_adc;         // 用于消抖检验
unsigned char real_last_adc;    // 消抖前最后一次
unsigned char nav_key3_getadc_flag;

void Button_Init(){
    // key1,key2
    EX1 = EX0 = EA = 1;
    PX0 = PX1 = 0;
    IE0 = IE1 = 0;          // 清除中断标志位
    IT0 = IT1 = 0;          // 0上下降沿均触发，1下降沿触发
    P3M1 |= 0x0c;
    P3M0 &= 0xf3;
    // navigation key
    EADC = 1;
    PADC = 0;
    P1ASF = 0x80;           //P1.7作为模拟功能A/D使用
    ADC_CONTR = cstAdcPower | cstAdcStart | cstAdcChs17;
    ADC_RES = 0;            //转换结果清零
    CLK_DIV = 0X00;         //ADRJ = 0    ADC_RES存放高八位结果
    P1M1 |= 0x80;
    P1M0 &= 0x7f;
    
    nav_key3_getadc_flag = 0;
    last_adc = real_last_adc = 0x7;
}

void Int0_Routine() interrupt 0
{
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

void Nav_Key3_GetAdc(){
    ADC_CONTR = cstAdcPower | cstAdcStart | cstAdcSpeed90 | cstAdcChs17;
    nav_key3_getadc_flag = 1;
}

void ADC_Routine() interrupt 5{
    unsigned char nav_key3_adc;
    ADC_CONTR &= ~cstAdcFlag;
    nav_key3_adc = (ADC_RES >> 5) & 0x7;
    if(last_adc == nav_key3_adc && real_last_adc != nav_key3_adc){
        switch (nav_key3_adc){
            case 0:     
                // key3 press
                if(real_last_adc == 7 && CallbackForKey3Press != 0){CallbackForKey3Press();}
                break;
            case 1:
                // right
                if(real_last_adc != 0 && CallbackForNavKeyIsRight != 0){CallbackForNavKeyIsRight();}
                break;
            case 2:
                // down
                if(real_last_adc != 0 && CallbackForNavKeyIsDown != 0){CallbackForNavKeyIsDown();}
                break;
            case 3:
                // center
                if(real_last_adc != 0 && CallbackForNavKeyIsCenter != 0){CallbackForNavKeyIsCenter();}
                break;
            case 4:
                // left
                if(real_last_adc != 0 && CallbackForNavKeyIsLeft != 0){CallbackForNavKeyIsLeft();}
                break;
            case 5:
                // top
                if(real_last_adc != 0 && CallbackForNavKeyIsTop != 0){CallbackForNavKeyIsTop();}
                break;
            case 7:
                // key3 release
                if(real_last_adc == 0 && CallbackForKey3Release != 0){CallbackForKey3Release();}
                // nav release
                else if(real_last_adc > 0 && real_last_adc < 6 && CallbackForNavKeyIsRelease != 0){CallbackForNavKeyIsRelease();}
                break;
            default: break;
        }
        real_last_adc = last_adc;
    }
    last_adc = nav_key3_adc;
    nav_key3_getadc_flag = 0;
}