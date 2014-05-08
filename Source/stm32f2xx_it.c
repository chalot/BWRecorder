/**
  ******************************************************************************
  * @file    stm32fxxx_it.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    22-July-2011
  * @brief   This file includes the interrupt handlers for the application
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

/* Includes ------------------------------------------------------------------*/
//#include "usb_bsp.h"
//#include "usb_hcd_int.h"
//#include "usbh_core.h"
#include "stm32f2xx_it.h"
#include <stm32f2xx_conf.h>
#include <qp_port.h>
#include <key.h>

//#include "ir.h"
//#include "gps.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//CanRxMsg RxMessage;
//extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
//extern USBH_HOST                    USB_Host;
//

/* Private function prototypes -----------------------------------------------*/
extern void USB_OTG_BSP_TimerIRQ (void);

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
/**
  * @brief  NMI_Handler
  *         This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void) __attribute__((__interrupt__));
void NMI_Handler(void)
{
}

/**
  * @brief  HardFault_Handler
  *         This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
//void HardFault_Handler(void)
//{
//  /* Go to infinite loop when Hard Fault exception occurs */
//  while (1)
//  {
//  }
//}

void hard_fault_handler_c()
{

}

/**
  * @brief  MemManage_Handler
  *         This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void) __attribute__((__interrupt__));
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  BusFault_Handler
  *         This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void) __attribute__((__interrupt__));
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  UsageFault_Handler
  *         This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void) __attribute__((__interrupt__));
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  SVC_Handler, qk_port.S
  *         This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  DebugMon_Handler
  *         This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void) __attribute__((__interrupt__));
void DebugMon_Handler(void)
{
}


/**
  * @brief  PendSV_Handler, qk_port.S
  *         This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  EXTI1_IRQHandler
  *         This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void) __attribute__((__interrupt__));
void EXTI1_IRQHandler(void)
{
	QK_ISR_ENTRY();
//  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
//  {
//      USB_Host.usr_cb->OverCurrentDetected();
//      EXTI_ClearITPendingBit(EXTI_Line1);
//  }
	QK_ISR_EXIT();
}


/**
  * @brief  OTG_FS_IRQHandler
  *          This function handles USB-On-The-Go FS global interrupt request.
  *          requests.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_FS
void OTG_FS_IRQHandler(void) __attribute__((__interrupt__));
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
//  USBH_OTG_ISR_Handler(&USB_OTG_Core);
}




/**
  * @brief  SysTick_Handler
  *         This function handles Sys Tick  Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}




/**
  * @brief  WWDG_IRQHandler
  *         This function handles Window WatchDog Handler.
  * @param  None
  * @retval None
  */
void WWDG_IRQHandler(void) __attribute__((__interrupt__));
void  WWDG_IRQHandler(void)
{
}



/**
  * @brief  PVD_IRQHandler
  *         This function handles PVD through EXTI Line detection  Handler.
  * @param  None
  * @retval None
  */
void PVD_IRQHandler(void) __attribute__((__interrupt__));
void  PVD_IRQHandler(void)
{
}



/**
  * @brief  TAMP_STAMP_IRQHandler
  *         This function handles Tamper and TimeStamps through the EXTI line Handler.
  * @param  None
  * @retval None
  */
void TAMP_STAMP_IRQHandler(void) __attribute__((__interrupt__));
void  TAMP_STAMP_IRQHandler(void)
{
}



/**
  * @brief  RTC_WKUP_IRQHandler
  *         This function handles RTC Wakeup through the EXTI line Handler.
  * @param  None
  * @retval None
  */
void RTC_WKUP_IRQHandler(void) __attribute__((__interrupt__));
void  RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	}
}



/**
  * @brief  FLASH_IRQHandler
  *         This function handles FLASH Handler.
  * @param  None
  * @retval None
  */
void FLASH_IRQHandler(void) __attribute__((__interrupt__));
void  FLASH_IRQHandler(void)
{
}



/**
  * @brief  RCC_IRQHandler
  *         This function handles RCC Handler.
  * @param  None
  * @retval None
  */
