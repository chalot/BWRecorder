/*
 *	CAN������
 *
 * ���ܣ������ϲ��CAN֡�������͡�ԭʼ����֡���ϲ㴫�ݡ�
 * ˵����1��Ӳ����������ܶ�Ӧ����߼�������·�㣨1��N����
 2������������ϲ�������·����²�CAN�����֮�䡣ӵ���������������ջ����ͷ��ͻ�����
 ������·�㷢�͹��̣����������������뷢�Ͷ��У�CAN����֡�ϲ�֪ͨ������֡����ն��У�
 ���ύ���ݸ�������·�㡣
 ע�⣺���еĶ�����CAN֡����ڵ���ͳһ���ڴ��������ģ�������ֻ����ڵ�ָ�룬������ʵ��CAN֡��

 �ڴ������ԣ�
 ����֡����������·��Է������ݽ��б�Ҫ�ķֿ飬����ǰ�����ڴ��������һ���ڴ�ڵ㣬��䷢��֡��Ϣ��
 ��������ķ��Ͷ��У����������첽�������һ֡����ʽ���ڴ���ͷŸýڵ㡣
 ����֡����⵽Ӳ��CAN֡��������Ϣ�󣬴��ڴ������һ���ڵ㣬��ȡCAN֡�����浽�ڵ㣬��ڵ�����ն��У�
 ���ʵ���ʱ��ʱ�����������֡ID����֡ת�����ϲ���Ӧ��������·�㴦����������·���������Ȳ�
 �����ͷš�

 [�޸�] 2013-05-29
 ��·�㲻�ٱ����ڲ��������У�����ͳһֱ�ӷ����ڴ�ء��շ����ݵ�ַ��Ϊ�źŲ������ݣ���QP��ܵ�AO�����
 ���¼����У����ʵ����Ҳ���Ǳ���ؽ�ԭ�����滻��AO���¼����С�����������ʡ����·�㱾��ά�����еĴ��ۡ�


 */

#include <stm32f2xx_conf.h>
#include <qp_port.h>
#include <mempool.h>
#include <can.h>
#include <systimer.h>
#include <can_interface.h>
#include <qevents.h>
#include <qactiveobjs.h>
#include <trace.h>
#include <bsp.h>
#include <qevt.h>
#include <type.h>

Q_DEFINE_THIS_MODULE("can.c")

/*���뿪�أ�����ʾSD���洢��Ϣ*/
#define		DEBUG_SD


#define NODE_MAX	100  //֡�����С
typedef struct {	u16 read;	//��ָ��
	u16 write;	//дָ��
	void* NodeList[NODE_MAX];	//�ڵ�����
} NODELIST; //�ڵ�����

static NODELIST nodeList; //�ڵ�洢����̬����

/**
 * @Func	����CANԭʼ֡
 * @Param	id		[in]	֡ID
 * 			data	[in]	֡����
 * 			len		[in]	֡���ݳ���
 * @Ret		��
 */
void CAN_SendRawFrame(u32 id, u8 *data, u8 len) {
	/*���õײ��ִ�������ͣ�δ������ֵ*/
	CAN_Send_Msg(id, CAN_ID_STD, data, len);
}

/**
 * @Func	����CAN֡����ʱ���Ͳ����壬�ɵ����߿��Ʒ���ʱ��
 * @Param	pFrame		[in]	֡�ڴ�ڵ�
 * @Ret		��
 */
void CAN_SendFrame(CANRAWFRAME *pFrame) {
	BOOL bret;
	//��ӡԭʼ֡����
	#ifdef TRACERAWFRAMEINFO
	//	TRACE_CANRawFrameInfo();
	#endif

	/*���õײ��ִ�������ͣ�δ������ֵ*/
	bret = CAN_Send_Msg(pFrame->id, CAN_ID_STD, pFrame->data, pFrame->len);
//
//	//���ü����CAN���͹���
//	if(!bret)
//	{
////		SetErrType(CAN_CLASS, CAN_FAULT_TX);	//���÷��͹���
//	}
//	else
//	{	//����������������͹���
//		if(CheckSpecifiedErrTypeExisted(CAN_CLASS, CAN_FAULT_TX))
//		{
//			ClearErrType(CAN_CLASS, CAN_FAULT_TX);
//		}
//	}
}

/**
 * @Func	����CAN֡
 * @Param	pFrame		[in]	֡�ڴ�ڵ�
 * @Ret		��
 */
