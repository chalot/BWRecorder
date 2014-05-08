/**   
* @Title: gprs.h  
* @Description: TODO(��һ�仰�������ļ���ʲô) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-14 ����9:42:51 
* @version V1.0   
*/ 


#ifndef GPRS_H_
#define GPRS_H_

#include <qp_port.h>
#include <stm32_gpio.h>
#include "type.h"


#define		_TCP_		0
#define		_UDP_		1
#define		_MSG_		1

#define 	CONNMODE  _TCP_   //UDP
//���UIM��
#define		_UIM_DETECT			0

//#define 	BUFFER_LENGTH_GPRSFRAMERx					(1200)		//��������֡����������С
#define		MAX_DATA_LEN								1500
#define		BUFFER_LENGTH_SMSRx							160			//���Ž��ջ�����

//��������״̬
#define 	NET_STATE_UNKNOWN							0	 		//δ֪
#define		NET_STATE_LINKED							1			//����������

//#define		GPRS_SMG_CNT_		10


//#define	LINK_MAINTAIN_CYCLE_SHORT					10				//�ȴ���������¼����������
//#define	TIMEOUT_SERVER_DISCONNECT					120				//��·û�����ӳ�ʱ
//#define	TIMEOUT_EM310_READY							3				//EM310�����������ݰ�
//
//#define	SIGNAL_DENSITY_CHECK_TIMEOUT				40				//��ѯ�ź�ǿ��
//
//#define	SOCKET_ID										0			//socketͨ����
//#define	CDMA_IPSENDEX_MODE								1			//����ת�����ͷ�ʽ
//#define	CDMA_LOACL_PORT									8855		//���ض˿ں�
//#define	CDMA_DELALL_READ								3			//ɾ�����ű�ʶ��3��ʾɾ�����ж���


//����״̬
#define NET_STATE_REGISTED            0X01 //��ע��
#define NET_STATE_CONNECTING          0X02 //���ӷ�����
#define NET_STATE_CONNECTED           0X03 //ͨ���ѽ���
#define NET_STATE_SERVERCONNECTED     0X03 //ͨ���ѽ���
#define NET_STATE_UNKOWN              0XFF //δ֪


//�绰״̬
typedef enum
{

	ePHONE_STATE_IDLE = 0,						//����
	ePHONE_STATE_DIAL,							//�Ѳ���
	ePHONE_STATE_CALLING,						//�Է�����
	ePHONE_STATE_TALKING,						//ͨ����
	ePHONE_STATE_BUSY,							//�绰æ
	ePHONE_STATE_HANGING,						//�һ���
	ePHONE_STATE_BREAK							//�Ҷ�
}ePHONESTATE;

//�绰����״̬
typedef enum
{
	ePHONE_CTRL_IDLE = 0,						//����
	ePHONE_CTRL_DIAL,						//���󲦺�
	ePHONE_CTRL_ANSWER,						//�������
	ePHONE_CTRL_HANG						//����Ҷ�
}ePHONECTRL;

//�绰�������
#define	GPRS_PHONE_ANSWERCLIP_COUNTS   2	//��������n�κ����

//��������״̬
typedef enum
{
	ePHONE_MONITOR_IDLE = 0,						//����
	ePHONE_MONITOR_REQ,							//�������
	ePHONE_MONITOR_MONITORING				//���ڼ���
}ePHONEMONITOR;


//����������״̬����
typedef enum
{
	eSERVER_CONN_IDLE = 0,						//��Ч
	eSERVER_CONN_UNCONND,						//δ����

	eSERVER_CONN_SERV_CONNED,					//�Ѿ�����
	eSERVER_CONN_SERVER_APP_CONNED                 //������·�Ѿ�����
}eSERVERCONNSTATE;


