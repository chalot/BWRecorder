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
#define		GPIOA_CDMA_USART2_CTS_				GPIO_Pin_0					//CDMA����USART2 CTS�ź�
#define		GPIOA_CDMA_USART2_DTR_				GPIO_Pin_1					//CDMA����USART2 DTR�ź�
#define		GPIOA_CDMA_USART2_TX_				GPIO_Pin_2					//CDMA����USART2 TX�ź�	(USART2 REMAP = 0)
#define		GPIOA_CDMA_USART2_RX_				GPIO_Pin_3					//CDMA����USART2 RX�ź�

#define		GPIOA_SFLASH_CS_					GPIO_Pin_4					//����FLASHƬѡ�ź�
#define		GPIOA_SFLASH_CLK_					GPIO_Pin_5					//����FLASH CLK�ź�
#define		GPIOA_SFLASH_MISO_					GPIO_Pin_6					//����FLASH MISO�ź�
#define		GPIOA_SFLASH_MOSI_					GPIO_Pin_7					//����FLASH MOSI�ź�

//#define		GPIOA_CDMA_LED_						GPIO_Pin_8					//CDMA LED��
#define		GPIOA_CAN_LED_						GPIO_Pin_8					//GPS LED��
/*JTAG���ô��ڣ�*/
#define		JTAG_UART1_TX_					GPIO_Pin_9
#define		JTAG_UART1_RX_					GPIO_Pin_10
//#define		GPIOA_CAN_LED_						GPIO_Pin_10				//Trigger Det�ź�
#define		GPIOA_GPRS_PWRKEY_					GPIO_Pin_11				//MG323ģ��PWR KEY
#define		GPIOA_CDMA_CTRL_					GPIO_Pin_12				//CDMAģ���Դ����


//PB.0 --- PB.15
#define		GPIOB_RELAY1_CTRL_					GPIO_Pin_0					//�̵���1����
#define		GPIOB_PWR_CONVERTER_				GPIO_Pin_1					//PWR CONVERTER�ź�
//#define		GPIOB_SDIO_CS						GPIO_Pin_5					//SDIO CS�ź�
#define		GPIOB_CAN2_RX_						GPIO_Pin_5					//SDIO CS�ź�
#define		GPIOB_CAN2_TX_						GPIO_Pin_6					//LED������USART1 TX  (USART1 REMAP = 1)
#define		GPIOB_SDCARD_DET_					GPIO_Pin_7					//LED������USART1 RX
#define		GPIOB_CAN1_RX_						GPIO_Pin_8					//CAN RX
#define		GPIOB_CAN1_TX_						GPIO_Pin_9					//CAN TX
#define		GPIOB_GPS_USART3_TX_				GPIO_Pin_10					//GPS����USART3 TX  (USART3 REMAP = 01)
#define		GPIOB_GPS_USART3_RX_				GPIO_Pin_11					//GPS����USART3 RX

#define		GPIOB_SPI2_CS_						GPIO_Pin_12					//SPI2 Ƭѡ�ź�
#define		GPIOB_SPI2_CLK_						GPIO_Pin_13					//SPI2 CLK�ź�
#define		GPIOB_SPI2_MISO_					GPIO_Pin_14					//SPI2 MISO�ź�
#define		GPIOB_SPI2_MOSI_					GPIO_Pin_15					//SPI2 MOSI�ź�

//PC
#define		GPIOC_GPSANT_DET_					GPIO_Pin_0					//GPS ���߼��
#define		GPIOC_GPS_CTRL_						GPIO_Pin_1					//GPS��Դ����
#define		GPIOC_SIM_DET_						GPIO_Pin_2					//SIM�����
#define		GPIOC_RELAY2_CTRL_					GPIO_Pin_3					//�̵���1����
#define		GPIOC_GPS_RST_						GPIO_Pin_4					//GPS��λ
#define		GPIOC_CAN_CTRL						GPIO_Pin_5					//CAN��Դ����
#define		GPIOC_CAN1_ON						GPIO_Pin_6					//CAN1����
#define		GPIOC_CAN2_ON						GPIO_Pin_7					//CAN2����
#define		GPIOC_GPS_LED						GPIO_Pin_8					//GPSָʾ��
#define		GPIOC_CDMA_LED						GPIO_Pin_9					//����ָʾ��
#define		GPIOC_CHARGE_DET					GPIO_Pin_10					//�����
#define		GPIOC_WDG							GPIO_Pin_12					//ι���ź�

//SDIOģ��IO�ڶ���
#define		GPIOC_SDIO_D0						GPIO_Pin_8
#define		GPIOC_SDIO_D1						GPIO_Pin_9
#define		GPIOC_SDIO_D2						GPIO_Pin_10
#define		GPIOC_SDIO_D3						GPIO_Pin_11
#define		GPIOC_SDIO_SCK						GPIO_Pin_12

//PD
#define		GPIOD_ACC_DET_						GPIO_Pin_2					//ACC DET�ź�



#endif /* STM32_GPIO_H_ */
