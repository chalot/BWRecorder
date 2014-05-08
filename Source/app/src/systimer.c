/**   
* @Title: systimer.c  
* @Description: TODO(��һ�仰�������ļ���ʲô) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-7-5 ����8:42:49 
* @version V1.0   
*/ 
#include "systimer.h"
#include "utility.h"
#include "parameter.h"
#include "bsp.h"
#include <mutex.h>

static SYSTIMER systimer; //ϵͳʱ��


/*����������*/
DECLARE_MUTEX(mux)

//��FLASH��ȡ��ʼʱ��
void SysTick_Init()
{
	systimer.year = 14;
	systimer.month = 1;
	systimer.day = 1;
	systimer.hour = 1;
	systimer.minute = 1;
	systimer.second = 0;

}

/*ϵͳʱ��*/
void SysTimer_Set(SYSTIMER *timer)
{
	MUTEX_LOCK(mux);
	memcpy_((u8*)&systimer, (u8*)timer, sizeof(SYSTIMER));
	MUTEX_UNLOCK(mux);
}

/**
 * @Func	ȡ��ǰϵͳʱ�䣬BCD���ʾ
 * @Param	time		[in]	�洢
 * @Ret		��
 */
void SysTimer_Get(SYSTIMER *time)
{
	MUTEX_LOCK(mux);

	time->year = ((systimer.year / 10) << 4) + (systimer.year % 10);
	time->month = ((systimer.month / 10) << 4) + (systimer.month % 10);
	time->day = ((systimer.day / 10) << 4) + (systimer.day % 10);
	time->hour = ((systimer.hour / 10) << 4) + (systimer.hour % 10);
	time->minute = ((systimer.minute / 10) << 4) + (systimer.minute % 10);
	time->second = ((systimer.second / 10) << 4) + (systimer.second % 10);

	MUTEX_UNLOCK(mux);
}

/**
 * @Func	ȡ��ǰϵͳʱ�䣬ʮ���Ʊ�ʾ
 * @Param	time		[in]	�洢
 * @Ret		��
 */
void SysTimer_GetRaw(TIME *time)
{
	MUTEX_LOCK(mux);

	time->year = systimer.year;
	time->month = systimer.month;
	time->day = systimer.day;
	time->hour = systimer.hour;
	time->minute = systimer.minute;
	time->second = systimer.second;

	MUTEX_UNLOCK(mux);
}

/**
 * �����
 */
void SysTimer_refreshPerSecond()
{
	MUTEX_LOCK(mux);

	u8	u8DaysofMonth = 0;

	//ʱ�����
	systimer.second++;
	if(systimer.second >= 60)
	{
		systimer.second = 0;

		systimer.minute++;

		if(	systimer.minute >= 60)
		{
			systimer.minute = 0;
			systimer.hour++;
			if(	systimer.hour >= 24)
			{
				systimer.hour = 0;

				//ȡ�õ�ǰ���ж�����
				u8DaysofMonth = MISC_DaysofMonth(systimer.year + 2000, systimer.month);

				if(systimer.day >= u8DaysofMonth)
				{
					systimer.month += 1;
					systimer.day = 1;

					if(systimer.month > 12)
					{
						systimer.month = 1;
						systimer.year += 1;
					}
				}

				//���㵱ǰʱ��
				ResetCurTicks();
			}
		}
	}
	MUTEX_UNLOCK(mux);

}

//����ϵͳʱ�䣬
void SysTimer_UpdateAdditionalSeconds(u32 seconds)
{
	MUTEX_LOCK(mux);

	while(seconds-- > 0)
	{
		SysTimer_refreshPerSecond();
	}

	MUTEX_UNLOCK(mux);
}
