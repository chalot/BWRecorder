/*
 * mempool.c
 *
 * �������ݵ�Ԫ�ķ���/�ͷš�

 * ���ڿ������ԣ��Զ������ڴ�������������ڴ����������������ʧ��
 * �̰߳�ȫ�����̷߳���
 * ʱ�临�Ӷ� O(1)
 *
 *  Created on: Nov 16, 2011
 *      Author: gao
 *
 * BUG Report
 * [2011-11-18] �ڴ�������޷�������������в����������ڴ���Ѿ�����
 * ԭ�����һ���ɷ��䵥Ԫ�����������ͷ=NULL����ÿ�η��䶼����������ͷ����������������
 * �޸ģ��ͷ�ʱ���������ͷ�Ƿ�Ϊ�գ�Ϊ��������ͷָ��ǰ���䵥Ԫ
 *
 */

#include "mempool.h"
#include "utility.h"
#include "qp_port.h"
#include <mutex.h>
#include <qevt.h>

Q_DEFINE_THIS_MODULE("mempool.c")

/* Ԥ�����ڴ�飬Ϊ�ڵ����� */
static u8 block[NODES_IN_BLOCK * sizeof(DATANODE)];

/* �����ڴ��ʵ����������� */
static MEMPOOL mp;

/*����������*/
//DECLARE_MUTEX(mux)
/*
 * �ڴ�س�ʼ��
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
 * �����ڴ�أ�Ԥ����һ���顣ֻ�ܵ���һ��
 */
int MP_Create() {
	//Ԥ����һ���洢��
	int ret = MP_AllocBlock(0);

	//�ڴ�ش���ʧ�ܣ����ش���
	if (ret < 0) {
		TRACE_(QS_USER, NULL,
				"\nERROR! mempool create failed!  system should abort");
		return -1;
	}

	//��ʼ������ͷ/β
	mp.m_freeHead = (DATANODE*) mp.m_BlockAddr[0];
	mp.m_freeTail = mp.m_freeHead + NODES_IN_BLOCK - 1;

	mp.m_allocHead = NULL;
	mp.m_allocTail = NULL;

	return 0;
}

/*
 * ��������
 */
int MP_AllocBlock(int blockId) {
	unsigned long allocbytes;
	void* pBlock;
	int i = 0;

	allocbytes = NODES_IN_BLOCK * sizeof(DATANODE);

	pBlock = (void*) &block;

	memset_(pBlock, 0, allocbytes);

	//��ʼ������NODE����
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

	//��β������
	node->next = NULL;

#ifdef _MPOOL_DEBUG_
	*((int*)node->data) = i;
#endif

	//�����ѷ������
	mp.m_AllocBlocks++;

	//������ַ
	mp.m_BlockAddr[blockId] = pBlock;

	mp.m_FreeNodes += NODES_IN_BLOCK;
	mp.m_TotalAllocNodes += NODES_IN_BLOCK;

#ifdef _DEBUG_
	MP_GetState();
#endif

	return 0;
}

/*
 * �����ڴ����
 */
DATANODE* MP_AllocNode() {
	DATANODE* pNode = NULL;
	int blockId = 0;
	int ret = 0;

	MUTEX_LOCK(mux)

	/*����Ϊ�գ���������*/
	if (mp.m_freeHead == NULL) {
		MUTEX_UNLOCK(mux)
		TRACE_(QS_USER, NULL, "\n ERROR! lost node head!");
		return NULL ;
	}

	pNode = mp.m_freeHead;	//ֱ�ӷ��ؿ�������
	mp.m_freeHead = mp.m_freeHead->next; //�����Ƶ�����һ��Ԫ��

	/*�޿���Ԫ�أ���̬�����¿�*/
	if (mp.m_freeHead == NULL) {
		if (mp.m_AllocBlocks == BLOCKS_MAX) //��ǰ�Ѵﵽϵͳ�������������������޷��ٽ����ڴ����
		{
			TRACE_(QS_USER, NULL,
					"\n !!!!! warning! mempool is full, can no longer alloc extra mem!");
			MUTEX_UNLOCK(mux)
			return NULL ;
		}

		blockId = mp.m_AllocBlocks;
		ret = MP_AllocBlock(blockId);	//�����¿�
		if (ret < 0)	//�ڴ�ش���ʧ��
				{
			MUTEX_UNLOCK(mux)
			return NULL ;
		}

		/*�������ӵ��¿�*/
		mp.m_freeHead = (DATANODE*) mp.m_BlockAddr[blockId];
		mp.m_freeTail = mp.m_freeHead + NODES_IN_BLOCK - 1;
	}

	mp.m_AllocNodes++;
	mp.m_FreeNodes--;

	/*���ڵ������������������ӵ�β��*/
#ifdef SUPPORT_DUMP
	if (mp.m_allocHead == NULL)	//��������Ϊ�գ�ͷβָ��ͬһ�ڵ�
	{
		mp.m_allocHead = pNode;
		mp.m_allocTail = pNode;
		pNode->next = NULL;
		pNode->prev = pNode;
	} else //��������ɿգ��ҵ�����β��ʹ��˫������
	{
		mp.m_allocTail->next = pNode;
		pNode->next = NULL;
		pNode->prev = mp.m_allocTail;
		mp.m_allocTail = pNode;
	}

	pNode->reference = 1; //�ڵ�ɹ����䣬���ô�����ʼ��Ϊ1����Ϊ�״�ʹ��
#endif

	MUTEX_UNLOCK(mux)

#ifdef _DEBUG_
//	GetState();
#endif

	return pNode;
}

/*
 * �ͷ�ĳ���ڴ����
 */
int MP_FreeNode(DATANODE *node) {
	MUTEX_LOCK(mux)

	if (node == NULL) //�������
	{
		MUTEX_UNLOCK(mux)
		return -1;
	}

	if (mp.m_freeTail->next != NULL)	//�������󣬲�Ӧ����
	{
		TRACE_(QS_USER, NULL,
				"\n ERROR! Tail is not NULL! something has must been wrong!");
		MUTEX_UNLOCK(mux)
		return -1;
	}

#ifdef SUPPORT_DUMP
	/*�ڵ���������ü�����Ϊ0ʱ�������ͷ�*/
	Q_ASSERT(node->reference > 0); //���ڵ��ͷ�ǰ�ر�ʹ�ù�
	node->reference--;
	if (node->reference > 0) //�ڵ��л��ڱ�ʹ�ã������ͷ�
		return -1;

	/*�ڵ��˳���������*/
	node->prev->next = node->next;
	node->next->prev = node->prev;

#endif

	/*����Ϊ�գ�˵�������ڴ��ѱ������꣬��ͷ�������óɵ�ǰ�ڵ�*/
	if (mp.m_freeHead == NULL) {
		mp.m_freeHead = node;
	}

	mp.m_freeTail->next = node; //�����������
	mp.m_freeTail = node;

	node->prev = NULL;	//���ǰ�������
	node->next = NULL;

	mp.m_AllocNodes--;
	mp.m_FreeNodes++;

	MUTEX_UNLOCK(mux)
	return 0;
}

/*
 * ����ڴ��
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
 * �����ڴ��
 */
int MP_Destroy() {

	return 0;
}

/*
 * ȡ�����������
 */
DATANODE* MP_GetAllocNodeHead() {
	return mp.m_allocHead;
}

/*
 * ���ٽڵ�����
 */
void MP_DecountReference(DATANODE* pNode) {
	if (pNode) {
		if (pNode->reference > 0)
			pNode->reference--;
	}
}

//���Դ���
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

	//��ʼ������NODE����
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

