/*
 * 2DigitsDispaly.c
 *
 *  Created on: Jan 11, 2024
 *      Author: andrey
 */


#include <CustomDrivers.h>
#include "FreeRTOS.h"
#include "rtc.h"
#include "cmsis_os.h"
#include "usart.h"
#include <stdio.h>
#include "stdbool.h"
#include "adc.h"

uint32_t currentTick;
uint32_t lastTick;
bool DpState;
bool MinutesState;
bool HoursState;

uint8_t HoursFirstDigit;
uint8_t HoursSecondDigit;

uint8_t MinutsFirstDigit;
uint8_t MinutsSecondDigit;
uint8_t prevHour = 0;



void setSegments(uint8_t number) {
    // Segment pattern for numbers 0-9 in binary
    static const uint8_t segmentMap[10] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };

    uint8_t segments = segmentMap[number];

    GPIO_PinState PinA_State = (segments & 0b00000001) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState PinB_State = (segments & 0b00000010) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState PinC_State = (segments & 0b00000100) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState PinD_State = (segments & 0b00001000) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState PinE_State = (segments & 0b00010000) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState PinF_State = (segments & 0b00100000) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState PinG_State = (segments & 0b01000000) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    // Corresponding HAL_GPIO_WritePin calls
    HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, PinA_State);
    HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, PinB_State);
    HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, PinC_State);
    HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, PinD_State);
    HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, PinE_State);
    HAL_GPIO_WritePin(F_GPIO_Port, F_Pin, PinF_State);
    HAL_GPIO_WritePin(G_GPIO_Port, G_Pin, PinG_State);
}

void SetCathode(uint8_t Digit)
{
	GPIO_PinState Cathode1_Pin_State = (Digit & 0b00000001)? GPIO_PIN_RESET : GPIO_PIN_SET;
	GPIO_PinState Cathode2_Pin_State = (Digit & 0b00000010)? GPIO_PIN_RESET : GPIO_PIN_SET;
    HAL_GPIO_WritePin(Cathode_1_GPIO_Port, Cathode_1_Pin, Cathode1_Pin_State);
    HAL_GPIO_WritePin(Cathode_2_GPIO_Port, Cathode_2_Pin, Cathode2_Pin_State);
}

void WriteDigit(uint8_t Digit,uint8_t number)
{
	setSegments(number);
	SetCathode(Digit);

}

void WriteNumber(uint8_t number)
{


    uint8_t firstDigit = number / 10;  // Extract the first digit
    uint8_t secondDigit = number % 10; // Extract the second digit
    WriteDigit(1, firstDigit);
    osDelay(pdMS_TO_TICKS(1)); // Wait for 50ms

    // Display the second digit
    WriteDigit(2, secondDigit);
    osDelay(pdMS_TO_TICKS(1)); // Wait for 50ms
}

void FourDigitsetSegments(uint8_t number) {
    static const uint8_t segmentMap[17] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111, // 9
        0b10000000, // 10 (DP)
        0b00000000, // 11 (OFF)
        0b01110110, // 12 (H)
        0b01010000, // 13 (r)
        0b01110111, // 14 (A)
        0b00110111, // 15 (M)
        0b01110011  // 16 (P)
    };


    uint8_t segments = segmentMap[number];

    GPIO_PinState FourDigitsPinA_State = (segments & 0b00000001) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState FourDigitsPinB_State = (segments & 0b00000010) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState FourDigitsPinC_State = (segments & 0b00000100) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState FourDigitsPinD_State = (segments & 0b00001000) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState FourDigitsPinE_State = (segments & 0b00010000) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState FourDigitsPinF_State = (segments & 0b00100000) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState FourDigitsPinG_State = (segments & 0b01000000) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState FourDigitsPinDP_State = (segments & 0b10000000) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    // Corresponding HAL_GPIO_WritePin calls
    HAL_GPIO_WritePin(FourDigitA_GPIO_Port, FourDigitA_Pin, FourDigitsPinA_State);
    HAL_GPIO_WritePin(FourDigitB_GPIO_Port, FourDigitB_Pin, FourDigitsPinB_State);
    HAL_GPIO_WritePin(FourDigitC_GPIO_Port, FourDigitC_Pin, FourDigitsPinC_State);
    HAL_GPIO_WritePin(FourDigitD_GPIO_Port, FourDigitD_Pin, FourDigitsPinD_State);
    HAL_GPIO_WritePin(FourDigitE_GPIO_Port, FourDigitE_Pin, FourDigitsPinE_State);
    HAL_GPIO_WritePin(FourDigitF_GPIO_Port, FourDigitF_Pin, FourDigitsPinF_State);
    HAL_GPIO_WritePin(FourDigitG_GPIO_Port, FourDigitG_Pin, FourDigitsPinG_State);
    HAL_GPIO_WritePin(FourDigitDP_GPIO_Port, FourDigitDP_Pin, FourDigitsPinDP_State);

}

