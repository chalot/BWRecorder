/**   
 * @Title: gprs.c
 * @Description: HUAWEI MG323ͨ��ģ��
 * @author Zhengd.Gao zhengdgao@163.com
 * @date 2013-6-14 ����9:43:02
 * @version V1.0
 */

#include <stm32f2xx_conf.h>
#include "gprs.h"
#include "comm.h"
#include "bsp.h"
#include "parameter.h"
#include "utility.h"
#include "trace.h"
#include "protocol.h"
#include "qp_port.h"
#include "board.h"
#include <qevents.h>
#include <qactiveobjs.h>
#include <systimer.h>
#include <qevt.h>

Q_DEFINE_THIS_MODULE("gprs.c")

static volatile u8 s_csq = 0; //��ǰ�ź�ǿ��

#define		GPRS_MAIN_ID		0	//�����Ӻ�
#define		GPRS_VICE_ID		1	//�������Ӻ�
#define 	SOCKET_ID			0

//�����շ�����
#define RX_BUF_SIZE		1024
#define TX_BUF_SIZE		1056
static u8 buf_Rx[RX_BUF_SIZE];	//���ջ�����
static u8 buf_Tx[TX_BUF_SIZE];	//���ͻ�����

static tCOMM Comm_Tx;  //���ͻ���
static tCOMM Comm_Rx;  //���ͻ���

//��������֡������
#define BUFFER_LENGTH_GPRSFRAMERx 			1024
static u8 au8Buffer_GPRSFrameRx[BUFFER_LENGTH_GPRSFRAMERx];

#define CHAR_CR 	0x0D  //�س�
#define CHAR_LF 	0x0A  //����/*AT����*/
const char* ATCMD[] = {
/*������������*/
"AT\r",							//��·ͬ��
		"ATE0\r",						//ȡ������

		/*ʶ��������*/
		"ATI\r",							//��Ʒ��ʶ��Ϣ��ѯ
		"AT+CGMI\r",					//������Ϣ��ѯ
		"AT+CGMM\r",					//��Ʒ���Ʋ�ѯ
		"AT+CGMR\r",					//��ע������汾�ţ����㼼���ϵĽ�����ͨ Ŀǰ��ӳ��Ϊƽ̨�汾
		"AT+CGSN\r",					//ESN��ѯ

		/*��ȫ�������� */
		"AT+CPIN?\r",					//R-UIM ����λ�� PIN1 ��״̬��ѯ

		/*�������ӿ�����*/
		"AT+CSQ\r",						//��鵱�ص������ź�ǿ�ȣ�31 ���0 ��С
		"AT+CREG?\r",					//��ѯ����ע��

		/*����ҵ��������*/
		"AT+CGACT=1\r",					//����PDP����״̬
		"AT+CGATT?\r",					//����GPRS����״̬

		/*INTERNET����ӿ�����*/
		"AT^SICS",						//����Internet���ӵ�Profile֮��������
		"AT^SICS",						//����Internet���ӵ�Profile֮APN
		"AT^SICS",						//����Internet���ӵ�Profile֮PASSWD
		"AT^SICS",						//����Internet���ӵ�Profile֮USER
		"AT^SISS",						//���������ID
		"AT^SISS",						//����internet�����socket
		"AT^SISS",						//����internet�����IP��ַ
		"AT^SISO",						//������
		"AT^SISC",						//�ر�����
		"AT^SISW",						//��������
		"AT^SISR",						//��ȡ����

		"AT^IOMODE=1,0\r",				 //����IOģʽ

		/*����Ϣҵ��ӿ�����*/
		"AT+CNMI=1,1,0,1,0\r",//���ö��Ž����ϱ��Ļ��ƣ����ö���Ϣ֪ͨ��ʽΪ�Ƚ�����Ϣ�洢��MT�У�Ȼ���ϱ��洢λ�ã����洢����Ϣ״̬���棬ֱ���ϱ�
		"AT+CPMS=\"SM\"\r",				//ѡ����Ŵ洢��
		"AT+CMGD=",	//ɾ������		 ��ʽΪAT+CMGD=<index>,<deltag>,��deltagΪ3��ʱ��ɾ�������Ѷ�����
		"AT+CMGR=",					//��ȡ������Ϣ
		"AT+CMGL=\r",					//�г���ǰѡ����Ŵ洢��ȫ�����ŵ���ź�״̬
		"AT+CMGF=0\r",					//���ö��Ÿ�ʽΪ PDU //�ı�
		/*�绰ҵ��ӿ�����*/
		"AT+CLIP=1\r", "ATD",							//����绰
		"ATA",							//��ͨ�绰
		"AT+CHUP",						//�Ҷϵ绰

		/*����ģʽ��������*/
		"AT+CFUN=1",						//���ù���ģʽ��ȫ�ٹ���״̬

		//��ѯϵͳʱ��
		"AT+CCLK=?",

};

/*ATӦ���ʽ*/
const char* ATACK[] = {
/*��������*/
"OK", "ATE0",

/*��ʶ��ѯ����Ӧ��*/
"Huawei Technologies", "Manufacturer:", "Revision:", "ESN",

/*��ȫ��������Ӧ��*/
"+CPIN: READY",

/*�������ӿ�����Ӧ��*/
"+CSQ: ", "+CGATT: ",

/*����ҵ��������Ӧ��*/
"+CREG: ",

		/*INTERNET����ӿ�����Ӧ��*/
		"^SISW:",						//д�����ݵ���������ʾ
		"^SISR:",						//�ӻ�������ȡ������ʾ

		/*����Ϣҵ��ӿ�����Ӧ��*/
		"+CPMS: ",				//���ö��Ŵ洢λ��
		"+CNMI:",				//�¶���֪ͨ��ʽ
		"+CMTI:",				//�յ�������ʾ
		"+CMGL:",				//�оٶ���״̬
		"+CMGR:",				//�յ��Ķ�����Ϣ
		"+CMGD:",				//ʣ���ɾ������

		/*�绰ҵ�����Ӧ��*/
		"+CLIP:",				//��������

		"+CCLK:",			//ϵͳʱ��

		/*FinalResultCode*/
		"CONNECT", "+CRING:", "NO CARRIER", "ERROR", "NO DIALTONE", "BUSY",
		"NO ANSWER", "+CME ERROR:", "+CMS ERROR:", "COMMAND NOT SUPPORT",
		"TOO MANY PARAMETERS",

};

/*ATӦ���ʽ*/
const char* CMEErrCode[] = { "R-UIM not inserted",					//UIM������λ��
		"Connection already established",			//�����ѽ���
		"Destination address required",				//��Ҫ�ƶ�Ŀ���ַ
		};

static const u8 aInitSteps[INIT_STEPS_AMOUNT] = {
/*ģ���ʼ��*/
_ATCMD_AT,			//AT
		_ATCMD_ATE0,
//	_ATCMD_SETMODE,
		_ATCMD_CPIN,		//R-UIM ����λ�� PIN1 ��״̬��ѯ
		_ATCMD_CSQ,			//��鵱�ص������ź�ǿ�ȣ�31 ���0 ��С��
//	_ATCMD_IOMODE,

#if		_MSG_
		_ATCMD_CPMS,		//���ö��Ŵ洢λ��
		_ATCMD_CNMI,		//���ö��Ž����ϱ��Ļ��ƣ��������Ҫ���Ź��ܿ��Բ�����
		_ATCMD_CMGF,		//���ö���ģʽΪPDUģʽ
#endif
//	_ATCMD_CGACT,		//����PDP����״̬
		_ATCMD_CGATT,		//����GPRS����״̬
		_ATCMD_PHONE_TIPS,    //������������

//	_ATCMD_GETSYSTIME,	//��ѯϵͳʱ��

		};

static const u8 aConnectSteps[CONNECT_STEPS_AMOUNT] = { _ATCMD_CLOSE,	//�ر�����
		_ATCMD_SETCONNTYPE,		//������������
		_ATCMD_SETPWD,			//����Password
		_ATCMD_SETUSR,			//����User
		_ATCMD_SETAPN,			//����APN
		_ATCMD_SETSOCKET,		//��������Socket
		_ATCMD_SETID,			//��������ID
		_ATCMD_SETADDR,			//���������ַ
		_ATCMD_OPEN				//������

		};

static u8 attach_state = 0;
static u8 register_state = 0;
static u16 queryBytes = 0;
static u8 net_State = 0;

static u16 s_u8PDUSMSRx_LenCount = 0;
static u8 au8Buffer_SMSRx[BUFFER_LENGTH_SMSRx];
static u8 u8SMGCount = 0;		//��������
static u8 u8MaxSMGCount = 0;	//�����������
static u8 u8ReadIndex = 0;		//���Ŷ�����
static u16 u16ReadDataTimeout = 0;	//�������ݶ�ȡ���ʱ
static u8 u8GPRS_7bitDecode_LeftCode = 0;
static u8 u8GPRS_7bitDecode_Sequence = 0;
static u8 u8PDUSMSRx_LenCount = 0;
static u8 fGPRS_7bitDecode_Skip = 0;

#if		_MSG_
#define		GPRS_SMG_CNT				30					//�������鳤��/*GPRS���ն���Ϣ����״̬*/
typedef enum {
	_eGPRS_SMG_IDLE = 0,					//����̬
	_eGPRS_SMG_READ_REQ,				//�Ѿ����Ͷ���Ϣ�Ķ�����
	_eGPRS_SMG_READ_OK,					//����Ϣ�Ѿ��Ķ�
	_eGPRS_SMG_READ_TIMEOUT,			//����Ϣ�Ķ�����ʱû��Ӧ��
	_eGPRS_SMG_DEL_REQ,					//�Ѿ����Ͷ���Ϣɾ������
	_eGPRS_SMG_DEL_OK,					//����Ϣ�Ѿ�ɾ��
	_eGPRS_SMG_DEL_TIMEOUT,				//ɾ�����ʱ

	_eGPRS_SMG_INVALID = 0xFF
} eGPRSSMGSTATUS;

typedef struct {
	u8 u8Smg_Cnt;					//"ME"�б����Ķ�����Ŀ
	u8 u8CurSmg_Index;				//��ǰ��ȡ���ŵ����к�
	u8 u8ReadSmg_Rx;				//���ն�Ϣ��ָ��
	u8 u8WriteSmg_Rx;			  	//���ն�Ϣдָ��
	u8 au8MsgCnt[GPRS_SMG_CNT];	//���ն�Ϣ��������
	u16 u16Smg_Timeout;				//���Ŵ�������ʱ
	u16 u16CheckSmg_Timeout;		//��ѯ"ME"�ж���״̬��ʱ
	u8 u8SMsgLen;					//�����������ն�Ϣ����
	u8 u8PendingSmsg;				//�Ƿ�Ӧ����ȡ����������
	eGPRSSMGSTATUS eCdmaMsgStatus;		//GPRS���ն���Ϣ����״̬
} tGPRSSMGFSMType;

