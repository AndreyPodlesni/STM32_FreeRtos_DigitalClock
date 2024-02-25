/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define F_Pin GPIO_PIN_0
#define F_GPIO_Port GPIOC
#define E_Pin GPIO_PIN_1
#define E_GPIO_Port GPIOC
#define G_Pin GPIO_PIN_2
#define G_GPIO_Port GPIOC
#define A_Pin GPIO_PIN_0
#define A_GPIO_Port GPIOA
#define B_Pin GPIO_PIN_1
#define B_GPIO_Port GPIOA
#define C_Pin GPIO_PIN_4
#define C_GPIO_Port GPIOA
#define TempretureSensor_Pin GPIO_PIN_5
#define TempretureSensor_GPIO_Port GPIOA
#define FourDigitD4_Pin GPIO_PIN_7
#define FourDigitD4_GPIO_Port GPIOA
#define FourDigitF_Pin GPIO_PIN_4
#define FourDigitF_GPIO_Port GPIOC
#define PushButtonDown_Pin GPIO_PIN_5
#define PushButtonDown_GPIO_Port GPIOC
#define PushButtonDown_EXTI_IRQn EXTI9_5_IRQn
#define D_Pin GPIO_PIN_0
#define D_GPIO_Port GPIOB
#define FourDigitB_Pin GPIO_PIN_1
#define FourDigitB_GPIO_Port GPIOB
#define FourDigitA_Pin GPIO_PIN_2
#define FourDigitA_GPIO_Port GPIOB
#define FourDigitE_Pin GPIO_PIN_13
#define FourDigitE_GPIO_Port GPIOB
#define FourDigitD_Pin GPIO_PIN_14
#define FourDigitD_GPIO_Port GPIOB
#define FourDigitC_Pin GPIO_PIN_15
#define FourDigitC_GPIO_Port GPIOB
#define PushButtonUp_Pin GPIO_PIN_6
#define PushButtonUp_GPIO_Port GPIOC
#define PushButtonUp_EXTI_IRQn EXTI9_5_IRQn
#define FourDigitG_Pin GPIO_PIN_8
#define FourDigitG_GPIO_Port GPIOA
#define FourDigitD3_Pin GPIO_PIN_9
#define FourDigitD3_GPIO_Port GPIOA
#define FourDigitD2_Pin GPIO_PIN_10
#define FourDigitD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Cathode_1_Pin GPIO_PIN_10
#define Cathode_1_GPIO_Port GPIOC
#define Cathode_2_Pin GPIO_PIN_12
#define Cathode_2_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define FourDigitDP_Pin GPIO_PIN_4
#define FourDigitDP_GPIO_Port GPIOB
#define FourDigitD1_Pin GPIO_PIN_5
#define FourDigitD1_GPIO_Port GPIOB
#define PushButtonMode_Pin GPIO_PIN_8
#define PushButtonMode_GPIO_Port GPIOB
#define PushButtonMode_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
