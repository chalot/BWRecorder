#ifndef _CAN_DRIVER_LAYER_H_
#define _CAN_DRIVER_LAYER_H_

#include "mempool.h"

//#define KWP_DRIVER_LAYER_ENPOOL		/*编译开关，打开使用内存队列*/

/**
 * @Func	发送CAN原始帧
 * @Param	id		[in]	帧ID
 * 			data	[in]	帧数据
 * 			len		[in]	帧数据长度
 * @Ret		无
 */
void CAN_SendRawFrame(u32 id, u8 *data, u8 len);

/**
 * @Func	发送CAN帧
 * @Param	pFrame		[in]	帧内存节点
 * @Ret		无
 */
void CAN_SendFrame(CANRAWFRAME *pFrame);

/**
 * @Func	接收CAN帧
 * @Param	pFrame		[in]	帧内存节点
 * @Ret		无
 */
void CAN_ReceiveFrame(CANRAWFRAME *pFrame);

/**
 * @Func	检查帧ID是否是主动上报帧
 * @Param	canid		[in]	帧ID
 * @Ret		无
 */
//BOOL CAN_CheckIfAutoRptStatusFrame(u32 canid);
/**
 * @Func	检查主动上报帧故障诊断位是否置位
 * @Param	pFrame		[in]	帧内容
 * @Ret		无
 */
//BOOL CAN_IsDTCMarked(u32 canid, u8 *pFrame);
/**
 * @Func	CAN接收帧中断入口，将帧存入内存节点并向外部AO发送新帧到达消息，帧存储地址作为事件参数传递
 * @Param	无
 * @Ret		无
 */
void ISR_CANRecvFrame();

/**
 * @Func	提取接收帧
 * @Param	无
 * @Ret		无
 */
DATANODE* CAN_TakeRxMsg();

/**
 * @Func	检查DTC故障码
 * @Param	pFrame		[in]	CAN帧
 * @Ret		无
 */
//void CAN_CheckDTC(CANRAWFRAME* pFrame);
/**
 * @Func	提取接收帧
 * @Param	无
 * @Ret		无
 */
void CAN_MemInit();

#endif
