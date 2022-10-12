#include <STC15F2K60S2.H>
#include <math.h>
#include <intrins.h>    // 固有函数
#include <absacc.h>     // 访问特殊功能寄存器（可能用不到）
#include <stdio.h>      // 动态分配内存函数
#include <stdlib.h>     // 标准库

// 以上用不用的到不知道，反正先放着

// debug
unsigned char ledtest = 0x0f;
unsigned char leftbit = 0;

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

unsigned char seg_display_buf[8] = {0,0,0,0,0,0,0,0};    // 存储各个数码管显示位的具体值
unsigned char led_display_buf = 0;                       // 存储led显示值

unsigned char display_pos = 0;

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

#define MAIN_Fosc       11059200L
#define Main_Fosc_KHZ	(MAIN_Fosc / 1000)
#define TIMESLICE_MS	1
#define Fosc_1ms        (65536 - MAIN_Fosc*TIMESLICE_MS/12/1000)

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

    if(timer_count % 10 == 0){}
    if(timer_count % 100 == 0){
        // 流水灯测试
        leftbit = (ledtest >> 7) & 1;
        ledtest <<= 1;
        ledtest |= leftbit;
        Led_Print(ledtest);
    }
    if(timer_count == 0){}

    timer_count ++;
    if(timer_count >= 1000)
        timer_count = 0;
    // TH0 = (Fosc_1ms & 0xff00) >> 8;  TL0 = Fosc_1ms & 0xff;     // 重置定时器（16位自动重载了，不需要 文档P499
}

unsigned char hello[8] = {0x76, 0x79, 0x38, 0x38, 0x5c, 0, 0, 0};

int main(){
    Seg_Led_Init();
    Timer0_Init();
    AllSeg_Print(hello);
    Led_Print(0);

    while(1);
}