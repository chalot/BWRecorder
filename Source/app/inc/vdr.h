#ifndef	__GPS_VDR_COMM_H__
#define	__GPS_VDR_COMM_H__

#include <stm32f2xx_conf.h>
#include <type.h>
#include <board.h>

#pragma 	pack(1)

#define UART_VDR	USART6


#define    	VDR_Send_Enable()			{USART_ITConfig(UART_VDR, USART_IT_TXE,ENABLE);}
#define		VDR_Send_Disable()			{USART_ITConfig(UART_VDR, USART_IT_TXE,DISABLE);}
#define    	VDR_Recv_Enable()			{USART_ITConfig(UART_VDR, USART_IT_RXNE,ENABLE);}
#define		VDR_Recv_Disable()			{USART_ITConfig(UART_VDR, USART_IT_RXNE,DISABLE);}


#define		VDR_VINCODE_LEN				17					//VIN�볤��
#define		VDR_CARDNUM_LEN				12					//��ʻ֤�ź��볤��
#define		VDR_CARDSPECIFY_LEN			12					//���Ʒ��೤��
#define		VDR_DRVCODE_LEN				3					//��ʻԱ���볤��
#define		VDR_DRVNUM_LEN				18					//��ʻ֤���볤��
#define		VDR_MAX_DRVIE_FATIGUE_NUM	15					//��ʻ��ʱ��¼�������ֵ
#define		VDR_MAX_DRIVERCNT			5					//��¼�Ǽ�¼��˾����Ŀ


//��ʱ����
#define		VDR_SERIAL_CHAR_TIMEOUT			5				//50ms�ֽڽ��ռ����ʱ
#define		VDR_PACKAGE_HANDLE_TIMEOUT		100				// 1s���ݰ�����ʱ

//������ͷ����
#define		_PC2VDR_COMM_FRAME_HEAD1		0xAA				//ͨѶ�����͵���¼����ʼ��ͷ1
#define		_PC2VDR_COMM_FRAME_HEAD2		0x75				//ͨѶ�����͵���¼����ʼ��ͷ2

#define		_VDR2PC_COMM_FRAME_HEAD1		0x55				//��¼�Ƿ��͵�ͨѶ����ʼ��ͷ1
#define		_VDR2PC_COMM_FRAME_HEAD2		0x7A				//��¼�Ƿ��͵�ͨѶ����ʼ��ͷ2

//�ɼ�����������ճ���ʱӦ������־��
#define		_VDR2PC_COMM_GET_ERR_TAG			0xFA

//���ò����������ݽ��ճ���ʱӦ������־��
#define		_PC2VDR_COMM_SET_ERR_TAG			0xFB

typedef struct
{
	u8		u8Head1;
	u8		u8Head2;
	u8		u8Cmd;
	u16		u16DataLength;
	u8		u8Reserved;
}tFRAMEHEAD;

//ͨѶ�����¼��ͨѶ������֡��ʽ
typedef struct
{
	u8		u8Head1;
	u8		u8Head2;
	u8		u8Cmd;
	u16		u16DataLength;
	u8		u8Reserved;
	u8		*pu8Data;
	u8		u8Checksum;
}tFRAME;

//����Ӧ��֡��ʽ����
typedef struct
{
	u8		u8Head1;
	u8		u8Head2;
	u8		u8ErrTag;
	u8		u8Reserved;
	u8		u8Checksum;
}tACK_ERRFRAME;


//�ɼ����������ֶ���
//#define		VDR_CMD_GET_VERSION					0x00	//�ɼ���¼�ǰ汾
//#define		VDR_CMD_GET_DRIVERINFO				0x01	///��ʻ����Ϣ
//#define		VDR_CMD_GET_RTC						0x02	///ʱ��
//#define		VDR_CMD_GET_DISTANCE				0x03	///���
//#define		VDR_CMD_GET_PULSE					0x04	///����ϵ��
//#define		VDR_CMD_GET_CARINFO					0x05	///������Ϣ
//#define		VDR_CMD_GET_STATECONFIG_INFO		0x06	///״̬�ź�������Ϣ
//#define		VDR_CMD_GET_ID						0x07	///Ψһ��ID
//#define		VDR_CMD_GET_SPEED					0x08	///��ʻ�ٶ�
//#define		VDR_CMD_GET_POSITION				0x09 	///λ��
//#define		VDR_CMD_GET_ACCIDENT				0x10	///�¹��ɵ�
//#define		VDR_CMD_GET_OVERTIME_DRIVING		0x11	///��ʱ��ʻ
//#define		VDR_CMD_GET_DRIVER_LOG_INFO			0x12	///����ǩ��
//#define		VDR_CMD_GET_POWER					0x13	///����
//#define		VDR_CMD_GET_PARAM_CHANGE			0x14	///�����޸�
//#define		VDR_CMD_GET_SPEED_STATUS_LOG		0x15	///�ٶ�״̬��־

