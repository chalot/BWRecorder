/*
 * board.h
 *
 *  Created on: 2014年4月22日
 *      Author: ThinkPad User
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <stm32f2xx_conf.h>

/**PE0~PE3,IC卡*/
#define IC_SW_PIN     				GPIO_Pin_0
#define IC_SW_GPIO_PORT             GPIOE
#define IC_SW_GPIO_CLK              RCC_AHB1Periph_GPIOE

#define IC_RST_PIN     				GPIO_Pin_1
#define IC_RST_GPIO_PORT            GPIOE
#define IC_RST_GPIO_CLK             RCC_AHB1Periph_GPIOE

#define IC_SCK_PIN     				GPIO_Pin_2
#define IC_SCK_GPIO_PORT            GPIOE
#define IC_SCK_GPIO_CLK             RCC_AHB1Periph_GPIOE

#define IC_DATA_PIN     			GPIO_Pin_3
#define IC_DATA_GPIO_PORT           GPIOE
#define IC_DATA_GPIO_CLK            RCC_AHB1Periph_GPIOE

/**PHONE SHUNT*/
#define PHONE_SHUNT_PIN     		GPIO_Pin_4
#define PHONE_SHUNT_GPIO_PORT       GPIOE
#define PHONE_SHUNT_GPIO_CLK        RCC_AHB1Periph_GPIOE

/**TTS SHUNT*/
#define TTS_SHUNT_PIN     			GPIO_Pin_5
#define TTS_SHUNT_GPIO_PORT       	GPIOE
#define TTS_SHUNT_GPIO_CLK        	RCC_AHB1Periph_GPIOE

#define GSM_PWR_PIN     			GPIO_Pin_6
#define GSM_PWR_GPIO_PORT       	GPIOE
#define GSM_PWR_GPIO_CLK       	 	RCC_AHB1Periph_GPIOE

#define V5_CTRL_PIN     			GPIO_Pin_11
#define V5_CTRL_GPIO_PORT       	GPIOE
#define V5_CTRL_GPIO_CLK        	RCC_AHB1Periph_GPIOE

#define V3P3_CTRL_PIN     			GPIO_Pin_12
#define V3P3_CTRL_GPIO_PORT       	GPIOE
#define V3P3_CTRL_GPIO_CLK        	RCC_AHB1Periph_GPIOE

#define VIDEO_CTRL_PIN     			GPIO_Pin_13
#define VIDEO_CTRL_GPIO_PORT       	GPIOE
#define VIDEO_CTRL_GPIO_CLK        	RCC_AHB1Periph_GPIOE

//#define PRINT_CTRL_PIN     			GPIO_Pin_14
//#define PRINT_CTRL_GPIO_PORT       	GPIOE
//#define PRINT_CTRL_GPIO_CLK        	RCC_AHB1Periph_GPIOE

#define CHARGE_CTRL_PIN     		GPIO_Pin_15
#define CHARGE_CTRL_GPIO_PORT       GPIOE
#define CHARGE_CTRL_GPIO_CLK        RCC_AHB1Periph_GPIOE

/**PF*/
/*I2C2，铁电/3D加速度传感器共用*/
#define I2C_SDA_PIN     			GPIO_Pin_0
#define I2C_SDA_GPIO_PORT       	GPIOF
#define I2C_SDA_GPIO_CLK        	RCC_AHB1Periph_GPIOF

#define I2C_SCL_PIN     			GPIO_Pin_1
#define I2C_SCL_GPIO_PORT      	 	GPIOF
#define I2C_SCL_GPIO_CLK        	RCC_AHB1Periph_GPIOF

#define PRINT_STROBE0_PIN     		GPIO_Pin_2
#define PRINT_STROBE0_GPIO_PORT     GPIOF
#define PRINT_STROBE0_GPIO_CLK      RCC_AHB1Periph_GPIOF

#define PRINT_STROBE1_PIN     		GPIO_Pin_3
#define PRINT_STROBE1_GPIO_PORT     GPIOF
#define PRINT_STROBE1_GPIO_CLK      RCC_AHB1Periph_GPIOF

#define PRINT_AIN_1_PIN     		GPIO_Pin_4
#define PRINT_AIN_1_GPIO_PORT     	GPIOF
#define PRINT_AIN_1_GPIO_CLK      	RCC_AHB1Periph_GPIOF

#define PRINT_AIN_2_PIN     		GPIO_Pin_5
#define PRINT_AIN_2_GPIO_PORT     	GPIOF
#define PRINT_AIN_2_GPIO_CLK      	RCC_AHB1Periph_GPIOF

#define PRINT_BIN_1_PIN     		GPIO_Pin_6
#define PRINT_BIN_1_GPIO_PORT   	GPIOF
#define PRINT_BIN_1_GPIO_CLK     	RCC_AHB1Periph_GPIOF

#define PRINT_BIN_2_PIN     		GPIO_Pin_7
#define PRINT_BIN_2_GPIO_PORT     	GPIOF
#define PRINT_BIN_2_GPIO_CLK      	RCC_AHB1Periph_GPIOF

#define KEY_DET_PIN     			GPIO_Pin_8	///ADC3_IN6
#define KEY_DET_GPIO_PORT     		GPIOF
#define KEY_DET_GPIO_CLK      		RCC_AHB1Periph_GPIOF

#define PWR_DET_PIN     			GPIO_Pin_9
#define PWR_DET_GPIO_PORT     		GPIOF
#define PWR_DET_GPIO_CLK      		RCC_AHB1Periph_GPIOF

#define SD_DET_PIN     				GPIO_Pin_10
#define SD_DET_GPIO_PORT     		GPIOF
#define SD_DET_GPIO_CLK      		RCC_AHB1Periph_GPIOF

#define SENSOR_INT_PIN     			GPIO_Pin_11
#define SENSOR_INT_GPIO_PORT     	GPIOF
#define SENSOR_INT_GPIO_CLK      	RCC_AHB1Periph_GPIOF

#define LCD_SDA_PIN     			GPIO_Pin_12
#define LCD_SDA_GPIO_PORT     		GPIOF
#define LCD_SDA_GPIO_CLK      		RCC_AHB1Periph_GPIOF

#define LCD_CLK_PIN     			GPIO_Pin_13
#define LCD_CLK_GPIO_PORT     		GPIOF
#define LCD_CLK_GPIO_CLK      		RCC_AHB1Periph_GPIOF

#define KEY_LED_PIN     			GPIO_Pin_14
#define KEY_LED_GPIO_PORT     		GPIOF
#define KEY_LED_GPIO_CLK      		RCC_AHB1Periph_GPIOF


#define TTS_RST_PIN     			GPIO_Pin_15
#define TTS_RST_GPIO_PORT     		GPIOF
#define TTS_RST_GPIO_CLK      		RCC_AHB1Periph_GPIOF

/**VIDEO2串口,UART4*/
//#define VIDEO2_UART4_TX_PIN     	GPIO_Pin_0
//#define VIDEO2_UART4_TX_GPIO_PORT   GPIOA
//#define VIDEO2_UART4_TX_GPIO_CLK    RCC_AHB1Periph_GPIOA
//
//#define VIDEO2_UART4_RX_PIN     	GPIO_Pin_1
//#define VIDEO2_UART4_RX_GPIO_PORT   GPIOA
//#define VIDEO2_UART4_RX_GPIO_CLK    RCC_AHB1Periph_GPIOA

///**GPS串口,UART2*/
//#define GPS_UART2_TX_PIN     		GPIO_Pin_2
//#define GPS_UART2_TX_GPIO_PORT    	GPIOA
//#define GPS_UART2_TX_GPIO_CLK     	RCC_AHB1Periph_GPIOA
//
//#define GPS_UART2_RX_PIN     		GPIO_Pin_3
//#define GPS_UART2_RX_GPIO_PORT    	GPIOA
//#define GPS_UART2_RX_GPIO_CLK     	RCC_AHB1Periph_GPIOA

/*音频芯片*/
#define AUDIO_SPI1_CS_PIN     		GPIO_Pin_4
#define AUDIO_SPI1_GPIO_PORT    	GPIOA
#define AUDIO_SPI1_GPIO_CLK     	RCC_AHB1Periph_GPIOA

#define AUDIO_SPI1_SCK_PIN     		GPIO_Pin_5
#define AUDIO_SPI1_SCK_PORT    		GPIOA
#define AUDIO_SPI1_SCK_CLK     		RCC_AHB1Periph_GPIOA

#define AUDIO_SPI1_MISO_PIN     	GPIO_Pin_6
#define AUDIO_SPI1_MISO_PORT    	GPIOA
#define AUDIO_SPI1_MISO_CLK     	RCC_AHB1Periph_GPIOA

#define AUDIO_SPI1_MOSI_PIN     	GPIO_Pin_7
#define AUDIO_SPI1_MOSI_PORT    	GPIOA
#define AUDIO_SPI1_MOSI_CLK     	RCC_AHB1Periph_GPIOA

/**小灯*/
#define CAR_SMALLLIGHT_PIN     		GPIO_Pin_8
#define CAR_SMALLLIGHT_PORT    		GPIOA
#define CAR_SMALLLIGHT_CLK     		RCC_AHB1Periph_GPIOA

/*继电器1*/
#define RELAY_1_PIN     			GPIO_Pin_9
#define RELAY_1_PORT    			GPIOA
#define RELAY_1_CLK     			RCC_AHB1Periph_GPIOA

/*继电器2*/
#define RELAY_2_PIN     			GPIO_Pin_10
#define RELAY_2_PORT    			GPIOA
#define RELAY_2_CLK     			RCC_AHB1Periph_GPIOA

#define USB_IDPLUS_PIN     			GPIO_Pin_11
#define USB_IDPLUS_PORT    			GPIOA
#define USB_IDPLUS_CLK     			RCC_AHB1Periph_GPIOA

#define USB_IDNEG_PIN     			GPIO_Pin_12
#define USB_IDNEG_PORT    			GPIOA
#define USB_IDNEG_CLK     			RCC_AHB1Periph_GPIOA

/*PC*/
/*打印机,SPI2*/
#define PRINT_TM_PIN     			GPIO_Pin_0
#define PRINT_TM_PORT    			GPIOC
#define PRINT_TM_CLK     			RCC_AHB1Periph_GPIOC

#define PRINT_ENABLE_PIN     		GPIO_Pin_1
#define PRINT_ENABLE_PORT    		GPIOC
#define PRINT_ENABLE_CLK     		RCC_AHB1Periph_GPIOC

#define PRINT_SCLK_PIN     			GPIO_Pin_2
#define PRINT_SCLK_PORT    			GPIOC
#define PRINT_SCLK_CLK     			RCC_AHB1Periph_GPIOC

#define PRINT_MOSI_PIN     			GPIO_Pin_3
#define PRINT_MOSI_PORT    			GPIOC
#define PRINT_MOSI_CLK     			RCC_AHB1Periph_GPIOC

#define PRINT_CTRL_PIN     			GPIO_Pin_4
#define PRINT_CTRL_PORT    			GPIOC
#define PRINT_CTRL_CLK     			RCC_AHB1Periph_GPIOC

#define PRINT_LATCH_PIN     		GPIO_Pin_5
#define PRINT_LATCH_PORT    		GPIOC
#define PRINT_LATCH_CLK     		RCC_AHB1Periph_GPIOC

/*近光灯*/
#define LAMP_FAR_PIN     			GPIO_Pin_6
#define LAMP_FAR_PORT    			GPIOC
#define LAMP_FAR_CLK     			RCC_AHB1Periph_GPIOC

/*远光灯*/
#define LAMP_NEAR_PIN     			GPIO_Pin_7
#define LAMP_NEAR_PORT    			GPIOC
#define LAMP_NEAR_CLK     			RCC_AHB1Periph_GPIOC

/*红外输入*/
#define IR_INPUT_PIN     			GPIO_Pin_13
#define IR_INPUT_PORT    			GPIOC
#define IR_INPUT_CLK     			RCC_AHB1Periph_GPIOC

/*PB*/
#define AUDIO_XDCS_PIN     			GPIO_Pin_0
#define AUDIO_XDCS_PORT    			GPIOB
#define AUDIO_XDCS_CLK     			RCC_AHB1Periph_GPIOB

#define AUDIO_DREQ_PIN     			GPIO_Pin_1
#define AUDIO_DREQ_PORT    			GPIOB
#define AUDIO_DREQ_CLK     			RCC_AHB1Periph_GPIOB

#define AUDIO_RST_PIN     			GPIO_Pin_2
#define AUDIO_RST_PORT    			GPIOB
#define AUDIO_RST_CLK     			RCC_AHB1Periph_GPIOB

#define CAN1_ON_PIN     			GPIO_Pin_5
#define CAN1_ON_PORT    			GPIOB
#define CAN1_ON_CLK     			RCC_AHB1Periph_GPIOB

//#define GSM_UART1_TX_PIN     		GPIO_Pin_6
//#define GSM_UART1_TX_PORT    		GPIOB
//#define GSM_UART1_TX_CLK     		RCC_AHB1Periph_GPIOB
//
//#define GSM_UART1_RX_PIN     		GPIO_Pin_7
//#define GSM_UART1_RX_PORT    		GPIOB
//#define GSM_UART1_RX_CLK     		RCC_AHB1Periph_GPIOB

#define CAN1_RX_PIN     			GPIO_Pin_8
#define CAN1_RX_PORT    			GPIOB
#define CAN1_RX_CLK     			RCC_AHB1Periph_GPIOB

#define CAN1_TX_PIN     			GPIO_Pin_9
#define CAN1_TX_PORT    			GPIOB
#define CAN1_TX_CLK     			RCC_AHB1Periph_GPIOB

#define ALARM_DET_PIN     			GPIO_Pin_10
#define ALARM_DET_PORT    			GPIOB
#define ALARM_DET_CLK     			RCC_AHB1Periph_GPIOB

#define ACC_DET_PIN     			GPIO_Pin_11
#define ACC_DET_PORT    			GPIOB
#define ACC_DET_CLK     			RCC_AHB1Periph_GPIOB

#define CAN2_RX_PIN     			GPIO_Pin_12
#define CAN2_RX_PORT    			GPIOB
#define CAN2_RX_CLK     			RCC_AHB1Periph_GPIOB

#define CAN2_TX_PIN     			GPIO_Pin_13
#define CAN2_TX_PORT    			GPIOB
#define CAN2_TX_CLK     			RCC_AHB1Periph_GPIOB

#define CAN2_ON_PIN     			GPIO_Pin_14
#define CAN2_ON_PORT    			GPIOB
#define CAN2_ON_CLK     			RCC_AHB1Periph_GPIOB

#define BUZZER_CTRL_PIN     		GPIO_Pin_15
#define BUZZER_CTRL_PORT    		GPIOB
#define BUZZER_CTRL_CLK     		RCC_AHB1Periph_GPIOB

#define TTS_READY_PIN     			GPIO_Pin_0
#define TTS_READY_PORT    			GPIOG
#define TTS_READY_CLK     			RCC_AHB1Periph_GPIOG

#define TTS_TX_PIN     				GPIO_Pin_1
#define TTS_TX_PORT    				GPIOG
#define TTS_TX_CLK     				RCC_AHB1Periph_GPIOG

#define SPEED_PULSE_OUT_PIN     	GPIO_Pin_2
#define SPEED_PULSE_OUT_PORT    	GPIOG
#define SPEED_PULSE_OUT_CLK     	RCC_AHB1Periph_GPIOG

#define SPEED_PULSE_IN_PIN     		GPIO_Pin_3
#define SPEED_PULSE_IN_PORT    		GPIOG
#define SPEED_PULSE_IN_CLK     		RCC_AHB1Periph_GPIOG

#define DIGIT_IN_BRAKE_PIN     		GPIO_Pin_4
#define DIGIT_IN_BRAKE_PORT    		GPIOG
#define DIGIT_IN_BRAKE_CLK     		RCC_AHB1Periph_GPIOG

#define DIGIT_IN_RT_TURN_PIN     	GPIO_Pin_5
#define DIGIT_IN_RT_TURN_PORT    	GPIOG
#define DIGIT_IN_RT_TURN_CLK     	RCC_AHB1Periph_GPIOG

#define DIGIT_IN_LT_TURN_PIN     	GPIO_Pin_6
#define DIGIT_IN_LT_TURN_PORT    	GPIOG
#define DIGIT_IN_LT_TURN_CLK     	RCC_AHB1Periph_GPIOG

#define DIGIT_IN_DOOR_PIN     		GPIO_Pin_7
#define DIGIT_IN_DOOR_PORT    		GPIOG
#define DIGIT_IN_DOOR_CLK     		RCC_AHB1Periph_GPIOG

#define DIGIT_IN_HANDBREAKE_PIN     GPIO_Pin_8
#define DIGIT_IN_HANDBREAKE_PORT    GPIOG
#define DIGIT_IN_HANDBREAKE_CLK     RCC_AHB1Periph_GPIOG

//#define DEBUG_UART6_RX_PIN     		GPIO_Pin_9
//#define DEBUG_UART6_RX_PORT    		GPIOG
//#define DEBUG_UART6_RX_CLK     		RCC_AHB1Periph_GPIOG
//
//#define DEBUG_UART6_PIN     		GPIO_Pin_10
//#define DEBUG_UART6_RX_PORT    		GPIOG
//#define DEBUG_UART6_RX_CLK     		RCC_AHB1Periph_GPIOG

#define GSM_DTR_PIN     			GPIO_Pin_12
#define GSM_DTR_PORT    			GPIOG
#define GSM_DTR_CLK     			RCC_AHB1Periph_GPIOG

#define GSM_PWKEY_PIN     			GPIO_Pin_13
#define GSM_PWKEY_PORT    			GPIOG
#define GSM_PWKEY_CLK     			RCC_AHB1Periph_GPIOG

#define DEBUG_UART6_TX_PIN     		GPIO_Pin_14
#define DEBUG_UART6_TX_PORT    		GPIOG
#define DEBUG_UART6_TX_CLK     		RCC_AHB1Periph_GPIOG

#define LCD_RST_PIN     			GPIO_Pin_15
#define LCD_RST_PORT    			GPIOG
#define LCD_RST_CLK     			RCC_AHB1Periph_GPIOG

#define SDIO_CMD_PIN     			GPIO_Pin_2
#define SDIO_CMD_PORT    			GPIOD
#define SDIO_CMD_CLK     			RCC_AHB1Periph_GPIOD

#define WATCHDOG_PIN     			GPIO_Pin_3
#define WATCHDOG_PORT    			GPIOD
#define WATCHDOG_CLK     			RCC_AHB1Periph_GPIOD

#define FLASH_RB_PIN     			GPIO_Pin_6
#define FLASH_RB_PORT    			GPIOD
#define FLASH_RB_CLK     			RCC_AHB1Periph_GPIOD

#define FLASH_CS_PIN     			GPIO_Pin_7
#define FLASH_CS_PORT    			GPIOD
#define FLASH_CS_CLK     			RCC_AHB1Periph_GPIOD

//#define VIDEO1_UART3_TX_PIN     	GPIO_Pin_8
//#define VIDEO1_UART3_TX_PORT    	GPIOD
//#define VIDEO1_UART3_TX_CLK     	RCC_AHB1Periph_GPIOD
//
//#define VIDEO1_UART3_RX_PIN     	GPIO_Pin_9
//#define VIDEO1_UART3_RX_PORT    	GPIOD
//#define VIDEO1_UART3_RX_CLK     	RCC_AHB1Periph_GPIOD

#define GPS_RST_PIN     			GPIO_Pin_10
#define GPS_RST_PORT    			GPIOD
#define GPS_RST_CLK     			RCC_AHB1Periph_GPIOD

#define GPS_ANT_DET_PIN     		GPIO_Pin_13
#define GPS_ANT_DET_PORT    		GPIOD
#define GPS_ANT_DET_CLK     		RCC_AHB1Periph_GPIOD


/*显示屏*/
#define LCD_A0_GPIOG				GPIO_Pin_11
#define LCD_RST_GPIOG				GPIO_Pin_15
#define LCD_CS_GPIOG				GPIO_Pin_10


/*NANDFLASH模块IO口定义*/
#define		GPIOD_NF_RB						GPIO_Pin_6
#define		GPIOD_NF_CS						GPIO_Pin_7
//FSMC复用信号,D0~D7
#define		GPIOD_FSMC_D0					GPIO_Pin_14
#define		GPIOD_FSMC_D1					GPIO_Pin_15
#define		GPIOD_FSMC_D2					GPIO_Pin_0
#define		GPIOD_FSMC_D3					GPIO_Pin_1
#define		GPIOE_FSMC_D4					GPIO_Pin_7
#define		GPIOE_FSMC_D5					GPIO_Pin_8
#define		GPIOE_FSMC_D6					GPIO_Pin_9
#define		GPIOE_FSMC_D7					GPIO_Pin_10
//FSMC读写信号
#define		GPIOD_FSMC_RD					GPIO_Pin_4
#define		GPIOD_FSMC_WR					GPIO_Pin_5
//FSMC地址锁存命令锁存
#define		GPIOD_FSMC_CLE					GPIO_Pin_11
#define		GPIOD_FSMC_ALE					GPIO_Pin_12


#define COMn	5	///串口总数

typedef enum
{
  COM_GPRS = 0,
  COM_GPS = 1,
  COM_VIDEO1,
  COM_VIDEO2,
  COM_DEBUG,

} COM_TypeDef;

typedef struct
{
	USART_TypeDef* UART;				//    USART1
	u32 COM_CLK;					//    RCC_APB2Periph_USART1
	u16 COM_TX_PIN;					//                 GPIO_Pin_6
	GPIO_TypeDef* COM_TX_GPIO_PORT;	//           GPIOB
	u32 COM_TX_GPIO_CLK;			//            RCC_AHB1Periph_GPIOB
	u8 COM_TX_SOURCE;				//              GPIO_PinSource6
	u8 COM_TX_AF;					//                  GPIO_AF_USART1
	u16 COM_RX_PIN;					//                 GPIO_Pin_7
	GPIO_TypeDef* COM_RX_GPIO_PORT;//           GPIOB
	u32 COM_RX_GPIO_CLK;			//            RCC_AHB1Periph_GPIOB
	u8 COM_RX_SOURCE;				//              GPIO_PinSource7
	u8 COM_RX_AF;					//                  GPIO_AF_USART1
	u8 COM_IRQn;					//                   USART1_IRQn

} COM_ConfigDef;


/**
 * GSM UART1,TX:B6,RX:B7
 */
#define GSM_COM                        USART1
#define GSM_COM_CLK                    RCC_APB2Periph_USART1
#define GSM_COM_TX_PIN                 GPIO_Pin_6
#define GSM_COM_TX_GPIO_PORT           GPIOB
#define GSM_COM_TX_GPIO_CLK            RCC_AHB1Periph_GPIOB
#define GSM_COM_TX_SOURCE              GPIO_PinSource6
#define GSM_COM_TX_AF                  GPIO_AF_USART1
#define GSM_COM_RX_PIN                 GPIO_Pin_7
#define GSM_COM_RX_GPIO_PORT           GPIOB
#define GSM_COM_RX_GPIO_CLK            RCC_AHB1Periph_GPIOB
#define GSM_COM_RX_SOURCE              GPIO_PinSource7
#define GSM_COM_RX_AF                  GPIO_AF_USART1
#define GSM_COM_IRQn                   USART1_IRQn

/**
 * GPS串口,UART2
 */
#define GPS_COM                        USART2
#define GPS_COM_CLK                    RCC_APB1Periph_USART2
#define GPS_COM_TX_PIN                 GPIO_Pin_2
#define GPS_COM_TX_GPIO_PORT           GPIOA
#define GPS_COM_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define GPS_COM_TX_SOURCE              GPIO_PinSource2
#define GPS_COM_TX_AF                  GPIO_AF_USART2
#define GPS_COM_RX_PIN                 GPIO_Pin_3
#define GPS_COM_RX_GPIO_PORT           GPIOA
#define GPS_COM_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define GPS_COM_RX_SOURCE              GPIO_PinSource3
#define GPS_COM_RX_AF                  GPIO_AF_USART2
#define GPS_COM_IRQn                   USART2_IRQn

/**
 * VIDEO1串口,UART3
 */
#define VIDEO1_COM                     USART3
#define VIDEO1_COM_CLK                 RCC_APB1Periph_USART3
#define VIDEO1_COM_TX_PIN              GPIO_Pin_8
#define VIDEO1_COM_TX_GPIO_PORT        GPIOD
#define VIDEO1_COM_TX_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define VIDEO1_COM_TX_SOURCE           GPIO_PinSource8
#define VIDEO1_COM_TX_AF               GPIO_AF_USART3
#define VIDEO1_COM_RX_PIN              GPIO_Pin_9
#define VIDEO1_COM_RX_GPIO_PORT        GPIOD
#define VIDEO1_COM_RX_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define VIDEO1_COM_RX_SOURCE           GPIO_PinSource9
#define VIDEO1_COM_RX_AF               GPIO_AF_USART3
#define VIDEO1_COM_IRQn                USART3_IRQn

/**
 * VIDEO2串口,UART4
 */
#define VIDEO2_COM                        UART4
#define VIDEO2_COM_CLK                    RCC_APB1Periph_UART4
#define VIDEO2_COM_TX_PIN                 GPIO_Pin_0
#define VIDEO2_COM_TX_GPIO_PORT           GPIOA
#define VIDEO2_COM_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define VIDEO2_COM_TX_SOURCE              GPIO_PinSource0
#define VIDEO2_COM_TX_AF                  GPIO_AF_UART4
#define VIDEO2_COM_RX_PIN                 GPIO_Pin_1
#define VIDEO2_COM_RX_GPIO_PORT           GPIOA
#define VIDEO2_COM_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define VIDEO2_COM_RX_SOURCE              GPIO_PinSource1
#define VIDEO2_COM_RX_AF                  GPIO_AF_UART4
#define VIDEO2_COM_IRQn                   UART4_IRQn

/**
 * DEBUG串口，UART6
 */
#define DEBUG_COM                        USART6
#define DEBUG_COM_CLK                    RCC_APB2Periph_USART6
#define DEBUG_COM_TX_PIN                 GPIO_Pin_9
#define DEBUG_COM_TX_GPIO_PORT           GPIOG
#define DEBUG_COM_TX_GPIO_CLK            RCC_AHB1Periph_GPIOG
#define DEBUG_COM_TX_SOURCE              GPIO_PinSource9
#define DEBUG_COM_TX_AF                  GPIO_AF_USART6
#define DEBUG_COM_RX_PIN                 GPIO_Pin_10
#define DEBUG_COM_RX_GPIO_PORT           GPIOG
#define DEBUG_COM_RX_GPIO_CLK            RCC_AHB1Periph_GPIOG
#define DEBUG_COM_RX_SOURCE              GPIO_PinSource10
#define DEBUG_COM_RX_AF                  GPIO_AF_USART6
#define DEBUG_COM_IRQn                   USART6_IRQn


//定义系统使用音源
#define	_AUDIO_NONE_		1		//未选择音源
#define	_AUDIO_TTS_			2		//TTS语音芯片
#define	_AUDIO_PHONE_		3		//电话语音


//定义系统使用的定时器
#define	TIMTts				TIM1	//TIM1
#define	TIMIr				TIM2	//TIM2
#define	TIM10ms				TIM3	//TIM3
#define	TIMPrt				TIM4	//TIM4


void SPI1_Lowlevel_Init();
void I2C2_Lowlevel_Init();
void USART_Configuration(COM_TypeDef COM, u16 u16Baund);


#endif /* BOARD_H_ */
