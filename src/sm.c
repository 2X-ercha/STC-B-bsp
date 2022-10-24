#include <STC15F2K60S2.H>
#include "sm.h"

// sm相关寄存器说明
/*
 * P44 - S4, P43 - S3, P42 - S2, P41 - S1
 * 设为推挽输出 01
 */

void SM_Init(){
    // 将p4_4-1设为推挽输出
    P4 &= 0xe1;         // 1110_0001
    P4M1 &= 0xe1;
    P4M0 |= 0x1e;
}

void SM_Print(char data_row){
    /*
     * description:
     *  设置SM四位值（只取低四位）
     * parameter:
     *  data_row: 设置值，只取低四位，从高往低为 S4-1
     */
    P44 = (data_row >> 3) & 0X1;
    P43 = (data_row >> 2) & 0X1;
    P42 = (data_row >> 1) & 0X1;
    P41 = (data_row >> 0) & 0X1;
}