void FourDigitsSetCathode(uint8_t Digit)
{
	GPIO_PinState FourDigitsCathodeD1_Pin_State = (Digit == 1)? GPIO_PIN_RESET : GPIO_PIN_SET;
	GPIO_PinState FourDigitsCathodeD2_Pin_State = (Digit == 2)? GPIO_PIN_RESET : GPIO_PIN_SET;
	GPIO_PinState FourDigitsCathodeD3_Pin_State = (Digit == 3)? GPIO_PIN_RESET : GPIO_PIN_SET;
	GPIO_PinState FourDigitsCathodeD4_Pin_State = (Digit == 4)? GPIO_PIN_RESET : GPIO_PIN_SET;



    HAL_GPIO_WritePin(FourDigitD1_GPIO_Port, FourDigitD1_Pin, FourDigitsCathodeD1_Pin_State);
    HAL_GPIO_WritePin(FourDigitD2_GPIO_Port, FourDigitD2_Pin, FourDigitsCathodeD2_Pin_State);
    HAL_GPIO_WritePin(FourDigitD3_GPIO_Port, FourDigitD3_Pin, FourDigitsCathodeD3_Pin_State);
    HAL_GPIO_WritePin(FourDigitD4_GPIO_Port, FourDigitD4_Pin, FourDigitsCathodeD4_Pin_State);
}
void FourDigitsWrite(uint8_t Digit,uint8_t number)
{
	FourDigitsetSegments(number);
	FourDigitsSetCathode(Digit);
}

void FourDigitsWriteNumber(uint16_t number)
{
    uint16_t fourthDigit = number / 1000;      // Extract the first digit
    uint16_t thirdDigit = (number / 100) % 10; // Extract the second digit
    uint16_t secondDigit = (number / 10) % 10;   // Extract the third digit
    uint16_t firstDigit = number % 10;        // Extract the fourth digit




    // Display the first digit
    FourDigitsWrite(1, firstDigit);
    osDelay(pdMS_TO_TICKS(1)); // Wait for 50ms

    // Display the second digit
    FourDigitsWrite(2, secondDigit);
    osDelay(pdMS_TO_TICKS(1)); // Wait for 50ms


    FourDigitsWrite(3, thirdDigit);
    osDelay(pdMS_TO_TICKS(1)); // Wait for 50ms

    FourDigitsWrite(4, fourthDigit);
    osDelay(pdMS_TO_TICKS(1)); // Wait for 50ms


}

void SetDP(GPIO_PinState State)
{
	  FourDigitsSetCathode(3);
	  HAL_GPIO_WritePin(FourDigitDP_GPIO_Port, FourDigitDP_Pin, State);
}

