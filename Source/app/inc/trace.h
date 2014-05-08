/**   
* @Title: trace.h  
* @Description: TODO(用一句话描述该文件做什么) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-4 上午11:37:02 
* @version V1.0   
*/ 


#ifndef TRACE_H_
#define TRACE_H_

#include <stm32f2xx_conf.h>
#include "qp_port.h"

#define UART_TRACE  USART2 	//打印信息输出串口号

#define TRACE_BUFSIZE	512		//打印串口缓区容量

/*输出单个字符*/
//#define PUTCHAR(c) Trace_SendData((u8*)&c, 1)
/*输出字符串*/
//#define PUTSTRING(c) Trace_SendData((u8*)&c[0], strlen(c))

/*不用库函数实现可变长打印输出函数*/
//
/**
 * @Func	格式化字符串
 * @Param	pBuf		[in]	目标缓区
 * 			format		[in]	至少一个固定参数，此处为格式化字符串
 * 			...			[in]	变成参数
 * @Ret		无
 */
void sprintf_(u8 *pBuf, const char *format, ...);

/**
 * @Func	打印帧详细信息
 * @Param	eState		[in]	记录类型
 * 			pAO			[in]	状态机
 * 			format		[in]	格式化字符串
 * @Ret		无
 */
void TRACE_(u8 eState, QActive* pAO, const char *format, ...);



/**
 * @Func	不用库函数实现可变长打印输出函数
 * @Param	format		[in]	至少一个固定参数，此处为格式化字符串
 * 			...			[in]	变成参数
 * @Ret		无
 */
void TRACE(const char *format, ...);

/**
 * @Func	打印帧详细信息
 * @Param	pFrame		[in]	帧数据
 * @Ret		无
 */
//void TRACE_CANRawFrame(CANRAWFRAME *pFrame);

void TRACE_RAW_BUFFER(u8 *pMsg, u16 msgLen);

#if 0
 /**
  * @Func	打印帧详细信息
  * @Param	pFrame		[in]	帧数据
  * @Ret		无
  */
 void TRACE_CANRawFrame(CANRAWFRAME *pFrame);

/**
 * @Func	打印串口配置
 * @Param	无
 * @Ret		无
 */
void Trace_configuration();

/**
 * @Func	打印输出，如超过打印缓区剩余容量，则截取，尾部丢掉
 * @Param	pData		[in]	待打印数据
 * 			len 		[in]	待打印数据长度
 * @Ret		无
 */
void Trace_SendData(u8 *pData , u16 len);

/**
 * @Func	串口数据发送中断入口，采用中断方式将发送缓区内的数据发送完
 * @Param	无
 * @Ret		无
 */
void ISR_UART_TRACE(void);

#endif

#endif /* TRACE_H_ */
