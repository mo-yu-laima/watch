#include "IR_rec.h"
#include "stm32f1xx_hal_tim.h"

static IR_states IR = 
{
    .htim = &htim2,
    .Channel = TIM_CHANNEL_3,
    .addressed = 0,
    .command = 0,
    .flag = 1,
    .width = 0,
    .index = 0,
    .recflag = 0,
    .RepeatFlag = 0,
    .Data = 0
};


/**
* @brief 初始化红外接收模块
*
* 初始化红外接收模块相关的定时器，启动定时器中断和输入捕获中断。
*
* @param IR 指向IR_states结构体的指针，包含红外接收模块的配置信息
*
* 作者：继续mo鱼
*/
void IR_Init(void)
{
    HAL_TIM_Base_Start_IT(IR.htim);
    HAL_TIM_IC_Start_IT(IR.htim,IR.Channel );
}




/**
* @brief HAL_TIM_IC_CaptureCallback 回调函数
*
* 此函数为HAL库中的定时器输入捕获回调函数，用于处理定时器输入捕获事件。
*
* @param htim 指向TIM句柄的指针，包含定时器配置信息。
*
* @return 无返回值。
*
* 作者：继续mo鱼
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) 
{
    
    if(htim->Instance == TIM2) // 判断是否是定时器1的外部捕获口
    {
        //LED_ON;
        if (IR.flag) 
        {
					//LED_ON;
            IR.flag = 0;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, IR.Channel, TIM_ICPOLARITY_FALLING);  
            __HAL_TIM_SET_COUNTER(htim, 0); // 计数清零，从头开始计
		} 
        else 
        {
					//LED_ON;
            IR.flag = 1;
            IR.width = HAL_TIM_ReadCapturedValue(IR.htim,IR.Channel );  
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, IR.Channel, TIM_ICPOLARITY_RISING);  
            //LED_ON;

            if (IR.width > 4300 && IR.width < 4700) 
            {  
                //LED_ON;
                IR.recflag = 1;
            } 
            else if (((IR.width > 2050) && (IR.width < 2450))) 
			{
				//LED_ON;
				IR.RepeatFlag = 1; // 设置重发标志
			}    
            if (IR.recflag)  
            {
							 //LED_ON;

                if(IR.width > 1400 && IR.width <1900)
                {
                    
                    IR.Num[IR.index++ / 8] = (IR.Num[IR.index / 8] >> 1) | 0x80;
                    //IR.index++ ;
                }
				else if(IR.width >360 && IR.width <760)
                {
                    
                    IR.Num[IR.index++ / 8] = IR.Num[IR.index / 8] >> 1;
                    //IR.index++ ;
                }
                if(IR.index >=32)
                {
									 //LED_ON;
                    IR.index = 0;
                    IR.flag = 1;
                    IR.recflag = 0;
                    if((IR.Num[0] ==(uint8_t) ~IR.Num[1]) && (IR.Num[2] ==(uint8_t) ~IR.Num[3]))
                    {
						//LED_ON;
                        IR.addressed = IR.Num[0];
                        IR.command = IR.Num[2];
                        IR.Data = 1;
                    }
                }
            }
        }
    }
}






/**
* @brief 获取红外接收数据
*
* 该函数用于检查红外接收模块是否有接收到数据，并返回相应的结果。
*
* @return 如果红外接收模块有接收到数据，则返回1；否则返回0。
*
* 作者：继续mo鱼
*/
uint8_t IR_GetData(void)
{
    if(IR.Data)
    {
        IR.Data = 0;
        return 1;
    }
    else
        return 0;
}





/**
* @brief 获取红外设备的地址
*
* 从红外设备对象IR中获取其地址值。
*
* @return 返回红外设备的地址值，类型为uint16_t。
*
* 作者：继续mo鱼
*/
uint16_t IR_GetAddress(void)
{
    //LED_OFF;
    return IR.addressed;
}





/**
* @brief 获取红外接收模块的命令
*
* 该函数用于获取红外接收模块当前接收到的命令。
*
* @return 返回红外接收模块当前接收到的命令值，类型为uint16_t。
*
* 作者：继续mo鱼
*/
uint16_t IR_GetCommand(void)
{
    uint16_t command = IR.command;
    IR.command = 0;
    return command;
}




/**
* @brief 获取红外重复标志位
*
* 检查红外接收模块中是否有重复标志位被设置。
* 如果标志位被设置，将其重置并返回1，表示检测到重复信号；
* 否则返回0，表示没有检测到重复信号。
*
* @return uint8_t 如果检测到重复信号，返回1；否则返回0。
*
* 作者：继续mo鱼
*/
uint8_t IR_GetRepeatFlag(void)
{
    if(IR.RepeatFlag)
    {
        IR.RepeatFlag = 0;
        return 1;
    }
    else
        return 0;
}