#define VDR_CMD_GET_ERR_ACK		0xAA	///���������֣��Զ���
#define VDR_CMD_SET_ERR_ACK		0xBB	///���������֣��Զ���

/**
 * ���Ӧ���ʽ
 */
#define		VDR_CMD_GET_VERSION					0x00	//�ɼ���¼�ǰ汾
typedef struct {
	u8	u8Version;	///BCD
	u8	u8FixID;
} tVERSION;		///��A.6

#define tACK_VERSION tVERSION	///Ӧ��֡

#define		VDR_CMD_GET_DRIVERINFO				0x01	///��ʻ����Ϣ
typedef struct {
	u8	aDriverLicense[18];
} tDRIVERLICENSEINFO;	///��A.7

#define tACK_DRIVERLICENSEINFO tDRIVERLICENSEINFO		///Ӧ��֡

/*ʱ�����ڸ�ʽ*/
typedef struct {///BCD��
	u8	u8Year;
	u8	u8Month;
	u8	u8Day;
	u8	u8Hour;
	u8	u8Minute;
	u8	u8Second;
}tTIME;		///��A.8

#define		VDR_CMD_GET_RTC						0x02	///ʱ��
typedef struct {
	tTIME	time;
}tRTC;

#define tACK_RTC	tRTC	///Ӧ��֡

#define		VDR_CMD_GET_DISTANCE				0x03	///���
typedef struct {
	tTIME	time_RTC;
	tTIME	time_First;
	u8		aOriginDist[4];
	u8		aTotalDist[4];
}tINITDISTANCE;		///��A.9
#define tACK_INITDISTANCE	tINITDISTANCE	///Ӧ��֡

#define		VDR_CMD_GET_PULSE					0x04	///����ϵ��
typedef struct {
	tTIME	time_RTC;
	u16		u16Pulse;
}tPULSE;	///��A.10
#define tACK_PULSE	tPULSE	///Ӧ��֡


#define		VDR_CMD_GET_CARINFO					0x05	///������Ϣ
typedef struct {
	u8	aIdentification[17];	///����ʶ�����
	u8	aLicense[12];		///���������ƺ��룬��3���ֽڱ���
	u8	aClass[12];
}tCARINFO;	///��A.11
#define tACK_CARINFO	tCARINFO	///Ӧ��֡

#define		VDR_CMD_GET_STATECONFIG_INFO		0x06	///״̬�ź�������Ϣ
#define 	STATE_CONFIG_AMOUNT		5	///״̬�����ź��ֽڸ���
typedef struct {
	u8		aD0[10];
	u8		aD1[10];
	u8		aD2[10];
	u8		aD3[10];
	u8		aD4[10];
	u8		aD5[10];
	u8		aD6[10];
	u8		aD7[10];
} tSTATECONFIG;	///��A.13

typedef struct {
	tTIME	time_RTC;
	u8		u8Bytes;
	tSTATECONFIG	tConf[STATE_CONFIG_AMOUNT];
} tACK_STATECONFIG;	///��A.12


#define		VDR_CMD_GET_ID						0x07	///Ψһ��ID
typedef struct {
	u8		aCCC_Code[7];
	u8		aProductType[16];
	u8		u8Year;
	u8		u8Month;
	u8		u8Day;
	u32		u32SequenceNum;
	u8		u8Reserved[5];
}tVDRID;	///��A.14
#define tACK_UNIQID	tVDRID

#define		VDR_CMD_GET_SPEED					0x08	///��ʻ�ٶ�
/*����*/
typedef struct {
	tTIME	time_Begin;
	tTIME	time_End;
	u16		u16PackAmnt;
}tCMD_SPEED;

typedef struct {
	u8	u8AvrSpeed;
	u8	u8Status;
} tITEM_SPEED;
/*Ӧ��*/
typedef struct {
	tTIME	time_Start;
	tITEM_SPEED atItem[60];
}tACK_SPEED;	///��A.17


