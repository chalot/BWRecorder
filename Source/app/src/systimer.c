/**   
* @Title: systimer.c  
* @Description: TODO(用一句话描述该文件做什么) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-7-5 下午8:42:49 
* @version V1.0   
*/ 
#include "systimer.h"
#include "utility.h"
#include "parameter.h"
#include "bsp.h"
#include <mutex.h>

static SYSTIMER systimer; //系统时钟


/*声明互斥量*/
DECLARE_MUTEX(mux)

//从FLASH读取起始时间
void SysTick_Init()
{
	systimer.year = 14;
	systimer.month = 1;
	systimer.day = 1;
	systimer.hour = 1;
	systimer.minute = 1;
	systimer.second = 0;

}

/*系统时钟*/
void SysTimer_Set(SYSTIMER *timer)
{
	MUTEX_LOCK(mux);
	memcpy_((u8*)&systimer, (u8*)timer, sizeof(SYSTIMER));
	MUTEX_UNLOCK(mux);
}

/**
 * @Func	取当前系统时间，BCD码表示
 * @Param	time		[in]	存储
 * @Ret		无
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
 * @Func	取当前系统时间，十进制表示
 * @Param	time		[in]	存储
 * @Ret		无
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
 * 秒更新
 */
void SysTimer_refreshPerSecond()
{
	MUTEX_LOCK(mux);

	u8	u8DaysofMonth = 0;

	//时间更新
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

				//取得当前月有多少天
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

				//清零当前时刻
				ResetCurTicks();
			}
		}
	}
	MUTEX_UNLOCK(mux);

}

//更新系统时间，
void SysTimer_UpdateAdditionalSeconds(u32 seconds)
{
	MUTEX_LOCK(mux);

	while(seconds-- > 0)
	{
		SysTimer_refreshPerSecond();
	}

	MUTEX_UNLOCK(mux);
}