void RCC_IRQHandler(void) __attribute__((__interrupt__));
void  RCC_IRQHandler(void)
{
}



/**
  * @brief  EXTI0_IRQHandler
  *         This function handles EXTI Line0 Handler.
  * @param  None
  * @retval None
  */
//void EXTI0_IRQHandler(void) __attribute__((__interrupt__));
//void  EXTI0_IRQHandler(void)
//{
//}




/**
  * @brief  EXTI2_IRQHandler
  *         This function handles EXTI Line2 Handler.
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void) __attribute__((__interrupt__));
void  EXTI2_IRQHandler(void)
{
}



/**
  * @brief  EXTI3_IRQHandler
  *         This function handles EXTI Line3 Handler.
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void) __attribute__((__interrupt__));
void  EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}



/**
  * @brief  EXTI4_IRQHandler
  *         This function handles EXTI Line4  Handler.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void) __attribute__((__interrupt__));
void  EXTI4_IRQHandler(void)
{
}



/**
  * @brief  DMA1_Stream0_IRQHandler
  *         This function handles DMA1 Stream 0 Handler.
  * @param  None
  * @retval None
  */
void DMA1_Stream0_IRQHandler(void) __attribute__((__interrupt__));
void  DMA1_Stream0_IRQHandler(void)
{
}



/**
  * @brief  DMA1_Stream1_IRQHandler
  *         This function handles DMA1 Stream 1 Handler.
  * @param  None
  * @retval None
  */
void DMA1_Stream1_IRQHandler(void) __attribute__((__interrupt__));
void  DMA1_Stream1_IRQHandler(void)
{
}



/**
  * @brief  DMA1_Stream2_IRQHandler
  *         This function handles DMA1 Stream 2 Handler.
  * @param  None
  * @retval None
  */
void DMA1_Stream2_IRQHandler(void) __attribute__((__interrupt__));
void  DMA1_Stream2_IRQHandler(void)
{
}



/**
  * @brief  DMA1_Stream3_IRQHandler
  *         This function handles DMA1 Stream 3 Handler.
  * @param  None
  * @retval None
  */
void DMA1_Stream3_IRQHandler(void) __attribute__((__interrupt__));
void  DMA1_Stream3_IRQHandler(void)
{
}



/**
  * @brief  DMA1_Stream4_IRQHandler
  *         This function handles DMA1 Stream 4 Handler.
  * @param  None
  * @retval None
  */
void DMA1_Stream4_IRQHandler(void) __attribute__((__interrupt__));
void  DMA1_Stream4_IRQHandler(void)
{
}



/**
  * @brief  DMA1_Stream5_IRQHandler
  *         This function handles DMA1 Stream 5 Handler.
  * @param  None
  * @retval None
  */
void DMA1_Stream5_IRQHandler(void) __attribute__((__interrupt__));
void  DMA1_Stream5_IRQHandler(void)
{
}



/**
  * @brief  DMA1_Stream6_IRQHandler
  *         This function handles DMA1 Stream 6 Handler.
  * @param  None
  * @retval None
  */
void DMA1_Stream6_IRQHandler(void) __attribute__((__interrupt__));
void  DMA1_Stream6_IRQHandler(void)
{
}



/**
  * @brief  ADC_IRQHandler，按键检测
  *         This function handles DC1, ADC2 and ADC3s  Handler.
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void) __attribute__((__interrupt__));
void  ADC_IRQHandler(void)
{
   static volatile u16 Keyvl;
	/* Enable AWD interupt */
	if(ADC_GetITStatus(ADC3,ADC_IT_AWD))
	{
		ADC_ClearITPendingBit(ADC3,ADC_IT_AWD);

		ISR_ADC_Key();
	}
}


/**
  * @brief  CAN1_TX_IRQHandler
  *         This function handles CAN1 TX  Handler.
  * @param  None
  * @retval None
  */
void CAN1_TX_IRQHandler(void) __attribute__((__interrupt__));
void  CAN1_TX_IRQHandler(void)
{
}



