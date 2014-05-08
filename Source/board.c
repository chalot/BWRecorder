/*
 * board.c
 *
 *  Created on: 2014年4月22日
 *      Author: ThinkPad User
 */

#include <stm32f2xx_conf.h>
#include <board.h>
#include <trace.h>
#include <type.h>
#include <acc.h>
#include <audio.h>
#include <beep.h>
#include <Can_Interface.h>
#include <can.h>
#include <digital_in.h>
#include <fram.h>
#include <gprs.h>
#include <gps.h>
#include <iccard.h>
#include <ir.h>
#include <key.h>
#include <lcd.h>
#include <nandflash.h>
#include <printer.h>
#include <sensor.h>
#include <speedpulse.h>
#include <tts.h>
#include <usb.h>
#include <video.h>

/**串口定义*/
static const COM_ConfigDef COM_Config[COMn] = { { .UART = GSM_COM, .COM_CLK =
		GSM_COM_CLK, .COM_TX_PIN = GSM_COM_TX_PIN, .COM_TX_GPIO_PORT =
		GSM_COM_TX_GPIO_PORT, .COM_TX_GPIO_CLK = GSM_COM_TX_GPIO_CLK,
		.COM_TX_SOURCE = GSM_COM_TX_SOURCE, .COM_TX_AF = GSM_COM_TX_AF,
		.COM_RX_PIN = GSM_COM_RX_PIN, .COM_RX_GPIO_PORT = GSM_COM_RX_GPIO_PORT,
		.COM_RX_GPIO_CLK = GSM_COM_RX_GPIO_CLK, .COM_RX_SOURCE =
				GSM_COM_RX_SOURCE, .COM_RX_AF = GSM_COM_RX_AF, .COM_IRQn =
				GSM_COM_IRQn },

{ .UART = GPS_COM, .COM_CLK = GPS_COM_CLK, .COM_TX_PIN = GPS_COM_TX_PIN,
		.COM_TX_GPIO_PORT = GPS_COM_TX_GPIO_PORT, .COM_TX_GPIO_CLK =
				GPS_COM_TX_GPIO_CLK, .COM_TX_SOURCE = GPS_COM_TX_SOURCE,
		.COM_TX_AF = GPS_COM_TX_AF, .COM_RX_PIN = GPS_COM_RX_PIN,
		.COM_RX_GPIO_PORT = GPS_COM_RX_GPIO_PORT, .COM_RX_GPIO_CLK =
				GPS_COM_RX_GPIO_CLK, .COM_RX_SOURCE = GPS_COM_RX_SOURCE,
		.COM_RX_AF = GPS_COM_RX_AF, .COM_IRQn = GPS_COM_IRQn },

{ .UART = VIDEO1_COM, .COM_CLK = VIDEO1_COM_CLK,
		.COM_TX_PIN = VIDEO1_COM_TX_PIN, .COM_TX_GPIO_PORT =
				VIDEO1_COM_TX_GPIO_PORT, .COM_TX_GPIO_CLK =
				VIDEO1_COM_TX_GPIO_CLK, .COM_TX_SOURCE = VIDEO1_COM_TX_SOURCE,
		.COM_TX_AF = VIDEO1_COM_TX_AF, .COM_RX_PIN = VIDEO1_COM_RX_PIN,
		.COM_RX_GPIO_PORT = VIDEO1_COM_RX_GPIO_PORT, .COM_RX_GPIO_CLK =
				VIDEO1_COM_RX_GPIO_CLK, .COM_RX_SOURCE = VIDEO1_COM_RX_SOURCE,
		.COM_RX_AF = VIDEO1_COM_RX_AF, .COM_IRQn = VIDEO1_COM_IRQn },

{ .UART = VIDEO2_COM, .COM_CLK = VIDEO2_COM_CLK,
		.COM_TX_PIN = VIDEO2_COM_TX_PIN, .COM_TX_GPIO_PORT =
				VIDEO2_COM_TX_GPIO_PORT, .COM_TX_GPIO_CLK =
				VIDEO2_COM_TX_GPIO_CLK, .COM_TX_SOURCE = VIDEO2_COM_TX_SOURCE,
		.COM_TX_AF = VIDEO2_COM_TX_AF, .COM_RX_PIN = VIDEO2_COM_RX_PIN,
		.COM_RX_GPIO_PORT = VIDEO2_COM_RX_GPIO_PORT, .COM_RX_GPIO_CLK =
				VIDEO2_COM_RX_GPIO_CLK, .COM_RX_SOURCE = VIDEO2_COM_RX_SOURCE,
		.COM_RX_AF = VIDEO2_COM_RX_AF, .COM_IRQn = VIDEO2_COM_IRQn },

{ .UART = DEBUG_COM, .COM_CLK = DEBUG_COM_CLK, .COM_TX_PIN = DEBUG_COM_TX_PIN,
		.COM_TX_GPIO_PORT = DEBUG_COM_TX_GPIO_PORT, .COM_TX_GPIO_CLK =
				DEBUG_COM_TX_GPIO_CLK, .COM_TX_SOURCE = DEBUG_COM_TX_SOURCE,
		.COM_TX_AF = DEBUG_COM_TX_AF, .COM_RX_PIN = DEBUG_COM_RX_PIN,
		.COM_RX_GPIO_PORT = DEBUG_COM_RX_GPIO_PORT, .COM_RX_GPIO_CLK =
				DEBUG_COM_RX_GPIO_CLK, .COM_RX_SOURCE = DEBUG_COM_RX_SOURCE,
		.COM_RX_AF = DEBUG_COM_RX_AF, .COM_IRQn = DEBUG_COM_IRQn },

};