void DisplayTime(RTC_TimeTypeDef Time, RTC_DateTypeDef Date, osMutexId_t UartMutexHandle)
{


	 HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	 HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);


	  HoursFirstDigit = Time.Hours / 10;  // Extract the first digit
	  HoursSecondDigit=Time.Hours % 10; // Extract the second digit

	  MinutsFirstDigit = Time.Minutes / 10;  // Extract the first digit
	  MinutsSecondDigit=Time.Minutes % 10; // Extract the second digit


	    // Display the first digit
	    FourDigitsWrite(4, HoursFirstDigit);
	    osDelay(pdMS_TO_TICKS(1));

	    // Display the second digit
	    FourDigitsWrite(3, HoursSecondDigit);
	    osDelay(pdMS_TO_TICKS(1));


	    FourDigitsWrite(2, MinutsFirstDigit);
	    osDelay(pdMS_TO_TICKS(1));

	    FourDigitsWrite(1, MinutsSecondDigit);
	    osDelay(pdMS_TO_TICKS(1));

		  if (hrtc.Init.HourFormat == RTC_HOURFORMAT_12)
		  {

		    if (Time.TimeFormat == RTC_HOURFORMAT12_PM) {
		    	   FourDigitsWrite(1, 10);
		    	osDelay(pdMS_TO_TICKS(1));
		    }
		  }





	    	uint32_t currentTick = osKernelGetTickCount();
	    	 if (currentTick - lastTick >= 500) {
	    		 DpState = !DpState;
	    	       lastTick = currentTick;
	    	        }
	    if(DpState)
	    {
	    	FourDigitsWrite(3, 10);
	    }
	    else
	    {
	    	FourDigitsWrite(3, 11);
	    }
	    osDelay(pdMS_TO_TICKS(1));

	    //Update AM PM manualy
	    if (hrtc.Init.HourFormat == RTC_HOURFORMAT_12)
	    {
	           // Check if the hour has transitioned from 12 to 1
	           if (prevHour == 12 && Time.Hours == 1)
	           {
	               // Toggle AM/PM
	               Time.TimeFormat = (Time.TimeFormat == RTC_HOURFORMAT12_AM) ? RTC_HOURFORMAT12_PM : RTC_HOURFORMAT12_AM;
	               HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN); // Update the RTC time
	           }

	    }
	    prevHour = Time.Hours;
}

void DisplaySetMinutes(RTC_TimeTypeDef Time, RTC_DateTypeDef Date)
{


	 HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	 HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

	  HoursFirstDigit = Time.Hours / 10;
	  HoursSecondDigit=Time.Hours % 10;

	  MinutsFirstDigit = Time.Minutes / 10;
	  MinutsSecondDigit=Time.Minutes % 10;

	 uint32_t currentTick = osKernelGetTickCount();

	 if (currentTick - lastTick >= 500) {
		 MinutesState = !MinutesState;
	 	    	       lastTick = currentTick;
	 	    	        }


	    FourDigitsWrite(4, HoursFirstDigit);
	    osDelay(pdMS_TO_TICKS(1));


	    FourDigitsWrite(3, HoursSecondDigit);
	    osDelay(pdMS_TO_TICKS(1));

		  if (hrtc.Init.HourFormat == RTC_HOURFORMAT_12)
		  {

		    if (Time.TimeFormat == RTC_HOURFORMAT12_PM) {
		    	   FourDigitsWrite(1, 10);
		    	osDelay(pdMS_TO_TICKS(1)); // Wait for 50ms
		    }

		  }




	    if(MinutesState)
	    {
		    FourDigitsWrite(2, MinutsFirstDigit);
		    osDelay(pdMS_TO_TICKS(1));

		    FourDigitsWrite(1, MinutsSecondDigit);
		    osDelay(pdMS_TO_TICKS(1));
	    }
	    else
	    {
	    	FourDigitsWrite(3, 11);
	    	osDelay(pdMS_TO_TICKS(1));
	    }

}


void DisplaySetHours(RTC_TimeTypeDef Time, RTC_DateTypeDef Date)
{


	 HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	 HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);


	  HoursFirstDigit = Time.Hours / 10;
	  HoursSecondDigit=Time.Hours % 10;

	  MinutsFirstDigit = Time.Minutes / 10;
	  MinutsSecondDigit=Time.Minutes % 10;

	  if (hrtc.Init.HourFormat == RTC_HOURFORMAT_12)
	  {

	    if (Time.TimeFormat == RTC_HOURFORMAT12_PM) {
	    	   FourDigitsWrite(1, 10);
	    	osDelay(pdMS_TO_TICKS(1));
	    }

	  }



	 uint32_t currentTick = osKernelGetTickCount();

	 if (currentTick - lastTick >= 500) {
		 HoursState = !HoursState;
	 	    	       lastTick = currentTick;
	 	    	        }
	 	 	 	FourDigitsWrite(2, MinutsFirstDigit);
	 		    osDelay(pdMS_TO_TICKS(1));

	 		    FourDigitsWrite(1, MinutsSecondDigit);
	 		    osDelay(pdMS_TO_TICKS(1));
	    if(HoursState)
	    {
		    FourDigitsWrite(4, HoursFirstDigit);
		    osDelay(pdMS_TO_TICKS(1));

		    FourDigitsWrite(3, HoursSecondDigit);
		    osDelay(pdMS_TO_TICKS(1));
	    }
	    else
	    {
	    	FourDigitsWrite(3, 11);
	    	osDelay(pdMS_TO_TICKS(1));
	    }
}