#define		VDR_CMD_GET_POSITION				0x09 	///λ��
/*����֡��ʽͬ�ɼ��ٶ�*/
#define 	tCMD_POSITION	tCMD_SPEED

typedef struct {
	u32		u32Long;
	u32		u32Lat;
	u16		u16Height;
}tPOSITION;	///��A.20

typedef struct {
	tPOSITION	tPos;
	u8		u8AvrSpeed;
}tITEM_POS;

typedef struct {
	tTIME	time_Start;
	tITEM_POS	atItem[60];
}tACK_POS;	///��A.19

#define		VDR_CMD_GET_ACCIDENT				0x10	///�¹��ɵ�
/*����֡��ʽͬ�ɼ��ٶ�*/
#define 	tCMD_ACCIDENT	tCMD_SPEED

typedef struct {
	u8	u8Speed;
	u8	u8Status;
}tITEM_ACCIDENT;

typedef struct {
	tTIME	time_End;
	u8		aLicense[18];
	tITEM_ACCIDENT	atItem[100];
	tPOSITION	tLastValidPos;
}tACK_ACCIDENT;		///��A.22

#define		VDR_CMD_GET_OVERTIME_DRIVING		0x11	///��ʱ��ʻ
/*����֡��ʽͬ�ɼ��ٶ�*/
#define 	tCMD_OVERTIME_DRIVING	tCMD_SPEED

typedef struct {
	u8		aLicense[18];
	tTIME	time_Begin;
	tTIME	time_End;
	tPOSITION	tPos_Begin;
	tPOSITION	tPos_End;
}tACK_OVERTIMEDRIVING;	///��A.24


#define		VDR_CMD_GET_DRIVER_LOG_INFO			0x12	///����ǩ��

typedef struct {
	tTIME	time;
	u8		aLicense[18];
	u8		u8EvtType;	///01:��½��02���˳�
}tACK_DRIVERLOG;	///��A.26


#define		VDR_CMD_GET_POWER					0x13	///����
/*����֡��ʽͬ�ɼ��ٶ�*/
#define 	tCMD_POWER	tCMD_SPEED

typedef struct {
	tTIME	time;
	u8		u8EvtType;	///01:ͨ�磻02���ϵ�
}tACK_POWER;	///��A.28

#define		VDR_CMD_GET_PARAM_CHANGE			0x14	///�����޸�
/*����֡��ʽͬ�ɼ��ٶ�*/
#define 	tCMD_PARAM_CHANGE	tCMD_SPEED

typedef struct {
	tTIME	time;
	u8		u8EvtType;	///Ϊ���ò���������
}tACK_PARAMCHANGE;		///��A.30

#define		VDR_CMD_GET_SPEED_STATUS_LOG		0x15	///�ٶ�״̬��־
/*����֡��ʽͬ�ɼ��ٶ�*/
#define 	tCMD_SPEED_STATUS_LOG	tCMD_SPEED

typedef struct {
	u8		u8Speed_Log;	///��¼�ٶȣ������ٶȴ�����
	u8		u8Speed_Ref;	///�ο��ٶȣ�����GPS
}tITEM_SPEEDSTATE;

typedef struct {
	u8		u8SpeedState;	///01��������02���쳣
	tTIME	time_Begin;
	tTIME	time_End;
	tITEM_SPEEDSTATE	tItem[60];	///�ӿ�ʼʱ���1�����ڣ�ÿ���¼һ��
}tACK_SPEEDSTATUS;		///��A.32


/**
 * ���ò�����
 */
#define VDR_CMD_SET_VECHICLE_INFO		0x82	///������Ϣ��A.11
/*����֡����Ӧ��֡*/
#define tCMD_SET_VECHICLE_INFO 	tACK_CARINFO

#define VDR_CMD_SET_FIRST_INSTLL_DATA	0x83	///���ΰ�װ����	A.8
/*����֡����Ӧ��֡*/
#define tCMD_SET_INSTALL_DATA	tTIME

#define VDR_CMD_SET_STATUS_CONFIG_INFO	0x84	///״̬��������Ϣ	A.13
/*����֡����Ӧ��֡*/
#define tCMD_SET_STAUS_CONFIG tITEM_STATECONFIG

#define VDR_CMD_SET_TIME				0xC2	///ʱ��	A.8
/*����֡����Ӧ��֡*/
#define tCMD_SET_TIME	tTIME

