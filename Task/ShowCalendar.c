#include "ShowCalendar.h"

extern QueueHandle_t g_xQueueMenu;
extern osThreadId_t ShowmeuTaskHandle;

extern uint8_t flag;

extern u8g2_t u8g2;


int judge_year(int year)
{
	if(year%400==0||(year%4==0&&year%100!=0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int judge_week(int year)
{
	if(year==1){
		return 1;
	}
	
	int sum=0;
	for(int i=1;i<year;i++)
	{
		if(judge_year(i)==1)
		{
			sum=sum+366;
		}
		else
		{
			sum=sum+365;
		}
	}
	return (sum+1)%7;
}


int month_run(uint8_t n,uint16_t y)
{

	switch(n)
	{
		case 1:return 31;
		case 2:if(judge_year(y)) return 29;else return 28;
		case 3:return 31;
		case 4:return 30;
		case 5:return 31;
		case 6:return 30;
		case 7:return 31;      
		case 8:return 31;
		case 9:return 30;
		case 10:return 31;
		case 11:return 30;
		case 12:return 31;
	}
}



void ShowCalendarTask(void *params)
{

    struct Key_data	key_data;

    const char ucMonthDay[32][3] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"};
	const char ucWeekHeader[7][3] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
	uint16_t usWeekX[7] = {0,  17, 34 , 51, 68, 85, 102};
	uint16_t usWeekY[6] = {17, 26, 35 ,44, 53, 62};
	uint16_t usLineY[12] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99, 110, 121};
	uint8_t line_pos  = 0;

	uint8_t week_pos  = 0;
	uint32_t week_temp, week_temp_temp, month_temp, enter_temp;

	uint8_t month = 1;
	uint16_t wee = 0;

	uint16_t year = 2025; //年份

	u8g2_SetFont(&u8g2, u8g2_font_spleen5x8_mf);
	u8g2_SendBuffer(&u8g2);

    while(1)
    {
		u8g2_ClearBuffer(&u8g2);			
			
		for(int i=0; i<=6; i++)
        {
			u8g2_DrawStr(&u8g2, usWeekX[i], 8, ucWeekHeader[i]);			
		}  

        month_temp = month_run(month,year);
        week_temp = judge_week(year);
        wee = week_temp;

		for(int m=1; m<month; m++)
        {
            wee = (wee+month_run(m,year))%7;
        }

        week_temp = wee;		
		week_temp_temp = week_temp;

		for(int k=1; k<=month_temp; k++)
        {			
			enter_temp  = week_temp%7;
			week_temp++;

			if(k<=(7-week_temp_temp))
			{
				week_pos=0;
			}else if(enter_temp == 0)
			{
				week_pos = week_pos+1;			
			}

			u8g2_DrawStr(&u8g2, usWeekX[enter_temp], usWeekY[week_pos], ucMonthDay[k]);	
		}  

		u8g2_DrawLine(&u8g2, 115, 0, 115, 62);
		u8g2_DrawStr(&u8g2, 117, 32, ucMonthDay[line_pos+1]);
		u8g2_DrawHLine(&u8g2, usLineY[line_pos], 63, 11);

        u8g2_SendBuffer(&u8g2);

        if(xQueueReceive(g_xQueueMenu, &key_data, portMAX_DELAY) == pdTRUE)
        {

            if(key_data.ldata == 1)
            {
            	key_data.ldata = 0;
                month--;
                line_pos--;
                if(line_pos == 255) line_pos = 11;
                if(month < 1) month = 12;
            }
        

            if(key_data.rdata == 1)
            {
            	key_data.rdata = 0;
                month++;
                line_pos++; 
                if(line_pos > 11) line_pos = 0;
                if(month > 12) month = 1;
            }
            
            if(key_data.exdata == 1)
            {
    			key_data.exdata = 0;
    			flag = 0;
    			u8g2_Clear();

    			vTaskResume(ShowmeuTaskHandle);
    			vTaskSuspend(NULL);
            }
        }
    }    
}
