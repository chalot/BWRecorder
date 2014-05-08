/**
  ******************************************************************************
  * @file    stm32_eval.h
  * @author  MCD Application Team
  * @version V4.6.1
  * @date    18-April-2011
  * @brief   Header file for stm32_eval.c module.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_EVAL_H
#define __STM32_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/

/** @addtogroup Utilities
  * @{
  */ 
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @defgroup STM32_EVAL_Abstraction_Layer
  * @{
  */
  
/** @defgroup STM32_EVAL_HARDWARE_RESOURCES
  * @{
  */

/**
@code  
 The table below gives an overview of the hardware resources supported by each 
 STM32 EVAL board.
     - LCD: TFT Color LCD (Parallel (FSMC) and Serial (SPI))
     - IOE: IO Expander on I2C
     - sFLASH: serial SPI FLASH (M25Pxxx)
     - sEE: serial I2C EEPROM (M24C08, M24C32, M24C64)
     - TSENSOR: Temperature Sensor (LM75)
     - SD: SD Card memory (SPI and SDIO (SD Card MODE)) 
  =================================================================================================================+
    STM32 EVAL     | LED | Buttons  | Com Ports |    LCD    | IOE  | sFLASH | sEE | TSENSOR | SD (SPI) | SD(SDIO)  |
  =================================================================================================================+
   STM3210B-EVAL   |  4  |    8     |     2     | YES (SPI) | NO   |  YES   | NO  |   YES   |    YES   |    NO     |
  -----------------------------------------------------------------------------------------------------------------+
   STM3210E-EVAL   |  4  |    8     |     2     | YES (FSMC)| NO   |  YES   | NO  |   YES   |    NO    |    YES    |
  -----------------------------------------------------------------------------------------------------------------+
   STM3210C-EVAL   |  4  |    3     |     1     | YES (SPI) | YES  |  NO    | YES |   NO    |    YES   |    NO     |
  -----------------------------------------------------------------------------------------------------------------+
   STM32100B-EVAL  |  4  |    8     |     2     | YES (SPI) | NO   |  YES   | NO  |   YES   |    YES   |    NO     |
  -----------------------------------------------------------------------------------------------------------------+
   STM32L152-EVAL  |  4  |    8     |     2     | YES (SPI) | NO   |  NO    | NO  |   YES   |    YES   |    NO     |
  -----------------------------------------------------------------------------------------------------------------+
   STM32100E-EVAL  |  4  |    8     |     2     | YES (FSMC)| YES  |  YES   | YES |   YES   |    YES   |    NO     |      
  -----------------------------------------------------------------------------------------------------------------+
   STM322xG-EVAL   |  4  |    3     |     1     | YES (FSMC)| YES  |  NO    | YES |   NO    |    NO    |    YES    |
  =================================================================================================================+
@endcode
*/

/**
  * @}
  */
  
/** @defgroup STM32_EVAL_Exported_Types
  * @{
  */
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;

typedef enum 
{  
  BUTTON_WAKEUP = 0,
  BUTTON_USER1 = 1,
  BUTTON_USER2 = 2,
  BUTTON_RIGHT = 3,
  BUTTON_LEFT = 4,
  BUTTON_UP = 5,
  BUTTON_DOWN = 6,
  BUTTON_SEL = 7
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum 
{ 
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef
;

typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;   
/**
  * @}
  */ 
  
/** @defgroup STM32_EVAL_Exported_Constants
  * @{
  */ 

#if 0
/** 
  * @brief  STM322xG_EVAL Define Legacy  
  */ 
#ifdef USE_STM3220F_EVAL       
  #define USE_STM322xG_EVAL
#endif
#define STM3220F_LCD_Init       STM322xG_LCD_Init

#if defined USE_STM322xG_EVAL
 #include "stm32f2xx.h"
 #include "stm322xg_eval/stm322xg_eval.h"
#else 
 #error "Please select first the STM32 EVAL board to be used (in stm32_eval.h)"
#endif                      
#endif

/** 
  * @brief  STM32 Button Defines Legacy  
  */ 
#define Button_WAKEUP        BUTTON_WAKEUP
#define Button_TAMPER        BUTTON_TAMPER
#define Button_KEY           BUTTON_KEY
#define Button_RIGHT         BUTTON_RIGHT
#define Button_LEFT          BUTTON_LEFT
#define Button_UP            BUTTON_UP
#define Button_DOWN          BUTTON_DOWN
#define Button_SEL           BUTTON_SEL
#define Mode_GPIO            BUTTON_MODE_GPIO
#define Mode_EXTI            BUTTON_MODE_EXTI
#define Button_Mode_TypeDef  ButtonMode_TypeDef
#define JOY_CENTER           JOY_SEL
#define JOY_State_TypeDef    JOYState_TypeDef 

/** 
  * @brief  LCD Defines Legacy  
  */ 
#define White               LCD_COLOR_WHITE
#define Black               LCD_COLOR_BLACK
#define Grey                LCD_COLOR_GREY
#define Blue                LCD_COLOR_BLUE
#define Blue2               LCD_COLOR_BLUE2
#define Red                 LCD_COLOR_RED
#define Magenta             LCD_COLOR_MAGENTA
#define Green               LCD_COLOR_GREEN
#define Cyan                LCD_COLOR_CYAN
#define Yellow              LCD_COLOR_YELLOW
#define Line0               LCD_LINE_0
#define Line1               LCD_LINE_1
#define Line2               LCD_LINE_2
#define Line3               LCD_LINE_3
#define Line4               LCD_LINE_4
#define Line5               LCD_LINE_5
#define Line6               LCD_LINE_6
#define Line7               LCD_LINE_7
#define Line8               LCD_LINE_8
#define Line9               LCD_LINE_9
#define Horizontal          LCD_DIR_HORIZONTAL
#define Vertical            LCD_DIR_VERTICAL


/**
  * @}
  */ 

/** @defgroup STM32_EVAL_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32_EVAL_Exported_Functions
  * @{
  */ 
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


#endif /* __STM32_EVAL_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */   

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
