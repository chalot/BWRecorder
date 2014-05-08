/**   
* @Title: comm.h  
* @Description: TODO(��һ�仰�������ļ���ʲô) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-20 ����9:43:56 
* @version V1.0   
*/ 


#ifndef COMM_H_
#define COMM_H_

#include "type.h"


//����ͨ��״̬��
typedef	struct tCOMMTag
{
	u8 *pBuf;	 /*����*/
	u16 read;	/*��λ��*/
	u16 write;  /*дλ��*/
	u16 dataSize;  /*��Ч���ݳ���*/
	u16 bufSize;  /*��������*/


} tCOMM;

//��ʼ��
void Comm_Init(tCOMM *pComm, u8 *pBuf, u16 bufSize);

//��λ����״̬
void Comm_Clear(tCOMM *pComm);

#endif /* COMM_H_ */