DMA_InitTypeDef sEEDMA_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
RTC_TimeTypeDef RTC_TimeStruct;		//时间信息
RTC_DateTypeDef RTC_DateStruct;		//日期信息

#define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)
__IO uint16_t u16Pwr_ADC3ConvertedValue = 0; 	///电源AD检测值

/**本地函数声明***************************************************************************/
static void COM_Lowlevel_Init(COM_TypeDef COM,
		USART_InitTypeDef* USART_InitStruct);
static void NVIC_ADC_IRQSet(CONTROL cntl);
static void RTC_Configuration();
static void Relay_Lowlevel_Init();
static void PWR_Lowlevel_Init();
static void PWR_ADC_Lowlevel_Init();
static void WatchDog_Lowlevel_Init();
static void DEBUG_Lowlevel_Init();
static void ADC3_CH7_DMA_Config(void);

/****************************************************************************************/

/**
 * RCC_APBClockEnable
 *
 */
void RCC_APBClockEnable(void) {
	//使能AHB1外围总线上的GPIOA/GPIOB/GPIOC/GPIOD/GPIOE/GPIOF/GPIOG/AFIO时钟
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA 	      //GPIOA
	| RCC_AHB1Periph_GPIOB 		  //GPIOB
			| RCC_AHB1Periph_GPIOC 		  //GPIOC
			| RCC_AHB1Periph_GPIOD 		  //GPIOD
			| RCC_AHB1Periph_GPIOE 		  //GPIOE
			| RCC_AHB1Periph_GPIOF 		  //GPIOF
			| RCC_AHB1Periph_GPIOG,	  //GPIOG
			ENABLE);

	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);  //OTG FS

	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);    //FSMC

	//APB1外部总线时钟
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2	 	   //TIM2
	| RCC_APB1Periph_TIM3	 	   //TIM3
			| RCC_APB1Periph_TIM4	 	   //TIM4
			| RCC_APB1Periph_TIM5	 	   //TIM5
			| RCC_APB1Periph_SPI2		   //SPI2
			| RCC_APB1Periph_USART2 	   //USART2
			| RCC_APB1Periph_USART3		   //USART3
			| RCC_APB1Periph_UART4		   //UART4
			| RCC_APB1Periph_CAN1	  	   //CAN1
			| RCC_APB1Periph_CAN2	  	   //CAN2
	, ENABLE);
	//APB2外部总线时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1	 	   //SPI1
	//		| RCC_APB2Periph_SDIO 	  	   //SDIO
	//		| RCC_APB2Periph_ADC		   //ADC
			| RCC_APB2Periph_ADC3		   //ADC3时钟
			| RCC_APB2Periph_USART6		   //USART6
			| RCC_APB2Periph_USART1	  	   //USART1
			| RCC_APB2Periph_SYSCFG			//SYSCFG
			| RCC_APB2Periph_TIM1		   //TIM1
	//			| RCC_APB2Periph_TIM8		   //TIM8
			, ENABLE);

}

/**
 * NVIC_Configuration 中断向量配置
 *
 */
void NVIC_Configuration(void) {

	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH /*+ 0x4000*/, 0x0);
#endif
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

