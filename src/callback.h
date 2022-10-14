#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#define enumEvent_1ms 0
#define enumEvent_10ms 1
#define enumEvent_100ms 2
#define enumEvent_1s 3
#define enumEvent_Key1_Press 4
#define enumEvent_key1_Release 5
#define enumEvent_Key2_Press 6
#define enumEvent_key2_Release 7

typedef void (*EventCallback)(void);
extern EventCallback CallbackFor1ms;
extern EventCallback CallbackFor10ms;
extern EventCallback CallbackFor100ms;
extern EventCallback CallbackFor1s;
extern EventCallback CallbackForKey1Press;
extern EventCallback CallbackForKey1Release;
extern EventCallback CallbackForKey2Press;
extern EventCallback CallbackForKey2Release;

void SetEventCallback(unsigned char event_interrupt_api, void (*fp)());

#endif