#define VDR_CMD_SET_PULSE				0xC3	///����ϵ��		A.10
/*����֡����Ӧ��֡*/
#define tCMD_SET_PULSE	tACK_PULSE

#define VDR_CMD_SET_INITIALDIST			0xC4	///��ʼ���		A.9
/*����֡����Ӧ��֡*/
#define tCMD_SET_INITIALDIST	tACK_DISTANCE


/**
 * �춨�����ּ����ݿ��ʽ
 */
#define tCMD_DIAG_ENTER_KEEP			0xE0	///����춨״̬
/*����֡�գ�Ӧ��֡��*/

#define tCMD_DIAG_DIST_ERR_MEASURE		0xE1	///���������
/*����֡�գ�Ӧ��֡*/
typedef struct {
	u8	aUniqID[35];	///ΨһID
	u16 u16Pulse;	///����ϵ��
	u16 u16Speed;	///��ǰ�ٶ�
	u32	u32CheckedDist;	///�춨���
	u8 	u8State;	///״̬�ź�
}tACK_DIAG_DIST_CHECK;

#define tCMD_DIAG_PULSE_ERR_MEASURE		0xE2	///����ϵ��������
/*����֡�գ�Ӧ��֡��*/

#define tCMD_DIAG_RTC_ERR_MEASURE		0xE3	///ʱ��������
/*����֡�գ�Ӧ��֡��*/

#define tCMD_DIAG_RETURN_NORMAL			0xE4	///������������״̬
/*����֡�գ�Ӧ��֡��*/

/*��¼��ȫ�ֲ���*/

/*��������*/
typedef enum {
	VDR_PARAM_CARINFO = 0,
	VDR_PARAM_INSTALLTIME,
	VDR_PARAM_STATECONFIG,
	VDR_PARAM_RTC,
	VDR_PARAM_PULSE,
	VDR_PARAM_INITDIST,
	VDR_PARAM_LICENSE,
	VDR_PARAM_ID,
	VDR_PARAM_END,
} eVDRPARAM;

///������Ϣ����
typedef struct {
	tCARINFO 	tCarInfo;
	u8 			u8CRC;
}tCARINFO_ST;

///��¼��ʱ�����
typedef struct {
	tTIME 	time;
	u8 		u8CRC;
}tTIME_ST;

///״̬���ò���
typedef struct {
	tACK_STATECONFIG 	tStateConf;
	u8 				u8CRC;
}tSTATECONFIG_ST;

///����ϵ������
typedef struct {
	tPULSE		tPulse;
	u8 			u8CRC;
}tPULSE_ST;

///��ʼ��̲���
typedef struct {
	tINITDISTANCE 	tInitDist;
	u8 				u8CRC;
}tINITDISTANCE_ST;

///���պ������
typedef struct {
	u8 aLicense[18];
	u8 u8CRC;
}tLICENSE_ST;

///��¼��ΨһID����
typedef struct {
	tVDRID 	tID;
	u8 		u8CRC;
}tUNIQID_ST;


/**
 * ��¼��ȫ�ֲ�����ע�⣬����������������
 */
typedef struct {
	tCARINFO_ST			tCarInfo_St;		///������Ϣ	A.11
	tTIME_ST 			tInstData_St;	///��װ���� ��A.8
	tSTATECONFIG_ST 	tStateConf_St;		///״̬��������Ϣ  ��A.13
	tTIME_ST 			tRTC_St;		///��¼��ʱ�� ��A.8
	tPULSE_ST			tPulse_St;			///����ϵ��	 ��A.10
	tINITDISTANCE_ST	tInitDist_St;		///���  ��A.9
	tLICENSE_ST			tLicense_St;	///��ʻ֤����
	tUNIQID_ST			tID_St;			///��¼��ΨһID

} tVDRPARAM_ST;


