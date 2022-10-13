#ifndef _TIMER0_H_
#define _TIMER0_H_

#include "bsp_global.h"

#define TIMESLICE_MS	1
#define Fosc_1ms        (65536 - MAIN_Fosc*TIMESLICE_MS/12/1000)

extern unsigned int timer_count;

void Timer0_Init();
void Timer0_Rountine();

#endif