//���������ӹ���
typedef enum
{

	_eCONN_IDLE = 0,					//����
	_eBEGIN_CONN_PRMSRV,			//��ʼ������������
	_eCONN_PRMSRV_FINISH,			//�������������������
	_eBEGIN_CONN_VICESRV,			//��ʼ���Ӹ���������
	_eCONN_VICESRV_FINISH			//���Ӹ����������������

}eCONNPROCEDURE;

//��Դ״̬
typedef enum
{
	_eGPRS_POWER_IDLE 			= 0,						 //��Դ��
	_eGPRS_POWER_ON,									 //��Դ��
	_eGPRS_POWER_BEFOE_ONING,								 //��Դ��������
	_eGPRS_POWER_ONING,								 //��Դ��������
	_eGPRS_POWER_OFF,									 //��Դ��
	_eGPRS_POWER_OFFING,								 //��Դ�ع�����
	_eGPRS_POWER_RESETING								 //��Դ��λ������
} _eGPRSPOWERSTATE;

//��Դ����״̬
typedef enum
{
	_eGPRS_POWER_REQ_IDLE = 0,								  //������
	_eGPRS_POWER_REQ_OFF, 									 //����رյ�Դ
	_eGPRS_POWER_REQ_ON,									 //����򿪵�Դ
    _eGPRS_POWER_REQ_RESET						 			 //����λ��Դ
} _eGPRSPOWERREQ;

//GSM���߿���
#define	GPRS_SLEEP_IDLE									0			//����
#define	GPRS_SLEEP_PROCESS								1			//���ߴ�����
#define	GPRS_SLEEP_OK									2			//����

//�绰�������
#define	GPRS_PHONE_ANSWERCLIP_COUNTS				2			//��������n�κ����


//���ӵķ���������
#define	SERVER_PRIMARY									1  			//��������������
#define	SERVER_VICE										2			//���÷�����������
#define	SERVER_SPECIFIED_PRIM							3			//ָ����������

#define	GPRS_DATA_CR_									0x0D
#define	GPRS_DATA_LF_									0x0A




//AT��������
typedef	enum
{
/*������������ 2*/
	_ATCMD_AT = 0,			//AT			   0
	_ATCMD_ATE0,			//ȡ������	 	   1

/*��ʶ��Ϣ��ѯ���� 5*/
	_ATCMD_I,				//��Ʒ��ʶ��Ϣ��ѯ	 2
	_ATCMD_CGMI,			//������Ϣ��ѯ		 3
	_ATCMD_CGMM,			//��Ʒ���Ʋ�ѯ		 4
	_ATCMD_CGMR,			//��ѯ����汾��	 5
	_ATCMD_CGSN,			//ESN��ѯ			 6

/*��ȫ�������� 1*/
	_ATCMD_CPIN,			//R-UIM ����λ�� PIN1 ��״̬��ѯ	7


/*�������ӿ�����*/
	_ATCMD_CSQ,				//��鵱�ص������ź�ǿ�ȣ�31 ���0 ��С��	 8
	_ATCMD_CREG,			//����ע������								 9

/*����ҵ��������*/
	_ATCMD_CGACT,			//����PDP����״̬							10
	_ATCMD_CGATT,			//����GPRS����״̬							11

/*INTERNET����ӿ����� 5*/
	_ATCMD_SETCONNTYPE,		//������������								12
	_ATCMD_SETAPN,			//����APN									13
	_ATCMD_SETPWD,			//����Password								14
	_ATCMD_SETUSR,			//����User									15
	_ATCMD_SETID,			//��������ID								 16
	_ATCMD_SETSOCKET,		//��������Socket							 17
	_ATCMD_SETADDR,			//���������ַ								 18
	_ATCMD_OPEN,			//������									 19
	_ATCMD_CLOSE,			//�ر�����									 20
	_ATCMD_SEND,			//��������									 21
	_ATCMD_READ,			//��ȡ����									 22

	_ATCMD_IOMODE,			//����IOģʽ								 23


/*����Ϣҵ��ӿ����� 5*/
 	_ATCMD_CNMI,			//���ö��Ž����ϱ��Ļ��ƣ��������Ҫ���Ź��ܿ��Բ�����		24
	_ATCMD_CPMS,			//���ö��Ŵ洢λ��										   25
 	_ATCMD_CMGD,			//ɾ������												   26
	_ATCMD_CMGR,			//��ȡ������Ϣ											   27
	_ATCMD_CMGL,			//�г���ǰѡ����Ŵ洢��ȫ�����ŵ���ź�״̬			   28
	_ATCMD_CMGF,			//���ö��Ÿ�ʽ											   29
/*�绰ҵ��ӿ�����*/
	_ATCMD_PHONE_TIPS,					//��������
	_ATCMD_PHONE_DIAL,					//����绰
	_ATCMD_PHONE_ANSWERCALL,			//��ͨ�绰
	_ATCMD_PHONE_HANGCALL,			//�Ҷϵ绰

/*����ģʽ��������*/
	_ATCMD_SETMODE,						//���ù���ģʽ

	_ATCMD_GETSYSTIME,		//��ѯϵͳʱ��

	_ATCMD_NUM_
}_eATCMD;


