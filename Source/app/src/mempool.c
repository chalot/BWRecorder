/*
 * mempool.c
 *
 * 定长数据单元的分配/释放。

 * 基于块分配策略，自动扩充内存池容量，超过内存池最大容量，则分配失败
 * 线程安全，多线程访问
 * 时间复杂度 O(1)
 *
 *  Created on: Nov 16, 2011
 *      Author: gao
 *
 * BUG Report
 * [2011-11-18] 内存池满后，无法继续进行入队列操作，哪怕内存池已经读空
 * 原因：最后一个可分配单元被分配后，链接头=NULL，而每次分配都仅返回链接头，因而造成这种现象
 * 修改：释放时，检查链接头是否为空，为空则将链接头指向当前分配单元
 *
 */

#include "mempool.h"
#include "utility.h"
#include "qp_port.h"
#include <mutex.h>
#include <qevt.h>

Q_DEFINE_THIS_MODULE("mempool.c")

/* 预分配内存块，为节点数组 */
static u8 block[NODES_IN_BLOCK * sizeof(DATANODE)];

/* 本地内存池实例，互斥访问 */
static MEMPOOL mp;

/*声明互斥量*/
//DECLARE_MUTEX(mux)
/*
 * 内存池初始化
 */
void MP_Init() {
	u8 i = 0;
	mp.m_freeHead = NULL;
	mp.m_freeTail = NULL;
	mp.m_allocHead = NULL;
	mp.m_allocTail = NULL;

	mp.m_AllocNodes = 0;
	mp.m_FreeNodes = 0;
	mp.m_TotalAllocNodes = 0;

	mp.m_AllocBlocks = 0;

	for (i = 0; i < BLOCKS_MAX; i++)
		mp.m_BlockAddr[i] = NULL;
}

/*
 * 创建内存池，预分配一个块。只能调用一次
 */
int MP_Create() {
	//预申请一个存储块
	int ret = MP_AllocBlock(0);

	//内存池创建失败，严重错误
	if (ret < 0) {
		TRACE_(QS_USER, NULL,
				"\nERROR! mempool create failed!  system should abort");
		return -1;
	}

	//初始化链接头/尾
	mp.m_freeHead = (DATANODE*) mp.m_BlockAddr[0];
	mp.m_freeTail = mp.m_freeHead + NODES_IN_BLOCK - 1;

	mp.m_allocHead = NULL;
	mp.m_allocTail = NULL;

	return 0;
}

/*
 * 请求分配块
 */
int MP_AllocBlock(int blockId) {
	unsigned long allocbytes;
	void* pBlock;
	int i = 0;

	allocbytes = NODES_IN_BLOCK * sizeof(DATANODE);

	pBlock = (void*) &block;

	memset_(pBlock, 0, allocbytes);

	//初始化块内NODE链表
	DATANODE *node = (DATANODE *) pBlock;

	for (i = 0; i < NODES_IN_BLOCK - 1; i++) {
#ifdef _MPOOL_DEBUG_
		*((int*)node->data) = i;
#endif
		node->next = (node + 1);

#ifdef SUPPORT_DUMP
		node->prev = NULL;
		node->reference = 0;
#endif
		node++;
	}

	//块尾无链接
	node->next = NULL;

#ifdef _MPOOL_DEBUG_
	*((int*)node->data) = i;
#endif

	//更新已分配块数
	mp.m_AllocBlocks++;

	//保存块地址
	mp.m_BlockAddr[blockId] = pBlock;

	mp.m_FreeNodes += NODES_IN_BLOCK;
	mp.m_TotalAllocNodes += NODES_IN_BLOCK;

#ifdef _DEBUG_
	MP_GetState();
#endif

	return 0;
}

/*
 * 申请内存对象
 */
