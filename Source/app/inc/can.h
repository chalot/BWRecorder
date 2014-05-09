/**   
 * @Title: can.h  
 * @Description: TODO(用一句话描述该文件做什么) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-5-29 上午8:41:31 
 * @version V1.0   
 */

#ifndef CAN_H_
#define CAN_H_

#include <stm32f2xx_conf.h>
#include <stm32_gpio.h>
#include <mempool.h>

//CAN1开电
#define CAN1_PWR_ON()		\
					do {	\
						GPIO_SetBits(GPIOC,GPIOC_CAN_CTRL);	\
						GPIO_ResetBits(GPIOC,GPIOC_CAN1_ON);	\
					}while(0)

//CAN2开电
#define CAN2_PWR_ON()		\
					do {	\
						GPIO_SetBits(GPIOC,GPIOC_CAN_CTRL);	\
						GPIO_ResetBits(GPIOC,GPIOC_CAN2_ON);	\
					}while(0)
//CAN1开电
#define CAN1_PWR_OFF()		\
					do {	\
						GPIO_ResetBits(GPIOC,GPIOC_CAN_CTRL);	\
						GPIO_SetBits(GPIOC,GPIOC_CAN1_ON);	\
					}while(0)

//CAN2开电
#define CAN2_PWR_OFF()		\
					do {	\
						GPIO_ResetBits(GPIOC,GPIOC_CAN_CTRL);	\
						GPIO_SetBits(GPIOC,GPIOC_CAN2_ON);	\
					}while(0)

//网络指示灯打开/关闭
#define CAN_LED_ON()  GPIO_ResetBits(GPIOA, GPIOA_CAN_LED_)
#define CAN_LED_OFF()  GPIO_SetBits(GPIOA, GPIOA_CAN_LED_)

//LED指示灯闪烁
#define CAN_FLASH_LED()  \
                            GPIO_WriteBit(GPIOA, GPIOA_CAN_LED_,   \
                            (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOA, GPIOA_CAN_LED_))))


/**
  * @brief  禁止CAN1接收，关闭接收中断
  *
  * @param  CANx: where x can be 1 or 2 to to select the CAN peripheral.
  * @param  CAN_IT: specifies the CAN interrupt sources to be enabled or disabled.
  *   This parameter can be: CAN_IT_TME, CAN_IT_FMP0, CAN_IT_FF0,
  *   CAN_IT_FOV0, CAN_IT_FMP1, CAN_IT_FF1,
  *   CAN_IT_FOV1, CAN_IT_EWG, CAN_IT_EPV,
  *   CAN_IT_LEC, CAN_IT_ERR, CAN_IT_WKU or
  *   CAN_IT_SLK.
  * @param  NewState: new state of the CAN interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
#define CAN_RECIEVE_DISABLE()		CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FMP1, DISABLE)


/**
  * @brief  使能CAN1接收，打开接收中断
  *
  * @param  CANx: where x can be 1 or 2 to to select the CAN peripheral.
  * @param  CAN_IT: specifies the CAN interrupt sources to be enabled or disabled.
  *   This parameter can be: CAN_IT_TME, CAN_IT_FMP0, CAN_IT_FF0,
  *   CAN_IT_FOV0, CAN_IT_FMP1, CAN_IT_FF1,
  *   CAN_IT_FOV1, CAN_IT_EWG, CAN_IT_EPV,
  *   CAN_IT_LEC, CAN_IT_ERR, CAN_IT_WKU or
  *   CAN_IT_SLK.
  * @param  NewState: new state of the CAN interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
#define CAN_RECIEVE_ENABLE()		CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FMP1, ENABLE)

/**
 * 发送CAN原始帧
 * @param id		帧ID
 * @param data		帧数据
 * @param len		帧数据长度
 */
void CAN_SendRawFrame(u32 id, u8 *data, u8 len);

/**
 * 发送CAN帧
 * @param pFrame
 */
void CAN_SendFrame(CANRAWFRAME *pFrame);

/**
 * 接收CAN帧
 * @param pFrame		帧内存节点
 */
void CAN_ReceiveFrame(CANRAWFRAME *pFrame);

/**
 * CAN接收帧中断入口，将帧存入内存节点并向外部AO发送新帧到达消息，帧存储地址作为事件参数传递
 */
void ISR_CANRecvFrame();

/**
 * 提取接收帧
 * @return		帧内存节点
 */
DATANODE* CAN_TakeRxMsg();

/**
 * 提取接收帧
 */
void CAN_MemInit();

/**
 * 打印帧详细信息
 *
 * @param pFrame		帧数据
 */
void TRACE_CANRawFrame(CANRAWFRAME *pFrame);

#endif /* CAN_H_ */
