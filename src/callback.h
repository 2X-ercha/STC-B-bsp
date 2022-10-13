#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#define enumEvent_1ms 0
#define enumEvent_10ms 1
#define enumEvent_100ms 2
#define enumEvent_1s 3
typedef void (*EventCallback)(void);
EventCallback CallbackFor1ms;
EventCallback CallbackFor10ms;
EventCallback CallbackFor100ms;
EventCallback CallbackFor1s;

void SetEventCallback(unsigned char event_interrupt_api, void (*fp)());

#endif