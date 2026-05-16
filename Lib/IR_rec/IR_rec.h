#ifndef _IR_REC_H_
#define _IR_REC_H_

#include "main.h"
#include "tim.h"

#define Left_Button 0x44
#define Right_Button 0x40
#define Up_Button 0x15
#define Down_Button 0x07
#define Ok_Button 0x43
#define Exit_Button 0x16

typedef struct 
{
    TIM_HandleTypeDef * htim;
    uint32_t            Channel;
    uint16_t            addressed;    //设备地址
    uint16_t            command;      //命令码
    uint16_t            width;        //脉冲宽度
    uint8_t             flag;         //标志位
    uint16_t            index;        //数据索引
    uint8_t             recflag;      //接收标志位
    uint8_t             RepeatFlag;   //重复标志位
    uint8_t             Data;         //是否有数据
    uint8_t             Num[4];       //数据缓存区
}IR_states;

void IR_Init(void);
uint8_t IR_GetData(void);
uint16_t IR_GetAddress(void);
uint16_t IR_GetCommand(void);
uint8_t IR_GetRepeatFlag(void);


#endif
