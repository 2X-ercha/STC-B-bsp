#include "callback.h"

EventCallback CallbackFor1ms = 0;
EventCallback CallbackFor10ms = 0;
EventCallback CallbackFor100ms = 0;
EventCallback CallbackFor1s = 0;
EventCallback CallbackForKey1Press = 0;
EventCallback CallbackForKey1Release = 0;
EventCallback CallbackForKey2Press = 0;
EventCallback CallbackForKey2Release = 0;
EventCallback CallbackForKey3Press = 0;
EventCallback CallbackForKey3Release = 0;
EventCallback CallbackForNavKeyIsRight = 0;
EventCallback CallbackForNavKeyIsDown = 0;
EventCallback CallbackForNavKeyIsCenter = 0;
EventCallback CallbackForNavKeyIsLeft = 0;
EventCallback CallbackForNavKeyIsTop = 0;
EventCallback CallbackForNavKeyIsRelease = 0;
EventCallback CallbackForUSBcomReceive = 0;

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
        case enumEvent_Key3_Press:      CallbackForKey3Press = fp;      break;
        case enumEvent_Key3_Release:    CallbackForKey3Release = fp;    break;
        case enumEvent_NavKet_IsRight:  CallbackForNavKeyIsRight = fp;  break;
        case enumEvent_NavKet_IsDown:   CallbackForNavKeyIsDown = fp;   break;
        case enumEvent_NavKet_IsCenter: CallbackForNavKeyIsCenter = fp; break;
        case enumEvent_NavKet_IsLeft:   CallbackForNavKeyIsLeft = fp;   break;
        case enumEvent_NavKet_IsTop:    CallbackForNavKeyIsTop = fp;    break;
        case enumEvent_NavKet_IsRelease:CallbackForNavKeyIsRelease = fp;break;
        case enumEvent_USBcom_Receive:  CallbackForUSBcomReceive = fp;  break;
        default: break;
    }
}