static tGPRSSMGFSMType tGPRSSMGFSM;

static u8 GPRS_GetPDUSMS_HexByte(u8* pu8GPRSMessage);
static u32 GPRS_SMS_7BitsDecode(u8 u8PDUByte, u8 u8Startup, u8 u8LeftBits);
static u8 PhoneNo_ConvertBaseToAscii(u8 *pcNo, _GPRS_SENDPDUSMSType *ptBaseNo,
		u8 nInternational);
static u8 GPRS_StoreSMSContent(u8 u8SMS_DCS, u16 u16Code, s16 s16Index,
		s16 s16Bytes, s16 s16SequenceLongSMS);
static u8 SwapNibble(u8 u8Byte);
static u8 BCD2Decimal(u8 u8BCD);
static u8 GPRS_StoreSMSContent(u8 u8SMS_DCS, u16 u16Code, s16 s16Index,
		s16 s16Bytes, s16 s16SequenceLongSMS);

#endif

void GPRS_SetAttachState(u8 state) {
	attach_state = state;
}

u8 GPRS_GetAttachState() {
	return attach_state;
}

void GPRS_SetRegState(u8 state) {
	register_state = state;
}

u8 GPRS_GetRegState() {
	return register_state;
}

void GPRS_SetNetState(u8 state) {
	net_State = state;
}

u8 GPRS_GetNetState() {
	return net_State;
}

u8 GPRS_CheckCmd_IsAttaching(u8 step) {
	if (aInitSteps[step] == _ATCMD_CGATT)
		return 1;
	return 0;
}

//���ӷ���������
#define SERVER_TYPE	 SERVER_PRIMARY	// ptRuntimeParam->ServerType#define DIST(next, tail, size)  ((tail > next) ? (tail - next) : (size - next + tail))

//���غ�������
static u8 GPRS_WriteData(u8* pu8Data, u16 u16Length);
static void UART_COMM_StartTransmit(void);
static _eATACK GPRS_GetRxFrameType(u8* pu8Buffer);
static u16 GPRS_TakeRxFrame(u8* pu8Buffer, u16 begin, u16 end);
static u16 GPRS_TakeRxSMG(u8* pu8Buffer);
//static void GPRS_TCPIPRecData_Process(u8 *pData, u16 len);
static void GPRS_ParseRxFrame(u8 *pBuffer, u16 len);

/**
 * @brief	�ڴ��ʼ��
 * @param	��
 * @return		��
 */
void GPRS_Init() {
	/*��ʼ���շ�����*/
	Comm_Init(&Comm_Tx, &buf_Tx[0], TX_BUF_SIZE);
	Comm_Init(&Comm_Rx, &buf_Rx[0], RX_BUF_SIZE);

	ZeroMem(&buf_Tx[0], TX_BUF_SIZE);
	ZeroMem(&buf_Rx[0], RX_BUF_SIZE);

	/*ʹ��GPRS���ڽ����ж�*/
	USART_Cmd(GSM_COM, ENABLE);
}

/**
 * @brief	����IO���³�ʼ��
 * @param	��
 * @return		��
 */void GPRS_UartReconfigure() {
#if 0
	GPIO_InitTypeDef GPIO_InitStructure;

	//�ܽ� = UART2-Tx�����츴��
	GPIO_InitStructure.GPIO_Pin = GPIOA_CDMA_USART2_TX_;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//�ܽ� = UART2-Rx����������
	GPIO_InitStructure.GPIO_Pin = GPIOA_CDMA_USART2_RX_;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_Cmd(GSM_COM, ENABLE);
#endif
}

/**
 * @brief	ִ�г�ʼ������
 * @param	initStep	[in]	AT����
 * @return
 */
void GPRS_doInit(u8 initStep) {
	Q_ASSERT(initStep < INIT_STEPS_AMOUNT);
	if (initStep >= INIT_STEPS_AMOUNT)
		return;

	TRACE_(QS_USER, NULL, "[initialing], step = %d, cmd = %s", initStep,
			ATCMD[aInitSteps[initStep]]);

	//�����ύ�����ʹ���
	GPRS_WriteData((u8*) ATCMD[aInitSteps[initStep]],
			strlen_(ATCMD[aInitSteps[initStep]]));
}

/**
 * @brief	���ڷ�������
 * @param	pData		[in]	����
 * 			dataLen		[in]	���ݳ���
 * @return
 */
u8 GPRS_WriteData(u8* pu8Data, u16 u16Length) {
	if ((pu8Data == NULL ) || (u16Length == 0)) {
		return 0;
	}

	//���ͻ�����������?
	if (u16Length > TX_BUF_SIZE) {
		return 0;
	}

	//��λ��дָ��
	Comm_Tx.read = 0;

	//�����������ݵ����ͻ���
	memcpy_(Comm_Tx.pBuf, pu8Data, u16Length);

	//������������ݳ���
	Comm_Tx.dataSize = u16Length;

	//�������ڷ�������
	UART_COMM_StartTransmit();

	return 1;
}

/**
 * @brief	����GPRS�������ݷ���
 * @param	NULL
 * @return		NULL
 */
void UART_COMM_StartTransmit(void) {
	u8 u8SendData;

	//�������������봮��TXBUFR
	u8SendData = *(Comm_Tx.pBuf + Comm_Tx.read++);
	USART_SendData(GSM_COM, u8SendData);

	//�������ж�
	USART_ITConfig(GSM_COM, USART_IT_TXE, ENABLE);
}

//��ѭ���Ļ����Ƚ�
BOOL memcmp_wrap_(u8 *pBuf, u8 start, u8 end, u16 bufSize, char *pString) {
	u8 matchTimes = 0;
	u16 strlen = strlen_(pString);

	while ((start != end) && (matchTimes < strlen)) {
		if (*(pBuf + start) != *pString++) {
			break;
		}
		start++;
		WRAP_AROUND(start, bufSize);

		matchTimes++;
	}

	if (matchTimes == strlen)
		return TRUE;

	return FALSE;
}

/**
 * @brief	GPRS�жϷ������
 * @param	NULL
 * @return		NULL
 */
void ISR_GSM_COM(void) {
#define PREFIX_POS_UNKOWN	0xFFFF
	static u8 u8Data = 0;
	static u16 head = PREFIX_POS_UNKOWN;
	static u16 tail = 0;
	u16 u16RecvData;
#define STRING_MAX	7
	static char* ATRESP[] = { "OK", "ERROR", "^SISR", "^SISW", "+CMS ERROR",
			"+CME ERROR", "NO CARRIER", };

	//������� ���ҷ����ж�ʹ��
	if (USART_GetITStatus(GSM_COM, USART_IT_TXE)) {
		USART_ClearITPendingBit(GSM_COM, USART_IT_TXE);

		//���ݷ�������ˣ��ط����ж�
		if (Comm_Tx.read == Comm_Tx.dataSize) {
			//�رշ����ж�
			USART_ITConfig(GSM_COM, USART_IT_TXE, DISABLE);
		} else {
			//�������������봮��TXBUFR
			u8Data = *(Comm_Tx.pBuf + Comm_Tx.read++);
			USART_SendData(GSM_COM, u8Data);
		}
	}

	//�����ж�
	if (USART_GetITStatus(GSM_COM, USART_IT_RXNE)) {
		USART_ClearITPendingBit(GSM_COM, USART_IT_RXNE);

		//��ȡ����
		u16RecvData = USART_ReceiveData(GSM_COM);

		u8Data = (u8) u16RecvData;

		//�洢����
		*(Comm_Rx.pBuf + Comm_Rx.write) = u8Data;

#if 0
		/*
		 * 		�ڽ���ISR���ж���Ч����֡�������¼�֪ͨ��ʽ������Ӧ�ò�ȥ��ȡ֡
		 ����MG323AT�ֲᣬ�����κ�ATָ���ȷ����Ӧ��ʽ��<CR><LF>OK<CR><LF>��
		 �������Ӧ��ʽ�����¼��ࣺ
		 <CR><LF>ERROR<CR><LF>
		 <CR><LF>+CME ERROR:<err><CR><LF>
		 <CR><LF>+CMS ERROR:<err><CR><LF>
		 ��ˣ������յ������κ�һ֡ʱ����ʾ��ǰ���յ�һ��������Ч֡��

		 ����һ�ָ����ķ�����<CR><LF>������<CR><LF>���ǳɶԳ��ֵģ���������򵥵�
		 ��ԾͿ����ˡ�����Ҫע�⴦��<CR><LF>���ܳ��������ݰ��е������
		 ��ʵ�ֲ��ú�һ�ַ�����
		 */

		/*
		 * ƥ��һ��AT���֡���������ڽ��ն����б�ǳ�ǰ׺<CR><LF>�ͺ�׺<CR><LF>��λ�ã���
		 * ��׺��������ƥ���м��ִ������Ƿ���AT�����Ӧ�����ǣ���˵��ƥ��ɹ������򣬽�
		 * ��׺��Ϊǰ׺�������յ��µĺ�׺����������ƥ�䡣����������ԭ���Ǿ����ܼ��ٸ����ڴ�
		 * ������ͬʱ��֤ƥ��Ч����ߡ�
		 *
		 * */
#if 1
		//ƥ���һ��ǰ׺��head_previous��¼ǰ׺<CR>λ��
		if(head == PREFIX_POS_UNKOWN)
		{
			if(u8Data == CHAR_LF)
			{
				u8 head_previous;
				if(Comm_Rx.write == 0)
				head_previous = RX_BUF_SIZE - 1;
				else
				head_previous = Comm_Rx.write - 1;

				//ƥ�䵽ǰ׺<CR><LF>
				//           ��
				//			head
				if(*(Comm_Rx.pBuf + head_previous) == CHAR_CR)
				{
					head = head_previous;
				}
			}
		}
		//�ҵ�ǰ׺����λ��׺
		else
		{
			if(u8Data == CHAR_LF)
			{
				u16 tail_previous;

				if(Comm_Rx.write == 0)
				tail_previous = RX_BUF_SIZE - 1;
				else
				tail_previous = Comm_Rx.write - 1;

				//�ҵ���׺<CR><LF>
				//         ��
				//	      tail
				if(*(Comm_Rx.pBuf + tail_previous) == CHAR_CR)
				{
					ATMsgEvt *pe;
					u16 next = 0;

					tail = tail_previous;				//Comm_Rx.write;

					//��ָ��ָ��ǰ׺<LF>
					next = head + 1;
					WRAP_AROUND(next, RX_BUF_SIZE);

					//��ָ��ָ��ǰ׺<CR><LF>��һ���ֽ�
					next ++;
					WRAP_AROUND(next, RX_BUF_SIZE);

					//ǰ׺�ͺ�׺���ǽ���<CR><LF><CR><LF>
					//							����
					//						  next tail
					//˵���м�������

					if((next != tail) && ((DIST(next, tail, RX_BUF_SIZE)) > 1))
					{
						u8 i;

						//��ƥ��
						for(i = 0; i < STRING_MAX; i++)
						{
							//��ѭ���Ļ����Ƚ�
							if(memcmp_wrap_(Comm_Rx.pBuf, next, tail, RX_BUF_SIZE, ATRESP[i]))
							break;
						}

						if(i < STRING_MAX) //ƥ��ɹ���˵����ȡ��һ�����֡
						{
							//����֡������Ϣ
							pe = Q_NEW(ATMsgEvt, AT_DATAREADY_SIG);
							Q_ASSERT(pe);
							pe->begin = 0;//֡ͷ����λ��
							pe->end = Comm_Rx.write + 1;//����λ��Ϊ���һ����Ч�ֽڵ���һ���ֽ�
							WRAP_AROUND(pe->end, RX_BUF_SIZE);
							QACTIVE_POST(AO_Gprs, (QEvt*)pe, NULL);
						}
					}   //if(next != previous)

					//׼����һ��ƥ��
					head = tail;

					//����ƥ��֡ͷ
//					head = PREFIX_POS_UNKOWN;
				}
			}
		}   //else
#endif
#endif

		//���ջ�����дָ���ͷ
		Comm_Rx.write++; //ȡ��һ���ַ�
		WRAP_AROUND(Comm_Rx.write, RX_BUF_SIZE);
	}

	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_ORE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_ORE);        //��SR

		//��ȡ����
		u16RecvData = USART_ReceiveData(GSM_COM);		  //��DR

	}
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_PE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_PE);        //��SR

		//��ȡ����
		u16RecvData = USART_ReceiveData(GSM_COM);		  //��DR

	}
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_FE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_FE);        //��SR

		//��ȡ����
		u16RecvData = USART_ReceiveData(GSM_COM);		  //��DR

	}
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_NE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_NE);        //��SR

		//��ȡ����
		u16RecvData = USART_ReceiveData(GSM_COM);		  //��DR

	}
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_IDLE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_IDLE);        //��SR

		//��ȡ����
		u16RecvData = USART_ReceiveData(GSM_COM);		  //��DR

	}
	//LBD LIN�Ͽ�����־
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_LBD) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_LBD);        //��SR
	}
	//������ɼ���־
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_TC) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_TC);        //��SR
	}
	//CTS��־
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_CTS) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_CTS);        //��SR
	}
}
#if 0
/*
 �޳�   <CR><LF>.....<CR><LF><CR><LF>....<CR><LF>
 |    ֡A             | 		֡B	           |
 ���֡β֡ͷƥ��
 */
