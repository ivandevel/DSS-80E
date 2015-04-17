/** @page TIM5_Synchronisation_with_TIM6 Synchronisation of TIM5 using TIM6

  @verbatim
  ******************** (C)COPYRIGHT 2011 STMicroelectronics *******************
  * @file    TIM5/TIM5_Synchronisation_with_TIM6/readme.txt
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Description of the TIM5 Synchronisation using TIM6 Example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim
   

  @par Example description

  This example provides a short description of how to use the TIM5 peripheral
  synchronised by TIM6. TIM5 is configured as a slave of TIM6. 

  TIM5 is configured to be enabled if it detects an edge on its input trigger (TRGI).
  TIM6 is configurated to generate an edge through its (TRGO) at the update event.
  TIM5 TRGI is configurated to be connected (internal connection) to TIM6 TRGO.

  If line "TIM6_Cmd(ENABLE);" in main.c is commented, TIM6 is not enabled => no Edge is detected on TIM5 TRGI,
  ==> Result variable value will be equal to 0 (FAILED).
  If line "TIM6_Cmd(ENABLE);" in main.c is uncommented, TIM6 is enabled => TIM6 generate an Edge on its TRGO and 
  an Edge is detected on TIM5 TRGI. ==> Result variable value will be equal to 1 (PASS).


  @par Directory contents

  - TIM5\TIM5_Synchronisation_With_TIM6\main.c                    Main file containing the "main" function
  - TIM5\TIM5_Synchronisation_With_TIM6\stm8s_conf.h              Library Configuration file
  - TIM5\TIM5_Synchronisation_With_TIM6\stm8s_it.c                Interrupt routines source
  - TIM5\TIM5_Synchronisation_With_TIM6\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example runs only on STM8S903 devices.
  
  - This example has been tested with a STMicroelectronics generic board 
    (STM8S903 device) and can be easily tailored to any other development board. 

  
@par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Example result can be displayed using "Result" variable.
 
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
