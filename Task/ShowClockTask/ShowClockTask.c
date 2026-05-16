#include "ShowClockTask.h"

extern osThreadId_t ShowClockHandle;
extern osThreadId_t ShowmeuTaskHandle;

extern TimerHandle_t g_Clock_Timer;

extern QueueHandle_t g_xQueueMenu;	

uint16_t millisecond;
uint8_t len1, len2;
uint8_t clock_flag = 0;
int8_t seclect_flag = 0;
uint16_t g_num_x[] = {1, 9, 25, 33, 41, 17}, g_num_y[] = {22, 52}, num_w = 6, num_h = 8;
uint16_t g_seclect_x[] = {1, 9, 25, 33, 41};
int8_t g_clock_num[] = {0, 0, 0, 0};
int8_t g_real_time[] = {0, 0, 0, 0};

extern uint8_t flag;
extern u8g2_t u8g2;

uint8_t k =0,r = 0,l = 0;

void ShowClock()
{
	len1 = u8g2_DrawStr(&u8g2, 0, 30, "Set:");
	len2 = u8g2_DrawStr(&u8g2, 0, 60, "Ret:");

	for(int i = 0; i < 4; i++)
	{
		u8g2_DrawXBMP(&u8g2, len1+g_num_x[i], g_num_y[0], num_w, num_h, Num_6x8[g_clock_num[i]]);	
		u8g2_DrawXBMP(&u8g2, len2+g_num_x[i], g_num_y[1], num_w, num_h, Num_6x8[g_real_time[i]]);
	}
	u8g2_DrawXBMP(&u8g2, len1+g_num_x[5], g_num_y[0], num_w, num_h, Num_6x8[10]);/* : */
	u8g2_DrawXBMP(&u8g2, len1+g_num_x[4], g_num_y[0], num_w, num_h, Num_6x8[11]);/* > */
	u8g2_DrawXBMP(&u8g2, len2+g_num_x[5], g_num_y[1], num_w, num_h, Num_6x8[10]);/* : */


	u8g2_DrawCircle(&u8g2, 104, 31, 22, U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2, 104, 31, 23, U8G2_DRAW_ALL);
	u8g2_DrawDisc(&u8g2, 104, 31, 1, U8G2_DRAW_ALL);
	
	u8g2_DrawXBMP(&u8g2, 94, 12, 20, 40, BigNum[millisecond]);			

	u8g2_DrawFrame(&u8g2, len1+g_seclect_x[seclect_flag]-1, g_num_y[0]-2, num_w+2, num_h+3);	

}


void ShowClockTask(void *pvParameters)
{
    struct Key_data	key_data;

    while(1)
    {
        u8g2_ClearBuffer(&u8g2);
		ShowClock();		
		u8g2_SendBuffer(&u8g2);


        xQueueReceive(g_xQueueMenu, &key_data, portMAX_DELAY) ;

		if(key_data.rdata == 1)
		{
			r++;
            key_data.rdata = 0;
			seclect_flag++;
			if(seclect_flag > 4) seclect_flag = 0;
		}

		if(key_data.ldata == 1)
		{
			l++;
            key_data.ldata = 0;
			seclect_flag--;
			if(seclect_flag < 0) seclect_flag = 4;
		}

		if(key_data.updata == 1)
		{
			key_data.updata = 0;
			g_clock_num[seclect_flag]++;
			if(g_clock_num[seclect_flag] > 9) g_clock_num[seclect_flag] = 0;
		}

		if(key_data.downdata == 1)
		{
			k++;
			g_clock_num[seclect_flag]--;
			if(g_clock_num[seclect_flag] < 0) g_clock_num[seclect_flag]= 9;
			key_data.downdata = 0;
		}

		if(key_data.okdata == 1)
		{
			key_data.okdata = 0;
			if(seclect_flag == 4)
			{
				if(g_Clock_Timer != NULL)
				{
					xTimerStart(g_Clock_Timer, 0);
					clock_flag = 1;
				}
			}
		}

		if(key_data.exdata == 1)
		{
            key_data.exdata = 0;
			clock_flag = 0;
			xTimerStop(g_Clock_Timer, 0);
			RGB_Set(0, 0, 0);

			for(uint8_t y =0 ; y < 4 ; y++)
			{
				g_real_time[y] = 0;
				u8g2_DrawXBMP(&u8g2, len2+g_num_x[y], g_num_y[1], num_w, num_h, Num_6x8[g_real_time[y]]);
			}

			u8g2_Clear();

            flag = 0;		

			vTaskResume(ShowmeuTaskHandle);
			vTaskSuspend(NULL);
		}	

		if(clock_flag == 1)
		{
			if(g_clock_num[0]==g_real_time[0]&&g_clock_num[1]==g_real_time[1]&&g_clock_num[2]==g_real_time[2]&&g_clock_num[3]==g_real_time[3])
			{
				clock_flag = 0;
				xTimerStop(g_Clock_Timer, 0);
				RGB_Set(1, 0, 0);
			}
		}
    }
}




void ClockTimerCallBackFun(TimerHandle_t xTimer)
{  
	millisecond++;
	if(millisecond>9)
	{
		millisecond = 0;		
		g_real_time[3]++;
		if(g_real_time[3]>9)
		{
			g_real_time[2]++;
			g_real_time[3]=0;
			if(g_real_time[2]>5)
			{
				g_real_time[1]++;
				g_real_time[2] = 0;
				if(g_real_time[1]>9)
				{
					g_real_time[0]++;
					g_real_time[1] = 0;
				}
			}
		}	
	}
}