/**
  * @brief  CAN1_RX0_IRQHandler
  *         This function handles CAN1 RX0  Handler.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void) __attribute__((__interrupt__));
void  CAN1_RX0_IRQHandler(void)
{
//	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
}



/**
  * @brief  CAN1_RX1_IRQHandler
  *         This function handles CAN1 RX1 Handler.
  * @param  None
  * @retval None
  */
void CAN1_RX1_IRQHandler(void) __attribute__((__interrupt__));
void  CAN1_RX1_IRQHandler(void)
{
}



/**
  * @brief  CAN1_SCE_IRQHandler
  *         This function handles CAN1 SCE Handler.
  * @param  None
  * @retval None
  */
void CAN1_SCE_IRQHandler(void) __attribute__((__interrupt__));
void  CAN1_SCE_IRQHandler(void)
{
}



/**
  * @brief  EXTI9_5_IRQHandler
  *         This function handles External Line[9:5]s  Handler.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void) __attribute__((__interrupt__));
void  EXTI9_5_IRQHandler(void)
{
}



/**
  * @brief  TIM1_BRK_TIM9_IRQHandler
  *         This function handles TIM1 Break and TIM9 Handler.
  * @param  None
  * @retval None
  */
void TIM1_BRK_TIM9_IRQHandler(void) __attribute__((__interrupt__));
void  TIM1_BRK_TIM9_IRQHandler(void)
{
}



/**
  * @brief  TIM1_UP_TIM10_IRQHandler
  *         This function handles TIM1 Update and TIM10 Handler.
  * @param  None
  * @retval None
  */
void TIM1_UP_TIM10_IRQHandler(void) __attribute__((__interrupt__));
void  TIM1_UP_TIM10_IRQHandler(void)
{
}



/**
  * @brief  TIM1_TRG_COM_TIM11_IRQHandler
  *         This function handles  TIM1 Trigger and Commutation and TIM11 Handler.
  * @param  None
  * @retval None
  */
void TIM1_TRG_COM_TIM11_IRQHandler(void) __attribute__((__interrupt__));
void  TIM1_TRG_COM_TIM11_IRQHandler(void)
{
}




/**
  * @brief  TIM1_CC_IRQHandler
  *         This function handles TIM1 Capture Compare  Handler.
  * @param  None
  * @retval None
  */
void TIM1_CC_IRQHandler(void) __attribute__((__interrupt__));
void  TIM1_CC_IRQHandler(void)
{
//	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
//
//		T1_Voice_Interrupt();
//	}
}




/**
  * @brief  TIM2_IRQHandler
  *         This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void) __attribute__((__interrupt__));
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

//		Isr_IR_Timeout100us();
	}

}


/**
  * @brief  TIM3_IRQHandler
  *         This function handles TIM3 Handler.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void) __attribute__((__interrupt__));
void  TIM3_IRQHandler(void)
{

}



/**
  * @brief  TIM4_IRQHandler
  *         This function handles TIM4 Handler.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void) __attribute__((__interrupt__));
void  TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_FLAG_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);

		//打印机定时处理
//		TP_TIM_Interrupt();
	}
}



/**
  * @brief  I2C1_EV_IRQHandler
  *         This function handles I2C1 Event  Handler.
  * @param  None
  * @retval None
  */
void I2C1_EV_IRQHandler(void) __attribute__((__interrupt__));
void  I2C1_EV_IRQHandler(void)
{
}



/**
  * @brief  I2C1_ER_IRQHandler
  *         This function handles I2C1 Error Handler.
  * @param  None
  * @retval None
  */
void I2C1_ER_IRQHandler(void) __attribute__((__interrupt__));
void  I2C1_ER_IRQHandler(void)
{
}




/**
  * @brief  I2C2_EV_IRQHandler
  *         This function handles I2C2 Event Handler.
  * @param  None
  * @retval None
  */
void I2C2_EV_IRQHandler(void) __attribute__((__interrupt__));
void  I2C2_EV_IRQHandler(void)
{
}



/**
  * @brief  I2C2_ER_IRQHandler
  *         This function handles I2C2 Error  Handler.
  * @param  None
  * @retval None
  */
