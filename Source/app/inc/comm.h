/**   
* @Title: comm.h  
* @Description: TODO(用一句话描述该文件做什么) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-20 上午9:43:56 
* @version V1.0   
*/ 


#ifndef COMM_H_
#define COMM_H_

#include "type.h"


//串口通信状态机
typedef	struct tCOMMTag
{
	u8 *pBuf;	 /*缓区*/
	u16 read;	/*读位置*/
	u16 write;  /*写位置*/
	u16 dataSize;  /*有效数据长度*/
	u16 bufSize;  /*缓区容量*/


} tCOMM;

//初始化
void Comm_Init(tCOMM *pComm, u8 *pBuf, u16 bufSize);

//复位缓区状态
void Comm_Clear(tCOMM *pComm);

#endif /* COMM_H_ */