#if		1
#if		1
	// Enable the TIM1 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the TIM2 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if  1
	// Enable the TIM3 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the TIM4 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
#if		1
	// Enable the TIM5 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
#if	1
	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the RTC Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the RTC Wakeup Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the USART Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//配置CAN RX0接收中断中断向量
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//配置CAN2 RX0接收中断中断向量
	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//配置EXTI中断
	//PD3，EXTI3
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

#endif
#endif
	// Enable the ADC Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * AD中断
 */
void NVIC_ADC_IRQSet(CONTROL cntl) {
	NVIC_InitTypeDef NVIC_InitStructure;
	//Control the ADC Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = cntl;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * GPIO初始化
 */
void GPIO_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	///GSM DTR未初始化

	//3  按键引脚检测引脚
	//在Key_LowLevel_Init()中配置

	//4CAN总线配置
	//在CAN_Configuration()函数中配置

	//5 语音模块和蜂鸣器IO口定义
	////语音模块IO口定义
	// Configure GPIOE_TTS_RST & TTS_RX as output push up
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = TTS_RST_PIN;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TTS_TX_PIN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	// Configure TTS SHUNT as output  function
	GPIO_InitStructure.GPIO_Pin = TTS_SHUNT_PIN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

//	/*蜂鸣器*/
//	GPIO_InitStructure.GPIO_Pin = BUZZER_CTRL_PIN;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	//BUZZER信号初始化时拉低
//	GPIO_ResetBits(GPIOF, BUZZER_CTRL_PIN);

	GPIO_InitStructure.GPIO_Pin = TTS_READY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	//6 OLED屏
	//OLED屏IO口在LED_CtrlLinesConfig中初始化

	//7SD 卡
//	//SDIO IO口在SD_LowLevel_Init()中初始化
//	  /*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
//	  GPIO_InitStructure.GPIO_Pin = SD_DETECT_PIN;
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	  GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStructure);

	//8打印机
	//PRT IO口中Prt_LowLevel_Init()中初始化

	//9 USB HOST IO口中USB_Init()中初始化
}

//外部中断配置
/*******************************************************************************
 * Function Name  : EXTI_Configuration
 * Description    : Configures EXTI Line9.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void EXTI_Configuration(void) {
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

#if		0
	//配置RTC ALARM中断，唤醒系统
	/* EXTI configuration *******************************************************/
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
#endif

	//配置ACC中断，唤醒系统
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);

	/* Configure EXTI Line3 to generate an event or an interrupt on falling edge */
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
 *
 */
void ADC_Configuration(void) {

}

//检查和配置系统时钟
void RTC_CheckAndConfig(void) {
	static u8 u8TimeValid = 0;

#define	RTC_BACKUP_VALUE	0x5A5A	//定义RTC后备寄存器
	//系统时钟没有配置
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_BACKUP_VALUE) {
		//系统时间未配置
		u8TimeValid = 0;

		//配置系统时钟
		RTC_Configuration();
	} else {
		//系统时间已经配置
		u8TimeValid = 1;

		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	}
}

/**
 * RTC配置
 *
 */
void RTC_Configuration(void) {
	RTC_InitTypeDef RTC_InitStructure;
	u8 SynchPrediv = 0xFF;
	u8 AsynchPrediv = 0x7F;
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
	/* The RTC Clock may varies due to LSI frequency dispersion. */
	/* Enable the LSI OSC */
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	SynchPrediv = 0xFF;
	AsynchPrediv = 0x7F;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
	/* Enable the LSE OSC */
	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
	}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	SynchPrediv = 0xFF;
	AsynchPrediv = 0x7F;

#else
#error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */

	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro(); /* Enable the PWR clock */

	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv = 0xFF; /* (32KHz / 128) - 1 = 0xFF*/
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);

	RTC_WakeUpCmd(DISABLE);

	/* Configure the RTC WakeUp Clock source: CK_SPRE (1Hz) */
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	RTC_SetWakeUpCounter(0);

	/* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
	EXTI_ClearITPendingBit(EXTI_Line22);

	/* Clear the RTC wakeup Flag */
	RTC_ClearFlag(RTC_FLAG_WUTF);

	/* Enable the RTC Wakeup Interrupt */
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	/* Enable Wakeup Counter */
	RTC_WakeUpCmd(ENABLE);

}

#if 0
/************************************************************************************************************
 *功能描述：100uS定时器设置
 *入	   口 ：	无
 *出    口：	无
 ************************************************************************************************************/
