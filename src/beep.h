#ifndef _BEEP_H_
#define _BEEP_H_

#include "bsp_global.h"

#define enumBeepMusicPlayer_Play 0
#define enumBeepMusicPlayer_Pause 1
#define enumBeepMusicPlayer_Stop 2

extern unsigned int music_pos;
extern unsigned int tone_time;
extern unsigned char beep_used;
extern unsigned char music_used;

void Timer1_Init();
void Beep_Init();
void BeepMusic_Reset();
void Beep_Print(unsigned int rate, unsigned int time);
void BeepMusicPlayer_StatusSet(unsigned char status);
void BeepMusicPlayer_MusicInit(unsigned char music_melody, unsigned char music_speed, unsigned char *music_array, unsigned int music_size);
void Timer1_Rountine();

#endif