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
	CAN_InitStructure.CAN_Prescaler=RCC_ClocksStatus.PCLK1_Frequency/(16*250000);//濞夈垻澹掗悳鍥吀缁犳绱�6M/閿涳拷+8+7閿涳拷4=250k

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

	//閺�垱瀵旈崣宀冪熅CAN閿涘苯鍨垫慨瀣CAN2
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
	CAN_InitStructure.CAN_Prescaler=RCC_ClocksStatus.PCLK1_Frequency/(16*250000); //濞夈垻澹掗悳鍥吀缁犳绱�6M/閿涳拷+8+7閿涳拷4=250k
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
 * 閸旂喕鍏橀幓蹇氬牚閿涳拷CAN鎼存洖鐪伴惃鍕灥婵瀵查敍灞藉瘶閹风悂O缁犅ゅ壖閸掓繂顫愰崠鏍电礉CAN濞夈垻澹掗悳鍥╃搼閸掓繂顫愰崠锟�
 * 閸忥拷   閸欙綇绱�閺冿拷
 * 閸戯拷   閸欙綇绱�閺冿拷
 * 鐠囷拷閺勫函绱�闁插洦鐗遍悙鍦畱鐠佸墽鐤嗛敍锟絁1939閸楀繗顔呯憴鍕暰CAN閻ㄥ嫰鍣伴弽椋庡仯閸︹晽75%,87.5%]娴ｅ秶鐤嗛敍灞惧娴犮儴顔曠純顔芥闂傜繝璐�5TSeq,閸氬本顒炵捄瀹犳祮鐎硅棄瀹虫稉锟�
 TSeq,BS1鐠佸墽鐤嗘稉锟�TSeq閿涘瓓S2鐠佸墽鐤嗘稉锟絋Seq閵嗭拷
 ***********************************************************************************************************/
void CAN_Lowlevel_Init(void) {
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
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;	//闁插秵鏌婇崥灞绢劄鐠哄疇绌�鏂も枆(SJW)
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;//闁插洦鐗遍悙閫涚秴缂冿拷
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;//閸欐垿锟介悙閫涚秴缂冿拷
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

	//閸忓牏顩﹀銏″复閺�拷
	CAN_RecieveDisable();

#endif
}

//閸欐垿锟紺AN濞戝牊浼�
BOOL CAN_Send_Msg(u32 u32MsgId, eCANIDType eIdType, u8* pu8Data, u8 u8DataLength) {
#if 0
	CanTxMsg tCanTransmitMsg;
	uint8_t transmit_mailbox;
	u8 ret;

	//閸欏倹鏆熷Λ锟界叀
	if(pu8Data == NULL || u8DataLength > 8)
	{
		return FALSE;
	}

	//缂佹挻鐎担鎾寸缁岋拷
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
	//闂堢偠绻欑粙瀣姎
	tCanTransmitMsg.RTR = 0;

	//閺佺増宓侀梹鍨
	tCanTransmitMsg.DLC = u8DataLength;

	//閹风柉绀夐弫鐗堝祦
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

