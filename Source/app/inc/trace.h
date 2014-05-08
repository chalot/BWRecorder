/**   
* @Title: trace.h  
* @Description: TODO(��һ�仰�������ļ���ʲô) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-4 ����11:37:02 
* @version V1.0   
*/ 


#ifndef TRACE_H_
#define TRACE_H_

#include <stm32f2xx_conf.h>
#include "qp_port.h"

#define UART_TRACE  USART2 	//��ӡ��Ϣ������ں�

#define TRACE_BUFSIZE	512		//��ӡ���ڻ�������

/*��������ַ�*/
//#define PUTCHAR(c) Trace_SendData((u8*)&c, 1)
/*����ַ���*/
//#define PUTSTRING(c) Trace_SendData((u8*)&c[0], strlen(c))

/*���ÿ⺯��ʵ�ֿɱ䳤��ӡ�������*/
//
/**
 * @Func	��ʽ���ַ���
 * @Param	pBuf		[in]	Ŀ�껺��
 * 			format		[in]	����һ���̶��������˴�Ϊ��ʽ���ַ���
 * 			...			[in]	��ɲ���
 * @Ret		��
 */
void sprintf_(u8 *pBuf, const char *format, ...);

/**
 * @Func	��ӡ֡��ϸ��Ϣ
 * @Param	eState		[in]	��¼����
 * 			pAO			[in]	״̬��
 * 			format		[in]	��ʽ���ַ���
 * @Ret		��
 */
void TRACE_(u8 eState, QActive* pAO, const char *format, ...);



/**
 * @Func	���ÿ⺯��ʵ�ֿɱ䳤��ӡ�������
 * @Param	format		[in]	����һ���̶��������˴�Ϊ��ʽ���ַ���
 * 			...			[in]	��ɲ���
 * @Ret		��
 */
void TRACE(const char *format, ...);

/**
 * @Func	��ӡ֡��ϸ��Ϣ
 * @Param	pFrame		[in]	֡����
 * @Ret		��
 */
//void TRACE_CANRawFrame(CANRAWFRAME *pFrame);

void TRACE_RAW_BUFFER(u8 *pMsg, u16 msgLen);

#if 0
 /**
  * @Func	��ӡ֡��ϸ��Ϣ
  * @Param	pFrame		[in]	֡����
  * @Ret		��
  */
 void TRACE_CANRawFrame(CANRAWFRAME *pFrame);

/**
 * @Func	��ӡ��������
 * @Param	��
 * @Ret		��
 */
void Trace_configuration();

/**
 * @Func	��ӡ������糬����ӡ����ʣ�����������ȡ��β������
 * @Param	pData		[in]	����ӡ����
 * 			len 		[in]	����ӡ���ݳ���
 * @Ret		��
 */
void Trace_SendData(u8 *pData , u16 len);

/**
 * @Func	�������ݷ����ж���ڣ������жϷ�ʽ�����ͻ����ڵ����ݷ�����
 * @Param	��
 * @Ret		��
 */
void ISR_UART_TRACE(void);

#endif

#endif /* TRACE_H_ */
