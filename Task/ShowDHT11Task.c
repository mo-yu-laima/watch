#include "ShowDHT11Task.h"
#include <stdint.h>

extern QueueHandle_t g_xQueueMenu;
extern osThreadId_t ShowmeuTaskHandle;
extern osThreadId_t ShowTimerHandle;
extern osThreadId_t defaultTaskHandle;
extern TimerHandle_t g_Timer;

extern u8g2_t u8g2;
extern uint8_t flag;



void ShowDHT11Task(void *pvParameters)
{
    struct Key_data	key_data;
    uint8_t hum_int, hum_dec, temp_int, temp_dec;
    uint8_t hum,temp;
    uint8_t hum1, hum2, hum3 = 0, temp1, temp2 ,temp3 = 0;

    while(1)
    { 
        if(DHT11_ReadData(&hum_int, &hum_dec, &temp_int, &temp_dec))
        {
        	hum = hum_int;
        	temp = temp_int;
        }

            hum3 = hum;
            temp3 = temp;

            temp1 = temp_int / 10;
            temp2 = temp_int % 10;

            hum1 = hum_int / 10;
            hum2 = hum_int % 10;

            u8g2_DrawXBMP(&u8g2, 10, 20, 20, 40, BigNum[temp2]);
            u8g2_DrawXBMP(&u8g2, 35, 20, 20, 40, BigNum[temp1]);
                
            u8g2_DrawXBMP(&u8g2, 75, 20, 20, 40, BigNum[hum2]);
            u8g2_DrawXBMP(&u8g2, 100, 20, 20, 40, BigNum[hum1]);

 

        u8g2_DrawStr(&u8g2, 20, 10, "Temp");
		u8g2_DrawStr(&u8g2, 90, 10, "Hum");

        u8g2_SendBuffer(&u8g2);

        // 使用非阻塞方式接收队列消息，如果队列为空则立即返回
        if(xQueueReceive(g_xQueueMenu, &key_data, 0) == pdTRUE)
        {
			if(key_data.exdata == 1)
			{
				key_data.exdata = 0;
				flag = 0;
				vTaskResume(ShowmeuTaskHandle);
				vTaskSuspend(NULL);

			}
        }


        osDelay(10);
    }
}
