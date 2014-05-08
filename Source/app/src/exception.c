/**   
* @Title: exception.c  
* @Description: TODO(��һ�仰�������ļ���ʲô) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-8-13 ����3:30:56 
* @version V1.0   
*/ 

#include "exception.h"
#include "qp_port.h"
#include <qevt.h>

Q_DEFINE_THIS_MODULE("exception.c")

static u8 ErrCode[MAX_CLASS];	//��������

/**
 * @Func	���ô�������
 * @Param	err			[IN]	�������
 * @Ret		��
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
 * @Func	��ȡ��������
 * @Param	��
 * @Ret		u32 	[OUT]	ϵͳ������
 */
u8 GetErrType(eFAULT_CLASS class)
{
	Q_ASSERT(class < MAX_CLASS);
	return ErrCode[class];
}

/**
 * @Func	���ĳ����������
 * @Param	err			[IN]	�������
 * @Ret		��
 */
void ClearErrType(eFAULT_CLASS class, u8 err)
{
	Q_ASSERT(class < MAX_CLASS);
	ErrCode[class] &= ~err;
}

/**
 * @Func	���ô�������
 * @Param	��
 * @Ret		��
 */
void ResetErrType(eFAULT_CLASS class)
{
	Q_ASSERT(class < MAX_CLASS);
	ErrCode[class] &= 0;
}

/**
 * @Func	���ô�������
 * @Param	��
 * @Ret		��
 */
BOOL CheckSpecifiedErrTypeExisted(eFAULT_CLASS class, u8 err)
{
	Q_ASSERT(class < MAX_CLASS);
	if(0 != (ErrCode[class] & err))
		return TRUE;

	return FALSE;
}

/**
 * @Func	���ϵͳ�Ƿ��й��ϳ���
 * @Param	��
 * @Ret		TRUE=�й��ϣ�FALSE=�޹���
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