/*��¼�Ǹ������������еĴ洢ƫ�Ƶ�ַ*/
#define FRAM_ADDR_VDR_PARAM_START		0x3333	///��¼�ǲ�����ʼƫ�Ƶ�ַ
#define FRAM_ADDR_VDR_PARAM_CARINFO		(FRAM_ADDR_VDR_PARAM_START)
#define FRAM_ADDR_VDR_PARAM_INSTALLTIME	(FRAM_ADDR_VDR_PARAM_CARINFO + sizeof(tCARINFO_ST))
#define FRAM_ADDR_VDR_PARAM_STATECONFIG	(FRAM_ADDR_VDR_PARAM_INSTALLTIME + sizeof(tTIME_ST))
#define FRAM_ADDR_VDR_PARAM_RTC			(FRAM_ADDR_VDR_PARAM_STATECONFIG + sizeof(tSTATECONFIG_ST))
#define FRAM_ADDR_VDR_PARAM_PULSE		(FRAM_ADDR_VDR_PARAM_RTC + sizeof(tTIME_ST))
#define FRAM_ADDR_VDR_PARAM_INITDIST	(FRAM_ADDR_VDR_PARAM_PULSE + sizeof(tPULSE_ST))
#define FRAM_ADDR_VDR_PARAM_LICENSE		(FRAM_ADDR_VDR_PARAM_INITDIST + sizeof(tINITDISTANCE_ST))
#define FRAM_ADDR_VDR_PARAM_ID			(FRAM_ADDR_VDR_PARAM_LICENSE + sizeof(tLICENSE_ST))



#if 0

/**оƬ���õ���K9F1G08U0B��оƬ��СΪ128MB����Ϊ1024��BLOCK(��)��ÿ���СΪ128KB��ÿ��ֳ�64ҳ��ÿҳ��СΪ2KB��*/
/**
 * ��ʻ�ٶȼ�¼��1s���������¼���洢��ʻ״̬������ʱ�䣬ƽ���ٶȣ�״̬�����������48Сʱ
 * 48*3600=172800�� *2B= 345600B = 35K,
 * �洢���ԣ�
 * ��NANDFLASHֻ�ܰ�ҳΪ��λ��д�������3���飬2��Ϊ�����飬��֤���Ͳ�����������ͬһ���ϣ���һ����Ϊ���ݿ飬�ݴ��ã��滻���顣
 * ��ҳ�洢��ҳ��������ʱ����Ϣ��8B
 * ����2K�ڴ�ռ䣬��Ϊ�洢��������ʱд�룻
 * �������б���������Ϣ��������ǰ��дλ�ã�
 * Ϊʹ�ü������㣬�洢��ʽ���ϴ���ʽ����һ�£�ҳ�ڴ���֯��
 * tSPEEDLOGPERSEC1 ������ tSPEEDLOGPERSEC 7
 * sizeof(tSPEEDLOGPERSEC) = 126B
 * 1ҳ���Դ��8��tSPEEDLOGPERSEC
 * ��8�������д��NANDFLASH
 *
 * ��ȡʱ��������ʼ������ʱ�䣬�ȸ��ݿ�ŵ����һҳ�Ŀ�����ʱ����ҿ�ţ���λ����ź��ٸ�����ʼҳ�ĵ�һ��ҳ�����������һҳ�Ŀ���������ʱ����ж��ֲ��ҡ�
 */
#define VDR_LOG_SPEED_NF_BLOCK_BEGIN	48
#define VDR_LOG_SPEED_NF_BLOCK_END		(VDR_LOG_SPEED_NF_BLOCK_BEGIN + 3)
#define VDR_LOG_SPEED_NF_BLOCK_SIZE		3
typedef struct {
	u16 u16BlockId_Start;
	u16 u16BlockId_Last;

	/*���ݴ洢��Ϣ*/
	u16 u16BlockId_Head;	///���ݿ���һ��д��λ�ã����
	u8 u8PageId_Head;		///���ݿ���һ��д��λ�ã�ҳ��
	u16 u16BlockId_Tail;	///���ݿ�βλ�ã���ţ�����д��ͷ����ʱ���µ���һ��
	u8 u8PageId_Tail;		///���ݿ�βλ�ã�ҳ�ţ�����д��ͷ����ʱ���µ���һҳ

	/*�ϴ�����������Ϣ*/
	u16 u16BlockId_UplBegin;	///���ݿ���һ����ȡλ�ã����
	u8 u8PageId_UplBegin;		///���ݿ���һ����ȡλ�ã�ҳ��
	u16 u16PageOffset_UplBegin;	///ҳ��ƫ��λ��
	u16 u16BlockId_UplEnd;	///���ݿ���һ����ȡλ�ã����
	u8 u8PageId_UplEnd;		///���ݿ���һ����ȡλ�ã�ҳ��
	u16 u16PageOffset_UplEnd;	///ҳ��ƫ��λ��
	u16 u16BlockId_UplCursor;	///���ݿ���һ����ȡλ�ã����
	u8 u8PageId_UplCursor;		///���ݿ���һ����ȡλ�ã�ҳ��
	u16 u16PageOffset_UplCursor;	///ҳ��ƫ��λ��

	/*��д����*/
	u8 aPageBuf[2048];	///д����
	u16 u16WtPtr;		///дָ��
	BOOL bPageEmpty;	///ҳ�����е�һ�����ݿ鱣��ʱ��
//	u8 aRdBuf[2048];
} tSPEEDLOGHEAD;
#endif