void CAN_ReceiveFrame(CANRAWFRAME *pFrame) {
	static CanRxMsg RxMessage;
	u8 i;
	TIME time;

	/*�����ʱ����*/
	ZeroMem((u8*) &RxMessage, sizeof(CanRxMsg));

	/*�������CAN֡*/
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	/*CAN ID*/
	if (RxMessage.IDE == CAN_ID_EXT) {
		pFrame->id = RxMessage.ExtId;
	}
	else {
		pFrame->id = RxMessage.StdId;
	}

	/*��������*/
	for (i = 0; i < 8; i++) {
		pFrame->data[i] = RxMessage.Data[i];
	}

	SysTick_Get(&time); //ȡϵͳʱ��

	/*���֡������Ϣ*/
	pFrame->dir = 1; /*���շ���*/
	pFrame->tick = (time.minute * 60 + time.second) * 1000 + (GetCurTicks() * 10); /*��ǰʱ��*/
}

/**
 * @Func	CAN����֡�ж���ڣ���֡�����ڴ�ڵ�����ⲿAO������֡������Ϣ��֡�洢��ַ��Ϊ�¼���������
 * @Param	��
 * @Ret		��
 */
void ISR_CANRecvFrame() {
	CANRAWFRAME *pFrame;
	DATANODE *pNode;
	static const QEvent msgEvt = { CAN_NEWRECVMSG_SIG, 0 };

	/*�����ڴ�ڵ�*/
	pNode = MP_AllocNode();
	if (pNode) /*���뵽һ�������ڴ�ڵ㣬���Ա���֡����*/
	{
		/*��ȡԭʼ֡����ת���ɴ洢��ʽ*/
		pFrame = (CANRAWFRAME*) pNode->data;

		/* ----- ����㺯��������ԭʼ֡ ----- */
		CAN_ReceiveFrame(pFrame);

		nodeList.NodeList[nodeList.write++] = pNode;	//�ڵ��뻺��
		if (nodeList.write >= NODE_MAX)
			nodeList.write = 0;

		pNode->reference++; //���Ӵ洢���ü���
	//		SD_RequestDumpFrame(pNode);
		QACTIVE_POST(AO_Can, (QEvt* ) &msgEvt, (void*) 0); //������֡������Ϣ��ʹ�þ�̬�¼��������¼�������
	}
}

#if 0
/**
 * @Func	CAN����֡�ж���ڣ���֡�����ڴ�ڵ�����ⲿAO������֡������Ϣ��֡�洢��ַ��Ϊ�¼���������
 * @Param	��
 * @Ret		��
 */
void ISR_CANRecvFrame()
{
CANRAWFRAME *pFrame;
DATANODE *pNode;
CANMsgEvt *te;

/*�����ڴ�ڵ�*/
pNode = MP_AllocNode();
Q_ASSERT(pNode);
if(pNode) /*���뵽һ�������ڴ�ڵ㣬���Ա���֡����*/
{
	/*��ȡԭʼ֡����ת���ɴ洢��ʽ*/
	pFrame = (CANRAWFRAME*)pNode->data;

	/* ----- ����㺯��������ԭʼ֡ ----- */
	CAN_ReceiveFrame(pFrame);

	/*���ͽ���һ����֡��Ϣ��֡�洢��ַ��Ϊ�¼���������*/
	te = Q_NEW(CANMsgEvt, CAN_NEWRECVMSG_SIG);
	Q_ASSERT(te);
	if(te)
	{
		te->pNode = pNode;
		QACTIVE_POST(AO_SessionMgr, (QEvt*)te, NULL);
//			QACTIVE_POST(AO_Session[0], (QEvt*)te, NULL);

//			CAN_FLASH_LED;
	}
}
}
#endif

/**
 * @Func	��ȡ����֡
 * @Param	��
 * @Ret		��
 */
void CAN_MemInit() {
ZeroMem((u8*) &nodeList, sizeof(NODELIST));
}

/**
 * @Func	��ȡ����֡
 * @Param	��
 * @Ret		��
 */
DATANODE* CAN_TakeRxMsg() {
u16 _write;
DATANODE *pNode;

_write = nodeList.write;

if (nodeList.read == _write) {
	return NULL ;
}

pNode = (DATANODE*) nodeList.NodeList[nodeList.read];
Q_ASSERT(pNode != NULL);
nodeList.read++;
if (nodeList.read >= NODE_MAX)
	nodeList.read = 0;

return pNode;
}

/**
 * ��ӡ֡��ϸ��Ϣ
 *
 * @param pFrame	[in]	֡����
 */
void TRACE_CANRawFrame(CANRAWFRAME *pFrame) {
TRACE_(CAN_STAT, NULL, "[RAWFRAMEINFO] %d, %x, %s, %d, %x %x %x %x %x %x %x %x ", pFrame->tick, pFrame->id,
		(pFrame->dir == 0) ? "Tx" : "Rx", pFrame->len, pFrame->data[0], pFrame->data[1], pFrame->data[2],
		pFrame->data[3], pFrame->data[4], pFrame->data[5], pFrame->data[6], pFrame->data[7]);
}