DATANODE* MP_AllocNode() {
	DATANODE* pNode = NULL;
	int blockId = 0;
	int ret = 0;

	MUTEX_LOCK(mux)

	/*链首为空，致命错误*/
	if (mp.m_freeHead == NULL) {
		MUTEX_UNLOCK(mux)
		TRACE_(QS_USER, NULL, "\n ERROR! lost node head!");
		return NULL ;
	}

	pNode = mp.m_freeHead;	//直接返回空闲链首
	mp.m_freeHead = mp.m_freeHead->next; //链首移到到下一个元素

	/*无可用元素，动态扩充新块*/
	if (mp.m_freeHead == NULL) {
		if (mp.m_AllocBlocks == BLOCKS_MAX) //当前已达到系统允许创建的最大块数，则无法再进行内存分配
		{
			TRACE_(QS_USER, NULL,
					"\n !!!!! warning! mempool is full, can no longer alloc extra mem!");
			MUTEX_UNLOCK(mux)
			return NULL ;
		}

		blockId = mp.m_AllocBlocks;
		ret = MP_AllocBlock(blockId);	//创建新块
		if (ret < 0)	//内存池创建失败
				{
			MUTEX_UNLOCK(mux)
			return NULL ;
		}

		/*重新链接到新块*/
		mp.m_freeHead = (DATANODE*) mp.m_BlockAddr[blockId];
		mp.m_freeTail = mp.m_freeHead + NODES_IN_BLOCK - 1;
	}

	mp.m_AllocNodes++;
	mp.m_FreeNodes--;

	/*将节点加入分配链表，总是链接到尾部*/
#ifdef SUPPORT_DUMP
	if (mp.m_allocHead == NULL)	//分配链表为空，头尾指向同一节点
	{
		mp.m_allocHead = pNode;
		mp.m_allocTail = pNode;
		pNode->next = NULL;
		pNode->prev = pNode;
	} else //分配链表飞空，挂到链表尾，使用双向链表
	{
		mp.m_allocTail->next = pNode;
		pNode->next = NULL;
		pNode->prev = mp.m_allocTail;
		mp.m_allocTail = pNode;
	}

	pNode->reference = 1; //节点成功分配，引用次数初始化为1，意为首次使用
#endif

	MUTEX_UNLOCK(mux)

#ifdef _DEBUG_
//	GetState();
#endif

	return pNode;
}

/*
 * 释放某个内存对象
 */
int MP_FreeNode(DATANODE *node) {
	MUTEX_LOCK(mux)

	if (node == NULL) //参数检查
	{
		MUTEX_UNLOCK(mux)
		return -1;
	}

	if (mp.m_freeTail->next != NULL)	//致命错误，不应出现
	{
		TRACE_(QS_USER, NULL,
				"\n ERROR! Tail is not NULL! something has must been wrong!");
		MUTEX_UNLOCK(mux)
		return -1;
	}

#ifdef SUPPORT_DUMP
	/*节点必须在引用计数器为0时才允许释放*/
	Q_ASSERT(node->reference > 0); //检查节点释放前必被使用过
	node->reference--;
	if (node->reference > 0) //节点尚还在被使用，不能释放
		return -1;

	/*节点退出分配链表*/
	node->prev->next = node->next;
	node->next->prev = node->prev;

#endif

	/*链首为空，说明所有内存已被分配完，将头重新设置成当前节点*/
	if (mp.m_freeHead == NULL) {
		mp.m_freeHead = node;
	}

	mp.m_freeTail->next = node; //加入空闲链表
	mp.m_freeTail = node;

	node->prev = NULL;	//清空前驱、后继
	node->next = NULL;

	mp.m_AllocNodes--;
	mp.m_FreeNodes++;

	MUTEX_UNLOCK(mux)
	return 0;
}

/*
 * 清空内存池
 */
int MP_Clear() {

	return 0;

}

void MP_GetState() {
#if 1
	TRACE_(QS_USER, NULL,
			"-------- MEMPOOL: allocNodes = %d, freeNodes = %d, totalNodes = %d",
			mp.m_AllocNodes, mp.m_FreeNodes, mp.m_TotalAllocNodes);
#endif
}

/*
 * 销毁内存池
 */
int MP_Destroy() {

	return 0;
}

/*
 * 取分配链表表首
 */
DATANODE* MP_GetAllocNodeHead() {
	return mp.m_allocHead;
}

/*
 * 减少节点引用
 */
void MP_DecountReference(DATANODE* pNode) {
	if (pNode) {
		if (pNode->reference > 0)
			pNode->reference--;
	}
}

//测试代码
#if 0

int main()
{
	CMemPool pool;
	int ret = pool.Create();
	if(ret < 0)
	{
		TRACE_(QS_USER, NULL, "\npool create failed!");
		return -1;
	}

	//初始化块内NODE链表
	DATANODE *node = pool.mp.m_freeHead;

	while(node)
	{
#ifdef _MPOOL_DEBUG_
		TRACE_(QS_USER, NULL, "\n [%d] node ", *((int*)node->data));
#endif
		node = node->next;
	}

	unsigned long i = 0;
	while(1)
	{
		DATANODE *node = pool.AllocNode();
		if(node == NULL)
		{
			TRACE_(QS_USER, NULL, "\n !!!! alloc error!");
			break;
		}

		TRACE_(QS_USER, NULL, "\n[%ld] alloc times", i++);

		pool.GetState();

		pool.FreeNode(node);

		TRACE_(QS_USER, NULL, "\n");
		pool.GetState();
	}

	pool.Destroy();

	return 0;
}

#endif