void I2C2_ER_IRQHandler(void) __attribute__((__interrupt__));
void  I2C2_ER_IRQHandler(void)
{
}



/**
  * @brief  SPI1_IRQHandler
  *         This function handles SPI1 Handler.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void) __attribute__((__interrupt__));
void  SPI1_IRQHandler(void)
{
}



/**
  * @brief  SPI2_IRQHandler
  *         This function handles SPI2 Handler.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void) __attribute__((__interrupt__));
void  SPI2_IRQHandler(void)
{
}



/**
  * @brief  USART1_IRQHandler
  *         This function handles USART1 Handler.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void) __attribute__((__interrupt__));
void  USART1_IRQHandler(void)
{
//	ISR_UART_GPRS();
}



/**
  * @brief  USART2_IRQHandler
  *         This function handles USART2 Handler.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void) __attribute__((__interrupt__));
void  USART2_IRQHandler(void)
{
//	ISR_UART_GPS();
}



/**
  * @brief  USART3_IRQHandler
  *         This function handles USART3 Handler.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void) __attribute__((__interrupt__));
void  USART3_IRQHandler(void)
{
}



/**
  * @brief  EXTI15_10_IRQHandler
  *         This function handles External Line[15:10]s      Handler.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void) __attribute__((__interrupt__));
void  EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
//      ISR_IRReceive();

      EXTI_ClearITPendingBit(EXTI_Line1);
  }


}



/**
  * @brief  RTC_Alarm_IRQHandler
  *         This function handles  RTC Alarm (A and B) through EXTI Line Handler.
  * @param  None
  * @retval None
  */
void RTC_Alarm_IRQHandler(void) __attribute__((__interrupt__));
void  RTC_Alarm_IRQHandler(void)
{
	 if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	 {
	    RTC_ClearITPendingBit(RTC_IT_ALRA);

	    EXTI_ClearITPendingBit(EXTI_Line17);


	 }
}



/**
  * @brief  OTG_FS_WKUP_IRQHandler
  *         This function handles USB OTG FS Wakeup through EXTI line Handler.
  * @param  None
  * @retval None
  */
void OTG_FS_WKUP_IRQHandler(void) __attribute__((__interrupt__));
void  OTG_FS_WKUP_IRQHandler(void)
{
}



/**
  * @brief  TIM8_BRK_TIM12_IRQHandler
  *         This function handles TIM8 Break and TIM12 Handler.
  * @param  None
  * @retval None
  */
void TIM8_BRK_TIM12_IRQHandler(void) __attribute__((__interrupt__));
void  TIM8_BRK_TIM12_IRQHandler(void)
{
}



/**
  * @brief  TIM8_UP_TIM13_IRQHandler
  *         This function handles TIM8 Update and TIM13  Handler.
  * @param  None
  * @retval None
  */
void TIM8_UP_TIM13_IRQHandler(void) __attribute__((__interrupt__));
void  TIM8_UP_TIM13_IRQHandler(void)
{
}



/**
  * @brief  TIM8_TRG_COM_TIM14_IRQHandler
  *         This function handles TIM8 Trigger and Commutation and TIM14 Handler.
  * @param  None
  * @retval None
  */
void TIM8_TRG_COM_TIM14_IRQHandler(void) __attribute__((__interrupt__));
void  TIM8_TRG_COM_TIM14_IRQHandler(void)
{
}



/**
  * @brief  TIM8_CC_IRQHandler
  *         This function handles TIM8 Capture Compare Handler.
  * @param  None
  * @retval None
  */
void TIM8_CC_IRQHandler(void) __attribute__((__interrupt__));
void  TIM8_CC_IRQHandler(void)
{
}



/**
  * @brief  DMA1_Stream7_IRQHandler
  *         This function handles DMA1 Stream7  Handler.
  * @param  None
  * @retval None
  */
void DMA1_Stream7_IRQHandler(void) __attribute__((__interrupt__));
void  DMA1_Stream7_IRQHandler(void)
{
}



