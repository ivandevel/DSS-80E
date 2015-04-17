/** @page SPI_SimplexLCD Communication with a monochrome LCD
  @verbatim
  ******************** (C)COPYRIGHT 2011 STMicroelectronics *******************
  * @file    SPI/SPI_SimplexLCD/readme.txt 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Description of the SPI Simplex LCD Example.
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

  This example provides a short description of how to use the SPI firmware library 
  in order to communicate with a monochrome LCD like the one present on the 
  STM8/128-EVAL eval-board by the mean of the associated eval LCD driver.
  Running the example allows the display of "SPI-LCD example" on the first LCD 
  line. While in the second line ST logo is scrolling to finally be displayed in
  the middle of the line.    


  @par Directory contents

  - SPI\SPI_SimplexLCD\main.c          Main file containing the "main" function
  - SPI\SPI_SimplexLCD\stm8s_conf.h    Library Configuration file
  - SPI\SPI_SimplexLCD\stm8s_it.c      Interrupt routines source (not used in this example)
  - SPI\SPI_SimplexLCD\stm8s_it.h      Interrupt routines declaration
    

 @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density and Low 
    density devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
    - The mono LCD
 
 
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8s_eval_lcd.c  (under Utilities\STM8S_EVAL\Common) 
  - Rebuild all files and load your image into target memory
  - Run the example
 
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
