#include "ShowTimer.h"
#include "u8g2.h"
#include "MyData.h"
#include "Buzzer.h"

extern QueueHandle_t g_xQueueMenu;
extern osThreadId_t ShowTimerHandle;
extern osThreadId_t ShowmeuHandle;
extern osThreadId_t ShowDHT11Handle;
extern osThreadId_t ShowRGBHandle;
extern osThreadId_t ShowCalendarHandle;
extern osThreadId_t ShowClockHandle;

extern const osThreadAttr_t RGBTask_attributes;
extern u8g2_t u8g2;

const char str[5][10] = {"Cleder", "RGB", "Hum", "Clock", "More"};

Image Left = {0, 0, 23, 10};
Image Right = {104, 0, 23, 10};
Image String = {53, 10, 0, 0};
Image Rec_select = {49, 16, 32, 32};

uint8_t dock[5] = {45, 55, 65, 75, 85};
uint8_t dock_y = 58, dock_r = 3; 
uint8_t dock_pos = 2;
uint8_t dock_status = 10;

int str_flag = 2;
int queue_flag = 0;
uint32_t end_flag = 0;
uint8_t flag = 0;


extern void ShowRGBTask(void *pvParameters);

void u8g2_MeuUI()
{
    u8g2_ClearBuffer(&u8g2);

    u8g2_DrawXBMP(&u8g2, Left.x, Left.y, Left.w, Left.h, LeftMove);
	u8g2_DrawXBMP(&u8g2, Right.x, Right.y, Right.w, Right.h, RightMove);
	
	u8g2_DrawStr(&u8g2, String.x, String.y, str[str_flag]);

	u8g2_DrawXBMP(&u8g2, cleder.x, cleder.y, cleder.w, cleder.h, cleder.data);
	u8g2_DrawXBMP(&u8g2, torch.x, torch.y, torch.w, torch.h, torch.data);	
	u8g2_DrawXBMP(&u8g2, hum.x, hum.y, hum.w, hum.h, hum.data);
	u8g2_DrawXBMP(&u8g2, clock.x, clock.y, clock.w, clock.h, clock.data);
	u8g2_DrawXBMP(&u8g2, setting.x, setting.y, setting.w, setting.h, setting.data);

	u8g2_DrawDisc(&u8g2, dock[dock_pos], dock_y, dock_r, U8G2_DRAW_ALL);
	for(int i = 0; i<5; i++)
	{
		u8g2_DrawCircle(&u8g2, dock[i], dock_y, dock_r, U8G2_DRAW_ALL);
	}	
	u8g2_DrawFrame(&u8g2, Rec_select.x, Rec_select.y, Rec_select.w, Rec_select.h);

	u8g2_SendBuffer(&u8g2);
}

void ShowmeuTask(void *pvParameters)
{

	flag = 0;

    struct Key_data	key_data;

    while (1)
    {

        if(!flag) u8g2_MeuUI();

		if(queue_flag == 0) xQueueReceive(g_xQueueMenu, &key_data, portMAX_DELAY);	

		if(key_data.rdata == 1)
		{
			end_flag = 0;
			flag = 0;

			if(dock_pos != 0)
			{
				ui_right(&cleder.x, 2);ui_right(&torch.x, 2);ui_right(&hum.x, 2);ui_right(&clock.x, 2);ui_right(&setting.x, 2);
				/* state_machine */
				if(dock_status==0)dock_status=1;
				dock_status--;
				switch(dock_pos)
				{
					case 2:if(dock_status!=0){ui_up(&cleder.y, 1);ui_up(&torch.y, 1);ui_down(&hum.y, 1);ui_down(&clock.y, 1);}break;
				    case 1:if(dock_status!=0){ui_up(&cleder.y, 1);ui_up(&setting.y, 1);ui_down(&torch.y, 1);ui_down(&hum.y, 1);}break;
					case 4:if(dock_status!=0){ui_up(&clock.y, 1);ui_up(&hum.y, 1);ui_down(&setting.y, 1);ui_down(&cleder.y, 1);}break;
					case 3:if(dock_status!=0){ui_up(&hum.y, 1);ui_up(&torch.y, 1);ui_down(&setting.y, 1);ui_down(&clock.y, 1);}break;
				}
			}
            queue_flag++;	
			if(queue_flag == 20)
			{
				dock_status=10;
				end_flag = 1;
				if(dock_pos != 0){dock_pos--;str_flag--;}
				queue_flag = 0;
				key_data.rdata = 0;
				key_data.ldata = 0;
				flag = 1;
				u8g2_MeuUI();
			}
			if(end_flag == 1)buzzer(1000);	
		}
		else if(key_data.ldata == 1)
		{
            end_flag = 0;
			flag = 0;

			if(dock_pos < 4)
			{		
				ui_left(&cleder.x, 2);ui_left(&torch.x, 2);ui_left(&hum.x, 2);ui_left(&clock.x, 2);ui_left(&setting.x, 2);
				/* state_machine */
				if(dock_status==0)dock_status=1;
				dock_status--;
				switch(dock_pos)
				{
					case 0:if(dock_status!=0){ui_up(&hum.y, 1);ui_up(&torch.y, 1);ui_down(&cleder.y, 1);ui_down(&setting.y, 1);}break;
				    case 1:if(dock_status!=0){ui_up(&hum.y, 1);ui_up(&clock.y, 1);ui_down(&torch.y, 1);ui_down(&cleder.y, 1);}break;
					case 2:if(dock_status!=0){ui_up(&clock.y, 1);ui_up(&setting.y, 1);ui_down(&hum.y, 1);ui_down(&torch.y, 1);}break;
					case 3:if(dock_status!=0){ui_up(&setting.y, 1);ui_up(&cleder.y, 1);ui_down(&clock.y, 1);ui_down(&hum.y, 1);}break;
				}				
			}
            queue_flag++;	
			if(queue_flag == 20) 
			{
				dock_status = 10;
				end_flag = 1;
				if(dock_pos < 4){dock_pos++;str_flag++;}
				queue_flag = 0;
				key_data.ldata = 0;
				key_data.rdata = 0;
				flag = 1;
				u8g2_MeuUI();
			}
			if(end_flag == 1)buzzer(1000);
		}
		else if(key_data.okdata == 1)
		{

			switch(dock_pos)
			{
				case 3:u8g2_Clear();vTaskResume(ShowClockHandle);key_data.okdata = 0;vTaskSuspend(NULL);break;
				case 2:u8g2_Clear();vTaskResume(ShowDHT11Handle); key_data.okdata = 0; vTaskSuspend(NULL);break;
				case 1:u8g2_Clear();vTaskResume(ShowRGBHandle);key_data.okdata = 0;vTaskSuspend(NULL);break;
				case 0:u8g2_Clear();vTaskResume(ShowCalendarHandle);key_data.okdata = 0;vTaskSuspend(NULL);break;
			}
		}
		else if(key_data.exdata == 1)
		{
			/* SysSound */
			key_data.exdata = 0;
			buzzer(1000);
			vTaskResume(ShowTimerHandle);
			vTaskSuspend(NULL);
		}

		u8g2_DrawDisc(&u8g2, dock[dock_pos], dock_y, dock_r, U8G2_DRAW_ALL);
		u8g2_DrawStr(&u8g2, String.x, String.y, str[str_flag]);
		u8g2_SendBuffer(&u8g2);
		osDelay(10);
    }
    
}



