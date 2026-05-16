#include "ShowTimer.h"
#include "cmsis_os2.h"
#include "u8g2.h"
#include "MyData.h"
#include "Buzzer.h"

#define BOX_R 1

extern osThreadId_t ShowmeuTaskHandle;
extern osThreadId_t ShowDHT11Handle;

extern osMutexId_t displayMutexHandle;

extern QueueHandle_t g_xQueueMenu;

typedef struct Time_param{
    int x[4];
	int y;
	int w;
	int h;
	int x_arg;
}T;
T time = { {8, 35, 71, 98}, 15, 20, 40, 98};

Image Box1 = {62, 22, 4, 4,};
Image Box2 = {62, 39, 4, 4,};

uint8_t sec_unit, sec_decade, min_unit, min_decade, hour_unit, hour_decade;

extern u8g2_t u8g2;
extern uint8_t flag;


void u8g2_TimeInit()
{
	u8g2_SetFont(&u8g2, u8g2_font_fur35_tf);
	u8g2_SendBuffer(&u8g2);
}



void u8g2_TimeUI()
{
	    u8g2_ClearBuffer(&u8g2);

        u8g2_DrawXBMP(&u8g2, 0, 0, 23, 10, ShowPower);
		u8g2_DrawXBMP(&u8g2, 105, 0, 23, 10, ShowGame);

        u8g2_DrawXBMP(&u8g2, time.x[3], time.y, time.w, time.h, BigNum[sec_unit]);
		u8g2_DrawXBMP(&u8g2, time.x[2], time.y, time.w, time.h, BigNum[sec_decade]);
		u8g2_DrawRBox(&u8g2, Box1.x, Box1.y, Box1.w, Box1.h, BOX_R);
		u8g2_DrawRBox(&u8g2, Box2.x, Box2.y, Box2.w, Box2.h, BOX_R);		
		u8g2_DrawXBMP(&u8g2, time.x[1], time.y, time.w, time.h, BigNum[min_unit]);
		u8g2_DrawXBMP(&u8g2, time.x[0], time.y, time.w, time.h, BigNum[min_decade]);

        u8g2_DrawXBMP(&u8g2, 56, 2, 6, 8, Num_6x8[hour_decade]);

        u8g2_DrawXBMP(&u8g2, 66, 2, 6, 8, Num_6x8[hour_unit]);

        u8g2_SendBuffer(&u8g2);
}



void ShowTimerTask(void *pvParameters)
{
	// 获取互斥锁后再进行显示操作
	u8g2_TimeInit();

    struct Key_data	key_data;

    while(1)
    {
		u8g2_TimeUI();
        osDelay(250);

        xQueueReceive(g_xQueueMenu, &key_data, 0);

		if(key_data.okdata == 1)
		{	
			key_data.okdata = 0;
			buzzer(1000);
			flag = 0;		
			u8g2_Clear();

			vTaskResume(ShowmeuTaskHandle);
			vTaskSuspend(NULL);
		}
    }
}



void TimerCallBackFun(TimerHandle_t xTimer)
{  
	/* handle time data */
	sec_unit++;		
	if(sec_unit>9){sec_unit = 0; sec_decade++;}
	if(sec_decade>5){sec_decade = 0; min_unit++;}
	if(min_unit>9){min_unit = 0; min_decade++;}
	if(min_decade>5){min_decade = 0; hour_unit++;}
	if(hour_unit>5){hour_unit = 0; hour_decade++;}
	
}
