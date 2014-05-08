/**   
* @Title: exception.c  
* @Description: TODO(用一句话描述该文件做什么) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-8-13 下午3:30:56 
* @version V1.0   
*/ 

#include "exception.h"
#include "qp_port.h"
#include <qevt.h>

Q_DEFINE_THIS_MODULE("exception.c")

static u8 ErrCode[MAX_CLASS];	//错误类型

/**
 * @Func	设置错误类型
 * @Param	err			[IN]	错误代码
 * @Ret		无
 */
void SetErrType(eFAULT_CLASS class, u8 err)
{
	Q_ASSERT(class < MAX_CLASS);
	if(FAULT_NONE == err)
		ErrCode[class] = FAULT_NONE;
	else
		ErrCode[class] |= err;
}

/**
 * @Func	读取错误类型
 * @Param	无
 * @Ret		u32 	[OUT]	系统错误码
 */
u8 GetErrType(eFAULT_CLASS class)
{
	Q_ASSERT(class < MAX_CLASS);
	return ErrCode[class];
}

/**
 * @Func	清除某个错误类型
 * @Param	err			[IN]	错误代码
 * @Ret		无
 */
void ClearErrType(eFAULT_CLASS class, u8 err)
{
	Q_ASSERT(class < MAX_CLASS);
	ErrCode[class] &= ~err;
}

/**
 * @Func	重置错误类型
 * @Param	无
 * @Ret		无
 */
void ResetErrType(eFAULT_CLASS class)
{
	Q_ASSERT(class < MAX_CLASS);
	ErrCode[class] &= 0;
}

/**
 * @Func	重置错误类型
 * @Param	无
 * @Ret		无
 */
BOOL CheckSpecifiedErrTypeExisted(eFAULT_CLASS class, u8 err)
{
	Q_ASSERT(class < MAX_CLASS);
	if(0 != (ErrCode[class] & err))
		return TRUE;

	return FALSE;
}

/**
 * @Func	检查系统是否有故障出现
 * @Param	无
 * @Ret		TRUE=有故障，FALSE=无故障
 */
BOOL CheckIfNoErrTypeOccured()
{
	if((FAULT_NONE == GetErrType(CAN_CLASS)) &&
		(FAULT_NONE == GetErrType(SYS_CLASS)) &&
		(FAULT_NONE == GetErrType(NET_CLASS)) &&
		(FAULT_NONE == GetErrType(GPS_CLASS)) &&
		(FAULT_NONE == GetErrType(SD_CLASS)) )
	{
		return TRUE;
	}
	else
		return FALSE;
}
