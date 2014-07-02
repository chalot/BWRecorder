#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f2xx_conf.h"
#include <board.h>


//������Ϣ����
typedef enum {
	KEY_OK = 0,		//ȷ����
	KEY_UP,			//���ϼ�
	KEY_DOWN,		//���¼�
	KEY_MENU,		//�˵���
	KEY_UNKNOWN,
} eKEY;


void Key_LowLevel_Init();

#endif