/**--------------------------------------------------------------------------------------------------
 * ��¼���ⲿ�洢����¼����
 *-------------------------------------------------------------------------------------------------*/
/**
�¹���Լ�¼�ǵ�����Ҫ�������
1��ͨ��ǰ�Լ�
��ͨ�翪ʼ����ʱ�����Ƚ����Լ죬�Լ�������Ӧ�������źŻ���ʾ����ʾʽָʾ�������������й�����Ӧ�Ժ����źŻ���ʾ����ʾ��ʽָʾ������Ϣ��
2����ʻ��¼
��¼������1���ʱ����������¼���洢������ʻ״̬���ݣ���ʻ״̬����Ϊ����������ʻ�����е�ʵʱʱ�䡢ÿ���Ӽ���ڶ�Ӧ��ƽ���ٶ��Լ���Ӧʱ���״̬�źš�
��Ч���ݼ�¼Ӧ���������48����λСʱ���ٶȼ�¼��λΪkm/h��������Χ����Ϊ0-220km/h���ֱ��ʵ��ڻ����1km/h��
3���¹��ɵ��¼
��¼������0.2���ʱ����������¼���洢��ʻ����ǰ20��ʵʱʱ���Ӧ����ʻ״̬���ݣ�����ʻ״̬����Ϊ��������ʻ�ٶȡ��ƶ���״̬�źź���ʻ����ʱ��λ����Ϣ��
4���ⲿͨ��Ͽ�ʱ
��¼������0.2���ʱ����������¼���洢�ϵ�ǰ20���ڵĳ�����ʻ״̬���ݣ�����ʻ״̬����Ϊ��������ʻ�ٶȡ������ƶ���״̬�źż��ϵ�ʱ��λ����Ϣ��
5������������ʻ״̬����Чλ����Ϣ10�����ޱ仯ʱ
��¼��Ӧ����0.2���ʱ����������¼���洢�ϵ�ǰ20���ڵĳ�����ʻ״̬���ݣ�������ʻ״̬����Ϊ��������ʻ�ٶȡ������ƶ���״̬�źż��ϵ�ʱ��λ����Ϣ���¹��ɵ��¼����Ӧ���������100����
6����ʱ��ʻ��¼
��¼���ܼ�¼��ʻ�˳�ʱ��ʻ�����ݣ��������ݰ�������������ʻ֤���롢������ʻ��ʼʱ�估����λ����Ϣ��������ʻ����ʱ�估����λ����Ϣ����¼����Ӧ���������100����
*/

/*��¼����*/
typedef enum {
	RECORD_SPEED = 0,	///��ʻ�ٶȼ�¼
	RECORD_ACCIDENT,	///�¹��ɵ��¼
	RECORD_OVERTIMEDRIVING,	///��ʱ��ʻ��¼
	RECORD_POS,		///λ����Ϣ��¼
	RECORD_DRIVERLOG,	///��ʻ����Ϣ��¼
	RECORD_POWER,		///�ⲿ�����¼
	RECORD_PARAMCHANGE,	///�����޸ļ�¼
	RECORD_SPEEDSTATUS,	///�ٶ�״̬��¼

	RECORD_END,
} eRECORD;

/**
 *  ��ʻ�ٶȼ�¼��1s���������¼���洢��ʻ״̬������ʱ�䣬ƽ���ٶȣ�״̬�����������48Сʱ
 *  �洢·���� 				/����speed
 *  						 |������ ��_��_��_ʱ1
 *  						 |������ ��_��_��_ʱ2
 *  д���ڣ�һ���ӣ�126B
 *  �洢���ݽṹ��
 *  tSPEEDLOGITEM_1		x x x x x x | d d | d d |... | d d
 *  					�� ��  ��     ʱ  ��   ��   | 1s  | 2s  |    | 60s
 *  tSPEEDLOGITEM_2
 *
 * 	�ļ�������λ����Сʱ�ƣ�ÿСʱ����һ�����ļ�
 *	ϵͳ�б������48���ļ���
 *	����48Сʱ���ļ����Զ�ɾ��
 */

