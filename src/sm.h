#ifndef _SM_H_
#define _SM_H_

extern void SM_Init();
extern void SM_Print(char data_row);

extern void StepMotor_Init();
extern void StepMotor_Next();
extern unsigned char StepMotor_Speed_Get();
extern void StepMotor_SpeedUp_Status_Set(unsigned char status);
extern unsigned char StepMotor_SpeedUp_Status_Get();
extern void StepMotor_Brake_Status_Set(unsigned char status);
extern unsigned char StepMotor_Brake_Status_Get();

#endif