next = head + 1;
WRAP_AROUND(next, RX_BUF_SIZE);

if(next != previous)
{
//						//ͬʱ�ҵ�ǰ׺�ͺ�׺��ƥ��ATָ��,��ǰ׺�Ľ�����һ���ֽڿ�ʼƥ��
//						//����֡������Ϣ
//						pe = Q_NEW(ATMsgEvt, AT_DATAREADY_SIG);
//						Q_ASSERT(pe);
//						pe->begin = next; //֡ͷ����λ��
//						pe->end = previous;   //֡β����λ��
//						QACTIVE_POST(AO_Gprs, (QEvt*)pe, NULL);
}
#endif

#if 0
head ++; //ȡ��һ���ַ�
WRAP_AROUND(head, RX_BUF_SIZE);

//���ݵ�һ���ַ���λƥ������
for(i = 0; i < 6; i++)
{
	if(*(Comm_Rx.pBuf + head) == ATRESP[i][0])
	break;
}

//�ҵ�ƥ��ĵ�һ���ַ����ڴ�
if(i < 6)
{
	//����ȫ�ַ�ƥ��
	u8 j = 0;
	for(j = 1; j < strlen_(ATRESP[i]); j++)
	{
		head ++; //ȡ��һ���ַ�
		WRAP_AROUND(head, RX_BUF_SIZE);

		if(*(Comm_Rx.pBuf + head) != ATRESP[i][j])
		break;
	}

	if(j == strlen_(ATRESP[i])) //ƥ��ɹ���˵����ȡ��һ�����֡
	{
		//����֡������Ϣ
		QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_DATAREADY_SIG), NULL);
	}
}
#endif

#if 0
//���ݵ�һ���ַ���λƥ������
for(i = 0; i < 6; i++)
{
	if(*(Comm_Rx.pBuf + next) == ATRESP[i][0])
	break;
}

//�ҵ�ƥ��ĵ�һ���ַ����ڴ�
if(i < 6)
{
	//����ȫ�ַ�ƥ��
	u8 j = 0;
	for(j = 1; j < strlen_(ATRESP[i]); j++)
	{
		next ++; //ȡ��һ���ַ�
		WRAP_AROUND(next, RX_BUF_SIZE);

		if(*(Comm_Rx.pBuf + next) != ATRESP[i][j])
		break;
	}

	if(j == strlen_(ATRESP[i])) //ƥ��ɹ���˵����ȡ��һ�����֡
	{
//								static const QEvt dataReadyEvt = {AT_DATAREADY_SIG, 0};

		/*
		 ע�⣬��Ϣ���ݿ����ϴ�����Ż���ʩ��
		 1��ʹ�þ�̬�¼��������ڴ���䣻
		 2������޶ȼ����źŷ��ʹ�����ȡ����ƥ����ԡ���ʵ���У�ֻ�Խ��֡����ƥ�䣬
		 ���ƥ��ɹ��ŷ����źš���ΪAT��Ӧһ��ǰ�������ݣ�����ӳɹ�/ʧ��״̬�����
		 ƥ��AT��������֮ǰ�Ľ��չ��̲����κθ��ţ����׿ɿ���
		 */
		//����֡������Ϣ
//								QACTIVE_POST(AO_Gprs, &dataReadyEvt, NULL);
		pe = Q_NEW(ATMsgEvt, AT_DATAREADY_SIG);
		Q_ASSERT(pe);
		pe->begin = 0;//head;//head_previous; //֡ͷ����λ��
		pe->end = Comm_Rx.write;//֡β����λ��
		QACTIVE_POST(AO_Gprs, (QEvt*)pe, NULL);
	}
}   //if(i < 6)
#endif

/**
 * @brief	ִ�г�ʼ������
 * @param	initStep	[in]	AT����
 * @return
 */
void GPRS_doConnect(u8 connectStep) {
	u8 au8Cmd[50];
	_eATCMD eCmd;
	u8 channelId = 0;

	Q_ASSERT(connectStep < CONNECT_STEPS_AMOUNT);
	if (connectStep >= CONNECT_STEPS_AMOUNT)
		return;

	//ÿ�ζ�Ҫ����������
	ZeroMem(au8Cmd, 50);

	//�������ӷ����������ò�ͬ��ͨ����
	channelId = GPRS_MAIN_ID;

	//ȡָ��
	eCmd = (_eATCMD) aConnectSteps[connectStep];

	//������������ʱ����AT�����ַ���
	if (eCmd == _ATCMD_CLOSE) {
		sprintf_((u8*) &au8Cmd[0], "AT^SISC=%d\r", channelId);
	}
	//������������ʱ����AT�����ַ���
	else if (eCmd == _ATCMD_SETCONNTYPE) {
		sprintf_((u8*) &au8Cmd[0], "AT^SICS=%d,conType,GPRS0\r", channelId);
	}
	//����Password
	else if (eCmd == _ATCMD_SETPWD) {
		sprintf_((u8*) &au8Cmd[0], "AT^SICS=%d,passwd,gprs\r", channelId);
	}
	//����User
	else if (eCmd == _ATCMD_SETUSR) {
		sprintf_((u8*) &au8Cmd[0], "AT^SICS=%d,user,cm\r", channelId);
	} else if (eCmd == _ATCMD_SETAPN) {
		sprintf_((u8*) &au8Cmd[0], "AT^SICS=%d,apn,UNINET\r", channelId);
	} else if (eCmd == _ATCMD_SETID) {
		sprintf_((u8*) &au8Cmd[0], "AT^SISS=%d,conId,0\r", channelId);
	} else if (eCmd == _ATCMD_SETSOCKET) {
		sprintf_((u8*) &au8Cmd[0], "AT^SISS=%d,srvType,socket\r", channelId);
	}
#if	CONNMODE  == _UDP_ //UDP����	else if(eCmd == _ATCMD_SETADDR)	{
		if(SERVER_TYPE == SERVER_PRIMARY)
		{
			sprintf_((u8*)&au8Cmd[0],"at^siss=%d,address,\"sockudp://%d.%d.%d.%d:%d\"\r",
					GPRS_MAIN_ID,
					ptWorkParam->ip[0],
					ptWorkParam->ip[1],
					ptWorkParam->ip[2],
					ptWorkParam->ip[3],
					ptWorkParam->port
			);
		}
		else if(SERVER_TYPE == SERVER_VICE)
		{
			sprintf_((u8*)&au8Cmd[0],"at^siss=%d,address,\"sockudp://%d.%d.%d.%d:%d\"\r",
					GPRS_VICE_ID,
					ptWorkParam->ip[0],
					ptWorkParam->ip[1],
					ptWorkParam->ip[2],
					ptWorkParam->ip[3],
					ptWorkParam->port
			);
		}
	}
#elif CONNMODE  == _TCP_
	else if (eCmd == _ATCMD_SETADDR) {
		if (SERVER_TYPE == SERVER_PRIMARY) {
//			sprintf_((u8*)&au8Cmd[0],"at^siss=%d,address,\"socktcp://%d.%d.%d.%d:%d\"\r",
//						GPRS_MAIN_ID,
//						ptWorkParam->ip[0],
//						ptWorkParam->ip[1],
//						ptWorkParam->ip[2],
//						ptWorkParam->ip[3],
//						ptWorkParam->port
//					);
		}
//		else if(SERVER_TYPE  == SERVER_VICE)
//		{
//			sprintf_((u8*)&au8Cmd[0],"at^siss=%d,address,\"socktcp://%d.%d.%d.%d:%d\"\r",
//						GPRS_VICE_ID,
//						ptCommParam->aViceServer_IP[0],
//						ptCommParam->aViceServer_IP[1],
//						ptCommParam->aViceServer_IP[2],
//						ptCommParam->aViceServer_IP[3],
//						ptCommParam->u16ViceServer_Port
//					);
//		}
	}

#endif
	else if (eCmd == _ATCMD_OPEN) {
		sprintf_((u8*) &au8Cmd[0], "AT^SISO=%d\r", channelId);
	} else {
		//δ֪���������
		return;
	}

	TRACE_(QS_USER, NULL, "[connecting], step = %d, cmd = %s ", connectStep,
			au8Cmd);

	//AT����д�봮��
	GPRS_WriteData(au8Cmd, strlen_(au8Cmd));
}

