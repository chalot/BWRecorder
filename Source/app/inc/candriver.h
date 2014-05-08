#ifndef _CAN_DRIVER_LAYER_H_
#define _CAN_DRIVER_LAYER_H_

#include "mempool.h"

//#define KWP_DRIVER_LAYER_ENPOOL		/*���뿪�أ���ʹ���ڴ����*/

/**
 * @Func	����CANԭʼ֡
 * @Param	id		[in]	֡ID
 * 			data	[in]	֡����
 * 			len		[in]	֡���ݳ���
 * @Ret		��
 */
void CAN_SendRawFrame(u32 id, u8 *data, u8 len);

/**
 * @Func	����CAN֡
 * @Param	pFrame		[in]	֡�ڴ�ڵ�
 * @Ret		��
 */
void CAN_SendFrame(CANRAWFRAME *pFrame);

/**
 * @Func	����CAN֡
 * @Param	pFrame		[in]	֡�ڴ�ڵ�
 * @Ret		��
 */
void CAN_ReceiveFrame(CANRAWFRAME *pFrame);

/**
 * @Func	���֡ID�Ƿ��������ϱ�֡
 * @Param	canid		[in]	֡ID
 * @Ret		��
 */
//BOOL CAN_CheckIfAutoRptStatusFrame(u32 canid);
/**
 * @Func	��������ϱ�֡�������λ�Ƿ���λ
 * @Param	pFrame		[in]	֡����
 * @Ret		��
 */
//BOOL CAN_IsDTCMarked(u32 canid, u8 *pFrame);
/**
 * @Func	CAN����֡�ж���ڣ���֡�����ڴ�ڵ㲢���ⲿAO������֡������Ϣ��֡�洢��ַ��Ϊ�¼���������
 * @Param	��
 * @Ret		��
 */
void ISR_CANRecvFrame();

/**
 * @Func	��ȡ����֡
 * @Param	��
 * @Ret		��
 */
DATANODE* CAN_TakeRxMsg();

/**
 * @Func	���DTC������
 * @Param	pFrame		[in]	CAN֡
 * @Ret		��
 */
//void CAN_CheckDTC(CANRAWFRAME* pFrame);
/**
 * @Func	��ȡ����֡
 * @Param	��
 * @Ret		��
 */
void CAN_MemInit();

#endif
