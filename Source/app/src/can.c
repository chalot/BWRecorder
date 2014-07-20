/*
 *	CAN驱动层
 *
 * 功能：负责上层的CAN帧的物理发送、原始接收帧的上层传递。
 * 说明：1、硬件驱动层可能对应多个逻辑数据链路层（1：N）。
 2、驱动层介于上层数据链路层和下层CAN物理层之间。拥有两个缓区，接收缓区和发送缓区。
 数据链路层发送过程：调用驱动层请求，入发送队列；CAN物理帧上层通知：物理帧入接收队列，
 并提交数据给数据链路层。
 注意：所有的队列中CAN帧保存节点是统一从内存池中申请的，队列中只保存节点指针，而不是实际CAN帧。

 内存管理策略：
 发送帧：由数据链路层对发送数据进行必要的分块，发送前，从内存池中申请一个内存节点，填充发送帧信息，
 入驱动层的发送队列；在驱动层异步发送完成一帧后，显式向内存池释放该节点。
 接收帧：检测到硬件CAN帧到来的消息后，从内存池申请一个节点，读取CAN帧并保存到节点，后节点入接收队列；
 在适当的时机时，驱动层根据帧ID，将帧转发到上层相应的数据链路层处理。在数据链路层进行组包等操
 作后释放。

 [修改] 2013-05-29
 链路层不再保留内部两个队列，而是统一直接访问内存池。收发数据地址作为信号参数传递，因QP框架的AO本身带
 有事件队列，因此实际上也就是变相地将原队列替换成AO的事件队列。这样做，节省了链路层本身维护队列的代价。


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

/*编译开关，打开显示SD卡存储信息*/
#define		DEBUG_SD


#define NODE_MAX	100  //帧缓冲大小
typedef struct {	u16 read;	//读指针
	u16 write;	//写指针
	void* NodeList[NODE_MAX];	//节点数组
} NODELIST; //节点数组

static NODELIST nodeList; //节点存储，静态变量

/**
 * @Func	发送CAN原始帧
 * @Param	id		[in]	帧ID
 * 			data	[in]	帧数据
 * 			len		[in]	帧数据长度
 * @Ret		无
 */
void CAN_SendRawFrame(u32 id, u8 *data, u8 len) {
	/*调用底层库执行物理发送，未处理返回值*/
	CAN_Send_Msg(id, CAN_ID_STD, data, len);
}

/**
 * @Func	发送CAN帧，即时发送不缓冲，由调用者控制发送时机
 * @Param	pFrame		[in]	帧内存节点
 * @Ret		无
 */
void CAN_SendFrame(CANRAWFRAME *pFrame) {
	BOOL bret;
	//打印原始帧内容
	#ifdef TRACERAWFRAMEINFO
	//	TRACE_CANRawFrameInfo();
	#endif

	/*调用底层库执行物理发送，未处理返回值*/
	bret = CAN_Send_Msg(pFrame->id, CAN_ID_STD, pFrame->data, pFrame->len);
//
//	//设置及清除CAN发送故障
//	if(!bret)
//	{
////		SetErrType(CAN_CLASS, CAN_FAULT_TX);	//设置发送故障
//	}
//	else
//	{	//发送正常，清除发送故障
//		if(CheckSpecifiedErrTypeExisted(CAN_CLASS, CAN_FAULT_TX))
//		{
//			ClearErrType(CAN_CLASS, CAN_FAULT_TX);
//		}
//	}
}

/**
 * @Func	接收CAN帧
 * @Param	pFrame		[in]	帧内存节点
 * @Ret		无
 */
void CAN_ReceiveFrame(CANRAWFRAME *pFrame) {
	static CanRxMsg RxMessage;
	u8 i;
	TIME time;

	/*清空临时变量*/
	ZeroMem((u8*) &RxMessage, sizeof(CanRxMsg));

	/*物理接收CAN帧*/
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	/*CAN ID*/
	if (RxMessage.IDE == CAN_ID_EXT) {
		pFrame->id = RxMessage.ExtId;
	}
	else {
		pFrame->id = RxMessage.StdId;
	}

	/*保存数据*/
	for (i = 0; i < 8; i++) {
		pFrame->data[i] = RxMessage.Data[i];
	}

	SysTick_Get(&time); //取系统时间

	/*填充帧附加信息*/
	pFrame->dir = 1; /*接收方向*/
	pFrame->tick = (time.minute * 60 + time.second) * 1000 + (GetCurTicks() * 10); /*当前时刻*/
}

/**
 * @Func	CAN接收帧中断入口，将帧存入内存节点后，向外部AO发送新帧到达消息，帧存储地址作为事件参数传递
 * @Param	无
 * @Ret		无
 */
void ISR_CANRecvFrame() {
	CANRAWFRAME *pFrame;
	DATANODE *pNode;
	static const QEvent msgEvt = { CAN_NEWRECVMSG_SIG, 0 };

	/*申请内存节点*/
	pNode = MP_AllocNode();
	if (pNode) /*申请到一个空闲内存节点，可以保存帧数据*/
	{
		/*读取原始帧，并转换成存储格式*/
		pFrame = (CANRAWFRAME*) pNode->data;

		/* ----- 物理层函数，接收原始帧 ----- */
		CAN_ReceiveFrame(pFrame);

		nodeList.NodeList[nodeList.write++] = pNode;	//节点入缓区
		if (nodeList.write >= NODE_MAX)
			nodeList.write = 0;

		pNode->reference++; //增加存储引用计数
	//		SD_RequestDumpFrame(pNode);
		QACTIVE_POST(AO_Can, (QEvt* ) &msgEvt, (void*) 0); //发送新帧到达消息，使用静态事件，减少事件池容量
	}
}

#if 0
/**
 * @Func	CAN接收帧中断入口，将帧存入内存节点后，向外部AO发送新帧到达消息，帧存储地址作为事件参数传递
 * @Param	无
 * @Ret		无
 */
void ISR_CANRecvFrame()
{
CANRAWFRAME *pFrame;
DATANODE *pNode;
CANMsgEvt *te;

/*申请内存节点*/
pNode = MP_AllocNode();
Q_ASSERT(pNode);
if(pNode) /*申请到一个空闲内存节点，可以保存帧数据*/
{
	/*读取原始帧，并转换成存储格式*/
	pFrame = (CANRAWFRAME*)pNode->data;

	/* ----- 物理层函数，接收原始帧 ----- */
	CAN_ReceiveFrame(pFrame);

	/*发送接收一个新帧消息，帧存储地址作为事件参数传递*/
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
 * @Func	提取接收帧
 * @Param	无
 * @Ret		无
 */
void CAN_MemInit() {
ZeroMem((u8*) &nodeList, sizeof(NODELIST));
}

/**
 * @Func	提取接收帧
 * @Param	无
 * @Ret		无
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
 * 打印帧详细信息
 *
 * @param pFrame	[in]	帧数据
 */
void TRACE_CANRawFrame(CANRAWFRAME *pFrame) {
TRACE_(CAN_STAT, NULL, "[RAWFRAMEINFO] %d, %x, %s, %d, %x %x %x %x %x %x %x %x ", pFrame->tick, pFrame->id,
		(pFrame->dir == 0) ? "Tx" : "Rx", pFrame->len, pFrame->data[0], pFrame->data[1], pFrame->data[2],
		pFrame->data[3], pFrame->data[4], pFrame->data[5], pFrame->data[6], pFrame->data[7]);
}
