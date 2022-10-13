#include "seg_led.h"
#include <STC15F2K60S2.H>

// seg & led 寄存器备注说明
/*
 * P0: P07 - P00 高电平有效
 * 功能1：数码管单个段选
 *      管脚编码位：（小数点为8)
 *  	 ----		 0
 *  	|    |     5   1
 *  	 ----	     6
 *  	|    |     4   2
 *  	 ---- -		 5	 7
 *      管脚对应寄存器即为 P00 - P07
 * 功能2: led灯选择（刚好8个，从左往右 P07 - P00 ）
 * 
 * P2:
 *  P22 - P20
 *      选择第n个数码管点亮（高速循环来完成视觉暂留）
 *  P23 （注 E3 连接到数码管中）
 *      设为0则数码管显示，设为1则led显示
 */
/*
 * 推挽等寄存器初始化相关
 * P0: （芯片数据手册 p324）
 *  例如：（00：准双向口    01：推挽输出    10：仅为输入    11：开漏（可读可写）
 *          P07     P06     P05     P04     P03     P02     P01     P00
 *  P0M1:   1       0       1       0       0       0       0       0
 *  P0M0:   1       1       0       0       0       0       0       0
 *          开漏    推挽    输入     准双向    
 * 
 * P2: （芯片数据手册 p323）
 *  同上
 * 这里对于seg和led而言，显然只需要设为推挽输出即可
 */

data unsigned char seg_display_buf[8] = {0,0,0,0,0,0,0,0};   // 存储各个数码管显示位的具体值
data unsigned char led_display_buf = 0;      // 存储led显示值
data unsigned char display_pos = 0;

void Seg_Led_Init(){
    // P0、P2口全部设为推挽输出
    P0M1 = 0x00;
    P0M0 = 0xff;
    P2M1 &= 0xf0;
    P2M0 |= 0x0f;
}

void OneSeg_Print(unsigned char num, unsigned char displaychar){
    /*
     * description:
     *  单字符数码管显示修改
     * parameter:
     *  num: 修改的目标数码管编号，从左到右依次为 0-7
     *  displaychar: 修改后显示的字符 
     */
    seg_display_buf[num] = displaychar;
}

void AllSeg_Print(unsigned char* displaychars){
    /*
     * description:
     *  8位数码管全显示修改
     * parameter:
     *  displaychars: 显示字符的编码数组，长度为8
     */
    seg_display_buf[7] = displaychars[7];
    seg_display_buf[6] = displaychars[6];
    seg_display_buf[5] = displaychars[5];
    seg_display_buf[4] = displaychars[4];
    seg_display_buf[3] = displaychars[3];
    seg_display_buf[2] = displaychars[2];
    seg_display_buf[1] = displaychars[1];
    seg_display_buf[0] = displaychars[0];
}

void Led_Print(unsigned char displaychar){
    led_display_buf = displaychar;
}

void display_pos_next(){
    /* 该函数应该被写入调度，不对外提供api */
    if(display_pos < 7)
        display_pos ++;
    else
        display_pos = 0;
}

void segled_display(){
    P23 = ~P23;
    if(P23 == 0){
        P0 = seg_display_buf[display_pos];
        P20 = display_pos & 0x1;
        P21 = (display_pos & 0x2) >> 1;
        P22 = (display_pos & 0x4) >> 2;
    }
    else{
        P0 = led_display_buf;
    }
}