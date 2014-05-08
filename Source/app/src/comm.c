/**   
* @Title: comm.c  
* @Description: TODO(用一句话描述该文件做什么) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-20 上午10:03:14 
* @version V1.0   
*/ 

#include "comm.h"
#include "qp_port.h"
#include "utility.h"
#include <stm32f2xx_conf.h>
#include <qevt.h>

Q_DEFINE_THIS_FILE

/**
 * @Func	串口结构体初始化
 * @Param	pBuf			[in]	内存块地址
 * 			bufSize		 	[in]	内存块大小
 * @Ret		无
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


//复位发送缓区
void Comm_Clear(tCOMM *pComm)
{
	Q_ASSERT((pComm != NULL) && (pComm->pBuf != NULL) && (pComm->bufSize > 0));

	ZeroMem(pComm->pBuf, pComm->bufSize);

	pComm->dataSize = 0;
	pComm->read = 0;
	pComm->write = 0;
}