void TIM_Ir_Configuration()
{
	/*
	 TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef		 	TIM_OCInitStructure;

	 //Time base configuration
	 TIM_TimeBaseStructure.TIM_Period = 75;
	 TIM_TimeBaseStructure.TIM_Prescaler = 79;
	 TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_TimeBaseInit(TIMIr, &TIM_TimeBaseStructure);

	 TIM_ClearFlag(TIMIr,TIM_FLAG_Update);

	 //TIM2 IT enable
	 TIM_ITConfig(TIMIr, TIM_IT_Update , ENABLE);

	 //  TIM_Cmd(TIMIr,ENABLE);
	 *
	 */
}

/************************************************************************************************************
 *功能描述：10MS定时器设置
 *入	   口 ：	无
 *出    口：	无
 ************************************************************************************************************/
void TIM10ms_Configuration()
{

//	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef		 	TIM_OCInitStructure;
//
//
//#ifdef	0
//	TIM_TimeBaseStructure.TIM_Period = 10;
//	TIM_TimeBaseStructure.TIM_Prescaler = 79;
//#else
//	//Time base configuration
//	TIM_TimeBaseStructure.TIM_Period = 75;
//
//	TIM_TimeBaseStructure.TIM_Prescaler = 7999;
//#endif
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//
//	TIM_TimeBaseInit(TIM10ms, &TIM_TimeBaseStructure);
//
//	TIM_ClearFlag(TIM10ms,TIM_FLAG_Update);
//
//	//TIM IT enable
//	TIM_ITConfig(TIM10ms, TIM_IT_Update , ENABLE);
//
//	TIM_Cmd(TIM10ms,ENABLE);

}

//TIM4定时器配置
void TIM_Prt_Configuration(u16 u16Period)
{

//	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef		 	TIM_OCInitStructure;
//
//	TIM_Cmd(TIMPrt,DISABLE);
//	//Time base configuration
//	TIM_TimeBaseStructure.TIM_Period = u16Period;             // 20000
//	TIM_TimeBaseStructure.TIM_Prescaler = 0xa0;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//
//	TIM_TimeBaseInit(TIMPrt, &TIM_TimeBaseStructure);
//
//	TIM_ClearFlag(TIMPrt,TIM_FLAG_Update);
//	//TIM IT enable
//	TIM_ITConfig(TIMPrt, TIM_IT_Update , ENABLE);
//
//	//  TIM_Cmd(TIMPrt,ENABLE);
}
#endif

/**
 * 独立看门狗配置
 *
 */
void IWDG_Configuration(void) {
#if 0
	/* IWDG timeout equal to 350ms (the timeout may varies due to LSI frequency
	 dispersion) -------------------------------------------------------------*/
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* IWDG counter clock: 32KHz(LSI) / 32 = 1KHz */
	IWDG_SetPrescaler(IWDG_Prescaler_32);

	/* Set counter reload value to 349 */
	IWDG_SetReload(349);

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
#endif
}

/**
 * 窗口看门狗配置
 *
 */
void WWDG_Configuration(void) {
#if		0
	WWDG_DeInit();

	/* WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  */
	WWDG_SetPrescaler(WWDG_Prescaler_8);

	/* Set Window value to 0x41 */
	WWDG_SetWindowValue(0x41);

	/* Enable WWDG and set counter value to 0x7F, WWDG timeout = ~4 ms * 64 = 262 ms */
	WWDG_Enable(0x7F);

	/* Clear EWI flag */
	WWDG_ClearFlag();

	/* Enable EW interrupt */
	// WWDG_EnableIT();
#endif
}

/**
 * SysTick配置
 *
 */
void SysTick_Configuration(void) {
	/* Select HCLK/8 as SysTick clock source */
	SysTick_Config(72000);
}

/**
 * 中断使能
 *
 */
void INT_Enable(void) {
#if	0
#ifdef _IWDG_ENABLE_
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
#endif

#ifdef _WWDG_ENABLE_
	/* Enable EW interrupt */
	WWDG_EnableIT();
#endif
#endif
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);

	TIM_Cmd(TIM3, ENABLE);
}

/**
 * 串口初始化
 *
 */
void USART_Configuration(COM_TypeDef COM, u16 u16Baund) {
	USART_InitTypeDef USART_InitStructure;
//	USART_ClockInitTypeDef USART_ClockInitStruct;

	//GSM串口初始化
	USART_DeInit(COM_Config[COM].UART);
	USART_InitStructure.USART_BaudRate = u16Baund;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	COM_Lowlevel_Init(COM, &USART_InitStructure);
}

