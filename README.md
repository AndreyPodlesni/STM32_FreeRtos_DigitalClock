## Personal note

This project is for my portfolio, as a means to mastering STM32 development using CubeMX and HAL drivers. It also serves as a practical application in learning and utilizing FreeRTOS with CMSIS v2. this project relies solely on the standard HAL library and FreeRTOS library, without the use of any external libraries.

  

# A Digital Clock Based on STM32F411 Nucleo Board Using CMSIS v2 FreeRTOS with Temperature Reading

  ![enter image description here](https://github.com/bbas121/STM32_FreeRtos_DigitalClock/blob/main/Images/Image.jpg?raw=true)
  
  
A digital clock based on the  STM32F411 Nucleo board, Using a Real-Time Clock (RTC) module for timekeeping. The project uses CMSIS v2 FreeRTOS to manage different operations. It features a 4-digit 7-segment display for showing the time and a 2-digit 7-segment display to show temperature readings.
## Video
[![IMAGE ALT TEXT HERE](https://github.com/bbas121/STM32_FreeRtos_DigitalClock/blob/main/Images/YoutubeThimbnail.jpg?raw=true)](https://youtu.be/R6nLmzWAl1w)


  

## Functions

  

The digital clock offers the following functionalities:

  

-  **Show Time:** Displays the current time on a 4-digit 7-segment display.

-  **Set Minutes:** Allows the user to adjust the minute values of the clock.

-  **Set Hours:** Allows the user to adjust the hours values of the clock

-  **24-Hour / 12-Hour Format:** Switching between 24-hour and 12-hour formats.

-  **Set AM/PM:** Set the clock to AM or PM for the 12-hour format.

-  **Temperature Reading in Celsius:** Displays the current temperature in Celsius on a 2-digit 7-segment display.


## Usage Instructions

1.  **Switching Modes:** Toggle through different settings by short-pressing the Mode button.
2.  **Setting Minutes:** The first mode allows you to set the minutes.
3.  **Setting Hours:** The second mode is used to set the hours.

4.  **Changing Time Format:** The third mode toggles between a 24-hour and a 12-hour time format.
    
5.  **Adjusting AM/PM (For 12-Hour Format):** In the fourth mode, if the clock is in the 12-hour format, you can switch between AM and PM.
    
6.  **Automatic Adjustment for Format Change:** When switching from a 24-hour format to a 12-hour format, the time display and the AM/PM indicator will automatically adjust to reflect the change.
    
7.  **Returning to Time Display:** To exit the settings and return to the normal time display, long-press the Mode button.


  

## Hardware Design

  

The design of this digital clock is centred around the STM32F411 Nucleo board.

**components:**

  

-  **STM32F411 Nucleo Board**

-  **5461AS-1 4 Digits 7-segment Display:** To display The time

-  **2x 5161AS One digit 7-segment display**:For the temperature readings

-  **TMP36GZ Temperature Sensor**

-  **3 PushButtons**:Mode, Up ,Down.

-  **Solderable Breadboard**

-  **14x 100Ohm Resisotrs:** One Resistor for each digit on the 4-digit display and one for each digit on the 2x one-digit display.

## Pinout
![Pin configuration from CubeMx](https://github.com/bbas121/STM32_FreeRtos_DigitalClock/blob/main/Images/CubeMxPinout.jpg?raw=true)
-   A-G: Control pins for the temperature seven-segment display.
-   Cathode_1-2: Cathodes for the left digit of the temperature seven-segment display.
-   FourDigitA-G, DP: Control pins for the clock's seven-segment display, including decimal point.
-   FourDigitD1-4: Cathodes for the clock's seven-segment display, arranged from left to right.
-   TemperatureSensor: Analog input for reading temperature from the temperature sensor.
-   PushButtonDown: The left push button, designated for downward actions.
-   PushButtonUp: The right push button, designated for upward actions.
-   PushButtonMode: Blue Push button for changing modes.
  
   

## Firmware

 

- I wrote the firmware in C using STM32CubeIDE and  FreeRTOS CMSIS v2.
  All the display drivers were my own work – I built them from the ground up without any outside libraries. To get things like the ADC module, UART for debugging, and the GPIO pin modes set up, I used STM32CubeMX to sort out the initial code.

- I used a 12-bit ADC to take temperature readings, applying the polling method. Then, I applied a specific formula to convert these readings into actual temperature values.

- Buttons: Mode, Up, and Down the buttons are GPIO input with internal pull-down Resistors configured by software. with a debounce logic and logic for short press and long press for the Mode button

  

### Freertos Tasks

**ReadTempretureTask:** to read the temperature from the ADC

**ShowTimeModeTask:** to update the time and the temperature displays for the clock 4 digit display and the tempreture 2 digit display

**PushButtonsTask:** Handles reading the push button  states inculding debounce mechanism for all buttons, then sets the state for the appropriate setting 
this is the states in the typedef named state
ShowTime,
SetMinutes,
SetHours,
Set24or12,
SetAMPM



