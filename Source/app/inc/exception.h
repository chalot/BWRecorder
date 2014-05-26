/**   
 * @Title: exception.h  
 * @Description: TODO(ϵͳ���϶���)
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-8-13 ����3:11:42 
 * @version V1.0   
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include "type.h"

/*��������*/
typedef enum {
	CAN_CLASS = 0, SYS_CLASS, NET_CLASS, GPS_CLASS, SD_CLASS,

	MAX_CLASS,

} eFAULT_CLASS;

#define FAULT_NONE					0x00	//�޹���/*CAN����*///#define CAN_FAULT_NONE				0x01	//�޹���#define CAN_FAULT_TX				0x01	//CAN���͹���#define CAN_FAULT_RX				0x02	//CAN���չ���#define CAN_FAULT_OTHER				0x04	//��������/*ϵͳ�������*///#define SYS_FAULT_NONE				0x01	//�޹���#define SYS_FAULT_PWR				0x01	//��Դ����#define SYS_FAULT_EXCEPTION			0x02	//�쳣����#define SYS_FAULT_OTHER				0x04	//��������/*����ͨ�Ź���*///#define NET_FAULT_NONE				0x01	//�޹���#define NET_FAULT_OUTOFSERVICE		0x01	//Ƿ��#define NET_FAULT_SIMCARD			0x02	//SIM������#define NET_FAULT_UNREACHABLE		0x04	//���粻�ɴ�/*GPS��λ����*///#define GPS_FAULT_NONE				0x01	//�޹���#define GPS_FAULT_ANTENA			0x01	//GPS���߹���#define GPS_FAULT_INVALID			0x02	//��λ��Ч#define GPS_FAULT_OTHER				0x04	//����/*SD������*///#define SD_FAULT_NONE				0x01	//�޹���#define SD_FAULT_NOTDETECTED		0x01	//SD�����ʧ��#define SD_FAULT_READ				0x02	//SD��������#define SD_FAULT_WRITE				0x04	//SD��д����/*** @Func ���ô�������* @Param err [IN] �������* @Ret ��*/voidSetErrType(eFAULT_CLASS class, u8 err);
/**
 * @Func	��ȡ��������
 * @Param	��
 * @Ret		u32 	[OUT]	ϵͳ������
 */
u8
GetErrType(eFAULT_CLASS class);

/**
 * @Func	���ĳ����������
 * @Param	err			[IN]	�������
 * @Ret		��
 */
void
ClearErrType(eFAULT_CLASS class, u8 err);

/**
 * @Func	���ô�������
 * @Param	��
 * @Ret		��
 */
void
ResetErrType(eFAULT_CLASS class);

/**
 * @Func	���ô�������
 * @Param	��
 * @Ret		��
 */
BOOL
CheckSpecifiedErrTypeExisted(eFAULT_CLASS class, u8 err);

/**
 * @Func	���ϵͳ�Ƿ��й��ϳ���
 * @Param	��
 * @Ret		TRUE=�й��ϣ�FALSE=�޹���
 */
BOOL
CheckIfNoErrTypeOccured();

#endif /* EXCEPTION_H_ */
