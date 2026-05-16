#include "DHT11.h"
#include "cmsis_os2.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "gpio.h"


#define CPU_FREQUENCY_MHZ    72		// STM32时钟主频
GPIO_InitTypeDef GPIO_InitStruct = {0};

    void DHT11_OUT(void )//输出引脚
    {
    	  GPIO_InitStruct.Pin = DHT11_data_Pin;
    	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    	  GPIO_InitStruct.Pull = GPIO_PULLUP;
    	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    	  HAL_GPIO_Init(DHT11_data_GPIO_Port, &GPIO_InitStruct);
    }

    void DHT11_IN(void )//输入引脚
    {
    	GPIO_InitStruct.Pin = DHT11_data_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(DHT11_data_GPIO_Port, &GPIO_InitStruct);
    }
/**
* @brief 延时函数，通过SysTick定时器实现微秒级延时
*
* 使用SysTick定时器实现微秒级延时。该函数通过循环检查SysTick定时器的当前值来实现延时。
*
* @param delay 延时的微秒数
*/
void delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}







/**
* @brief 启动DHT11传感器
*
* 该函数用于启动DHT11传感器，以便进行后续的数据通信。
*
* 首先，将DHT11的数据引脚置为低电平，并保持20微秒。
* 然后，将DHT11的数据引脚置为高电平，并保持30微秒。
*
* @note 该函数假定已正确配置了GPIO端口和引脚。
*
* 作者：继续mo鱼
*/
void DHT11_Start()
{
	//DHT11_OUT();
    HAL_GPIO_WritePin(DHT11_data_GPIO_Port, DHT11_data_Pin, 0);
    HAL_Delay(20);
	
    HAL_GPIO_WritePin(DHT11_data_GPIO_Port, DHT11_data_Pin, 1);
    delay_us(40);

}




/**
* @brief 检查DHT11传感器是否准备好发送数据
*
* 该函数通过检测DHT11传感器的数据引脚电平来判断DHT11传感器是否准备好发送数据。 
* 如果数据引脚为低电平，则延时85微秒后，等待数据引脚变为高电平，表示DHT11传感器已准备好发送数据，函数返回1。 
* 如果数据引脚为高电平，表示DHT11传感器未准备好发送数据，函数返回0。 
* 
* @return uint8_t 返回1表示DHT11传感器已准备好发送数据，返回0表示未准备好或超时。
*
* 作者：继续mo鱼
*/
uint8_t DHT11_check_state()
{
    uint32_t timeout = 1000;
    //DHT11_IN();
    if(!HAL_GPIO_ReadPin(DHT11_data_GPIO_Port, DHT11_data_Pin))
    {

        delay_us(80);
			//LED_ON;
        while (HAL_GPIO_ReadPin(DHT11_data_GPIO_Port, DHT11_data_Pin) && timeout > 0)
        {
            HAL_Delay(1);
			timeout--;

        }
        return (timeout != 0 && timeout != 100);  
    }
    return 0;
}




/**
* @brief 从DHT11传感器读取一个字节的数据
*
* 从DHT11传感器读取一个字节的数据，每个字节包含8个位。
*
* @return 读取到的字节数据
*
* 作者：继续mo鱼
*/
uint8_t DHT11_Read_Byte()
{
    uint8_t i,Byte = 0x00;
    uint16_t timeout = 100;
    //DHT11_IN();
	for(i=0;i<8;i++)
	{
        //LED_ON;
		while(!HAL_GPIO_ReadPin(DHT11_data_GPIO_Port, DHT11_data_Pin));
		delay_us(50);
		//高电平持续时间26-28us为0,70us为1
		if(HAL_GPIO_ReadPin(DHT11_data_GPIO_Port, DHT11_data_Pin))
		{
			Byte |= (0x80>>i);//高位在前
		}
        while (HAL_GPIO_ReadPin(DHT11_data_GPIO_Port, DHT11_data_Pin) && timeout --)
        {
            osDelay(1);
			timeout--;
        }
		if(timeout == 0 && HAL_GPIO_ReadPin(DHT11_data_GPIO_Port, DHT11_data_Pin)) break;
	}
	return Byte;
}




/**
* @brief 从DHT11传感器读取温湿度数据
*
* 该函数从DHT11传感器读取湿度和温度数据，并将数据存储在提供的指针参数中。
*
* @param Hum_Int 指向存储湿度整数部分的指针
* @param Hum_Dec 指向存储湿度小数部分的指针
* @param Temp_Int 指向存储温度整数部分的指针
* @param Temp_Dec 指向存储温度小数部分的指针
*
* @return 读取数据是否成功，成功返回1，失败返回0
*
* @note 该函数首先发送起始信号，然后检查DHT11的状态。如果状态正常，则从DHT11读取5个字节的数据，
*       并存储在Buf数组中。之后，它会校验数据的完整性，如果校验通过，则将湿度和温度的整数和小数部分
*       分别存储在提供的指针参数中，并返回1表示成功。如果校验失败或DHT11状态异常，则返回0表示失败。
*
* 作者：继续mo鱼
*/
uint8_t DHT11_ReadData (uint8_t *Hum_Int, uint8_t *Hum_Dec, uint8_t *Temp_Int, uint8_t *Temp_Dec)
{
    uint8_t Buf[5];
	uint8_t i;
	DHT11_Start();//起始信号

    if(DHT11_check_state() == 1)
	{
		for(i=0;i<5;i++)
		{
			Buf[i] = DHT11_Read_Byte();//读取5个字节存在数组中
		}
		LED_ON;
		//delay_us(60);//保证完整读取

        if(Buf[0] + Buf[1] + Buf[2] + Buf[3] == Buf[4])//校验数据是否有效,如果有效
		{
			*Temp_Int = Buf[0];		//湿度整数部分
			*Temp_Dec = Buf[1];		//湿度小数部分
			*Hum_Int = Buf[2];		//温度整数部分
			*Hum_Dec = Buf[3];		//温度小数部分		//验证读取数据是否正确

			return 1;
		}
        else
        {
			*Temp_Int = 99;		//湿度整数部分
			*Temp_Dec = 0;		//湿度小数部分
			*Hum_Int = 99;		//温度整数部分
			*Hum_Dec = 0;

			return 0;
        }

    }
    else
        return 0;
}




//void DHT11_Task()
//{
//    uint8_t ret;
//    while(1)
//    {
//        ret =DHT11_ReadData(&Hum_Int, &Hum_Dec, &Temp_Int, &Temp_Dec);
//        if(ret)
//        {
//            Hum = Hum_Int + Hum_Dec;
//            Temp = Temp_Int + Temp_Dec;
//            OLED_ShowString(14, 4, (uint8_t *)"Hum:", 2);
//            OLED_ShowNum(48, 4, Hum, 3, 2);
//
//            OLED_ShowString(14, 6, (uint8_t *)"Tem:", 2);
//            OLED_ShowNum(48, 6, Temp, 3, 2);
//            osDelay(500);
//        }
//    }
//}
