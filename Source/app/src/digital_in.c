/*
 * digital_in.c
 *
 *  Created on: 2014年4月25日
 *      Author: ThinkPad User
 */

#include <board.h>
#include <stm32f2xx_conf.h>

/**
 * 数字信号初始化
 *
 */
void Digital_In_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DIGIT_IN_BRAKE_PIN | DIGIT_IN_RT_TURN_PIN |
	DIGIT_IN_LT_TURN_PIN | DIGIT_IN_DOOR_PIN | DIGIT_IN_HANDBREAKE_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

}