void AddOneMinuteToRTC(RTC_TimeTypeDef Time, RTC_DateTypeDef Date)
{
    // Get the current time and date
    HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

    // Add one minute
    Time.Minutes++;

    // Handle minute overflow (if it goes beyond 59, reset to 0)
    if (Time.Minutes >= 60)
    {
        Time.Minutes = 0;
        // No change to the hours
    }

    // Set the new time
    HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &Date, RTC_FORMAT_BIN); // Set the date again due to the RTC hardware design
}

void SubtractOneMinuteFromRTC(RTC_TimeTypeDef Time, RTC_DateTypeDef Date)
{
    // Get the current time and date
    HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);  // This line is necessary due to the RTC hardware design

    // Check if the minutes are at 0, and if so, wrap around to 59
    if (Time.Minutes == 0) {
        Time.Minutes = 59;
        // No change to the hours
    } else {
        // Subtract one minute
        Time.Minutes--;
    }

    // Set the new time
    HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &Date, RTC_FORMAT_BIN); // Set the date again due to the RTC hardware design
}

void AddOneHourToRTC(RTC_TimeTypeDef Time, RTC_DateTypeDef Date)
{
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);




    // Get the current time and date
    HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);  // This is necessary due to the RTC hardware design

    // Add one hour
    Time.Hours++;

    // Handle hour overflow (if it goes beyond 23, reset to 0)
    if (hrtc.Init.HourFormat == RTC_HOURFORMAT_12) {
        if (Time.Hours >= 13)
        {
            Time.Hours = 1;
        }
    }
    else if (hrtc.Init.HourFormat == RTC_HOURFORMAT_24)
    {
			if (Time.Hours >= 24)
			{
				Time.Hours = 0;
			}
    }
    HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &Date, RTC_FORMAT_BIN);
    __HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
}

void SubtractOneHourFromRTC(RTC_TimeTypeDef Time, RTC_DateTypeDef Date)
{
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
    // Get the current time and date
    HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);  // This is necessary due to the RTC hardware design



    if (hrtc.Init.HourFormat == RTC_HOURFORMAT_12) {
    	  	  if (Time.Hours == 1)
    	  	  {
    	  		  Time.Hours = 12;
    	  	  }
    	    else
    	    {

              Time.Hours--;
    	     }
       }

       else if (hrtc.Init.HourFormat == RTC_HOURFORMAT_24)
       {
    	   if (Time.Hours == 0) {
    	           Time.Hours = 23;
    	           // No change to the date
    	       } else {
    	           // Subtract one hour
    	           Time.Hours--;
    	       }
       }
    HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &Date, RTC_FORMAT_BIN);
    __HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
}

void ShowTimeFormat(RTC_TimeTypeDef Time, RTC_DateTypeDef Date)
{
	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
			FourDigitsWrite(1, 13);
			osDelay(pdMS_TO_TICKS(1));


			FourDigitsWrite(2, 12);
			osDelay(pdMS_TO_TICKS(1));


		if (hrtc.Init.HourFormat == RTC_HOURFORMAT_24)
		{
		    FourDigitsWrite(3, 4);
		    osDelay(pdMS_TO_TICKS(1));

		    FourDigitsWrite(4, 2);
		    osDelay(pdMS_TO_TICKS(1));
		}
		if (hrtc.Init.HourFormat == RTC_HOURFORMAT_12)
		{
		    FourDigitsWrite(3, 2);
		    osDelay(pdMS_TO_TICKS(1));

		    FourDigitsWrite(4, 1);
		    osDelay(pdMS_TO_TICKS(1));
		}
		__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
}