//ִ������ע������
void GPRS_doRegister() {
	char strCmd[30] = { 0 };

	TRACE_(QS_USER, NULL, "[registering], cmd = AT+CREG? ");

	sprintf_((u8*) &strCmd, "AT+CREG?\r");
	GPRS_WriteData(strCmd, strlen_(strCmd));
}

//������������
BOOL GPRS_doQueryData(void) {
	u8 au8Cmd[20] = { 0 };
	BOOL bret;

	//���������ַ���
	sprintf_(au8Cmd, "AT^SISR=%d,%d\r", SOCKET_ID, MAX_DATA_LEN);
	//��������
	bret = GPRS_WriteData(au8Cmd, strlen_(au8Cmd));

	TRACE_(QS_USER, NULL, "[TCP connected]:Query data, cmd = %s ", au8Cmd);

	return bret;
}

/* ����TCPIPǰ׺ */
void GPRS_SendTCPIPPrefix(u16 dataLen) {
	u8 au8Cmd[50] = { 0 };

	sprintf_(au8Cmd, "AT^SISW=%d,%d\r\n", SOCKET_ID, dataLen);

	TRACE_(GPRS_STAT, NULL, "[UART]send : %s", au8Cmd);

	//�ɹ�д�����ݵ����ͻ�����
	GPRS_WriteData(au8Cmd, strlen_(au8Cmd));
}

/* ���������������� */
void GPRS_FillinTCPIPData(u8 *pDataBuf, u16 dataLen) {
	if ((pDataBuf == NULL ) || (dataLen == 0)) {
		return;
	}

	*(pDataBuf + dataLen) = '\r';
	*(pDataBuf + dataLen + 1) = '\n';

	TRACE_(QS_USER, NULL, "[UART] frame sendout. waiting ret...... ");
	GPRS_WriteData(pDataBuf, dataLen + 2);
}

/*֡��ȡ*/
void GPRS_doRetrieveFrame(u16 begin, u16 end) {
	u16 u16FrameSize = 0;

	//ѭ����ȡ����֡����Ϊ����ͬʱ��������֡�ͽ��֡
	for (;;) {
		//��ȡ����֡
		u16FrameSize = GPRS_TakeRxFrame(au8Buffer_GPRSFrameRx, begin, end);

		//֡������Ч
		if (u16FrameSize == 0)
			break;

		if (u16FrameSize > 0) {
			//�ַ���������
			*(au8Buffer_GPRSFrameRx + u16FrameSize) = '\0';
			TRACE_(SYSTEM_STAT, NULL, "[UART] RX: %s", au8Buffer_GPRSFrameRx);

			//����������֡
			GPRS_ParseRxFrame(au8Buffer_GPRSFrameRx, u16FrameSize);
		}
	}
}

//������ȡTCP��
void GPRS_doTakeTCPPacketData() {
	u16 _len;
	u16 _read;

	_read = Comm_Rx.read;

	WRAP_REVERSE(_read, RX_BUF_SIZE);
	Q_ASSERT(*(Comm_Rx.pBuf + _read) == CHAR_LF);

	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<LF>
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<CR>
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //K
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //O
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<LF>
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<CR>

	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<LF>
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<CR>

	ZeroMem(au8Buffer_GPRSFrameRx, BUFFER_LENGTH_GPRSFRAMERx);
	_len = queryBytes;
	while (queryBytes > 0) {
		au8Buffer_GPRSFrameRx[queryBytes - 1] = *(Comm_Rx.pBuf + _read);
		WRAP_REVERSE(_read, RX_BUF_SIZE);

		queryBytes--;
	}

	au8Buffer_GPRSFrameRx[_len] = '\0';
	TRACE_(QS_USER, NULL, "[TCP] Get Msg : %s", au8Buffer_GPRSFrameRx);

	//�˺�����Ч�������ݣ���������Э������
	Protocol_TCPIPData_Process(au8Buffer_GPRSFrameRx, _len);
}

//֡��������
void GPRS_ParseRxFrame(u8 *pBuffer, u16 len) {
	u8 *pData = NULL;
	_eATACK eACK;

	//��ȡ֡����
	eACK = GPRS_GetRxFrameType(pBuffer);

	//ָ���������
	pData = pBuffer;

	//����AT����ͷ
	if (_ATACK_UNDEFINE != eACK) {
		pData += strlen_(ATACK[(u8) eACK]);
	}

	switch (eACK) {
	case _ATACK_OK:

		QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_OK_SIG), NULL);
		break;
	case _ATACK_CPIN:

//				QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_OK_SIG), NULL);
		break;

	case _ATACK_CGATT: //GPRS����״̬��0������״̬��1������״̬

		if (*pData == '1') {
//					QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_OK_SIG), NULL);
			GPRS_SetAttachState(1);
		} else {
			GPRS_SetAttachState(0);
		}
		break;

	case _ATACK_CREG:  //����ע��״̬

		pData += 2;

		if ((*pData == '1') || (*pData == '5')) {
			GPRS_SetRegState(1);
//					QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_NETREGGED_SIG), NULL);
		} else {
			GPRS_SetRegState(0);
		}
		break;

		//"+CSQ:<SQM>,<99>",SQM:�ź�ǿ�ȣ�0~31
	case _ATACK_CSQ: {
		u8 csq = 0;
		CSQEvt *pe = NULL;

		//�����ź�����
		while (*pData != ',') {
			if (*pData != 0x20) {
				csq = csq * 10 + (*pData - 0x30);
			}
			pData++;
		}

		GPRS_SetCSQ(csq);

//				//�㲥CSQֵ
//				pe = Q_NEW(CSQEvt, AT_CSQ_SIG);
//				Q_ASSERT(pe);
//				pe->csq = csq;
//				QACTIVE_POST(AO_Gprs, pe, NULL);
	}
		break;

		/*
		 * URC   <CR><LF>^SISW��<srvProfileId>, <urcCauseId>[,<unackData>]<CR><LF>
		 ����      ���ݵ������ϱ�����^SISR ֪ͨ�ϲ�����Ѿ���������д�����ݡ�
		 <srvProfileId>����ֵ�Ͳ�����Internet ����profile ��ʶ��ȡֵ��ΧΪ0~9
		 <reqWriteLength>�������Ͳ�����ָ��^SISW Ҫд��������ֽڳ��ȣ�ȡֵ��ΧΪ0~1500
		 */
	case _ATACK_SISW: //׼������������ʾ
	{
//				u8		u8Index = 0;
//				u8		u8Break = 0;

		//��ʾ������Է��͵��ֽ���
		if (memcmp_(pBuffer, (u8*) "^SISW: 0,1,", strlen_("^SISW: 0,1,"))) {
			//�����ѽ��ܵ��ȴ���ʾ��
//					QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_SENDOK_SIG), NULL);
		}
		//��ʾ���ͽ��
		else if (memcmp_(pBuffer, (u8*) "^SISW: 0,1", strlen_("^SISW: 0,1"))
				&& (len == strlen_("^SISW: 0,1"))) {
			//�������ݷ��ͳɹ�
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_SENDOK_SIG), NULL);
		} else {
			//�����ѽ��ܵ�����������ʾ��
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_FILLDATATIP_SIG), NULL);
		}
	}
		break;

		/*
		 * �����������
		 * һ�������գ����ݵ���ʱ�����ϱ�
		 URC <CR><LF>^SISR: <srvProfileId >,<urcCauseId><CR><LF>
		 ���� ���ݵ������ϱ�����^SISR ֪ͨ�ϲ��п��Զ�ȡ�����ݡ�
		 �������������·���ѯ��������AT+SISR����Ӧ
		 <CR><LF>^SISR:<srvProfileId>,<reqReadLength>[,<remainUdpPacketLength>]<CR><LF>
		 ����(���ض�ȡ��������)<CR><LF>
		 <CR><LF>OK<CR><LF>
		 */
	case _ATACK_SISR: {
		u16 _len = 0;
		_len = strlen_("^SISR: 0, 1");

		//���һ�����յ����������ϱ�
		if (memcmp_(pBuffer, "^SISR: 0, 1", _len) && (len == _len)) {
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvent, AT_DATAREADY_SIG), (void*) 0);
		}
		//��������ݲ���ȡ���ݣ���Ϊ��ʱ���ݿ�����δ������ȫ�����׵������Ǵ˴����������ݳ��ȣ�
		//�ȴ�<CR><LF>OK<CR><LF>��������ȡ���������ݱ��ѽ�����ȫ
		else {
			_len = 0;

			//��ѯ����Ӧ
			while (*pData != ',') {
				pData++;
			}

			pData++; //����','

			//�������ݳ���<reqReadLength>
			while ((*pData >= '0') && (*pData <= '9')) {
				_len = _len * 10 + (*pData - 0x30);
				pData++;
			}

			//��¼���������ݳ���
			queryBytes = _len;
		}
	}

		break;

		//ϵͳʱ��,��ʽΪ��yy/MM/dd,hh:mm:ss��
	case _ATACK_SYSTIME: {
		SYSTIMER timer;

		while (!((*pBuffer >= '0') && (*pBuffer <= '0'))) {
			pBuffer++;
		}

		while (*pBuffer != '/') {
			timer.year = 10 * timer.year + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while (*pBuffer != '/') {
			timer.month = 10 * timer.month + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while (*pBuffer != ',') {
			timer.day = 10 * timer.day + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while (*pBuffer != ':') {
			timer.hour = 10 * timer.hour + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while (*pBuffer != ':') {
			timer.minute = 10 * timer.minute + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while ((*pBuffer >= '0') && (*pBuffer <= '0')) {
			timer.second = 10 * timer.second + (*pBuffer - 0x30);
			pBuffer++;
		}

		SysTimer_Set(&timer);
	}
		break;

		//"CONNECT"
	case _ATACK_CONNECT:

		break;
#if	_PHONE_
		/*�绰����*/
		//"RING",����
		case _ATACK_RING:

		//�ۼ��������
		tGPRSFSM.u8PhoneRingCount++;

		break;

		//"+CLIP:"������
		case _ATACK_CLIP:

		//����<+CLIP: >��<,>,ָ�����ݳ����׵�ַ
		pData += 2;

		GPRS_ClipProcess(pData);

		break;

		//"NO CARRIER",
		case _ATACK_NO_CARRIER:
		//��ǰ�������壬�����ڽ����������ڲ��ţ������ں�����˵���Ѿ��Ҷ�
		if ( (tGPRSFSM.ePhoneState == ePHONE_STATE_DIAL) || (tGPRSFSM.ePhoneState == ePHONE_STATE_CALLING)
				|| (tGPRSFSM.ePhoneState == ePHONE_STATE_TALKING) || (tGPRSFSM.ePhoneState == ePHONE_STATE_BUSY)
				|| (tGPRSFSM.ePhoneState == ePHONE_STATE_HANGING) )
		{
			tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;

			//����ѡ��Ϊ��
			PERI_AudioSel(_AUDIO_NONE_);

		}

		//�������������󣬻����ڽ�����������
		if ( tGPRSFSM.eVoiceMonitorState == ePHONE_MONITOR_REQ)
		{
			//�������������ľܽӺ�������
			GPRS_MonitorRefusedPostProcess();
		}
		else if (tGPRSFSM.eVoiceMonitorState == ePHONE_MONITOR_MONITORING)
		{
			//��������
			tGPRSFSM.eVoiceMonitorState = ePHONE_MONITOR_IDLE;

			tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;
		}

		//�����ǰ����绰
		if (tGPRSFSM.u8ATCmd == _ATCMD_PHONE_DIAL)
		{
			tGPRSFSM.ePhoneState = ePHONE_STATE_DIAL;
			tGPRSFSM.u8ATCmd = _ATCMD_IDLE_;
		}
		break;

		//"NO DIALTONE",
		case _ATACK_NO_DIALTONE:

		break;

		//"BUSY",
		case _ATACK_BUSY:
		//��ǰ�������壬�����ڽ����������ڲ��ţ������ں�����˵���Ѿ��Ҷ�
		if (tGPRSFSM.ePhoneState == ePHONE_STATE_DIAL)
		{
			tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;

			//����ѡ��Ϊ��
			PERI_AudioSel(_AUDIO_NONE_);
		}
		//�������������󣬻����ڽ�����������
		if ( tGPRSFSM.eVoiceMonitorState == ePHONE_MONITOR_REQ)
		{
			//�������������ľܽӺ�������
			GPRS_MonitorRefusedPostProcess();
		}

		//�����ǰ����绰
		if (tGPRSFSM.u8ATCmd == _ATCMD_PHONE_DIAL)
		{
			tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;

			tGPRSFSM.u8ATCmd = _ATCMD_IDLE_;
		}
		break;

		//"NO ANSWER",
		case _ATACK_NO_ANSWER:
		tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;
		break;
#endif

#if	_MSG_
		/*����*/
		//���Ŵ洢������״̬
	case _ATACK_CPMS: {
		u8 u8Num = 0, u8Index = 1;

		ZeroMem((u8*) &tGPRSSMGFSM, sizeof(tGPRSSMGFSMType));

		while (*pData != ',') {
			u8Num = 10 * u8Num + (*pData - 0x30);
			pData++;
		}
		pData++;
		u8SMGCount = u8Num;

		u8Num = 0;
		while (*pData != ',') {
			u8Num = 10 * u8Num + (*pData - 0x30);
			pData++;
		}

		u8MaxSMGCount = u8Num;	//�����������
		u8ReadIndex = u8SMGCount;	//���Ŷ�����
	}
		break;
		//�оٶ���״̬
	case _ATACK_CMGL: {
		u16 u16Length;

		while (*pData != ',' || *(pData + 1) != ',') {
			pData++;
		}
		pData += 2;
		/*

		 //��ȡPDU��Ϣ���Ⱥ�����
		 u16Length = GPRS_GetPduCode(au8Buffer_GPRSFrameRx);

		 //����PDU����
		 GPRS_PDUSMS_Decode(au8Buffer_GPRSFrameRx,u16Length);
		 */

	}
		break;
		//ɾ������
	case _ATACK_CMGD:

		/*
		 //ɾ�����Ų���״̬�л�
		 if(tGPRSSMGFSM.eCdmaMsgStatus ==  _eGPRS_SMG_DEL_REQ)
		 {
		 tGPRSSMGFSM.eCdmaMsgStatus = _eGPRS_SMG_DEL_OK;
		 }
		 */
		break;
		//�¶�����ʾ
	case _ATACK_CMTI: {
		u8 u8Index = 0;
		while (*pData != ',') {
			pData++;
		}
		//����','
		pData++;

		while (*pData >= 0x30 && *pData <= 0x39) {
			u8Index = u8Index * 10 + *pData - 0x30;
			pData++;
		}
		if (u8Index > 0) {
			//������ѯ���ж���
			tGPRSSMGFSM.u16CheckSmg_Timeout = 1;

			u8ReadIndex = u8Index;

			++u8SMGCount;
		}

	}
		break;
		//��ȡ����
	case _ATACK_CMGR: {
		/*
		 while(*pData != ',' || *(pData + 1)!= ',')
		 {
		 pData++;
		 }
		 pData += 2;
		 //�յĶ���Ϣ��˵��������λ��û�д洢���ţ���ֱ�Ӳ����¸�λ��
		 if(*pData == 0x30)
		 {
		 --u8ReadIndex;

		 if(u8ReadIndex == 0 && !tGPRSFSM.fReadMsgFromHigh)
		 {
		 u8ReadIndex = u8MaxSMGCount;

		 tGPRSFSM.fReadMsgFromHigh = 1;
		 }
		 else
		 {
		 //����Ѿ��������Ż�����λ�ö�ȡ����ô�Ѿ���ȡ�����	u8ReadIndex == 0
		 if(tGPRSFSM.fReadMsgFromHigh == 1 && u8ReadIndex == 0)
		 {
		 tGPRSFSM.fReadMsgFromHigh = 0;

		 u8SMGCount = 0;

		 if(tGPRSFSM.u8PendingReconnectServer)
		 {
		 tGPRSFSM.u8PendingReconnectServer = 0;

		 //����������
		 GPRS_ReconnectServer();
		 }
		 //ϵͳ��λ��־��λ
		 if(tGPRSFSM.u8PendingResetProgram)
		 {
		 tGPRSFSM.u8PendingResetProgram = 0;

		 //����ϵͳCAN����
		 Save_CanMiles();

		 //ϵͳ��λ����������ϵͳ
		 SystemReset();
		 }
		 if(tGPRSFSM.u8PendingResetgprs)
		 {
		 tGPRSFSM.u8PendingResetgprs = 0;

		 GPRS_ResetPower();
		 }
		 }
		 }

		 tGPRSSMGFSM.eCdmaMsgStatus = _eGPRS_SMG_IDLE;


		 }
		 //�Ķ����Ų���״̬�л�
		 else
		 {
		 u16	u16Length;
		 if(tGPRSSMGFSM.eCdmaMsgStatus ==  _eGPRS_SMG_READ_REQ)
		 {
		 tGPRSSMGFSM.eCdmaMsgStatus = _eGPRS_SMG_READ_OK;
		 }
		 //��ȡPDU��Ϣ���Ⱥ�����
		 u16Length = GPRS_GetPduCode(au8Buffer_GPRSFrameRx);

		 //����PDU����
		 GPRS_PDUSMS_Decode(au8Buffer_GPRSFrameRx,u16Length);

		 }

		 }

		 //���ڽ���
		 UART_UNLOCK();

		 tGPRSFSM.u8ATCmd = _ATCMD_IDLE_;
		 */
	}
		break;
#endif
		/*������*/
		//"ERROR",
	case _ATACK_ERROR:

		//"+CMS ERROR:",
	case _ATACK_CMS_ERROR:

		//"COMMAND NOT SUPPORT",
	case _ATACK_COMMAND_NOT_SUPPORT:

		//"TOO MANY PARAMETERS",
	case _ATACK_TOO_MANY_PARAMETERS:

		//"+CME ERROR:"
	case _ATACK_CME_ERROR:

		//���д�������ͳһ����
		QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_ERROR_SIG), NULL);
		break;
		//δ֪����
	case _ATACK_UNDEFINE:
	default:
		break;

	}
}

u16 GPRS_TakeRxSMG(u8* pu8Buffer) {
#if 0
	u16 u16AvailableBytes = 0;

	if(pu8Buffer == NULL)
	{
		return 0;
	}

	while(*(tGPRSFSM.pu8Buffer_Rx + tGPRSFSM.u16Read_Rx) != GPRS_DATA_CR_)
	{
		*pu8Buffer++ = *(tGPRSFSM.pu8Buffer_Rx + tGPRSFSM.u16Read_Rx);

		++tGPRSFSM.u16Read_Rx;
		if ( tGPRSFSM.u16Read_Rx >= tGPRSFSM.u16BufferSize_Rx)
		{
			tGPRSFSM.u16Read_Rx = 0;
		}

		u16AvailableBytes++;
	}
	//ȥ������β����0x0D 0x0A
	tGPRSFSM.u16Read_Rx += 2;
	if ( tGPRSFSM.u16Read_Rx >= tGPRSFSM.u16BufferSize_Rx)
	{
		tGPRSFSM.u16Read_Rx -= tGPRSFSM.u16BufferSize_Rx;
	}
	return u16AvailableBytes;
#endif
	return 0;
}

/**
 * @brief	�Ӵ��ڽ��ջ���������ȡ����֡<CR><LF>[data]<CR><LF><CR><LF>[RESULT]<CR><LF>
 * @param	pu8Buffer		[in]	����֡����
 * 			begin			[in]	��ʼ��ȡλ�ã���Ч��ʵ��ΪComm_Rx.read
 * 			end				[in]	��ֹ��ȡλ�ã����˲�����Ч�����һ������ȡ�ַ�λ��
 * @return		ʵ�����ݳ���
 */
u16 GPRS_TakeRxFrame(u8* pu8Buffer, u16 begin, u16 end) {
#define PREFIX_POS_UNKOWN	0xFFFF
	static u16 _head = PREFIX_POS_UNKOWN;
	static u8 u8Data = 0;
	static u16 _tail = 0;
	static u16 _write = 0;
	static u16 _read = 0;
	u8 *pBuf = pu8Buffer;
	u16 len = 0;

	/*�ݴ滺������дλ��*/
	_write = Comm_Rx.write;
	_read = Comm_Rx.read;

//	if(_read >= _write)
//		return 0;

	while (_read != _write) {
		u8Data = *(Comm_Rx.pBuf + _read);

		//ƥ���һ��ǰ׺��head_previous��¼ǰ׺<CR>λ��
		if (_head == PREFIX_POS_UNKOWN) {
			if (u8Data == CHAR_LF) {
				u8 head_previous;
				if (_read == 0)
					head_previous = RX_BUF_SIZE - 1;
				else
					head_previous = _read - 1;

				//ƥ�䵽ǰ׺<CR><LF>
				//           ��
				//			_head
				if (*(Comm_Rx.pBuf + head_previous) == CHAR_CR) {
					_head = head_previous;
				}
			}
		}
		//�ҵ�ǰ׺����λ��׺
		else {
			if (u8Data == CHAR_LF) {
				u16 tail_previous;

				if (_read == 0)
					tail_previous = RX_BUF_SIZE - 1;
				else
					tail_previous = _read - 1;

				//�ҵ���׺<CR><LF>
				//         ��
				//	      _tail
				if (*(Comm_Rx.pBuf + tail_previous) == CHAR_CR) {
					//				ATMsgEvt *pe;
					u16 next = 0;

					_tail = tail_previous;				//Comm_Rx.write;

					//��ָ��ָ��ǰ׺<LF>
					next = _head + 1;
					WRAP_AROUND(next, RX_BUF_SIZE);

					//��ָ��ָ��ǰ׺<CR><LF>��һ���ֽ�
					next++;
					WRAP_AROUND(next, RX_BUF_SIZE);

					//ǰ׺�ͺ�׺���ǽ���<CR><LF><CR><LF>
					//							����
					//						  next _tail
					//˵���м�������
					while (next != _tail) {
						*pBuf++ = *(Comm_Rx.pBuf + next);
						next++;
						WRAP_AROUND(next, RX_BUF_SIZE);
						len++;
					}					//if(next != previous)

					//��ָ��ָ����һ������λ��
					_read++;
					WRAP_AROUND(_read, RX_BUF_SIZE);

					//�ҵ�һ������֡����ָ��ָ����һ֡���ֽڣ����򲻸��¶�ָ��λ��
					Comm_Rx.read = _read;

					//׼����һ��ƥ��
					_head = _tail;
					return len;
				}
			}
		}					//else

		_read++;
		WRAP_AROUND(_read, RX_BUF_SIZE);
	}					//end while

	return 0;
}

#if 0

while(start_cursor != end)
{
	//������ǰ׺�ַ�
	if((*(Comm_Rx.pBuf + start_cursor) != CHAR_CR) && (*(Comm_Rx.pBuf + start_cursor) != CHAR_LF))
	{
		start_cursor++;
	}
	else if()

	//����<CR><LF>ǰ׺�ַ�
	if(memcmp_wrap_(Comm_Rx.pBuf, start_cursor, end, RX_BUF_SIZE, "\r\n"))
	{
		start_cursor += 2;
	}
	else
	{
		break;
	}

	/*
	 //����<CR><LF>ǰ׺�ַ�
	 if((*(Comm_Rx.pBuf + start_cursor) != CHAR_CR) && (*(Comm_Rx.pBuf + start_cursor) != CHAR_LF))
	 {
	 start_cursor++;
	 }
	 */
}

//����
if(start_cursor == end)
{
	Comm_Rx.read = start_cursor;
	return 0;
}

//��ʼ������������
start_cursor++;
WRAP_AROUND(start_cursor, RX_BUF_SIZE);

//ת���������ݣ�ֱ��ƥ�䵽��׺ֹͣ
while(start_cursor != end)
{
	flag_cur = *(Comm_Rx.pBuf + start_cursor);

	//��׺���֣��˳�����
	if((flag_former == CHAR_CR) && (flag_cur == CHAR_LF))
	break;

	*pu8Buffer++ = *(Comm_Rx.pBuf + start_cursor++);
	WRAP_AROUND(start_cursor, RX_BUF_SIZE);
	dataLen++;

	flag_former = flag_cur;
}

//��λ���������֡��׺<LF>
start_cursor++;
WRAP_AROUND(start_cursor, RX_BUF_SIZE);

//���¶�λ�ã�ָ��β׺��һ�ַ�
Comm_Rx.read = start_cursor;

//����ʵ�����ݳ��ȣ�ȥ��һ����׺<CR>
return dataLen - 1;

#endif

/************************************************************************************************************
 *����������		�õ�GPRS��������֡����
 *��	   �ڣ�		pu8Buffer��		[IN]����֡������
 *��    �ڣ�		����֡����
 ************************************************************************************************************/
_eATACK GPRS_GetRxFrameType(u8* pu8Buffer) {
	u8 i = 0;

	//ָ��Ϊ�գ�����δ֪����
	if (pu8Buffer == NULL)
		return _ATACK_UNDEFINE;

	//�жϽ��յ�����֡����
	for (i = 0; i < (u8) _ATACK_END; i++) {
		if (memcmp_(pu8Buffer, (u8*) ATACK[i], strlen_(ATACK[i]))) {
			break;
		}
	}

	if (i < (u8) _ATACK_END) {
		return (_eATACK) i;
	}

	return _ATACK_UNDEFINE;
}

/**
 * @brief	��������ģʽ
 * @param	��
 * @return		��
 */
void GPRS_RequestSleep() {
	static const char cmd[] = "AT+CFUN=0\r";

	TRACE_(QS_USER, NULL, "[UART]Tx: %s", cmd);

	//�ɹ�д�����ݵ����ͻ�����
	GPRS_WriteData(cmd, strlen_(cmd));
}

/**
 * @brief	����ģʽ
 * @param	��
 * @return		��
 */
void GPRS_RequestWakeup() {
	static const char cmd[] = "AT+CFUN=1\r";

	TRACE_(QS_USER, NULL, "%s", cmd);

	//�ɹ�д�����ݵ����ͻ�����
	GPRS_WriteData(cmd, strlen_(cmd));
}

/**
 * @brief	����CSQ
 * @param	csq		[IN]	��ǰ�ź�ǿ��
 * @return		��
 */
void GPRS_SetCSQ(u8 csq) {
	s_csq = csq;
}

u8 GPRS_GetCSQ() {
	return s_csq;
}

/**
 * @brief	PDU����Ϣ����
 * @param	pu8GPRSMessage	[IN]	PDU��ָ��
 * 			u16GPRSMsgLen	[IN]	PDU������
 * @return		1=�ɹ���0=ʧ��
 */
u8 GPRS_PDUSMS_Decode(u8* pu8GPRSMessage, u16 u16GPRSMsgLen) {
	u8 i;
	u8 u8SMS_LongMsg;					//����Ϣ��־
	u8 u8SMS_Type;						//����Ϣ����
	_GPRS_SENDPDUSMSType tSMS_OA;	//Դ����
	u8 u8SMS_DCS;						//���ݱ��뷽ʽ
	u8 u8SMS_UDL;						//�û����ݳ���
	u8 u8SMS_UDHI_Len;					//�û�����ͷ��Ϣ����
	u8 u8SMS_UDHI_ElementA;			//�û�����ͷ��ϢԪ��A
	u8 u8SMS_UDHI_ElementA_Len;		//�û�����ͷ��ϢԪ��A�鳤��
	u8 u8SMS_LongMsg_Ref;				//��������Ϣ�ο�
	u8 u8SMS_LongMsg_Total;			//��������Ϣ��������
	u8 u8SMS_LongMsg_Sequence;			//��������Ϣ��ǰ���
	u16 u16SMS_Year;					//����Ϣ�ύ-��
	u8 u8SMS_Month;					//����Ϣ�ύ-��
	u8 u8SMS_Day;						//����Ϣ�ύ-��
	u8 u8SMS_Hour;						//����Ϣ�ύ-ʱ
	u8 u8SMS_Minute;					//����Ϣ�ύ-��
	u8 u8SMS_Second;					//����Ϣ�ύ-��
	s32 s32SMS_Store_Index;				//����Ϣ��ǰ�洢ָ��
	u8 u8SMS_LeftBits = 0;				//����Ϣ7bit����ʱ��������Ϣͷ������¶�����
	u8 u8SMSCLen;						//���ŷ������ĺ��볤��
	u8 u8Byte;							//����볤��(0/1/2�ֽ�)
	u32 u32Coded;
	u8 u8SMS_OA_Type;
	u8* pu8GPRSMsgTail = NULL;
	u8 cPhoneNum[20] = { 0 };
	u8 u8PhoneLen = 0;

	if (pu8GPRSMessage == NULL || u16GPRSMsgLen == 0) {
		return 0;
	}

	pu8GPRSMsgTail = pu8GPRSMessage + u16GPRSMsgLen;

	//��һ����ͨ��Ϣ
	u8SMS_LongMsg = 0;

	//��1��>:	���ŷ������ĺ���(SMSC)��1~12���ֽ�
	//SMSC��ʽ:	�ֽ������������ͣ��������ĺ���
	//ȡ���ŷ������ĺ���-�ֽ���
	u8SMSCLen = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);					//"08"
	pu8GPRSMessage += 2;

	//��ָ����������Ϣ���ĺ���
	pu8GPRSMessage += (u8SMSCLen << 1);						//"91683108501505F0"

	//��2��>	PDUЭ����������(Type)
	//Bit7:		Ӧ��·��(RP)����0����ʾδ����·������1����ʾ������Ӧ��·����һ��Ϊ��0����
	//Bit6:		�û�����ͷ��ǩ(UDHI)����0����ʾ�û�����(UD)���ֲ�����ͷ��Ϣ����1����ʾ��
	//			�����ݲ��ֵĿ�ʼ����ͷ��Ϣ�����ڳ���(>140�ַ�)����Ϣ��������Ϣ��ͼƬ��
	//			�����ȣ������ݳ��Ⱦ�����140�ֽڣ���ʱ��λ��Ϊ��1����
	//Bit5:		״̬����ָʾ(SRI)����0����ʾ״̬���治�����ڶ����ı��У���1����ʾ״̬
	//			���溬�ڶ����ı��С�
	//Bi4-3:	0
	//Bit2:		����(MMS)����ֵ�������������ã���0����ʾ�������Ļ����ڸ����δ���ʹ�
	//			�Ķ��ţ���1����ʾ���յ��������һ�����š����ն�ԭ���Ͽ��Բ������λ��
	//Bit1-0:	��Ϣ����(MTI)����00����ʾ���Ž��գ���01����ʾ���Ͷ��ţ���10����ʾ���
	//			��11��Ϊ������
	u8SMS_Type = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);				//"04"
	pu8GPRSMessage += 2;
	if ((u8SMS_Type & 0x03) != 0) {
		//����һ�����ն���Ϣ
		return 0;
	}

	//��3��:	�����ߺ���(caOA)��2~12�ֽ�
	//�����ߺ����ʽ:	�ֽ������������ͣ��������ĺ���
	//��3.1��> Դ���볤��(���ָ���)
	tSMS_OA.u8DigitalLength = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);	//"0B"
	pu8GPRSMessage += 2;
	if (tSMS_OA.u8DigitalLength > 20) {
		//�������̫��
		return 0;
	}

	//������Ч
	tSMS_OA.cValidTag = 0x55;

	//��3.2��> Դ�������ͣ�91H=���ʺ��룬A1H=���ں���
	u8SMS_OA_Type = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);
	pu8GPRSMessage += 2;
	if (u8SMS_OA_Type == 0xA1) {
		//����
		tSMS_OA.fNumberType = 0;
	} else if (u8SMS_OA_Type == 0x91) {
		//����
		tSMS_OA.fNumberType = 1;
	} else {
		//�������
		return FALSE;
	}

	//��3.3��> Դ����
	for (i = 0; i < (tSMS_OA.u8DigitalLength + 1) / 2; i++) {
		//ȡ����,BCD����
		tSMS_OA.au8Number[i] = SwapNibble(
				GPRS_GetPDUSMS_HexByte(pu8GPRSMessage));
		pu8GPRSMessage += 2;
	}

	//Դ����Ϸ��Լ��
	u8PhoneLen = PhoneNo_ConvertBaseToAscii(cPhoneNum, &tSMS_OA, 0);