/*�ٶ���Ϣ�ϴ��ṹ��*/
typedef struct {
	u8 u8AvgSPeed;	///ƽ���ٶ�
	u8 u8State;		///״̬
} tSPEEDPERSEC;

/*�ٶȼ�¼��ʽ*/
typedef struct {
	tTIME time_Start; ///��=0
	tSPEEDPERSEC tSpeed[60];
} tRECORD_SPEED;


/**
 * �¹��ɵ��¼��ʽ��0.2������������¼20s��������100��
 * �ж�����������ʻ״̬�£����������������ʱ��ʼ��¼
 * ���1:�ⲿ����Ͽ���
 * ���2��λ��10s���ޱ仯
 * �洢������ 234B*100 = 23400B=23K
 */
#define tRECORD_ACCIDENT	tACK_ACCIDENT

/**
 * ��ʱ��ʻ��¼��ʽ��������100��
 * �洢������50B*100 =5000B=5K
 */
typedef struct {
	tTIME	time;	///���ӵ�ʱ�䣬���������ʵ���ϴ�����Ҫȥ��
	tACK_OVERTIMEDRIVING	tInfo;
} tRECORD_OVERTIMEDRIVING;


/**
 * λ����Ϣ��¼��ʽ��1min�����360Сʱ,15��
 */
#define tRECORD_POS		tACK_POS


/**
 * ��ʻ����ݼ�¼����С��200��
 * �洢������25B*200=5000B=5K
 */
#define tRECORD_DRIVERLOG	tACK_DRIVERLOG

/**
 * ��̼�¼
 */
//�洢������4B
#define tRECORD_DIST	u32

/**
 * ��װ������¼
 */
#define tRECORD_INSTALLPARAM	NULL

/*--------- ��־��¼ -------------*/
/**
 * �ⲿ�����¼��������100��
 * �洢������7B*100=700B=1K
 */
#define tRECORD_POWER	tACK_POWER

/**
 * �����޸ļ�¼��������100��
 * �洢������7B*100=700B=1K
 */
#define tRECORD_PARAMCHANGE		tACK_PARAMCHANGE

/**
 * �ٶ�״̬��־���ӿ�ʼʱ������60s��������10��
 * ÿ���������¼���ж��ٶ�״̬һ�Σ�ͬʱ�洢�ٶ�״̬��־
 * �洢������133B*10=1330B=13K
 */
#define tRECORD_SPEEDSTATUS		tACK_SPEEDSTATUS


/**--------------------------------------------------------------------------------------------------
 * ��¼��USB���ݴ洢
 *-------------------------------------------------------------------------------------------------*/


#define VDR_USB_CODE_VERSION 	0x00	/// ִ�б�׼�汾��� ����A.6
#define VDR_USB_CODE_DRIVERINFO	0x01	/// ��ǰ��ʻ����Ϣ ����A.7
#define VDR_USB_CODE_RTC 		0x02 	///ʵʱʱ�� ����A.8
#define VDR_USB_CODE_DIST		0x03	/// �ۼ���ʻ��� ����A.9
#define VDR_USB_CODE_PULSE		0x04 	///����ϵ�� ����A.10
#define VDR_USB_CODE_CARINFO	0x05	/// ������Ϣ ����A.11
#define VDR_USB_CODE_CONFIG		0x06	/// ״̬�ź�������Ϣ ����A.12
#define VDR_USB_CODE_UNIQID		0x07 	///��¼��Ψһ�Ա�� ����A.14
#define VDR_USB_CODE_SPEED		0x08	///08H ��ʻ�ٶȼ�¼ ����A.16
#define VDR_USB_CODE_POSITION	0x09	//09H λ����Ϣ��¼ ����A.18
#define VDR_USB_CODE_ACCIDENT	0x10	//10H �¹��ɵ��¼ ����A.21 ȫ����¼
#define VDR_USB_CODE_OTDRIVE	0x11	//11H ��ʱ��ʻ��¼ ����A.23 ȫ����¼
#define VDR_USB_CODE_DRIVERLOG	0x12	//12H ��ʻ����ݼ�¼ ����A.25 ȫ����¼
#define VDR_USB_CODE_POWER		0x13	//13H �ⲿ�����¼ ����A.26 ȫ����¼
#define VDR_USB_CODE_PARAMCHG	0x14	//14H �����޸ļ�¼ ����A.29 ȫ����¼
#define VDR_USB_CODE_SPEEDSTATUS	0x15	//15H �ٶ�״̬��־ ����A.31 ȫ����¼

