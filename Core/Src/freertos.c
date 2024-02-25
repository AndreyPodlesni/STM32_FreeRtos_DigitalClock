/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include <stdio.h>
#include "usart.h"
#include "gpio.h"
#include "rtc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    ShowTime,
    SetMinutes,
	SetHours,
	Set24or12,
	SetAMPM
} StateType;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TOTAL_MODES 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

uint16_t number;
uint8_t TaskNumber;
RTC_TimeTypeDef Time ;
RTC_DateTypeDef Date;
uint8_t Tempreture;
volatile StateType currentState = ShowTime;
/* USER CODE END Variables */
/* Definitions for ReadTempreture */
osThreadId_t ReadTempretureHandle;
const osThreadAttr_t ReadTempreture_attributes = {
  .name = "ReadTempreture",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ShowTimeMode */
osThreadId_t ShowTimeModeHandle;
const osThreadAttr_t ShowTimeMode_attributes = {
  .name = "ShowTimeMode",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for PushButtons */
osThreadId_t PushButtonsHandle;
const osThreadAttr_t PushButtons_attributes = {
  .name = "PushButtons",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for UartMutex */
osMutexId_t UartMutexHandle;
const osMutexAttr_t UartMutex_attributes = {
  .name = "UartMutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void PushButtonDebounce(void);
/* USER CODE END FunctionPrototypes */

void ReadTempretureTask(void *argument);
void ShowTimeModeTask(void *argument);
void PushButtonsTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of UartMutex */
  UartMutexHandle = osMutexNew(&UartMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ReadTempreture */
  ReadTempretureHandle = osThreadNew(ReadTempretureTask, NULL, &ReadTempreture_attributes);

  /* creation of ShowTimeMode */
  ShowTimeModeHandle = osThreadNew(ShowTimeModeTask, NULL, &ShowTimeMode_attributes);

  /* creation of PushButtons */
  PushButtonsHandle = osThreadNew(PushButtonsTask, NULL, &PushButtons_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ReadTempretureTask */
/**
  * @brief  Function implementing the ReadTempreture thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ReadTempretureTask */
void ReadTempretureTask(void *argument)
{
  /* USER CODE BEGIN ReadTempretureTask */
  /* Infinite loop */
  for(;;)
  {
	  Tempreture = ReadTemperature();
	  osDelay(pdMS_TO_TICKS(1000));
  }
  /* USER CODE END ReadTempretureTask */
}

/* USER CODE BEGIN Header_ShowTimeModeTask */
/**
* @brief Function implementing the ShowTimeMode thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ShowTimeModeTask */
void ShowTimeModeTask(void *argument)
{
  /* USER CODE BEGIN ShowTimeModeTask */
		Time.Hours = 12;
		Time.Minutes = 00;
		Time.Seconds = 00;
		HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	  /* Infinite loop */
	  for(;;)
	  {
		  if(currentState ==ShowTime)
			  DisplayTime(Time,Date,UartMutexHandle);
		  if(currentState == SetMinutes)
		  DisplaySetMinutes(Time, Date);
		  if(currentState == SetHours)
		  DisplaySetHours(Time,Date);
		  if(currentState == Set24or12)
			  ShowTimeFormat(Time,Date);
		  if (currentState == SetAMPM)
		  DisplayAmPm();
		  //ReadTemperature();
		  WriteNumber(Tempreture);
	  }
  /* USER CODE END ShowTimeModeTask */
}

/* USER CODE BEGIN Header_PushButtonsTask */
/**
* @brief Function implementing the PushButtons thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PushButtonsTask */
void PushButtonsTask(void *argument)
{
  /* USER CODE BEGIN PushButtonsTask */
  /* Infinite loop */
  GPIO_PinState Mode, ModePrev = GPIO_PIN_RESET;
  GPIO_PinState Up, UpPrev = GPIO_PIN_RESET;
  GPIO_PinState Down, DownPrev = GPIO_PIN_RESET;
  uint32_t LastTimeMode = 0, LastTimeUp = 0, LastTimeDown = 0;
  const uint32_t debounceDelay = 50; // Debounce time in milliseconds
  const uint32_t longPressThreshold = 1000; // Long press threshold in milliseconds for Mode button



  for(;;)
  {
    // Read the current state of each button
    Mode = HAL_GPIO_ReadPin(PushButtonMode_GPIO_Port, PushButtonMode_Pin);
    Up = HAL_GPIO_ReadPin(PushButtonUp_GPIO_Port, PushButtonUp_Pin);
    Down = HAL_GPIO_ReadPin(PushButtonDown_GPIO_Port, PushButtonDown_Pin);

    if (osMutexAcquire(UartMutexHandle, 20) == osOK)
    {
      // Check Mode button for short/long press
      if(Mode != ModePrev && (HAL_GetTick() - LastTimeMode) > debounceDelay)
      {
        if(Mode == GPIO_PIN_SET)
        {
          uint32_t pressTime = HAL_GetTick();
          while((Mode == GPIO_PIN_SET) && (HAL_GetTick() - pressTime < longPressThreshold))
          {
            Mode = HAL_GPIO_ReadPin(PushButtonMode_GPIO_Port, PushButtonMode_Pin);
            osDelay(1);
          }
          if(Mode == GPIO_PIN_SET)
          {
        	  if(currentState == ShowTime)
        	  {
        	  currentState = SetMinutes;
        	  }
        	  else
        	  {
        		  currentState = ShowTime;
        	  }
        	  printf("Mode Long Pressed\n\r");
          }
          else
          {
            printf("Mode Short Pressed\n\r");
            if (currentState == SetMinutes)
            	currentState = SetHours;
            else if (currentState == SetHours)
            	currentState = Set24or12;
            else if(currentState == Set24or12)
            {
            	if(hrtc.Init.HourFormat == RTC_HOURFORMAT_24)
            	currentState = SetMinutes;
            	else if (hrtc.Init.HourFormat == RTC_HOURFORMAT_12)
            		currentState = SetAMPM;
            }
            else if (currentState == SetAMPM)
            	currentState = SetMinutes;
          }
        }
        LastTimeMode = HAL_GetTick();
      }

      // Check Up button for short press only
      if(Up != UpPrev && (HAL_GetTick() - LastTimeUp) > debounceDelay)
      {
        if(Up == GPIO_PIN_SET)
        {
        		if(currentState == SetMinutes)
        		AddOneMinuteToRTC(Time,Date);
        		else if(currentState == SetHours)
        		{
        			AddOneHourToRTC(Time,Date);
        		}
         		else if (currentState == Set24or12)
         			ChangeTimeFormat();
         		else if(currentState == SetAMPM)
         		{
         			ChangeAMPMFormat(Time,Date);
         		}
          printf("Up Short Pressed\n\r");
        }
        LastTimeUp = HAL_GetTick();
      }

      // Check Down button for short press only
      if(Down != DownPrev && (HAL_GetTick() - LastTimeDown) > debounceDelay)
      {
        if(Down == GPIO_PIN_SET)
        {
        	     	 if(currentState == SetMinutes)
        	     	 {
        	 		 SubtractOneMinuteFromRTC(Time, Date);
        	     	 }
        	     	 else if(currentState == SetHours)
        	     	 {
        	     		SubtractOneHourFromRTC(Time, Date);
        	     	 }
              		else if (currentState == Set24or12)
              			ChangeTimeFormat();
              		else if(currentState == SetAMPM)
              		{
              			ChangeAMPMFormat(Time,Date);
              		}
          printf("Down Short Pressed\n\r");
        }
        LastTimeDown = HAL_GetTick();
      }

      // Update the previous state for each button
      ModePrev = Mode;
      UpPrev = Up;
      DownPrev = Down;

      osMutexRelease(UartMutexHandle);
    }
    osDelay(1); // Adjust this delay as per your requirement
  }
  /* USER CODE END PushButtonsTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */

