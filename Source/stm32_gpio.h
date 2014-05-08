/*
 * stm32_gpio.h
 *
 *  Created on: 2013-5-15
 *      Author: gaozhengdong
 */

#ifndef STM32_GPIO_H_
#define STM32_GPIO_H_

#include <stm32f2xx_gpio.h>


//PA.0 --- PA.15
#define		GPIOA_CDMA_USART2_CTS_				GPIO_Pin_0					//CDMA串口USART2 CTS信号
#define		GPIOA_CDMA_USART2_DTR_				GPIO_Pin_1					//CDMA串口USART2 DTR信号
#define		GPIOA_CDMA_USART2_TX_				GPIO_Pin_2					//CDMA串口USART2 TX信号	(USART2 REMAP = 0)
#define		GPIOA_CDMA_USART2_RX_				GPIO_Pin_3					//CDMA串口USART2 RX信号

#define		GPIOA_SFLASH_CS_					GPIO_Pin_4					//串行FLASH片选信号
#define		GPIOA_SFLASH_CLK_					GPIO_Pin_5					//串行FLASH CLK信号
#define		GPIOA_SFLASH_MISO_					GPIO_Pin_6					//串行FLASH MISO信号
#define		GPIOA_SFLASH_MOSI_					GPIO_Pin_7					//串行FLASH MOSI信号

//#define		GPIOA_CDMA_LED_						GPIO_Pin_8					//CDMA LED灯
#define		GPIOA_CAN_LED_						GPIO_Pin_8					//GPS LED灯
/*JTAG复用串口：*/
#define		JTAG_UART1_TX_					GPIO_Pin_9
#define		JTAG_UART1_RX_					GPIO_Pin_10
//#define		GPIOA_CAN_LED_						GPIO_Pin_10				//Trigger Det信号
#define		GPIOA_GPRS_PWRKEY_					GPIO_Pin_11				//MG323模块PWR KEY
#define		GPIOA_CDMA_CTRL_					GPIO_Pin_12				//CDMA模块电源控制


//PB.0 --- PB.15
#define		GPIOB_RELAY1_CTRL_					GPIO_Pin_0					//继电器1控制
#define		GPIOB_PWR_CONVERTER_				GPIO_Pin_1					//PWR CONVERTER信号
//#define		GPIOB_SDIO_CS						GPIO_Pin_5					//SDIO CS信号
#define		GPIOB_CAN2_RX_						GPIO_Pin_5					//SDIO CS信号
#define		GPIOB_CAN2_TX_						GPIO_Pin_6					//LED屏串口USART1 TX  (USART1 REMAP = 1)
#define		GPIOB_SDCARD_DET_					GPIO_Pin_7					//LED屏串口USART1 RX
#define		GPIOB_CAN1_RX_						GPIO_Pin_8					//CAN RX
#define		GPIOB_CAN1_TX_						GPIO_Pin_9					//CAN TX
#define		GPIOB_GPS_USART3_TX_				GPIO_Pin_10					//GPS串口USART3 TX  (USART3 REMAP = 01)
#define		GPIOB_GPS_USART3_RX_				GPIO_Pin_11					//GPS串口USART3 RX

#define		GPIOB_SPI2_CS_						GPIO_Pin_12					//SPI2 片选信号
#define		GPIOB_SPI2_CLK_						GPIO_Pin_13					//SPI2 CLK信号
#define		GPIOB_SPI2_MISO_					GPIO_Pin_14					//SPI2 MISO信号
#define		GPIOB_SPI2_MOSI_					GPIO_Pin_15					//SPI2 MOSI信号

//PC
#define		GPIOC_GPSANT_DET_					GPIO_Pin_0					//GPS 天线检测
#define		GPIOC_GPS_CTRL_						GPIO_Pin_1					//GPS电源控制
#define		GPIOC_SIM_DET_						GPIO_Pin_2					//SIM卡检测
#define		GPIOC_RELAY2_CTRL_					GPIO_Pin_3					//继电器1控制
#define		GPIOC_GPS_RST_						GPIO_Pin_4					//GPS复位
#define		GPIOC_CAN_CTRL						GPIO_Pin_5					//CAN电源控制
#define		GPIOC_CAN1_ON						GPIO_Pin_6					//CAN1开电
#define		GPIOC_CAN2_ON						GPIO_Pin_7					//CAN2开电
#define		GPIOC_GPS_LED						GPIO_Pin_8					//GPS指示灯
#define		GPIOC_CDMA_LED						GPIO_Pin_9					//网络指示灯
#define		GPIOC_CHARGE_DET					GPIO_Pin_10					//充电检测
#define		GPIOC_WDG							GPIO_Pin_12					//喂狗信号

//SDIO模块IO口定义
#define		GPIOC_SDIO_D0						GPIO_Pin_8
#define		GPIOC_SDIO_D1						GPIO_Pin_9
#define		GPIOC_SDIO_D2						GPIO_Pin_10
#define		GPIOC_SDIO_D3						GPIO_Pin_11
#define		GPIOC_SDIO_SCK						GPIO_Pin_12

//PD
#define		GPIOD_ACC_DET_						GPIO_Pin_2					//ACC DET信号



#endif /* STM32_GPIO_H_ */
