/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.h 
  * @author  MCD Application Team
  * @version V0.0.2
  * @date    02/25/2010
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F2xx_IT_H
#define __STM32F2xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


void NMI_Handler(void);
//void HardFault_Handler(void);
void MemManage_Handler(void);
//void BusFault_Handler(void);
void UsageFault_Handler(void);
//void SVC_Handler(void);
void DebugMon_Handler(void);
//void PendSV_Handler(void);
//void SysTick_Handler(void);

void WWDG_IRQHandler(void);                   // Window WatchDog
//void PVD_IRQHandler(void);                    // PVD through EXTI Line detection
void TAMP_STAMP_IRQHandler(void);             // Tamper and TimeStamps through the EXTI line
void RTC_WKUP_IRQHandler(void);               // RTC Wakeup through the EXTI line
void FLASH_IRQHandler(void);                  // FLASH
void RCC_IRQHandler(void);                    // RCC
void EXTI0_IRQHandler(void);                  // EXTI Line0
void EXTI1_IRQHandler(void);                  // EXTI Line1
void EXTI2_IRQHandler(void);                  // EXTI Line2
void EXTI3_IRQHandler(void);                  // EXTI Line3
void EXTI4_IRQHandler(void);                  // EXTI Line4
void DMA1_Stream0_IRQHandler(void);           // DMA1 Stream 0
void DMA1_Stream1_IRQHandler(void);           // DMA1 Stream 1
void DMA1_Stream2_IRQHandler(void);           // DMA1 Stream 2
void DMA1_Stream3_IRQHandler(void);           // DMA1 Stream 3
void DMA1_Stream4_IRQHandler(void);           // DMA1 Stream 4
void DMA1_Stream5_IRQHandler(void);           // DMA1 Stream 5
void DMA1_Stream6_IRQHandler(void);           // DMA1 Stream 6
void ADC_IRQHandler(void);                    // ADC1, ADC2 and ADC3s
void CAN1_TX_IRQHandler(void);                // CAN1 TX
void CAN1_RX0_IRQHandler(void);               // CAN1 RX0
void CAN1_RX1_IRQHandler(void);               // CAN1 RX1
void CAN1_SCE_IRQHandler(void);               // CAN1 SCE
void EXTI9_5_IRQHandler(void);                // External Line[9:5]s
void TIM1_BRK_TIM9_IRQHandler(void);          // TIM1 Break and TIM9
void TIM1_UP_TIM10_IRQHandler(void);          // TIM1 Update and TIM10
void TIM1_TRG_COM_TIM11_IRQHandler(void);     // TIM1 Trigger and Commutation and TIM11
void TIM1_CC_IRQHandler(void);                // TIM1 Capture Compare
void TIM2_IRQHandler(void);                   // TIM2
void TIM3_IRQHandler(void);                   // TIM3
void TIM4_IRQHandler(void);                   // TIM4
void I2C1_EV_IRQHandler(void);                // I2C1 Event
void I2C1_ER_IRQHandler(void);                // I2C1 Error
void I2C2_EV_IRQHandler(void);                // I2C2 Event
void I2C2_ER_IRQHandler(void);                // I2C2 Error
void SPI1_IRQHandler(void);                   // SPI1
void SPI2_IRQHandler(void);                   // SPI2
void USART1_IRQHandler(void);                 // USART1
void USART2_IRQHandler(void);                 // USART2
void USART3_IRQHandler(void);                 // USART3
void EXTI15_10_IRQHandler(void);              // External Line[15:10]s
void RTC_Alarm_IRQHandler(void);              // RTC Alarm (A and B) through EXTI Line
void OTG_FS_WKUP_IRQHandler(void);            // USB OTG FS Wakeup through EXTI line
void TIM8_BRK_TIM12_IRQHandler(void);         // TIM8 Break and TIM12
void TIM8_UP_TIM13_IRQHandler(void);          // TIM8 Update and TIM13
void TIM8_TRG_COM_TIM14_IRQHandler(void);     // TIM8 Trigger and Commutation and TIM14
void TIM8_CC_IRQHandler(void);                // TIM8 Capture Compare
void DMA1_Stream7_IRQHandler(void);           // DMA1 Stream7
void FSMC_IRQHandler(void);                   // FSMC
void SDIO_IRQHandler(void);                   // SDIO
void TIM5_IRQHandler(void);                   // TIM5
void SPI3_IRQHandler(void);                   // SPI3
void UART4_IRQHandler(void);                  // UART4
void UART5_IRQHandler(void);                  // UART5
void TIM6_DAC_IRQHandler(void);               // TIM6 and DAC1&2 underrun errors
void TIM7_IRQHandler(void);                   // TIM7
void DMA2_Stream0_IRQHandler(void);           // DMA2 Stream 0
void DMA2_Stream1_IRQHandler(void);           // DMA2 Stream 1
void DMA2_Stream2_IRQHandler(void);           // DMA2 Stream 2
void DMA2_Stream3_IRQHandler(void);           // DMA2 Stream 3
void DMA2_Stream4_IRQHandler(void);           // DMA2 Stream 4
void ETH_IRQHandler(void);                    // Ethernet
void ETH_WKUP_IRQHandler(void);               // Ethernet Wakeup through EXTI line
void CAN2_TX_IRQHandler(void);                // CAN2 TX
void CAN2_RX0_IRQHandler(void);               // CAN2 RX0
void CAN2_RX1_IRQHandler(void);               // CAN2 RX1
void CAN2_SCE_IRQHandler(void);               // CAN2 SCE
void OTG_FS_IRQHandler(void);                 // USB OTG FS
void DMA2_Stream5_IRQHandler(void);           // DMA2 Stream 5
void DMA2_Stream6_IRQHandler(void);           // DMA2 Stream 6
void DMA2_Stream7_IRQHandler(void);           // DMA2 Stream 7
void USART6_IRQHandler(void);                 // USART6
void I2C3_EV_IRQHandler(void);                // I2C3 event
void I2C3_ER_IRQHandler(void);                // I2C3 error
void OTG_HS_EP1_OUT_IRQHandler(void);         // USB OTG HS End Point 1 Out
void OTG_HS_EP1_IN_IRQHandler(void);          // USB OTG HS End Point 1 In
void OTG_HS_WKUP_IRQHandler(void);            // USB OTG HS Wakeup through EXTI
void OTG_HS_IRQHandler(void);                 // USB OTG HS
void DCMI_IRQHandler(void);                   // DCMI
void CRYP_IRQHandler(void);                   // CRYP crypto
void HASH_RNG_IRQHandler(void);               // Hash and Rng

#ifdef __cplusplus
}
#endif

#endif /* __STM32F2xx_IT_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
