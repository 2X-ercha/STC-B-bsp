#ifndef _SM_H_
#define _SM_H_

extern void SM_Init();
extern void SM_Print(char data_row);

void StepMotor_Init();
void StepMotor_Next();
unsigned char StepMotor_Speed_Get();
void StepMotor_Speed_Set(unsigned char speed);
void StepMotor_SpeedUp_Status_Set(unsigned char status);
unsigned char StepMotor_SpeedUp_Status_Get();
void StepMotor_Brake_Status_Set(unsigned char status);
unsigned char StepMotor_Brake_Status_Get();

#endif