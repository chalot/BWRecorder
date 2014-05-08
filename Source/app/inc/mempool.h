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

#define SUPPORT_DUMP	//编译开关，使能存储列表

#if 0
typedef struct _CANRAWFRAME
{
	u32 id; /*ID*/
	u8 len; /*帧长度*/
	u8 data[8]; /*帧数据*/

}CANRAWFRAME;
#endif

#define	CAN_Tx	0	/*CAN发送*/
#define	CAN_Rx	1	/*CAN接收*/

/*CAN帧存储格式，在原始帧信息基础上添加了必要的字段（dir方向，tick时刻），便于存储*/
typedef struct _CANRAWFRAME {
	u32 id; /*ID*/
	u8 dir :1; /*收发方向，0：Tx，1：Rx*/
	u8 len :7; /*帧长度*/
	u8 data[8]; /*帧数据*/
	u32 tick; /*收发时刻，单位0.1ms。发送时，填充实际发送时刻值；接收时，填充实际接收时刻值*/
} CANRAWFRAME;

#define DATA_SIZE		sizeof(CANRAWFRAME)	/* 数据内容长度 */
#define NODES_IN_BLOCK	400	/* 块内节点数，块大小约200*16B = 3.5K */
#define	BLOCKS_MAX		1	/* 内存池内内存块数 */

/*
 * 数据节点定义
 */
struct _DATANODE {
	u8 data[DATA_SIZE];
#ifdef SUPPORT_DUMP
	u8 reference; /*引用计数*/
#endif
	struct _DATANODE *next; //后续节点
#ifdef SUPPORT_DUMP
	struct _DATANODE *prev; //前驱节点
#endif
};

typedef struct _DATANODE DATANODE;

/*
 * 内存池管理对象
 */
typedef struct MemPool {

	DATANODE* m_freeHead;
	DATANODE* m_freeTail;

#ifdef SUPPORT_DUMP
	DATANODE* m_allocHead;
	DATANODE* m_allocTail;
#endif

	unsigned short int m_AllocBlocks; //已分配块数
	void* m_BlockAddr[BLOCKS_MAX]; //块数组，记录已分配块的首地址

	unsigned long m_AllocNodes; //已分配的节点数
	unsigned long m_FreeNodes; //空闲节点数
	unsigned long m_TotalAllocNodes; //总分配节点数
} MEMPOOL;

/*
 * 创建内存池
 */
int MP_Create();

/*
 *清除内存池
 */
int MP_Clear();

/*
 * 销毁内存池
 */
int MP_Destroy();

/*
 * 初始化内存池
 */
void MP_Init();

/*
 * 分配存储单元
 */
DATANODE* MP_AllocNode();

/*
 * 释放存储单元
 */
int MP_FreeNode();

/*
 * 分配块
 */
int MP_AllocBlock();

/*
 * 取分配链表表首
 */
DATANODE* MP_GetAllocNodeHead();

/*
 * 减少节点引用
 */
void MP_DecountReference(DATANODE* pNode);

/*
 * 打印内存池状态
 */
void MP_GetState();

#pragma pack()

#endif /* CMP_H_ */

