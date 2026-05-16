#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "main.h"
#include "tim.h"


// 低音（L）、中音（M）、高音（H）频率表
#define L1 262    // 低音Do
#define L2 294    // 低音Re
#define L3 330    // 低音Mi
#define L4 349    // 低音Fa
#define L5 392    // 低音Sol
#define L6 440    // 低音La
#define L7 494    // 低音Si
 
#define M1 523    // 中音Do
#define M2 587    // 中音Re
#define M3 659    // 中音Mi
#define M4 698    // 中音Fa
#define M5 784    // 中音Sol
#define M6 880    // 中音La
#define M7 988    // 中音Si

#define H1 1047   // 高音Do
#define H2 1175   // 高音Re
#define H3 1319   // 高音Mi
#define H4 1397   // 高音Fa
#define H5 1568   // 高音Sol
#define H6 1760   // 高音La
#define H7 1976   // 高音Si

#define Z0 0

#define BELL_PWM_FREQ   500

typedef struct 
{
    TIM_HandleTypeDef * htim;
    uint32_t Channel;
    unsigned int freq;
    uint8_t state;
}Buzzer_states;


extern Buzzer_states Buzzer;

void Buzzer_init();
void Buzzer_set_freq();
uint8_t Buzzer_stop();
void buzzer(uint32_t delay);



#endif