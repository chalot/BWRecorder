#include "beep.h"
#include <stm32f2xx_conf.h>

/**
 * �����ʼ��
 *
 */
void BEEP_Lowlevel_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	 //ʹ��GPIOB�˿�ʱ��

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = BUZZER_CTRL_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB, BUZZER_CTRL_PIN);	 //�رշ��������
}

void BEEP_Init() {

}