typedef	enum
{
/*��������*/
	_ATACK_OK = 0,
	_ATACK_ATE0,

/*��ʶ��ѯ����Ӧ��*/
	_ATACK_CGMI,
	_ATACK_CGMM,
	_ATACK_CGMR,
	_ATACK_CGSN,

/*��ȫ��������Ӧ��*/
	_ATACK_CPIN,

/*�������ӿ�����Ӧ��*/
	_ATACK_CSQ,
	_ATACK_CGATT,

/*����ҵ��������Ӧ��*/
	_ATACK_CREG,

/*INTERNET����ӿ�����Ӧ��*/
	_ATACK_SISW,
	_ATACK_SISR,


/*����Ϣҵ��ӿ�����Ӧ��*/
	_ATACK_CPMS,				//���ö��Ŵ洢λ��
	_ATACK_CNMI,				//�����¶���֪ͨ��ʽ
	_ATACK_CMTI,				//�յ�������ʾ
	_ATACK_CMGL,				//�о����ж���״̬
	_ATACK_CMGR,				//�Ķ�����
	_ATACK_CMGD,				//ɾ������

/*�绰ҵ�����Ӧ��*/
	_ATACK_CLIP,				//��������

	_ATACK_SYSTIME,				//ϵͳʱ��

 /*����Ӧ��*/
	_ATACK_CONNECT,
	_ATACK_RING,
	_ATACK_NO_CARRIER,
	_ATACK_ERROR,
	_ATACK_NO_DIALTONE,
	_ATACK_BUSY,
	_ATACK_NO_ANSWER,
	_ATACK_CME_ERROR,
	_ATACK_CMS_ERROR,
	_ATACK_COMMAND_NOT_SUPPORT,
	_ATACK_TOO_MANY_PARAMETERS,

	_ATACK_END,

	_ATACK_UNDEFINE = 0xFF,


}	_eATACK;

//AT������
typedef enum
{
	_ATERR_UIM_NOT_INSERT_,						//UIM��δ����
	_ATERR_CNN_ALREADY_ESTABLISHED_,			//�����ѽ���
	_ATERR_DEST_ADDR_REQ_,						//δָ��Ŀ���ַ
	_ATERR_END_,
	_ATERR_UNDEFIN_ = 0xFF
}_eATERR;


//AT����ִ�н��
typedef	enum
{
	_ATRET_OK = 0,				//�ɹ�
	_ATRET_ERROR,				//ʧ��
	_ATRET_TIMEOUT,			//��ʱ
	_ATRET_PENDING,			//�ȴ����

}	_eATRET;

/*��ʼ����������*/
#if	_MSG_
#define	ATCMD_INIT_STEPS	    10		//��ʼ������
#else
#define	ATCMD_INIT_STEPS		7		//��ʼ������
#endif

