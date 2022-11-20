#ifndef __BSP_GLOBAL_H_
#define __BSP_GLOBAL_H_

#define MAIN_Fosc           11059200L
#define KEY_ANTISHAKE_MS    20              // 按键按下防抖检测时间

#define TIMESLICE_MS	1
#define Fosc_1ms        (65536 - MAIN_Fosc*TIMESLICE_MS/12/1000)

#endif