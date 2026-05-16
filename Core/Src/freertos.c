/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "timers.h"
#include "queue.h"
#include "IR_rec.h"
#include "MyData.h"
#include "iwdg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TimerHandle_t g_Timer;
TimerHandle_t g_Clock_Timer;

QueueHandle_t g_xQueueMenu;	

osMutexId_t displayMutexHandle;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

osThreadId_t ShowTimerHandle;
const osThreadAttr_t TimerTask_attributes = {
  .name = "ShowTimerTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t ShowmeuTaskHandle;
const osThreadAttr_t ShowmeuTask_attributes = {
  .name = "ShowmeuTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t ShowDHT11Handle;
const osThreadAttr_t Dht11Task_attributes = {
  .name = "ShowDht11Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t ShowRGBHandle;
const osThreadAttr_t RGBTask_attributes = {
  .name = "ShowRGBTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t ShowCalendarHandle;
const osThreadAttr_t CalendarTask_attributes = {
  .name = "ShowCalendarTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t ShowClockHandle;
const osThreadAttr_t clockTask_attributes = {
  .name = "ShowClockTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};



extern void ShowTimerTask(void *pvParameters);
extern void ShowmeuTask(void *pvParameters);
extern void ShowDHT11Task(void *pvParameters);
extern void ShowRGBTask(void *pvParameters);
extern void ShowCalendarTask(void *params);
extern void ShowClockTask(void *params);

extern void TimerCallBackFun(TimerHandle_t xTimer);
extern void ClockTimerCallBackFun(TimerHandle_t xTimer);

void BSP_Init()
{
  IR_Init();
  u8g2_Init();
}

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  displayMutexHandle = osMutexNew(NULL);

  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  	g_Timer = xTimerCreate("Timer1",
				1000,
				pdTRUE,
				NULL,
				(TimerCallbackFunction_t)TimerCallBackFun);

  
    g_Clock_Timer = xTimerCreate("Timer2",
				100,
				pdTRUE,
				NULL,
				(TimerCallbackFunction_t)ClockTimerCallBackFun);
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  g_xQueueMenu = xQueueCreate(1, 6);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  ShowmeuTaskHandle = osThreadNew(ShowmeuTask, NULL, &ShowmeuTask_attributes);
  ShowTimerHandle = osThreadNew(ShowTimerTask, NULL, &TimerTask_attributes);
  ShowDHT11Handle = osThreadNew(ShowDHT11Task, NULL, &Dht11Task_attributes);
  ShowRGBHandle = osThreadNew(ShowRGBTask, NULL, &RGBTask_attributes);
  ShowCalendarHandle = osThreadNew(ShowCalendarTask, NULL, &CalendarTask_attributes);
  ShowClockHandle = osThreadNew(ShowClockTask, NULL, &clockTask_attributes);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  vTaskSuspend(ShowRGBHandle);
  vTaskSuspend(ShowmeuTaskHandle);
  vTaskSuspend(ShowDHT11Handle);
  vTaskSuspend(ShowCalendarHandle);
  vTaskSuspend(ShowClockHandle);

  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  BSP_Init();

  uint16_t command;
  Key_data Key_data;

  if(g_Timer != NULL)
	{
		xTimerStart(g_Timer, 0);
	}
  /* Infinite loop */
  for(;;)
  {
      command = IR_GetCommand();
      if(command == Left_Button)
      {
        Key_data.ldata = 1;
        xQueueSend(g_xQueueMenu, &Key_data, 0);
        Key_data.ldata = 0;
      }
      else if(command == Right_Button)
      {
        Key_data.rdata = 1;
        xQueueSend(g_xQueueMenu, &Key_data, 0);
        Key_data.rdata = 0;
      }
      else if(command == Up_Button)
      {
        Key_data.updata = 1;
        xQueueSend(g_xQueueMenu, &Key_data, 0);
        Key_data.updata = 0;
      }
      else if(command == Down_Button)
      {
        Key_data.downdata = 1;
        xQueueSend(g_xQueueMenu, &Key_data, 0);
        Key_data.downdata = 0;
      }
      else if(command == Ok_Button)
      {
        Key_data.okdata = 1;
        xQueueSend(g_xQueueMenu, &Key_data, 0);
        Key_data.okdata = 0;
      }   
      else if(command == Exit_Button)
      {
        Key_data.exdata = 1;
        xQueueSend(g_xQueueMenu, &Key_data, 0);
        Key_data.exdata = 0;
      } 
    //HAL_IWDG_Refresh(&hiwdg);
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

