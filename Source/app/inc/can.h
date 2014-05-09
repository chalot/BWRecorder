/**   
 * @Title: can.h  
 * @Description: TODO(��һ�仰�������ļ���ʲô) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-5-29 ����8:41:31 
 * @version V1.0   
 */

#ifndef CAN_H_
#define CAN_H_

#include <stm32f2xx_conf.h>
#include <stm32_gpio.h>
#include <mempool.h>

//CAN1����
#define CAN1_PWR_ON()		\
					do {	\
						GPIO_SetBits(GPIOC,GPIOC_CAN_CTRL);	\
						GPIO_ResetBits(GPIOC,GPIOC_CAN1_ON);	\
					}while(0)

//CAN2����
#define CAN2_PWR_ON()		\
					do {	\
						GPIO_SetBits(GPIOC,GPIOC_CAN_CTRL);	\
						GPIO_ResetBits(GPIOC,GPIOC_CAN2_ON);	\
					}while(0)
//CAN1����
#define CAN1_PWR_OFF()		\
					do {	\
						GPIO_ResetBits(GPIOC,GPIOC_CAN_CTRL);	\
						GPIO_SetBits(GPIOC,GPIOC_CAN1_ON);	\
					}while(0)

//CAN2����
#define CAN2_PWR_OFF()		\
					do {	\
						GPIO_ResetBits(GPIOC,GPIOC_CAN_CTRL);	\
						GPIO_SetBits(GPIOC,GPIOC_CAN2_ON);	\
					}while(0)

//����ָʾ�ƴ�/�ر�
#define CAN_LED_ON()  GPIO_ResetBits(GPIOA, GPIOA_CAN_LED_)
#define CAN_LED_OFF()  GPIO_SetBits(GPIOA, GPIOA_CAN_LED_)

//LEDָʾ����˸
#define CAN_FLASH_LED()  \
                            GPIO_WriteBit(GPIOA, GPIOA_CAN_LED_,   \
                            (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOA, GPIOA_CAN_LED_))))


/**
  * @brief  ��ֹCAN1���գ��رս����ж�
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
  * @brief  ʹ��CAN1���գ��򿪽����ж�
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
 * ����CANԭʼ֡
 * @param id		֡ID
 * @param data		֡����
 * @param len		֡���ݳ���
 */
void CAN_SendRawFrame(u32 id, u8 *data, u8 len);

/**
 * ����CAN֡
 * @param pFrame
 */
void CAN_SendFrame(CANRAWFRAME *pFrame);

/**
 * ����CAN֡
 * @param pFrame		֡�ڴ�ڵ�
 */
void CAN_ReceiveFrame(CANRAWFRAME *pFrame);

/**
 * CAN����֡�ж���ڣ���֡�����ڴ�ڵ㲢���ⲿAO������֡������Ϣ��֡�洢��ַ��Ϊ�¼���������
 */
void ISR_CANRecvFrame();

/**
 * ��ȡ����֡
 * @return		֡�ڴ�ڵ�
 */
DATANODE* CAN_TakeRxMsg();

/**
 * ��ȡ����֡
 */
void CAN_MemInit();

/**
 * ��ӡ֡��ϸ��Ϣ
 *
 * @param pFrame		֡����
 */
void TRACE_CANRawFrame(CANRAWFRAME *pFrame);

#endif /* CAN_H_ */
