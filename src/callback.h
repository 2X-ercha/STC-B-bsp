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
#define enumEvent_Key3_Press 8
#define enumEvent_Key3_Release 9
#define enumEvent_NavKet_IsRight 10
#define enumEvent_NavKet_IsDown 11
#define enumEvent_NavKet_IsCenter 12
#define enumEvent_NavKet_IsLeft 13
#define enumEvent_NavKet_IsTop 14

typedef void (*EventCallback)(void);
extern EventCallback CallbackFor1ms;
extern EventCallback CallbackFor10ms;
extern EventCallback CallbackFor100ms;
extern EventCallback CallbackFor1s;
extern EventCallback CallbackForKey1Press;
extern EventCallback CallbackForKey1Release;
extern EventCallback CallbackForKey2Press;
extern EventCallback CallbackForKey2Release;
extern EventCallback CallbackForKey3Press;
extern EventCallback CallbackForKey3Release;
extern EventCallback CallbackForNavKeyIsRight;
extern EventCallback CallbackForNavKeyIsDown;
extern EventCallback CallbackForNavKeyIsCenter;
extern EventCallback CallbackForNavKeyIsLeft;
extern EventCallback CallbackForNavKeyIsTop;

void SetEventCallback(unsigned char event_interrupt_api, void (*fp)());

#endif