#if 0
	//���볤�ȴ���
	if(0 == u8PhoneLen || u8PhoneLen != ptCommParam->u8CenterMonitorPhoneLen)
	{
		return 0;
	}

	//���벻һ�£���ִ���κ�����
	if(!memcmp(cPhoneNum, ptCommParam->aCenterMonitorPhone, ptCommParam->u8CenterMonitorPhoneLen))
	{
		return 0;
	}
#endif

	//��4��>	Э���ǩ(PID)��1�ֽ�
	//Bit7-6:	���á���00����ʾָ���ġ����͡���Ч����01����ʾGSM3.4Э���ʶ����10��Ϊ������
	//			��11����ʾ�ر�ר���ڷ������ġ�
	//Bit5:		Э�顣��0����ʾ��Ե����ϢЭ�飬��ʹ��Զ�����磬��1����ʾʹ��Զ�����硣
	//Bit4-0:	���͡���00000����ʾ��������00001����ʾ�紫����00010����ʾGroup3���棬
	//			��00100����ʾ��������00101����ʾŷ��������Ϣϵͳ(ERMES)����00110����ʾ����ϵͳ��
	//			��10001����ʾ�κλ���X.400�Ĺ�����Ϣ����ϵͳ����10010����ʾEmail��
	GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);								//����"00"
	pu8GPRSMessage += 2;

	//��5��>	���ݱ��뷽ʽ(DCS)
	//Bit7-6:	0
	//Bit5:		ѹ������0����ʾ����Ϣ�ı���ѹ������1���ı���GSM��׼�㷨ѹ����
	//Bit4:		ָ������0����ʾ�����͡����������ã���1����ʾ�����͡�������Ч�����ն�Ҫ����ָ��
	//			�ġ����͡�������Ϣ�Ĵ���
	//Bit3-2:	���롣��00����ʾ�ı���8λ����ģʽ�²���7λ���룬�ַ����ñ�׼��GSM�ַ�����
	//			���ֱ��뷽ʽ�¿�ʵ�ʴ���160��7λ�ַ�(140 8/7=160)����01����ʾ8λ���뷽ʽ��
	//			��ʵ�ʴ���140�ֽ����ݣ���Ҫ�������ݴ��䡣��10����ʾUCS2���룬ΪUnicode��
	//			˫�ֽ��ַ��������ֶ���Ϣ�������ֱ��롣��11��Ϊ������
	//Bit1-0:	���͡���00��ΪClass 0����ʾ���ն�Ӧֱ�ӽ�����Ϣ��ʾ����Ļ��(��Ϊ����)�����洢��
	//			Ҳ���辭���κβ���������ʾ����01��ΪClass 1���������Ϣ��GSMģ����ƣ�����Ӧ��
	//			�洢��SIM����ģ��洢���С���10��ΪClass 2����Ҫ����SIM�����ض���Ϣ����11��Ϊ
	//			Class 3���������Ϣ����AT+CNMI������ơ��ر�أ�����ָ����Ϊ0ʱ�������͡�����
	//			��Ч������Ϣ���洢��SIM����ģ��Ĵ洢���С�
	u8SMS_DCS = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);				//���뷽ʽDCS
	pu8GPRSMessage += 2;

	//��6��> ���ŷ�������ʱ���(SCTS)
	u16SMS_Year = (u16) BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage))) + 2000;	//��(BCD)-"50"
	pu8GPRSMessage += 2;
	u8SMS_Month = BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//��(BCD)-"60"
	pu8GPRSMessage += 2;
	u8SMS_Day = BCD2Decimal(SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//��(BCD)-"01"
	pu8GPRSMessage += 2;
	u8SMS_Hour = BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//ʱ(BCD)-"90"
	pu8GPRSMessage += 2;
	u8SMS_Minute = BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//��(BCD)-"33"
	pu8GPRSMessage += 2;
	u8SMS_Second = BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//��(BCD)-"72"
	pu8GPRSMessage += 2;
	GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);							//����:ʱ��-"20"
	pu8GPRSMessage += 2;

	//��7��> �û����ݳ���(UDL)
	u8SMS_UDL = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);					//"1C"
	pu8GPRSMessage += 2;

	//����Ϣ�����û�����ͷ��Ϣ(UDHI)?
	if (u8SMS_Type & 0x40) {
		//�û�����ͷ����
		u8SMS_UDHI_Len = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);
		pu8GPRSMessage += 2;

		//7λ���뷽ʽ?
		if ((u8SMS_DCS & 0x0C) == 0x00) {

			//ʵ���û����ݶ���λ,�ڿ�ʼ����ǰҪ���Ե�λ(������Ϣͷ�İ���7λ��ʽ��ѹ��λ)
			u8SMS_LeftBits = ((u8SMS_UDHI_Len + 1) * 8) % 7;

			//�û����ݳ���-UDHI����
			u8SMS_UDL -= ((u8SMS_UDHI_Len + 1) * 8 / 7);
			if (u8SMS_LeftBits) {
				//����ж���λ�����ټ�1�ֽ�
				u8SMS_UDL--;
			}
		}
		//8λ��16λUnicode����
		else {
			//�û����ݳ���-UDHI����
			u8SMS_UDL -= (u8SMS_UDHI_Len + 1);
		}

		//������Ϣͷ
		while (u8SMS_UDHI_Len) {
			//��Ԫ�ر�ǩ
			u8SMS_UDHI_ElementA = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);
			pu8GPRSMessage += 2;

			//�û�����ͷ����-1
			u8SMS_UDHI_Len--;

			switch (u8SMS_UDHI_ElementA) {
			//Ԫ������00H�����ӣ�8λ
			case 0x00:
				//Ԫ��A����
				u8SMS_UDHI_ElementA_Len = GPRS_GetPDUSMS_HexByte(
						pu8GPRSMessage);
				pu8GPRSMessage += 2;

				//�û�����ͷ����-1
				u8SMS_UDHI_Len--;

				//���Ӷ���ϢԪ��A���ȱ���=3
				if (u8SMS_UDHI_ElementA_Len == 3) {
					//���Ӷ���Ϣ�ο�
					u8SMS_LongMsg_Ref = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);
					pu8GPRSMessage += 2;
					//���Ӷ���Ϣ����
					u8SMS_LongMsg_Total = GPRS_GetPDUSMS_HexByte(
							pu8GPRSMessage);
					pu8GPRSMessage += 2;
					//���Ӷ���Ϣ��ǰ˳���
					u8SMS_LongMsg_Sequence = GPRS_GetPDUSMS_HexByte(
							pu8GPRSMessage);
					pu8GPRSMessage += 2;

					//��һ������ĳ�������Ϣ
					u8SMS_LongMsg = 1;

				}
				//�����ͷ
				else {
					//��������Ϣͷ
					pu8GPRSMessage += (u8SMS_UDHI_ElementA_Len << 1);
				}

				//�û�����ͷ����-Ԫ�س���
				u8SMS_UDHI_Len -= u8SMS_UDHI_ElementA_Len;

				break;
			}
		}

	}								//end of ����Ϣ�����û�����ͷ��Ϣ(UDHI)?

	//������Ϣ�ǳ�������Ϣ��?
	if (u8SMS_LongMsg) {
	} else {
		//��ͨ����Ϣ
//		m_cGPRS_SMS_LongMsg_Total = 0;
//		m_cGPRS_SMS_LongMsg_Count = 0;

		s_u8PDUSMSRx_LenCount = 0;

		//��ͷ��ʼ�洢
		s32SMS_Store_Index = 0;

		//�洢����ͨ����Ϣ�洢����
		u8SMS_LongMsg_Sequence = 0;
	}

	//8λģʽ��7λ����?
	if ((u8SMS_DCS & 0x0C) == 0x00) {
		//����7λģʽ����
		GPRS_SMS_7BitsDecode(0, 1, u8SMS_LeftBits);
	}

	//��8��> �û�����(UD)
	while (u8SMS_UDL) {
		//8λģʽ��7λ����?
		if ((u8SMS_DCS & 0x0C) == 0x00) {
			//7λģʽ����
			u32Coded = GPRS_SMS_7BitsDecode(
					GPRS_GetPDUSMS_HexByte(pu8GPRSMessage), 0, 0);
			pu8GPRSMessage += 2;

			//�жϽ���볤
			if ((u32Coded & 0xFFFF0000) == 0xFFFF0000) {
				//���ʣ���ַ���Ϊ1����ֻȡ�������ֽ�
				if (u8SMS_UDL < 2) {
					//������룬ʵ���������һ���ַ������ֽ�Ϊ����(һ��Ϊ0)
					u8Byte = 1;
				} else {
					//˫�����
					u8Byte = 2;
				}
			} else if ((u32Coded & 0xFFFF0000) == 0x00FF0000) {
				//��1���������
				u8Byte = 1;
			} else if ((u32Coded & 0xFFFF0000) == 0xFF000000) {
				//��2���������,�Ƶ���λ
				u32Coded >>= 8;

				u8Byte = 1;
			} else {
				//�޽����
				u8Byte = 0;

				//��ָ�볬��PDU��β
				if (pu8GPRSMessage > pu8GPRSMsgTail) {
					return 0;
				}
			}

			if (u8Byte) {
				//�洢����Ϣ����
				if (!GPRS_StoreSMSContent(u8SMS_DCS, (u16) (u32Coded & 0xFFFF),
						s32SMS_Store_Index, u8Byte, u8SMS_LongMsg_Sequence)) {
					//������
					//		GPRS_SMS_Clear();
					s_u8PDUSMSRx_LenCount = 0;

					//��/����
					return 0;
				}

				//�洢����λ��+�ֽ���
				s32SMS_Store_Index += u8Byte;

				//�û����ݳ���-�ֽ���
				u8SMS_UDL -= u8Byte;
			}
		}

		//8λ����?
		else if ((u8SMS_DCS & 0x0C) == 0x04) {
			//�洢����Ϣ����
			if (!GPRS_StoreSMSContent(u8SMS_DCS,
					(u16) GPRS_GetPDUSMS_HexByte(pu8GPRSMessage),
					s32SMS_Store_Index, 1, u8SMS_LongMsg_Sequence)) {
				//������
				//		GPRS_SMS_Clear();

				s_u8PDUSMSRx_LenCount = 0;

				//��/����
				return 0;
			}

			pu8GPRSMessage += 2;

			//�洢����λ��+1
			s32SMS_Store_Index++;

			//�û����ݳ���-1
			u8SMS_UDL--;
		}

		//16λUnicode����?
		else if ((u8SMS_DCS & 0x0C) == 0x08) {
			u32Coded = ((((u16) GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)) << 8)
					& 0xFF00);
			pu8GPRSMessage += 2;
			u32Coded |= ((u16) GPRS_GetPDUSMS_HexByte(pu8GPRSMessage) & 0x00FF);
			pu8GPRSMessage += 2;

			//�洢����Ϣ����
			if (!GPRS_StoreSMSContent(u8SMS_DCS, u32Coded, s32SMS_Store_Index,
					2, u8SMS_LongMsg_Sequence)) {
				//������
				//		GPRS_SMS_Clear();

				s_u8PDUSMSRx_LenCount = 0;

				//��/����
				return 0;
			}
			//�洢����λ��+2
			s32SMS_Store_Index += 2;

			//�û����ݳ���-2
			u8SMS_UDL -= 2;
		}

		//δ֪����
		else {
			//�����¼
			//		GPRS_SMS_Clear();

			s_u8PDUSMSRx_LenCount = 0;

			return 0;
		}
	}
	if (u8SMS_LongMsg) {
		//����Ϣ���ճ�ʱʱ��=10����
//		m_dwGPRS_SMS_Timeout = 600000;
	}

