#ifndef _GPS_VDR_CAN_INTERFACE_H__
#define _GPS_VDR_CAN_INTERFACE_H__

#include <stm32f2xx_conf.h>
#include <mempool.h>

#pragma		pack(1)
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_CAN1
/* #define USE_CAN2 */


  #define CAN_CLK                    RCC_APB1Periph_CAN1
  #define CAN_RX_PIN                 GPIO_Pin_8
  #define CAN_TX_PIN                 GPIO_Pin_9
  #define CAN_GPIO_PORT              GPIOB
  #define CAN_GPIO_CLK               RCC_APB2Periph_GPIOB
  #define CAN_AF_PORT                GPIO_AF_CAN1
  #define CAN_RX_SOURCE              GPIO_PinSource8
  #define CAN_TX_SOURCE              GPIO_PinSource9       



typedef struct
{
 
  u32 u32ExtId;  	/*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */
  u8  au8Data[8]; 		/*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */
} CanAppMsg;

typedef		enum
{
	eID_STAND	= 0,		//标准ID
	eID_EXPAND
}eCANIDType;

///*CAN帧存储格式，在原始帧信息基础上添加了必要的字段（dir方向，tick时刻），便于存储*/
//typedef struct _CANRAWFRAME
//{
//	u32 id;				/*ID*/
//	u8	dir		:	1;	/*收发方向，0：Tx，1：Rx*/
//	u8	len		:	7;	/*帧长度*/
//	u8  data[8];		/*帧数据*/
//	u32	tick;			/*收发时刻，单位0.1ms。发送时，填充实际发送时刻值；接收时，填充实际接收时刻值*/
//} CANRAWFRAME;


BOOL CAN_Send_Msg(u32 u32MsgId, eCANIDType eIdType, u8* pu8Data, u8 u8DataLength);

/**
 * @Func	打印帧详细信息
 * @Param	pFrame		[in]	帧数据
 * @Ret		无
 */
void TRACE_CANRawFrame(CANRAWFRAME *pFrame);

//CAN接收中断函数
void ISR_CAN1_Receive();


void CAN_LowLevel_Init();


#pragma		pack()

#endif  //_GPS_VDR_CAN_INTERFACE_H__

