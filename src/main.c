#include <STC15F2K60S2.H>
#include <math.h>
#include <intrins.h>    // 固有函数
#include <absacc.h>     // 访问特殊功能寄存器（可能用不到）
#include <stdio.h>      // 动态分配内存函数
#include <stdlib.h>     // 标准库

// 以上用不用的到不知道，反正先放着

#include "bsp_global.h"
// #include "callback.h"
#include "timer0.h"
#include "seg_led.h"

// debug
unsigned char ledtest = 0x0f;
unsigned char leftbit = 0;
void waterled(){
    // 流水灯测试
    leftbit = (ledtest >> 7) & 1;
    ledtest <<= 1;
    ledtest |= leftbit;
    Led_Print(ledtest);
}
unsigned char hello[8] = {0x76, 0x79, 0x38, 0x38, 0x5c, 0, 0, 0};

int main(){
    Seg_Led_Init();
    Timer0_Init();
    AllSeg_Print(hello);
    Led_Print(0);

    // CallbackFor100ms = waterled;
    // SetEventCallback(enumEvent_100ms, waterled);

    while(1);
}