//	GPRS_NewSMS_Process(au8Buffer_SMSRx,s_u8PDUSMSRx_LenCount);

	s_u8PDUSMSRx_LenCount = 0;

	return 1;
}

/***********************************************************************************************************
 * ����������	��ȡPDU��ʽ����Ϣһ�ֽ�����
 * ��    �ڣ�	pu8GPRSMessage,	[IN]GPRS��Ӧ�ַ���ָ������
 * ��    �ڣ�	�ֽ�����
 * ˵	����
 ***********************************************************************************************************/
u8 GPRS_GetPDUSMS_HexByte(u8* pu8GPRSMessage) {
	u8 cResult, c;

	//��1�ֽ�
	c = *pu8GPRSMessage++;

	//0-9
	if (c <= 0x39) {
		c -= 0x30;
	}
	//A-F
	else {
		c -= 0x37;
	}

	cResult = c << 4;

	//��2�ֽ�
	c = *pu8GPRSMessage++;

	//0-9
	if (c <= 0x39) {
		c -= 0x30;
	}
	//A-F
	else {
		c -= 0x37;
	}

	//���
	cResult |= c;

	return cResult;
}

/***********************************************************************************************************
 * ����������	7λPDU����Ϣ����
 * ��    �ڣ�	u8PDUu8��	[IN] ��ǰ�ֽ�
 bStartup��	[IN] ������־,��=����
 cLeftBits��	[IN] ǰ������λ
 * ��    �ڣ�	u16��bit0-7Ϊ��1�����������
 bit8-15Ϊ��2�����������
 bit16-23ΪFF�����1���������Ч
 bit24-31ΪFF���2���������Ч
 * ˵	����
 ***********************************************************************************************************/
