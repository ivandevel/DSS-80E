/** @page ADC2_ContinuousConversion Displaying the ADC2 conversion result using LEDs
  @verbatim
  ******************** (C)COPYRIGHT 2011 STMicroelectronics *******************
  * @file    ADC2/ADC2_ContinuousConversion/readme.txt
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   This file contains the description of ADC2 Continuous Conversion example.
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim

  @par Example description
 
  This example provides a short description of how to use the ADC peripheral
  to convert ADC2 Channel 9 input voltage in Continuous conversion mode with interrupt.
  
  In the STM8/128-EVAL board, ADC2 Channel 9 is connected to RV1 Adjuster. 
  Turning RV1 adjuster provides a voltage which can vary from 0 Volt to 5 Volt.
  Depending on the voltage range, LEDs from 1 to 4 light up. 

  
  @par Directory contents

  - ADC2\ADC2_ContinuousConversion\main.c            Main file containing the "main" function
  - ADC2\ADC2_ContinuousConversion\stm8s_conf.h      Library Configuration file
  - ADC2\ADC2_ContinuousConversion\stm8s_it.c        Interrupt routines source
  - ADC2\ADC2_ContinuousConversion\stm8s_it.h        Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example runs on STM8S and STM8A high density devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - RV1 adjuster
     - LD1..LD4


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Turn the potentiometer RV1 on the board to change the voltage to be converted
  - The four LEDs (LD1, LD2, LD3 and LD4) follow the ADC2 value

  @note
  - High-Density STM8A devices are the STM8AF52xx STM8AF6269/8x/Ax,
    STM8AF51xx, and STM8AF6169/7x/8x/9x/Ax microcontrollers where the Flash memory
    density ranges between 32 to 128 Kbytes
  - Medium-Density STM8A devices are the STM8AF622x/4x, STM8AF6266/68,
    STM8AF612x/4x, and STM8AF6166/68 microcontrollers where the Flash memory 
    density ranges between 8 to 32 Kbytes
  - High-Density STM8S devices are the STM8S207xx, STM8S007 and STM8S208xx microcontrollers
    where the Flash memory density ranges between 32 to 128 Kbytes.
  - Medium-Density STM8S devices are the STM8S105x and STM8S005 microcontrollers
    where the Flash memory density ranges between 16 to 32-Kbytes.
  - Low-Density STM8S devices are the STM8S103xx, STM8S003 and STM8S903xx microcontrollers
    where the Flash density is 8 Kbytes.
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
