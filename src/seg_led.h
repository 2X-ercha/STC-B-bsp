#ifndef _SEG_LED_H_
#define _SEG_LED_H_

extern data unsigned char seg_display_buf[8];   // 存储各个数码管显示位的具体值
extern data unsigned char led_display_buf;      // 存储led显示值
extern data unsigned char display_pos;

void Seg_Led_Init();
void OneSeg_Print(unsigned char num, unsigned char displaychar);
void AllSeg_Print(unsigned char* displaychars);
void Led_Print(unsigned char displaychar);

void display_pos_next();
void segled_display();

#endif