/**   
* @Title: acc.h  
* @Description: TODO(��һ�仰�������ļ���ʲô) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-7-16 ����10:14:46 
* @version V1.0   
*/ 


#ifndef ACC_H_
#define ACC_H_
#include "stm32_gpio.h"

#define GET_ACC_STATE() 	GPIO_ReadInputDataBit(GPIOD,GPIOD_ACC_DET_)

#endif /* ACC_H_ */
