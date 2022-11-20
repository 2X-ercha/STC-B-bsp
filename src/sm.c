#include <STC15F2K60S2.H>
#include "sm.h"

// sm相关寄存器说明
/*
 * P44 - S4, P43 - S3, P42 - S2, P41 - S1
 * 设为推挽输出 01
 */

void SM_Init(){
    // 将p4_4-1设为推挽输出
    P4 &= 0xe1;         // 1110_0001
    P4M1 &= 0xe1;
    P4M0 |= 0x1e;
}

void SM_Print(char data_row){
    /*
     * description:
     *  设置SM四位值（只取低四位）
     * parameter:
     *  data_row: 设置值，只取低四位，从高往低为 S4-1
     */
    P44 = (data_row >> 3) & 0X1;
    P43 = (data_row >> 2) & 0X1;
    P42 = (data_row >> 1) & 0X1;
    P41 = (data_row >> 0) & 0X1;
}

unsigned char stepmotor_speed;
unsigned char stepmotor_speedup_status;
unsigned char stepmotor_brake_status;
unsigned char stepmotor_smbit;
unsigned char stepmotor_smbuf;
unsigned char stepmotor_timer;

void StepMotor_Init(){
    SM_Init();
    stepmotor_speed = 0;
    stepmotor_speedup_status = 0;
    stepmotor_brake_status = 0;
    stepmotor_smbuf = 0x88;
    stepmotor_timer = 0;
}

// stepmotor运行
void StepMotor_Next(){
    SM_Print(stepmotor_smbuf);
    stepmotor_timer ++;
    if(stepmotor_speed > 0){
        if(stepmotor_timer % ((unsigned char)255 * 5 / stepmotor_speed) == 0){
            stepmotor_smbit = stepmotor_smbuf & 1;
            stepmotor_smbuf >>= 1;
            stepmotor_smbuf |= stepmotor_smbit << 7;
        }
    }
    if(stepmotor_speedup_status && stepmotor_speed < 255)stepmotor_speed ++;
    if(stepmotor_brake_status && stepmotor_speed > 2)stepmotor_speed -= 2;
    else if(stepmotor_brake_status)stepmotor_speed = 0;
}

unsigned char StepMotor_Speed_Get(){return stepmotor_speed;}
void StepMotor_Speed_Set(unsigned char speed){stepmotor_speed = speed;}

// stepmotor加速状态设置
void StepMotor_SpeedUp_Status_Set(unsigned char status){if(stepmotor_brake_status == 0 && status)stepmotor_speedup_status = 1; else stepmotor_speedup_status = 0;}
// 加速状态获取
unsigned char StepMotor_SpeedUp_Status_Get(){return stepmotor_speedup_status;}

// stepmotor刹车状态设置
void StepMotor_Brake_Status_Set(unsigned char status){if(stepmotor_speedup_status == 1 && status)StepMotor_SpeedUp_Status_Set(0); stepmotor_brake_status = status;}
// 刹车状态获取
unsigned char StepMotor_Brake_Status_Get(){return stepmotor_brake_status;}