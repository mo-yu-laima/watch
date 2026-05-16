#ifndef _MYDATA_H_
#define _MYDATA_H_ 

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "u8g2.h"
#include <stdint.h>

typedef struct Images
{
	int x;
	int y;
	int w;
	int h;
}Image;

typedef struct Key_data
{
	uint8_t rdata;      //左按键
	uint8_t ldata;		//右按键
	uint8_t updata;		//上按键
    uint8_t downdata;   //下按键
	uint8_t okdata;		//确定键
	uint8_t exdata;      //返回键
}Key_data;

typedef struct UI
{
	const char name[20];
	uint8_t num;
	const uint8_t data[128];
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
}ui;

extern const uint8_t ShowPower[30];
extern const uint8_t ShowGame[30];

extern const uint8_t LeftMove[30];
extern const uint8_t RightMove[30];
extern ui cleder;
extern ui torch;
extern ui hum;
extern ui clock;
extern ui setting;

extern const uint8_t BigNum[10][120];
extern const uint8_t Num_6x8[12][8];
extern const uint8_t light[];

void ui_right(int32_t* a, int b);
void ui_left(int32_t* a, int b);
void ui_up(int32_t* a, int b);
void ui_down(int32_t* a, int b);
void u8g2_Init();
void u8g2_Clear();



#endif
