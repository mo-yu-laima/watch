#include "ShowRGBTask.h"

extern osThreadId_t ShowmeuTaskHandle;
extern QueueHandle_t g_xQueueMenu;
extern osMutexId_t displayMutexHandle;

extern u8g2_t u8g2;
extern uint8_t flag;

void ShowRGBTask(void *pvParameters)
{

    u8g2_DrawXBMP(&u8g2, 48, 16, 30, 30, light);
    u8g2_SendBuffer(&u8g2);

    struct Key_data	key_data;
    uint8_t light_flag = 0;
    
	while(1)
	{

        u8g2_ClearBuffer(&u8g2);

        if(xQueueReceive(g_xQueueMenu, &key_data, 0) == pdTRUE)
        {
			if(key_data.okdata == 1)
			{
				key_data.okdata =0;
				switch(light_flag)
				{
					case 0: u8g2_DrawBox(&u8g2, 0, 0, 128, 64);RGB_Set(1, 1, 1);light_flag++;break;
					case 1: u8g2_DrawXBMP(&u8g2, 48, 16, 30, 30, light);RGB_Set(0, 0, 0);light_flag--;break;
				}

			}
			if(key_data.exdata == 1)
			{
				key_data.exdata = 0;
				flag = 0;
				RGB_Set(0, 0, 0);
				u8g2_Clear();

				vTaskResume(ShowmeuTaskHandle);
				vTaskSuspend(NULL);
			}
			u8g2_SendBuffer(&u8g2);
        }
	}
}