typedef struct {
	u8 code;
	char* name;
} VDR_USBBLOCKINFO;

#define VDR_USB_BLOCK_AMOUNT	16

const VDR_USBBLOCKINFO  VDR_BLOCK[VDR_USB_BLOCK_AMOUNT] = {
	{VDR_USB_CODE_VERSION, "ִ�б�׼�汾���"},		///����A.6
	{VDR_USB_CODE_DRIVERINFO, "��ǰ��ʻ����Ϣ"},	/// ����A.7
	{VDR_USB_CODE_RTC, "ʵʱʱ��"}, 				/// ����A.8
	{VDR_USB_CODE_DIST, "�ۼ���ʻ���"},			///  ����A.9
	{VDR_USB_CODE_PULSE, "����ϵ��"},		 		/// ����A.10
	{VDR_USB_CODE_CARINFO, "������Ϣ"},			///  ����A.11
	{VDR_USB_CODE_CONFIG, "״̬�ź�������Ϣ"},		///  ����A.12
	{VDR_USB_CODE_UNIQID, "��¼��Ψһ�Ա��"},	 	/// ����A.14
	{VDR_USB_CODE_SPEED,"��ʻ�ٶȼ�¼"},			///08H  ����A.16
	{VDR_USB_CODE_POSITION,	"λ����Ϣ��¼"},		//09H  ����A.18
	{VDR_USB_CODE_ACCIDENT,"�¹��ɵ��¼"},		//10H  ����A.21 ȫ����¼
	{VDR_USB_CODE_OTDRIVE,"��ʱ��ʻ��¼"},			//11H  ����A.23 ȫ����¼
	{VDR_USB_CODE_DRIVERLOG,"��ʻ����ݼ�¼"},		//12H  ����A.25 ȫ����¼
	{VDR_USB_CODE_POWER,"�ⲿ�����¼"},			//13H  ����A.26 ȫ����¼
	{VDR_USB_CODE_PARAMCHG,"�����޸ļ�¼"},		//14H  ����A.29 ȫ����¼
	{VDR_USB_CODE_SPEEDSTATUS,"�ٶ�״̬��־"},		//15H  ����A.31 ȫ����¼
};

/**USB���ݿ�ṹ*/
typedef struct {
	u8 u8Code;	//���ݴ��루1���ֽڣ�
	u8 aName[18];	//�������ƣ�18���ֽڣ�
	u32 u32Len;	//���ݳ��ȣ�4���ֽڣ�
	u8  pData[0];
} VDRUSBBLOCKHEAD;

/**
���ݿ������2���ֽڣ�
���ݿ�1
���ݿ�2
����
���ݿ�n
У��ֵ��1���ֽڣ�
*/
typedef struct {
	u16 u16BlockAmt;
	VDRUSBBLOCKHEAD	blocks[0];
	u8 u8Checksum;

};

/**��¼��ȡ����*/
#define FORWARD		1	///����
#define BACKWARD	0	///����

/****************************************************************************************************************
 * ������д�ӿ�
 ***************************************************************************************************************/
int VDR_SaveParam(eVDRPARAM eParam, u8 *pBuf, u16 u16Len);
int VDR_LoadParam(void);
u8* VDR_GetParam(eVDRPARAM eParam);
void VDR_Err_Proc(u8 u8Cmd, int ret);


/****************************************************************************************************************
 * ���ڼ�USB�洢�ӿ�
 ***************************************************************************************************************/
int VDR_SDInit();
int VDR_DumpFiles2USBStorage(FIL *f, TIME *pTime);
void VDR_ClearQuerySession();
int VDR_NewQuerySession(eLOGCLASS eClass, TIME *pTime_Start, TIME *pTime_End);
int VDR_RetrieveSingleRecordForward(u8 *pBuf, u16 bufSize);
int VDR_RetrieveSingleRecordBackward(u8 *pBuf, u16 bufSize);

#pragma 	pack()

#endif //__GPS_VDR_COMM_H__
