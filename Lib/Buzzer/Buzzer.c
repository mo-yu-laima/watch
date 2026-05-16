#include "Buzzer.h"
#include "stm32f1xx_hal_tim.h"
#include "cmsis_os2.h"
#include <stdint.h>



Buzzer_states Buzzer = 
{
	.htim = &htim1,
	.Channel = TIM_CHANNEL_1,
};





void Reset_Timer()
{
	MX_TIM1_Init();
}



/**
 * @brief 初始化蜂鸣器
 * 
 * 初始化蜂鸣器并启动其PWM模式。
 *
 * @param Buzzer_states 蜂鸣器状态结构体指针
 * @return 无
 * 
 * @note 调用此函数前需确保相关GPIO时钟已使能
 * @warning 重复初始化可能导致不可预期的行为
 */
void Buzzer_init()
{
	HAL_TIM_PWM_Init(Buzzer.htim);// 初始化定时器的PWM模式
	HAL_TIM_PWM_Start(Buzzer.htim,Buzzer.Channel);// 启动定时器的PWM模式	
}



/**
 * @brief 设置蜂鸣器的频率
 * 
 * 根据传入的蜂鸣器状态结构体设置蜂鸣器的频率
 *
 * @param Buzzer_states 蜂鸣器状态结构体指针
 * @return 返回设置是否成功，成功返回1，失败返回0
 */
void Buzzer_set_freq()
{
	uint16_t ARR;
	uint16_t PSC = 72-1;
	uint32_t CLK = 72000000; 
	uint16_t Pulse;
	ARR = CLK/(Buzzer.freq*PSC)-1;
	Pulse = ARR/2;

    __HAL_TIM_SET_AUTORELOAD(Buzzer.htim, ARR);  // 修改TIM4的ARR值，改变PWM频率
    __HAL_TIM_SET_COMPARE(Buzzer.htim, Buzzer.Channel, Pulse);  //设置占空比

		//Reset_Timer();
	Buzzer_init();

}




/**
 * @brief 停止蜂鸣器
 * 
 * 停止蜂鸣器发声。
 *
 * @param Buzzer_states 指向蜂鸣器状态的指针
 * @return 返回1表示停止成功
 */
uint8_t Buzzer_stop()
{
    HAL_TIM_PWM_Stop(Buzzer.htim, Buzzer.Channel);
    return 1;
}





void buzzer(uint32_t delay)
{
    Buzzer.freq = M1;
    Buzzer_set_freq();
    osDelay(delay);
    Buzzer_stop();

}

// void Buzzer_Task(void *argument)
// {
	
// HAL_TIM_PWM_Start(Buzzer.htim,Buzzer.Channel);// 启动定时器的PWM模式	
//  const struct {
//         unsigned int freq;   // 音符频率（使用buzzer.h中的宏）
//         uint16_t duration;   // 音符持续时间
//     } music_notes[] = {
//         // 起风了主题
//         // 这一路上走走停停 顺着少年漂流的痕迹
//         {M1, 300}, {M1, 300}, {M5, 300}, {M5, 300},  // 1 1 5 5
//         {M6, 300}, {M6, 300}, {M5, 600},             // 6 6 5（长音）
//         {M4, 300}, {M4, 300}, {M3, 300}, {M3, 300},  // 4 4 3 3
//         {M2, 300}, {M2, 300}, {M1, 600},             // 2 2 1（长音）
//         {M5, 300}, {M5, 300}, {M4, 300}, {M4, 300},  // 5 5 4 4
//         {M3, 300}, {M3, 300}, {M2, 600},             // 3 3 2（长音）
//         {M5, 300}, {M5, 300}, {M4, 300}, {M4, 300},  // 5 5 4 4
//         {M3, 300}, {M3, 300}, {M2, 600},             // 3 3 2（长音）
//         {M1, 300}, {M1, 300}, {M5, 300}, {M5, 300},  // 1 1 5 5
//         {M6, 300}, {M6, 300}, {M5, 600},             // 6 6 5（长音）
//         {M4, 300}, {M4, 300}, {M3, 300}, {M3, 300},  // 4 4 3 3
//         {M2, 300}, {M2, 300}, {M1, 600}, {0, 1000}   // 2 2 1（长音）+ 停顿
//     };
//     uint16_t note_count = sizeof(music_notes) / sizeof(music_notes[0]);  // 音符总数

//     for(;;)  // 任务主循环（循环播放歌曲）
//     {
//         // 逐个播放音符
		
//         for(uint16_t i = 0; i < note_count; i++)
//         {
//             if(music_notes[i].freq == 0)
//             {
//                 // 频率为0时停止发声（停顿）
//                 Buzzer_stop();
//             }
//             else
//             {
//                 // 设置当前音符频率并启动PWM
//                 Buzzer.freq = music_notes[i].freq;
//                 Buzzer_set_freq();  // 内部会启动PWM
//             }
//             // 延时等待音符播放完成（使用FreeRTOS延时，释放CPU）
//             osDelay(music_notes[i].duration);
//         }
//         // 整首播放完后停顿1秒再重复
//         osDelay(1000);
//     }
// }