#include <STC15F2K60S2.H>
#include "beep.h"

/*
 * 定时器1并不直接启用timer1，而是由music_player和beep_set启用，结束时再将定时器关闭（不设置为自动重载即可）
 * 节拍的延时则使用定时器0完成
 * 
 * timer0对于音乐模块的节拍计时逻辑：
 *  计时开始(调用reset，启用timer1) -> 计时开始(由节拍速度决定：3750/speed) -> 计时结束(关闭timer1中断)
 * timer1对于每1/16拍的频率设置：
 *  timer1用于控制方波的产生频率，以此控制音标。每次reset时重载timer1定时
 */

unsigned int music_pos = 0;
unsigned int tone_time = 0;
unsigned char TH1_buff, TL1_buff;

unsigned char beep_used = 0;
unsigned char music_used = 0;

unsigned char tone_pause_flag = 0;
unsigned char __music_melody = 2;
unsigned char __music_speed = 60;
unsigned char *__music;
unsigned int __musicarray_len = 0;

// 不同音高对timer1的重载值
code unsigned char tone_to_RateCycle[21*2] = {
    /*
    // A调
    {},
    */
    // B调
    //  B           #C          #D          E           #F          #G          #A
    //  493.88      554.37      622.25      659.26      739.99      830.61      932.33
    // {
        0xA8, 0x85, 0xB2, 0x11, 0xBA, 0x94, 0xBE, 0x77, 0xC5, 0x9D, 0xC8, 0xFD, 0xD1, 0xAB,
        0xD4, 0x43, 0xD9, 0x09, 0xDD, 0x4A, 0xDF, 0x3C, 0xE2, 0xD2, 0xE5, 0xFE, 0xE8, 0xD3,
        0xEA, 0x24, 0xEC, 0x84, 0xEE, 0xA2, 0xEF, 0xA1, 0xF1, 0x66, 0xF2, 0xFF, 0xF4, 0x6C,
    // },
    /*
    // C调
    {},
    // D调
    {},
    // E调
    {},
    // F调
    {},
    // G调
    {},
    */
};

void Timer1_Init(){
    // p498
    TR1 = 0;        // 停止timer1
    TMOD &= 0x1F;   TMOD |= 0x10;   // timer1 mode设置为(0001xxxxb, .3:0 打开定时/计数器 .2:0 选择定时模式 .1-0:01 16位无重载)
    AUXR |= 0x40;   // 定时器1T1模式
    ET1 = EA = 1;   // 启用timer1中断
	PT1 = 0;		// 设置优先级
    TF1 = 0;		// 清除TF1标志
	// 暂时不启用timer1，reset蜂鸣器时设置
}

void Beep_Init(){
    // P34 - Beep
    Timer1_Init();
    P3M1 &= 0xEF;
    P3M0 |= 0x10;
    P34 = 0;
    beep_used = 0;
    music_used = 0;
}

void Beep_Print(unsigned int rate, unsigned int time){
    // rate: 频率; time: 持续时间ms
    if(!music_used && rate){
        unsigned int T_2 =  65536 - (MAIN_Fosc / 2 / rate);
        TH1_buff = T_2 >> 8;
        TL1_buff = T_2 & 0xf;
        tone_time = time;
        beep_used = 1;
        TR1 = 1;
    }
}

void BeepMusic_Reset(){
    // 一个音结束，下一个音开始，由timer0回调调用
    // 设置蜂鸣器方波频率
    unsigned char tone;
    unsigned char tone_to_RateCycle_index;
    if(music_pos < __musicarray_len && music_used && !beep_used && !tone_pause_flag){
        tone = __music[music_pos++];
        tone_to_RateCycle_index = (tone == 0) ? 0xff : (((tone >> 4) & 0xf) - 1) * 14 + ((tone & 0xf) - 1) * 2;
        TH1_buff = (tone_to_RateCycle_index == 0xff) ? 0 : tone_to_RateCycle[tone_to_RateCycle_index];
        TL1_buff = (tone_to_RateCycle_index == 0xff) ? 0 : tone_to_RateCycle[tone_to_RateCycle_index + 1];
        // 取出节拍时间
        tone_time = (__music[music_pos++] - 1) * (3750 / __music_speed);
        tone_pause_flag = 1;
        // 启用timer1
        if(tone_to_RateCycle_index != 0xff)
            TR1 = 1;
        else
            TR1 = 0;
            P34 = 0;
    }
    else if(tone_pause_flag){
        // 不同音间小停顿
        tone_pause_flag = 0;
        TR1 = 0;
        P34 = 0;
        tone_time = 3750 / __music_speed;
    }
    else{
        // 结束
        TR1 = 0;
        P34 = 0;
        music_used = 0;
        music_pos = 0;
    }
}

void BeepMusicPlayer_MusicInit(unsigned char music_melody, unsigned char music_speed, unsigned char *music_array, unsigned int music_size){
        /* 
         * 初始化音乐
         * 参数说明：
         *  music_melody: 音乐曲调，例如0xFA表示A大调；
         *  music_speed: 音乐速度，0-255
         *  music_array：音乐数组
         *  music_size: 数组大小，一般为sizeof(music_array)
         */
        __music_melody = music_melody - 0xFA;
        __music_speed = music_speed;
        __music = music_array;
        __musicarray_len = music_size/sizeof(unsigned char);
    }

void BeepMusicPlayer_StatusSet(unsigned char status){
    // 0: 播放；1: 暂停；2: 停止
    switch(status){
        case enumBeepMusicPlayer_Play:
            if(!music_used){
                music_used = 1;
                tone_time = 0;
            }
            break;
        case enumBeepMusicPlayer_Pause:
            music_used = 0;
            TR1 = 0;
            P34 = 0;
            break;
        case enumBeepMusicPlayer_Stop:
            music_used = 0;
            music_pos = 0;
            TR1 = 0;
            P34 = 0;
            break;
        default:
            break;
    }
}

void Timer1_Rountine() interrupt 3{
    TH1 = TH1_buff;
    TL1 = TL1_buff;
    P34 = ~P34;
}