/**
  * @brief  FSMC_IRQHandler
  *         This function handles FSMC Handler.
  * @param  None
  * @retval None
  */
void FSMC_IRQHandler(void) __attribute__((__interrupt__));
void  FSMC_IRQHandler(void)
{
}




/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/******************************************************************************/
/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void) __attribute__((__interrupt__));
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
//  SD_ProcessIRQSrc();
}


/**
  * @brief  TIM5_IRQHandler
  *         This function handles TIM5 Handler.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void) __attribute__((__interrupt__));
void  TIM5_IRQHandler(void)
{

  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

//	USB_OTG_BSP_TimerIRQ();
   }
}



/**
  * @brief  SPI3_IRQHandler
  *         This function handles SPI3 Handler.
  * @param  None
  * @retval None
  */
void SPI3_IRQHandler(void) __attribute__((__interrupt__));
void  SPI3_IRQHandler(void)
{
}



/**
  * @brief  UART4_IRQHandler
  *         This function handles UART4 Handler.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void) __attribute__((__interrupt__));
void  UART4_IRQHandler(void)
{
	//摄像头串口中断 UART4
//	ISR_UART_VDO();
}



/**
  * @brief  UART5_IRQHandler
  *         This function handles UART5 Handler.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void) __attribute__((__interrupt__));
void  UART5_IRQHandler(void)
{
}



/**
  * @brief  TIM6_DAC_IRQHandler
  *         This function handles TIM6 and DAC1&2 underrun errors Handler.
  * @param  None
  * @retval None
  */
void TIM6_DAC_IRQHandler(void) __attribute__((__interrupt__));
void  TIM6_DAC_IRQHandler(void)
{
}




/**
  * @brief  TIM7_IRQHandler
  *         This function handles TIM7 Handler.
  * @param  None
  * @retval None
  */
void TIM7_IRQHandler(void) __attribute__((__interrupt__));
void  TIM7_IRQHandler(void)
{
}




/**
  * @brief  DMA2_Stream0_IRQHandler
  *         This function handles void  DMA2_Stream0_IRQHandler(void);           // DMA2 Stream 0
 Handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream0_IRQHandler(void) __attribute__((__interrupt__));
void  DMA2_Stream0_IRQHandler(void)
{
}




/**
  * @brief  DMA2_Stream1_IRQHandler
  *         This function handles DMA2 Stream 1 Handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream1_IRQHandler(void) __attribute__((__interrupt__));
void DMA2_Stream1_IRQHandler(void)
{
}



/**
  * @brief  DMA2_Stream2_IRQHandler
  *         This function handles DMA2 Stream 2 Handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream2_IRQHandler(void) __attribute__((__interrupt__));
void  DMA2_Stream2_IRQHandler(void)
{
}



/**
  * @brief  DMA2_Stream3_IRQHandler
  *         This function handles DMA2 Stream 3 Handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream3_IRQHandler(void) __attribute__((__interrupt__));
void  DMA2_Stream3_IRQHandler(void)
{
}




/**
  * @brief  DMA2_Stream4_IRQHandler
  *         This function handles DMA2 Stream 4 Handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream4_IRQHandler(void) __attribute__((__interrupt__));
void  DMA2_Stream4_IRQHandler(void)
{
}



/**
  * @brief  ETH_IRQHandler
  *         This function handles Ethernet Handler.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void) __attribute__((__interrupt__));
void  ETH_IRQHandler(void)
{
}


/**
  * @brief  ETH_WKUP_IRQHandler
  *         This function handles Ethernet Wakeup through EXTI line  Handler.
  * @param  None
  * @retval None
  */
void ETH_WKUP_IRQHandler(void) __attribute__((__interrupt__));
void  ETH_WKUP_IRQHandler(void)
{
}



/**
  * @brief  CAN2_TX_IRQHandler
  *         This function handles CAN2 TX  Handler.
  * @param  None
  * @retval None
  */
void CAN2_TX_IRQHandler(void) __attribute__((__interrupt__));
void  CAN2_TX_IRQHandler(void)
{

}


