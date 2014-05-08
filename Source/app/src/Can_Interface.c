#include <stm32f2xx_conf.h>
#include "CAN_Interface.h"
//#include "type.h"

/**
 *
 */
void CAN_Configuration(void) {
#if 0
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	RCC_ClocksTypeDef RCC_ClocksStatus;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Configure CAN pin: TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	RCC_GetClocksFreq(&RCC_ClocksStatus);
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	// CAN cell init
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_13tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler=RCC_ClocksStatus.PCLK1_Frequency/(16*250000);//娉㈢壒鐜囪绠楋紝16M/锛�+8+7锛�4=250k

	CAN_Init(CAN1, &CAN_InitStructure);
	//CAN filter init
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

	//鏀寔鍙岃矾CAN锛屽垵濮嬪寲CAN2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Configure CAN pin: TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* CAN register init */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_13tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler=RCC_ClocksStatus.PCLK1_Frequency/(16*250000); //娉㈢壒鐜囪绠楋紝16M/锛�+8+7锛�4=250k
	CAN_Init(CAN2, &CAN_InitStructure);
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=14;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
#endif

}

/***********************************************************************************************************
 * 鍔熻兘鎻忚堪锛�CAN搴曞眰鐨勫垵濮嬪寲锛屽寘鎷琁O绠¤剼鍒濆鍖栵紝CAN娉㈢壒鐜囩瓑鍒濆鍖�
 * 鍏�   鍙ｏ細	鏃�
 * 鍑�   鍙ｏ細	鏃�
 * 璇�鏄庯細	閲囨牱鐐圭殑璁剧疆锛�J1939鍗忚瑙勫畾CAN鐨勯噰鏍风偣鍦╗75%,87.5%]浣嶇疆锛屾墍浠ヨ缃椂闂翠负15TSeq,鍚屾璺宠浆瀹藉害涓�
 TSeq,BS1璁剧疆涓�2TSeq锛孊S2璁剧疆涓�TSeq銆�
 ***********************************************************************************************************/
void CAN_LowLevel_Init(void) {
#if 0
	GPIO_InitTypeDef GPIO_InitStructure;

	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	/* CAN GPIOs configuration **************************************************/

	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* Connect CAN pins to AF9 */
	/* Configure CAN pin: RX */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);

	/* CAN configuration ********************************************************/
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = ENABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	// CAN_Mode_LoopBack;//

#if 0
	/* CAN Baudrate = 250KBps (CAN clocked at 18 MHz) */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;	//閲嶆柊鍚屾璺宠穬瀹斤▉(SJW)
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;//閲囨牱鐐逛綅缃�
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;//鍙戦�鐐逛綅缃�
	CAN_InitStructure.CAN_Prescaler = 9;
	CAN_Init(CAN1, &CAN_InitStructure);
#endif

#if 0
	/* CAN Baudrate = 500KBps (CAN clocked at 36 MHz) */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
	CAN_InitStructure.CAN_Prescaler = 3;
	CAN_Init(CAN1, &CAN_InitStructure);
#endif

#if 0
	/* CAN Baudrate = 500KBps (CAN clocked at 36 MHz) */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
	CAN_InitStructure.CAN_Prescaler = 3;
	CAN_Init(CAN1, &CAN_InitStructure);
#endif

#if 1
	/* CAN Baudrate = 500KBps (CAN clocked at 36 MHz) */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = 9;
	CAN_Init(CAN1, &CAN_InitStructure);
#endif

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FMP1, ENABLE);

	//鍏堢姝㈡帴鏀�
	CAN_RecieveDisable();

#endif
}

//鍙戦�CAN娑堟伅
BOOL CAN_Send_Msg(u32 u32MsgId, eCANIDType eIdType, u8* pu8Data,
		u8 u8DataLength) {
#if 0
	CanTxMsg tCanTransmitMsg;
	uint8_t transmit_mailbox;
	u8 ret;

	//鍙傛暟妫�煡
	if(pu8Data == NULL || u8DataLength > 8)
	{
		return FALSE;
	}

	//缁撴瀯浣撴竻绌�
	ZeroMem((u8*)&tCanTransmitMsg,sizeof(CanTxMsg));

	tCanTransmitMsg.StdId = u32MsgId;
	tCanTransmitMsg.ExtId = u32MsgId;

	if(eIdType == eID_STAND)
	{
		tCanTransmitMsg.IDE = CAN_ID_STD;
	}
	else
	{
		tCanTransmitMsg.IDE = CAN_ID_EXT;
	}
	//闈炶繙绋嬪抚
	tCanTransmitMsg.RTR = 0;

	//鏁版嵁闀垮害
	tCanTransmitMsg.DLC = u8DataLength;

	//鎷疯礉鏁版嵁
	memcpy_(tCanTransmitMsg.Data, pu8Data, u8DataLength);

	transmit_mailbox = CAN_Transmit(CAN1, &tCanTransmitMsg);

	if(CAN_NO_MB == transmit_mailbox)
	{
		return FALSE;
	}

	ret = CAN_TransmitStatus(CAN1, transmit_mailbox);
	if(CANTXOK == ret)
	return TRUE;
//	else if(CANTXPENDING == ret)
//		return FALSE;
	if(CANTXFAILED == ret)
	return FALSE;
#endif

	return TRUE;

}

