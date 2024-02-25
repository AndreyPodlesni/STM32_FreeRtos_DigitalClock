/*
 * 2DigitsDispaly.h
 *
 *  Created on: Jan 11, 2024
 *      Author: andrey
 */

#ifndef SRC_2DIGITSDISPALY_H_
#define SRC_2DIGITSDISPALY_H_

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
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
#define D_Pin GPIO_PIN_0
#define D_GPIO_Port GPIOB
#define Cathode_1_Pin GPIO_PIN_10
#define Cathode_1_GPIO_Port GPIOC
#define Cathode_2_Pin GPIO_PIN_12
#define Cathode_2_GPIO_Port GPIOC


#define FourDigitA_Pin GPIO_PIN_2
#define FourDigitA_GPIO_Port GPIOB

#define FourDigitB_Pin GPIO_PIN_1
#define FourDigitB_GPIO_Port GPIOB

#define FourDigitC_Pin GPIO_PIN_15
#define FourDigitC_GPIO_Port GPIOB

#define FourDigitD_Pin GPIO_PIN_14
#define FourDigitD_GPIO_Port GPIOB

#define FourDigitE_Pin GPIO_PIN_13
#define FourDigitE_GPIO_Port GPIOB

#define FourDigitF_Pin GPIO_PIN_4
#define FourDigitF_GPIO_Port GPIOC

#define FourDigitG_Pin GPIO_PIN_8
#define FourDigitG_GPIO_Port GPIOA

#define FourDigitDP_Pin GPIO_PIN_4
#define FourDigitDP_GPIO_Port GPIOB

#define FourDigitD1_Pin GPIO_PIN_5
#define FourDigitD1_GPIO_Port GPIOB

#define FourDigitD2_Pin GPIO_PIN_10
#define FourDigitD2_GPIO_Port GPIOA

#define FourDigitD3_Pin GPIO_PIN_9
#define FourDigitD3_GPIO_Port GPIOA

#define FourDigitD4_Pin GPIO_PIN_7
#define FourDigitD4_GPIO_Port GPIOA







#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

void setSegments(uint8_t number);
void SetCathode(uint8_t Digit);
void WriteDigit(uint8_t Digit,uint8_t number);
void WriteNumber(uint8_t number);

void FourDigitsetSegments(uint8_t number);
void FourDigitsSetCathode(uint8_t Digit);
void FourDigitsWrite(uint8_t Digit,uint8_t number);
void FourDigitsWriteNumber(uint16_t number);
void DisplayTime(RTC_TimeTypeDef Time,RTC_DateTypeDef Date,osMutexId_t UartMutexHandle);
void DisplaySetMinutes(RTC_TimeTypeDef Time, RTC_DateTypeDef Date);
void DisplaySetHours(RTC_TimeTypeDef Time, RTC_DateTypeDef Date);
void AddOneMinuteToRTC(RTC_TimeTypeDef Time, RTC_DateTypeDef Date);
void SubtractOneMinuteFromRTC(RTC_TimeTypeDef Time, RTC_DateTypeDef Date);
void AddOneHourToRTC(RTC_TimeTypeDef Time, RTC_DateTypeDef Date);
void SubtractOneHourFromRTC(RTC_TimeTypeDef Time, RTC_DateTypeDef Date);
void ShowTimeFormat(RTC_TimeTypeDef Time, RTC_DateTypeDef Date);
void ChangeAMPMFormat(RTC_TimeTypeDef Time, RTC_DateTypeDef Date);
void DisplayAmPm(RTC_TimeTypeDef Time, RTC_DateTypeDef Date);
uint8_t ReadTemperature(void);

#endif /* SRC_2DIGITSDISPALY_H_ */
