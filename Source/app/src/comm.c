/**   
* @Title: comm.c  
* @Description: TODO(��һ�仰�������ļ���ʲô) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-20 ����10:03:14 
* @version V1.0   
*/ 

#include "comm.h"
#include "qp_port.h"
#include "utility.h"
#include <stm32f2xx_conf.h>
#include <qevt.h>

Q_DEFINE_THIS_FILE

/**
 * @Func	���ڽṹ���ʼ��
 * @Param	pBuf			[in]	�ڴ���ַ
 * 			bufSize		 	[in]	�ڴ���С
 * @Ret		��
 */
void Comm_Init(tCOMM *pComm, u8 *pBuf, u16 bufSize)
{
	Q_ASSERT((pComm != NULL) && (pBuf != NULL) && (bufSize > 0));

	pComm->pBuf = pBuf;
	pComm->bufSize = bufSize;
	pComm->dataSize = 0;
	pComm->read = 0;
	pComm->write = 0;
}


//��λ���ͻ���
void Comm_Clear(tCOMM *pComm)
{
	Q_ASSERT((pComm != NULL) && (pComm->pBuf != NULL) && (pComm->bufSize > 0));

	ZeroMem(pComm->pBuf, pComm->bufSize);

	pComm->dataSize = 0;
	pComm->read = 0;
	pComm->write = 0;
}


