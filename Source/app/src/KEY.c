/***********************************************************************************************************
 * 文    件：	Key.c
 * 描    述：	按键处理
 * 日  	期：
 * 版 	本：	V1.0
 ***********************************************************************************************************/
#include "stm32f2xx.h"
#include <board.h>
#include <key.h>
#include <qactiveobjs.h>
#include <qp_port.h>

/**
 * 按键初始化
 */
void Key_LowLevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(
			KEY_MENU_GPIO_CLK | KEY_UP_GPIO_CLK | KEY_DOWN_GPIO_CLK | KEY_OK_GPIO_CLK,
			ENABLE);

	//输入悬浮
	GPIO_InitStructure.GPIO_Pin = KEY_MENU_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(KEY_MENU_GPIO_PORT, &GPIO_InitStructure);
	//输入悬浮
	GPIO_InitStructure.GPIO_Pin = KEY_OK_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(KEY_OK_GPIO_PORT, &GPIO_InitStructure);
	//输入悬浮
	GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(KEY_UP_GPIO_PORT, &GPIO_InitStructure);
	//输入悬浮
	GPIO_InitStructure.GPIO_Pin = KEY_DOWN_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(KEY_DOWN_GPIO_PORT, &GPIO_InitStructure);

	/* Configure EXTI Line3 to generate an event or an interrupt on falling edge */
	EXTI_ClearITPendingBit(KEY_MENU_EXTI_LINE);
	EXTI_InitStructure.EXTI_Line = KEY_MENU_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_ClearITPendingBit(KEY_UP_EXTI_LINE);
	EXTI_InitStructure.EXTI_Line = KEY_UP_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_ClearITPendingBit(KEY_DOWN_EXTI_LINE);
	EXTI_InitStructure.EXTI_Line = KEY_DOWN_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_ClearITPendingBit(KEY_OK_EXTI_LINE);
	EXTI_InitStructure.EXTI_Line = KEY_OK_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//配置外部中断
	SYSCFG_EXTILineConfig(KEY_MENU_EXTI_Port, KEY_MENU_EXTI_LINE);
	SYSCFG_EXTILineConfig(KEY_UP_EXTI_Port, KEY_UP_EXTI_LINE);
	SYSCFG_EXTILineConfig(KEY_DOWN_EXTI_Port, KEY_DOWN_EXTI_LINE);
	SYSCFG_EXTILineConfig(KEY_OK_EXTI_Port, KEY_OK_EXTI_LINE);
}
