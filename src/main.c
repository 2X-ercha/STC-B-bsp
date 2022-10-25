#include <STC15F2K60S2.H>

#include "bsp_global.h"
#include "callback.h"
#include "timer0.h"
#include "seg_led.h"
#include "button.h"
#include "sm.h"

#include <INTRINS.H>
// debug test fun
unsigned char ledtest = 0xf;
unsigned char smtest = 0x33;
unsigned char leftbit = 0;
void waterled(){
    // led & sm 流水灯测试
    leftbit = (ledtest >> 7) & 1;
    ledtest <<= 1;
    ledtest |= leftbit;
    Led_Print(ledtest);
    leftbit = (smtest >> 7) & 1;
    smtest <<= 1;
    smtest |= leftbit;
    SM_Print(smtest);
}
unsigned char hello[8] = {0x76, 0x79, 0x38, 0x38, 0x5c, 0, 0, 0};

void seg7print(){OneSeg_Print(7, 0);}
void seg7print1(){OneSeg_Print(7, 0x06);}
void seg7print2(){OneSeg_Print(7, 0x5b);}
void seg7print1_(){OneSeg_Print(7, 0x06|0X80);}
void seg7print2_(){OneSeg_Print(7, 0x5b|0X80);}

void Delay2000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 85;
	j = 12;
	k = 155;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

unsigned char led_count = 0;
void ledcount(){
    led_count ++;
    Led_Print(led_count);
}

// stepmotor
code unsigned char decode_table[] =	{
	0x3f,	0x06,	0x5b,	0x4f,	0x66,	0x6d,	0x7d,	0x07,	0x7f,	0x6f,
    0x77,	0x7c,	0x39,	0x5e,	0x79,	0x71,	0x3d,	0x76,	0x0f,	0x0e,
	0x75,	0x38,	0x37,	0x54,	0x5c,	0x73,	0x67,	0x31,	0x49,	0x78,
	0x3e,	0x1c,	0x7e,	0x64,	0x6e,	0x59
};
unsigned char speed[] = {0x6d, 0x73, 0x79, 0x79, 0x5e, 0, 0, 0};
unsigned char stepmotorspeed;
unsigned char turn_status; // 0 直 1 左 2 右
unsigned char stepmotor_ledleft;
unsigned char stepmotor_ledright;
unsigned char stepmotor_ledbit;

void segprintspeed(){
    stepmotorspeed = StepMotor_Speed_Get();
    OneSeg_Print(7, decode_table[stepmotorspeed & 0xf]);
    OneSeg_Print(6, decode_table[(stepmotorspeed >> 4) & 0xf]);
}

void ledprintdir(){
    Led_Print(stepmotor_ledleft & 0xf0 | stepmotor_ledright & 0xf);
    stepmotor_ledbit = (stepmotor_ledleft >> 7) & 1;
    stepmotor_ledleft <<= 1;
    stepmotor_ledleft |= stepmotor_ledbit;
    stepmotor_ledbit = stepmotor_ledright & 1;
    stepmotor_ledright >>= 1;
    stepmotor_ledright |= stepmotor_ledbit << 7;
}

void callback100ms(){segprintspeed(); ledprintdir();}
void speedup(){StepMotor_SpeedUp_Status_Set(1);}
void speedup_stop(){StepMotor_SpeedUp_Status_Set(0);}
void brake(){
    StepMotor_Brake_Status_Set(1);
    switch(turn_status){
        case 0: stepmotor_ledleft = stepmotor_ledright = 0xff; break;
        case 1: stepmotor_ledright = 0xff; break;
        case 2: stepmotor_ledleft = 0xff; break;
    }
}
void brake_stop(){
    StepMotor_Brake_Status_Set(0);
    switch(turn_status){
        case 0: stepmotor_ledleft = stepmotor_ledright = 0; break;
        case 1: stepmotor_ledright = 0; break;
        case 2: stepmotor_ledleft = 0; break;
    }
}
void turn_left(){turn_status = 1; stepmotor_ledleft = 0x11;}
void turn_right(){turn_status = 2; stepmotor_ledright = 0x88;}
void turn_on(){
    turn_status = 0; 
    if(StepMotor_Brake_Status_Get() == 0)stepmotor_ledleft = stepmotor_ledright = 0;
    else stepmotor_ledleft = stepmotor_ledright = 0xff;
}

// end debug

int main(){
    Seg_Led_Init();
    Timer0_Init();
    Button_Init();
    // SM_Init();
    StepMotor_Init();

    AllSeg_Print(speed);
    Led_Print(0);

    SetEventCallback(enumEvent_100ms, callback100ms);
    SetEventCallback(enumEvent_Key1_Press, speedup);
    SetEventCallback(enumEvent_key1_Release, speedup_stop);
    SetEventCallback(enumEvent_Key2_Press, brake);
    SetEventCallback(enumEvent_key2_Release, brake_stop);
    SetEventCallback(enumEvent_NavKet_IsLeft, turn_left);
    SetEventCallback(enumEvent_NavKet_IsRight, turn_right);
    SetEventCallback(enumEvent_NavKet_IsRelease, turn_on);

    while(1);
}