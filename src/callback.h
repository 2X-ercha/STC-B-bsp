#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#define enumEvent_1ms 0
#define enumEvent_10ms 1
#define enumEvent_100ms 2
#define enumEvent_1s 3
typedef void (*EventCallback)(void);
extern EventCallback CallbackFor1ms;
extern EventCallback CallbackFor10ms;
extern EventCallback CallbackFor100ms;
extern EventCallback CallbackFor1s;

void SetEventCallback(unsigned char event_interrupt_api, void (*fp)());

#endif