/**
 * 串口底层初始化
 *
 */
void COM_Lowlevel_Init(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct) {
	GPIO_InitTypeDef GPIO_InitStructure;
	COM_ConfigDef *pCOM;

	pCOM = &COM_Config[COM];

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(pCOM->COM_TX_GPIO_CLK | pCOM->COM_RX_GPIO_CLK,
			ENABLE);

	/*Enable UART clock*/
	if (COM == COM_GPRS || COM == COM_DEBUG) {
		/* Enable UART clock */
		RCC_APB2PeriphClockCmd(pCOM->COM_CLK, ENABLE);
	} else {
		RCC_APB1PeriphClockCmd(pCOM->COM_CLK, ENABLE);
	}

	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(pCOM->COM_TX_GPIO_PORT, pCOM->COM_TX_SOURCE,
			pCOM->COM_TX_AF);

	/* Connect PXx to USARTx_Rx*/
	GPIO_PinAFConfig(pCOM->COM_RX_GPIO_PORT, pCOM->COM_RX_SOURCE,
			pCOM->COM_RX_AF);

	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = pCOM->COM_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(pCOM->COM_TX_GPIO_PORT, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = pCOM->COM_RX_PIN;
	GPIO_Init(pCOM->COM_RX_GPIO_PORT, &GPIO_InitStructure);

	/* USART configuration */
	USART_Init(pCOM->UART, USART_InitStruct);

	/* Enable USART */
	USART_Cmd(pCOM->UART, ENABLE);
}

/**
 * 外设初始化
 *
 */
void Periph_Lowlevel_Config() {
	Relay_Lowlevel_Init();
	PWR_Lowlevel_Init();
	PWR_ADC_Lowlevel_Init();
	WatchDog_Lowlevel_Init();
	DEBUG_Lowlevel_Init();

	Digital_In_Lowlevel_Init();
	Key_LowLevel_Init();
	IC_Lowlevel_Init();
	TTS_Lowlevel_Init();
	GPRS_Lowlevel_Init();
	GPS_Lowlevel_Init();
	VIDEO_Lowlevel_Init();
	I2C2_Lowlevel_Init();	///铁电、3D传感器共用I2C2
	FRAM_Lowlevel_Init();	///铁电
	Sensor3D_Lowlevel_Init();
	LCD_Lowlevel_Init();
	AUDIO_Lowlevel_Init();
	USB_Lowlevel_Init();
	Print_Lowlevel_Init();
	IR_Lowlevel_Init(); ///红外
	CAN_Lowlevel_Init();
	BEEP_Lowlevel_Init();
	Speed_Pulse_Lowlevel_Init();
	NFLASH_LowLevel_Init();
}

/**
 * 继电器IO初始化
 */
void Relay_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = RELAY_1_PIN | RELAY_2_PIN;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * 电源控制IO初始化
 */
void PWR_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = V3P3_CTRL_PIN | V5_CTRL_PIN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
 * 外部看门狗初始化
 */
void WatchDog_Lowlevel_Init() {

}

/**
 * 调试输出串口初始化
 */
void DEBUG_Lowlevel_Init() {

}

/**
 * I2C2初始化
 */
void I2C2_Lowlevel_Init() {

}

/**
 * SPI1初始化
 */
void SPI1_Lowlevel_Init() {

}

/**
 * 电源检测,F9
 */
void PWR_ADC_Lowlevel_Init() {

	/* ADC3 configuration *******************************************************/
	/*  - Enable peripheral clocks                                              */
	/*  - DMA2_Stream0 channel2 configuration                                   */
	/*  - Configure ADC Channel7 pin as analog input                            */
	/*  - Configure ADC3 Channel7                                               */
	ADC3_CH7_DMA_Config();

	/* Start ADC3 Software Conversion */
	ADC_SoftwareStartConv(ADC3);
}

/**
 * ADC3、DMA配置,PF9
 * ADC3 channel07 with DMA configuration
 *
 */
void ADC3_CH7_DMA_Config(void) {
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* DMA2 Stream0 channel2 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr =
			(uint32_t) &u16Pwr_ADC3ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* Configure ADC3 Channel7 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = PWR_DET_PIN; //GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC3 regular channel7 configuration *************************************/
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_3Cycles);

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);

	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);
}

