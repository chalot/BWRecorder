/*
 * CMP.h
 *
 *  Created on: Nov 16, 2011
 *      Author: gao
 */

#ifndef _MP_H_
#define _MP_H_

//#include "port.h"
//#include "type.h"
#include <stm32f2xx_conf.h>

#pragma pack(1)

#define SUPPORT_DUMP	//���뿪�أ�ʹ�ܴ洢�б�

#if 0
typedef struct _CANRAWFRAME
{
	u32 id; /*ID*/
	u8 len; /*֡����*/
	u8 data[8]; /*֡����*/

}CANRAWFRAME;
#endif

#define	CAN_Tx	0	/*CAN����*/
#define	CAN_Rx	1	/*CAN����*/

/*CAN֡�洢��ʽ����ԭʼ֡��Ϣ����������˱�Ҫ���ֶΣ�dir����tickʱ�̣������ڴ洢*/
typedef struct _CANRAWFRAME {
	u32 id; /*ID*/
	u8 dir :1; /*�շ�����0��Tx��1��Rx*/
	u8 len :7; /*֡����*/
	u8 data[8]; /*֡����*/
	u32 tick; /*�շ�ʱ�̣���λ0.1ms������ʱ�����ʵ�ʷ���ʱ��ֵ������ʱ�����ʵ�ʽ���ʱ��ֵ*/
} CANRAWFRAME;

#define DATA_SIZE		sizeof(CANRAWFRAME)	/* �������ݳ��� */
#define NODES_IN_BLOCK	400	/* ���ڽڵ��������СԼ200*16B = 3.5K */
#define	BLOCKS_MAX		1	/* �ڴ�����ڴ���� */

/*
 * ���ݽڵ㶨��
 */
struct _DATANODE {
	u8 data[DATA_SIZE];
#ifdef SUPPORT_DUMP
	u8 reference; /*���ü���*/
#endif
	struct _DATANODE *next; //�����ڵ�
#ifdef SUPPORT_DUMP
	struct _DATANODE *prev; //ǰ���ڵ�
#endif
};

typedef struct _DATANODE DATANODE;

/*
 * �ڴ�ع������
 */
typedef struct MemPool {

	DATANODE* m_freeHead;
	DATANODE* m_freeTail;

#ifdef SUPPORT_DUMP
	DATANODE* m_allocHead;
	DATANODE* m_allocTail;
#endif

	unsigned short int m_AllocBlocks; //�ѷ������
	void* m_BlockAddr[BLOCKS_MAX]; //�����飬��¼�ѷ������׵�ַ

	unsigned long m_AllocNodes; //�ѷ���Ľڵ���
	unsigned long m_FreeNodes; //���нڵ���
	unsigned long m_TotalAllocNodes; //�ܷ���ڵ���
} MEMPOOL;

/*
 * �����ڴ��
 */
int MP_Create();

/*
 *����ڴ��
 */
int MP_Clear();

/*
 * �����ڴ��
 */
int MP_Destroy();

/*
 * ��ʼ���ڴ��
 */
void MP_Init();

/*
 * ����洢��Ԫ
 */
DATANODE* MP_AllocNode();

/*
 * �ͷŴ洢��Ԫ
 */
int MP_FreeNode();

/*
 * �����
 */
int MP_AllocBlock();

/*
 * ȡ�����������
 */
DATANODE* MP_GetAllocNodeHead();

/*
 * ���ٽڵ�����
 */
void MP_DecountReference(DATANODE* pNode);

/*
 * ��ӡ�ڴ��״̬
 */
void MP_GetState();

#pragma pack()

#endif /* CMP_H_ */

