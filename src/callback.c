#include "callback.h"

EventCallback CallbackFor1ms = 0;
EventCallback CallbackFor10ms = 0;
EventCallback CallbackFor100ms = 0;
EventCallback CallbackFor1s = 0;

void SetEventCallback(unsigned char event_interrupt_api, void (*fp)()){
    switch(event_interrupt_api){
        case 0: CallbackFor1ms = fp; break;
        case 1: CallbackFor10ms = fp; break;
        case 2: CallbackFor100ms = fp; break;
        case 3: CallbackFor1s = fp; break;
        default: break;
    }
}