/**
  * @brief  CAN2_RX0_IRQHandler
  *         This function handles CAN2 RX0  Handler.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void) __attribute__((__interrupt__));
void  CAN2_RX0_IRQHandler(void)
{
//	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
}

/**
  * @brief  CAN2_RX1_IRQHandler
  *         This function handles CAN2 RX1 Handler.
  * @param  None
  * @retval None
  */
void CAN2_RX1_IRQHandler(void) __attribute__((__interrupt__));
void  CAN2_RX1_IRQHandler(void)
{
}


/**
  * @brief  CAN2_SCE_IRQHandler
  *         This function handles CAN2 SCE  Handler.
  * @param  None
  * @retval None
  */
void CAN2_SCE_IRQHandler(void) __attribute__((__interrupt__));
void  CAN2_SCE_IRQHandler(void)
{
}



/**
  * @brief  DMA2_Stream5_IRQHandler
  *         This function handles DMA2 Stream 5  Handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream5_IRQHandler(void) __attribute__((__interrupt__));
void  DMA2_Stream5_IRQHandler(void)
{
}


/**
  * @brief  DMA2_Stream6_IRQHandler
  *         This function handles DMA2 Stream 6 Handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream6_IRQHandler(void) __attribute__((__interrupt__));
void  DMA2_Stream6_IRQHandler(void)
{
}



/**
  * @brief  DMA2_Stream7_IRQHandler
  *         This function handles DMA2 Stream 7  Handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream7_IRQHandler(void) __attribute__((__interrupt__));
void  DMA2_Stream7_IRQHandler(void)
{
}


/**
  * @brief  USART6_IRQHandler
  *         This function handles USART6 Handler.
  * @param  None
  * @retval None
  */
void USART6_IRQHandler(void) __attribute__((__interrupt__));
void  USART6_IRQHandler(void)
{
}

/**
  * @brief  I2C3_EV_IRQHandler
  *         This function handles I2C3 event Handler.
  * @param  None
  * @retval None
  */
void I2C3_EV_IRQHandler(void) __attribute__((__interrupt__));
void I2C3_EV_IRQHandler(void)
{
}


/**
  * @brief  I2C3_ER_IRQHandler
  *         This function handles I2C3 error Handler.
  * @param  None
  * @retval None
  */
void I2C3_ER_IRQHandler(void) __attribute__((__interrupt__));
void  I2C3_ER_IRQHandler(void)
{
}


/**
  * @brief  OTG_HS_EP1_OUT_IRQHandler
  *         This function handles USB OTG HS End Point 1 Out  Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_OUT_IRQHandler(void) __attribute__((__interrupt__));
void  OTG_HS_EP1_OUT_IRQHandler(void)
{
}
/**
  * @brief  OTG_HS_EP1_IN_IRQHandler
  *         This function handles USB OTG HS End Point 1 In  Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_IN_IRQHandler(void) __attribute__((__interrupt__));
void  OTG_HS_EP1_IN_IRQHandler(void)
{
}
/**
  * @brief  OTG_HS_WKUP_IRQHandler
  *         This function handles USB OTG HS Wakeup through EXTI Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_WKUP_IRQHandler(void) __attribute__((__interrupt__));
void  OTG_HS_WKUP_IRQHandler(void)
{
}

/**
  * @brief  DCMI_IRQHandler
  *         This function handles DCMI Handler.
  * @param  None
  * @retval None
  */
void DCMI_IRQHandler(void) __attribute__((__interrupt__));
void  DCMI_IRQHandler(void)
{
}
/**
  * @brief  CRYP_IRQHandler
  *         This function handles CRYP crypto  Handler.
  * @param  None
  * @retval None
  */
void CRYP_IRQHandler(void) __attribute__((__interrupt__));
void  CRYP_IRQHandler(void)
{
}

/**
  * @brief  HASH_RNG_IRQHandler
  *         This function handles Hash and Rng  Handler.
  * @param  None
  * @retval None
  */
void HASH_RNG_IRQHandler(void) __attribute__((__interrupt__));
void  HASH_RNG_IRQHandler(void)
{
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