#define	ATCMD_INIT_STEP1_		4		//��ʼ����һ�����裬����⵽UIM������󣬲ſ�ʼ���к����ĳ�ʼ������
										//���UIM����ⲻ��λ��������ĳ�ʼ�����̾�û�б�Ҫ������ȥ

#if	(_MSG_ == 0)
#define INIT_STEPS_AMOUNT	   6
#else
#define INIT_STEPS_AMOUNT	   9
#endif
#define CONNECT_STEPS_AMOUNT   9

/*����������������*/

#define	ATCMD_CONNECT_STEPS		9//8		//��ʼ������




//�绰������ṹ����
typedef	struct
{
	u8	au8Number[10];			//�绰����,BCD�롣���������ָ���Ϊ����ʱ���ԡ�F����������A��=��*��,��B��=��#������E��=��+��
	u8	u8DigitalLength;		//�绰�������ָ����� 3 <= ���� <= 20, ����Ϊ��0��ʱ��ʾ��Ч
	u8	au8Data[140];			//������������
	u8	u8Data_Len;				//�����������ݳ���
	u8	cValidTag;				//�绰������Ч��־����55H����ʾ��Ч������Ϊ��Ч
	u8	fNumberType	:	1;		//���ͣ�0=���ڣ�1=����
	u8	fUnused		:	7;

} _GPRS_SENDPDUSMSType;



void GPRS_Init();
void GPRS_UartReconfigure();
void GPRS_SendTCPIPPrefix(u16 dataLen);/* ����TCPIPǰ׺ */
void GPRS_FillinTCPIPData(u8 *pDataBuf, u16 dataLen);/* ���������������� */

void GPRS_doInit(u8 initStep); //ִ�г�ʼ������
void GPRS_doConnect(u8 connectStep);//ִ����������
void GPRS_doRegister();//ִ������ע������
BOOL GPRS_doQueryData(void);//������������


/* ��֯�������������� */
u16 GPRS_FormRawFrame( u8 *pSendBuf,u16 cmd,u16 sequence,
                                void* address, u16 dataSize, u16 offset);

/*֡��ȡ*/
void GPRS_doRetrieveFrame(u16 begin, u16 end);

/*ʹ��ģ������*/
void GPRS_doEnterIntoLowerPowerState();

/*��������ģʽ*/
void GPRS_RequestSleep();

/*����ģʽ*/
void GPRS_RequestWakeup();



void GPRS_SetAttachState(u8 state);
u8 GPRS_GetAttachState();
void GPRS_SetRegState(u8 state);
u8 GPRS_GetRegState();
void GPRS_SetNetState(u8 state);
u8 GPRS_GetNetState();
void GPRS_SetCSQ(u8 csq);
u8 GPRS_GetCSQ();
u8 GPRS_CheckCmd_IsAttaching(u8 step);

/*********************** GPRSģ���Դ���� *****************************************************/
#define GPRS_POWER_ON	GPIO_SetBits(GPIOA,GPIOA_CDMA_CTRL_)  //ģ�鿪��
#define GPRS_POWER_OFF	GPIO_ResetBits(GPIOA,GPIOA_CDMA_CTRL_) //ģ��ص�


//����ָʾ�ƴ�/�ر�
#define GPRS_LED_OFF()  GPIO_ResetBits(GPIOC, GPIOC_CDMA_LED)
#define GPRS_LED_ON()  GPIO_SetBits(GPIOC, GPIOC_CDMA_LED)

//LEDָʾ����˸
#define GPRS_FLASH_LED()  	\
						do {	\
                            GPIO_WriteBit(GPIOC, GPIOC_CDMA_LED,   \
                            (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOC, GPIOC_CDMA_LED))));	\
						}while(0)

void GPRS_Lowlevel_Init();

#endif /* GPRS_H_ */
