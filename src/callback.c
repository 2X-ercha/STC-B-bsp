#include "callback.h"

EventCallback CallbackFor1ms = 0;
EventCallback CallbackFor10ms = 0;
EventCallback CallbackFor100ms = 0;
EventCallback CallbackFor1s = 0;
EventCallback CallbackForKey1Press = 0;
EventCallback CallbackForKey1Release = 0;
EventCallback CallbackForKey2Press = 0;
EventCallback CallbackForKey2Release = 0;

void SetEventCallback(unsigned char event_interrupt_api, void (*fp)()){
    switch(event_interrupt_api){
        case enumEvent_1ms:             CallbackFor1ms = fp;            break;
        case enumEvent_10ms:            CallbackFor10ms = fp;           break;
        case enumEvent_100ms:           CallbackFor100ms = fp;          break;
        case enumEvent_1s:              CallbackFor1s = fp;             break;
        case enumEvent_Key1_Press:      CallbackForKey1Press = fp;      break;
        case enumEvent_key1_Release:    CallbackForKey1Release = fp;    break;
        case enumEvent_Key2_Press:      CallbackForKey2Press = fp;      break;
        case enumEvent_key2_Release:    CallbackForKey2Release = fp;    break;
        default: break;
    }
}