void ChangeTimeFormat(void) {
    RTC_TimeTypeDef Time;
    RTC_DateTypeDef Date;

    // Unlock the RTC
    __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);

    // Get the current time and date
    HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN); // Necessary to unlock the shadow registers

    // Check current format and switch
    if (hrtc.Init.HourFormat == RTC_HOURFORMAT_24) {
        // Switching to 12-hour format
        hrtc.Init.HourFormat = RTC_HOURFORMAT_12;

        // Convert 13:00 and later to PM
        if (Time.Hours >= 13) {
            Time.Hours -= 12;
            Time.TimeFormat = RTC_HOURFORMAT12_PM;
        } else if (Time.Hours == 0) {
            // Convert 00:00 (midnight) to 12 AM
            Time.Hours = 12;
            Time.TimeFormat = RTC_HOURFORMAT12_AM;
        } else {
            // Morning times remain the same, set to AM
            Time.TimeFormat = RTC_HOURFORMAT12_AM;
        }
    } else {
        // Switching to 24-hour format
        hrtc.Init.HourFormat = RTC_HOURFORMAT_24;

        // Convert 12 AM to 00
        if (Time.Hours == 12 && Time.TimeFormat == RTC_HOURFORMAT12_AM) {
            Time.Hours = 0;
        }
        // Convert PM times (except for 12 PM) by adding 12
        else if (Time.TimeFormat == RTC_HOURFORMAT12_PM && Time.Hours != 12) {
            Time.Hours += 12;
        }
    }

    // Re-initialize the RTC with the new settings
    HAL_RTC_Init(&hrtc);

    // Set the updated time and date
    HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &Date, RTC_FORMAT_BIN);

    // Lock the RTC
    __HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
}




void DisplayAmPm(RTC_TimeTypeDef Time, RTC_DateTypeDef Date)
{

	 HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	 HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);



	    FourDigitsWrite(1, 15); //M
	    osDelay(pdMS_TO_TICKS(1));

	    FourDigitsWrite(3, 11); //OFF
	    osDelay(pdMS_TO_TICKS(1));

	    FourDigitsWrite(4, 11); //OFF
	    osDelay(pdMS_TO_TICKS(1));


	 			if (Time.TimeFormat == RTC_HOURFORMAT12_AM)
	 			{

	 			    FourDigitsWrite(2, 14); //A
	 			    osDelay(pdMS_TO_TICKS(1));
	 			}
	 			else if (Time.TimeFormat == RTC_HOURFORMAT12_PM)
	 			{

	 			    FourDigitsWrite(2, 16); //P
	 			    osDelay(pdMS_TO_TICKS(1));
	 			   FourDigitsWrite(1, 10);
	 			  osDelay(pdMS_TO_TICKS(1)); // Show Dp in PM
	 			}
}

void ChangeAMPMFormat(RTC_TimeTypeDef Time, RTC_DateTypeDef Date) {
    // No need to declare sTime and sDate since we are using Time and Date passed as arguments

    // Get the current time and date
    HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN); // Necessary to unlock the shadow registers

    // Check if the RTC is in 12-hour format
    if (hrtc.Init.HourFormat == RTC_HOURFORMAT_12) {
        // Toggle AM/PM
        if (Time.TimeFormat == RTC_HOURFORMAT12_AM) {
            Time.TimeFormat = RTC_HOURFORMAT12_PM;
        } else {
            Time.TimeFormat = RTC_HOURFORMAT12_AM;
        }

        // Set the new time with updated AM/PM format
        HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
        HAL_RTC_SetDate(&hrtc, &Date, RTC_FORMAT_BIN); // Set the date again due to RTC hardware design
    }
}

uint8_t ReadTemperature(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    uint16_t adcValue = HAL_ADC_GetValue(&hadc1);

    //  reference voltage of 3.3V and a 12-bit ADC
    float voltage = ((float)adcValue * 3.3f * 1000) / 4096.0f; // Voltage in mV

    // Apply temperature conversion formula
    float temperatureFloat = (voltage - 500) / 10;

    // Ensure temperature is within the range of 0 to 255
    if (temperatureFloat < 0) {
        temperatureFloat = 0;
    } else if (temperatureFloat > 255) {
        temperatureFloat = 255;
    }

    uint8_t temperature = (uint8_t)temperatureFloat;
    return temperature;
}





