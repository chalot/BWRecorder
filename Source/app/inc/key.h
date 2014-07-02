#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f2xx_conf.h"
#include <board.h>


//按键消息定义
typedef enum {
	KEY_OK = 0,		//确定键
	KEY_UP,			//向上键
	KEY_DOWN,		//向下键
	KEY_MENU,		//菜单键
	KEY_UNKNOWN,
} eKEY;


void Key_LowLevel_Init();

#endif