u32 GPRS_SMS_7BitsDecode(u8 u8PDUByte, u8 u8Startup, u8 u8LeftBits) {
	u8 u8DecodedByte;
	u32 u32FinalDecoded;

	//���¿�ʼ����?
	if (u8Startup) {
		//���¿�ʼ
		u8GPRS_7bitDecode_LeftCode = 0;
		u8GPRS_7bitDecode_Sequence = u8LeftBits;

		//ǰ�����룬���׸��ַ�����,�������Ϊ6λ�����¸��ַ�������Ϊ��8λ���
		if (u8LeftBits) {
			fGPRS_7bitDecode_Skip = 1;
		}

		return 0;
	}

	//�޽����
	u32FinalDecoded = 0;

	u8DecodedByte = (u8PDUByte << u8GPRS_7bitDecode_Sequence)
			| u8GPRS_7bitDecode_LeftCode;
	u8GPRS_7bitDecode_LeftCode = u8PDUByte >> (7 - u8GPRS_7bitDecode_Sequence);

	//������λ����+1
	u8GPRS_7bitDecode_Sequence++;

	//��1�������
	u32FinalDecoded |= ((u8DecodedByte & 0x7F) | 0x00FF0000);

	//�������ַ�?
	if (fGPRS_7bitDecode_Skip) {
		fGPRS_7bitDecode_Skip = 0;

		u32FinalDecoded &= 0xFF00FFFF;
	}

	//ѭ����ÿ��7���ֽڣ�����1�ֽ�
	if (u8GPRS_7bitDecode_Sequence == 7) {
		//������λ����=0
		u8GPRS_7bitDecode_Sequence = 0;

		//��2�������
		u32FinalDecoded |= ((((u16) u8GPRS_7bitDecode_LeftCode) << 8) & 0x7F00);

		//˫�����
		u32FinalDecoded |= 0xFF000000;

		//��ʣ����
		u8GPRS_7bitDecode_LeftCode = 0;
	}

	return u32FinalDecoded;
}

/*******************************************************************************************
 ����������	�������ṹ����ת����ASCII��
 ��ڣ�	pcNo,			[OUT] ASCII����ָ��
 ptBaseNo,		[IN]  ��������ָ��
 nInternational,	[IN]  0=���ڣ�ֻ�������ں��벿�֣�1=���ʣ�����ȫ������
 ���أ�	u8,			���볤��(�ֽ���)
 ��ƣ�	��ӱ
 *******************************************************************************************/
u8 PhoneNo_ConvertBaseToAscii(u8 *pcNo, _GPRS_SENDPDUSMSType *ptBaseNo,
		u8 nInternational) {
	u8 nLen = 0;
	u8 nSkipDigits = 0;
	u8 i = 0;

	//������Ч?
	if ((ptBaseNo->cValidTag != 0x55) || (ptBaseNo->u8DigitalLength == 0)) {
		return 0;
	}

	//���ֻҪ����ں��룬�����������ǹ��ʺ��룬��ȥ����������
	if ((nInternational == 0) && (ptBaseNo->fNumberType == 1)) {
		if (ptBaseNo->au8Number[0] == 0x86) {
			//������������
			nSkipDigits = 2;
		}
	}

	//��������
	for (i = 0; i < (ptBaseNo->u8DigitalLength + 1) / 2; i++) {

		u8 cNo = ptBaseNo->au8Number[i];
		u8 c;

		//�������?
		if ((cNo & 0xF0) == 0xF0) {
			//����
			break;
		}

		if (nSkipDigits == 0) {
			//��λת����ASCII

			//�ǡ�*��?
			if ((cNo & 0xF0) == 0xA0) {
				c = '*';
			}
			//�ǡ�#��?
			else if ((cNo & 0xF0) == 0xB0) {
				c = '#';
			} else {
				c = ((cNo >> 4) & 0x0F) + 0x30;
			}

			//�洢
			*(pcNo + nLen++) = c;
		} else {
			//����1λ
			nSkipDigits--;
		}

		//�������?
		if ((cNo & 0x0F) == 0x0F) {
			//����
			break;
		}

		if (nSkipDigits == 0) {
			//��λת����ASCII

			//�ǡ�*��?
			if ((cNo & 0x0F) == 0x0A) {
				c = '*';
			}
			//�ǡ�#��?
			else if ((cNo & 0x0F) == 0x0B) {
				c = '#';
			} else {
				c = (cNo & 0x0F) + 0x30;
			}

			//�洢
			*(pcNo + nLen++) = c;
		} else {
			//����1λ
			nSkipDigits--;
		}
	}

	//���غ��볤��
	return nLen;
}

/***********************************************************************************************************
 * ����������	������λԪ��
 * ��    �ڣ�	u8Byte��	[IN]�����ֽ�
 *				0xMN -> 0xNM
 * ��    �ڣ�	����ֽ�
 ***********************************************************************************************************/
u8 SwapNibble(u8 u8Byte) {
	return ((u8Byte << 4) & 0xF0) | ((u8Byte >> 4) & 0x0F);
}

/***********************************************************************************************************
 * ����������	BCDת��Ϊʮ����
 * ��    �ڣ�	u8BCD��	[IN]�����ֽ�
 * ��    �ڣ�	����ֽ�
 ***********************************************************************************************************/
u8 BCD2Decimal(u8 u8BCD) {
	return ((u8BCD >> 4) * 10) + (u8BCD & 0x0F);
}

/*******************************************************************************************
 *����������	����Ϣ��Ԫ�洢
 *��	   �ڣ�	u8SMS_DCS				[IN] ���ű��뷽ʽ
 *			u16Code,				[IN] ��Ҫ�洢����Ԫ
 *			s16Index,				[IN] �洢λ������
 *			s16Bytes,				[IN] �ֽ���,1/2
 *			s16SequenceLongSMS,		[IN] ��Ϊ0ʱ����������Ϣ��ţ�=0ʱ��ʾ��ͨ����Ϣ
 *��    �ڣ�	1=�洢�ɹ���=0�洢ʧ��
 *******************************************************************************************/
u8 GPRS_StoreSMSContent(u8 u8SMS_DCS, u16 u16Code, s16 s16Index, s16 s16Bytes,
		s16 s16SequenceLongSMS) {
	//�ǳ�������Ϣ?
	if (s16SequenceLongSMS) {
		return 0;
	}

	//��ͨ����Ϣ
	//Unicode?
	if ((u8SMS_DCS & 0x0C) == 8) {
		//Unicode������2�ֽ�
		if (s16Bytes != 2) {
			return 0;
		}

		//�洢����?
		if (s_u8PDUSMSRx_LenCount < 139) {
			//�洢���ֽ�
			au8Buffer_SMSRx[s16Index] = (u16Code >> 8) & 0xFF;

			//�洢���ֽ�
			au8Buffer_SMSRx[s16Index + 1] = u16Code & 0xFF;

			//���鳤��+2
			s_u8PDUSMSRx_LenCount += 2;
		} else {
			//�洢����
			return 0;
		}
	}

	else {
		//�洢����?
		if (s_u8PDUSMSRx_LenCount < 160) {
			//�洢һ���ֽ�
			au8Buffer_SMSRx[s16Index] = u16Code & 0xFF;
			//���鳤��+1
			s_u8PDUSMSRx_LenCount++;
		}

		else {
			//�洢����
			return 0;
		}

		//˫�ֽ�?
		if (s16Bytes == 2) {
			//�洢����?
			if (s_u8PDUSMSRx_LenCount < 160) {
				//�洢һ���ֽ�
				au8Buffer_SMSRx[s16Index + 1] = (u16Code >> 8) & 0xFF;
				//���鳤��+1
				s_u8PDUSMSRx_LenCount++;
			}

			else {
				//�洢����
				return 0;
			}
		}
	}

	return 1;
}

/**
 * GPRSӲ����ʼ��
 *
 */
void GPRS_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = GSM_PWR_PIN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GSM_DTR_PIN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GSM_PWKEY_PIN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	///���ڳ�ʼ��
	USART_Configuration(COM_GPRS, 115200);
}
