/**   
 * @Title: systimer.h  
 * @Description: TODO(��һ�仰�������ļ���ʲô) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-7-5 ����8:42:38 
 * @version V1.0   
 */

#ifndef SYSTIMER_H_
#define SYSTIMER_H_

#include "utility.h"
#include "qp_port.h"
#include "type.h"

#pragma pack(1)

/*ϵͳʱ�䣬BCD���ʾ*/
//typedef struct {
//	u8 year;
//	u8 month;
//	u8 day;
//	u8 hour;
//	u8 minute;
//	u8 second;
//
//} SYSTIMER;

/*���ڸ�ʽ*/
typedef struct _TIME {
	u16 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
} TIME, *PTIMER;

/*ʱ�����ڣ�BCD��*/
typedef struct {
	u8 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 minute;
	u8 second;
}/*tTIME,*/BCDTIME;

/*ϵͳʱ��*/
void
SysTick_Init();
void
SysTick_Set(TIME *time);
void
SysTick_Get(BCDTIME *time);
void
SysTick_GetRaw(TIME *time);
void
SysTick_refreshPerSecond();
void
SysTick_UpdateAdditionalSeconds(u32 seconds);

#pragma pack()

#endif /* SYSTIMER_H_ */
