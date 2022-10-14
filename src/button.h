#ifndef _BUTTON_H_
#define _BUTTON_H_

#define enumKeyStatus_Press 0
#define enumKeyStatus_Release 1

extern unsigned char k1_press_flag;
extern unsigned char k2_press_flag;
extern unsigned char k1_press_timing;
extern unsigned char k2_press_timing;
extern unsigned char k1_release_flag;
extern unsigned char k2_release_flag;
extern unsigned char k1_release_timing;
extern unsigned char k2_release_timing;

void Button_Init();
void Int0_Routine();
void Int1_Routine();

#endif