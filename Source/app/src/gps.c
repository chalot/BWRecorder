/**   
 * @Title: gps.c  
 * @Description: TODO(��һ�仰�������ļ���ʲô) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-7-29 ����10:32:44 
 * @version V1.0   
 */
/*
 G595 GPS ģ�飬�ڲ��Ѽ�������������·�����ͻ�������˾��G595 GPS ģ��ʱ����
 �Բ����ⲿ����·�����ϣ�ֻ��Ҫ������������޸ģ��Ϳ���Ϊ�ͻ���Լ0.3$-0.37$�ĳɱ�ѽ�����ⲿ
 ���߳��ֶ�·���·ʱ�� ͨ����������� $GPTXT,01,01,02,ANTSTATUS=SHORT*6D �� ��
 ��$GPTXT,01,01,02,ANTSTATUS=OPEN*2B����������߿ͻ�������ߵ�״̬
 ��ϵͳ��⵽��$GPTXT,01,01,02,ANTSTATUS=SHORT*6D�����ʱ����ʾ���������Ѿ���·��
 ��ϵͳ��⵽��$GPTXT,01,01,02,ANTSTATUS=OPEN*2B�����ʱ����ʾ���������Ѿ���·
 ��������������ʱ��TXT �����û�������
 */

#include <stm32f2xx_conf.h>
#include <gps.h>
#include <comm.h>
#include <bsp.h>
#include <parameter.h>
#include <utility.h>
#include <trace.h>
#include <protocol.h>
#include <board.h>
#include <systimer.h>
#include <qevt.h>

Q_DEFINE_THIS_MODULE("gps.c")

//#define TRACE_GPS	//��ӡGPS������Ϣ

#define _GPS_RESPONSE_NUMBER_ 7	//GPS NMEA��Ӧ֡������
//GPS��Ӧ�ַ���
char* GPS_ResponseString[_GPS_RESPONSE_NUMBER_] = { "$GPRMC",	//GPS���00���Ƽ���λ��Ϣ
		"$GPGGA",		//GPS���01��GPS��λ��Ϣ
		"$GPGLL",		//GPS���02��GPS��λ������Ϣ
		"$GPGSA",		//GPS���03����ǰ������Ϣ
		"$GPGSV",		//GPS���04���ɼ�������Ϣ
		"$GPVTG",		//GPS���05�������ٶ���Ϣ
		"$GPTXT",		//����״̬����
		};

//GPS��Ӧ����
typedef enum {
	_GPS_RES_GPRMC_ = 0,	//GPS���00���Ƽ���λ��Ϣ
	_GPS_RES_GPGGA_,			//GPS���01��GPS��λ��Ϣ
	_GPS_RES_GPGLL_,			//GPS���01��GPS��λ��Ϣ
	_GPS_RES_GPGSA_,			//GPS���03����ǰ������Ϣ
	_GPS_RES_GPGSV_,			//GPS���04���ɼ�������Ϣ
	_GPS_RES_GPVTG_,			//GPS���05�������ٶ���Ϣ
	_GPS_RES_GPTXT_,
	_GPS_RES_UNKNOWN_ = 254,	//FE: δ֪
	_GPS_RES_EMPTY_ = 255	//FF: �գ�����Ӧ

} _eGPS_RESPONSE_;

#define		_GPS_NMEA_MSG_LEN_MAX_		500		//NMEAЭ��֡��󳤶�#define		_GPS_MSG_QUENE_LEN_			1024	//��Ϣ���г���static u8 aMsg_Buffer[_GPS_NMEA_MSG_LEN_MAX_];	//NMEAЭ������֡��ʱ����
static u8 *rpcGPSMessage = NULL;

//�����շ�����
#define RX_BUF_SIZE		1024	//���ջ�������static u8 buf_Rx[RX_BUF_SIZE];	//���ջ���
static tCOMM Comm_Rx;  			//���ջ���

static _tGPSInfo tGpsInfo;	//GPSʵʱ����

//�ٶ�ƽ������
typedef struct {
	u8 aSpeedArray[5];
	u16 u16SpeedArray_Counter;
	u16 u16AverageSpeed;

} tSPEEDSMOOTER;

//�߶�ƽ������
typedef struct {
	u8 aAltitudeArray[5];
	u32 u32AltitudeArray_Counter;
	u16 u16AverageAltitude;

} tALTITUDESMOOTER;

static tSPEEDSMOOTER tSpeedSmooth;	//�ٶ�ƽ����
static tALTITUDESMOOTER tAltitudeSmooth;	//�߶�ƽ����

static u8 gps_ErrType = 0;	//��������

//===================== ���غ������� =================================================================
static u16 GPS_GetResponseCode(void);	//����GPS��Ӧ����
static u16 GPS_GetSegmentResponseCode(u8 u8StartIndex, u8 u8EndIndex);//����GPS�ֶ���Ӧ����
static BOOL GPS_JumpToNextSegment(void);	//��������Ϣ��һ�ֶ�
static BOOL GPS_GetUTCTime(void);	//ȡUTCʱ��
static void GPS_GetUTCDate(u8 u8Format);	//ȡUTC����
static BOOL GPS_GetLatitude(void);	//ȡγ��
static BOOL GPS_GetLongitude(void);	//ȡ����
static void GPS_GetSpeed(void);	//ȡ�ٶ�
static void GPS_GetHeading(void);	//ȡ����
static void GPS_GetHeight(void);	//ȡ�ٶ�
static u16 GPS_StrToInteger(void);	//ASCII�����ַ���ת��������
static float GPS_StrToFloat(void);	//ASCII�����ַ���ת���ɸ�����
static u8 GPS_StrToByte(u8 u8Count);	//ȡ�ֽ�����
static u16 GPS_StrToWord(u8 u8Count);	//ȡ������
static u16 GPS_GetIntegerLen(void);	//���������������ֳ���
static u16 GPS_SpeedAverage(u16 u16Speed);	//����ƽ���ٶ�
static void GPS_AltitudeAverage(s16 s16Altitude);	//�߶�ƽ��
static void GPS_SyncSystemTime(void);	//ͬ��ϵͳʱ��
static u16 GPS_RetrieveMessage(u8* pu8Buffer, u16 read, u16 write);
//===================================================================================================

/**
 * @Func	GPS��ʼ��
 * @Param	��
 * @Ret		��
 */
void GPS_Init() {
	Comm_Init(&Comm_Rx, buf_Rx, RX_BUF_SIZE); //���ջ�����ʼ��

	tGpsInfo.u16GpsInfoValid |= _GPSINFO_FIRSTTIME; //�ȴ���һ�ζ�λ
}

/**
 * @Func	��������GPS����֡��ѭ����ȡ��ֱ����
 * @Param	��
 * @Ret		��
 */
void GPS_HandleMsg() {
//#define TRACE_GPS
	u16 msgLen = 0;
	/*�ݴ滺������дλ��*/
	u16 write = Comm_Rx.write;

	do {

		msgLen = GPS_RetrieveMessage(aMsg_Buffer, Comm_Rx.read, write);
		if (msgLen > 0) {
			aMsg_Buffer[msgLen] = '\0';
#ifdef TRACE_GPS
			TRACE_(QS_USER, NULL, "[GPS] %s", aMsg_Buffer);
#endif
			GPS_ParseMsg(aMsg_Buffer, msgLen);
			Comm_Rx.read += msgLen;
			if (Comm_Rx.read >= RX_BUF_SIZE)
				Comm_Rx.read -= RX_BUF_SIZE;
		}
	} while (msgLen > 0);

}

/**
 * @Func	��ȡGPS����֡
 * @Param	��
 * @Ret		��
 */
u16 GPS_RetrieveMessage(u8* pu8Buffer, u16 read, u16 write) {
	u8 u8Data = 0;
	u16 _tail = 0;
	u16 _write = 0;
	u16 _read = 0;
	u8 *pMsg = pu8Buffer;
	u16 len = 0;
	u8 i = 0;
	u8 bMsgReceive = 0;

//	/*�ݴ滺������дλ��*/
//	_write = Comm_Rx.write;
//	_read = Comm_Rx.read;
	_read = read;
	_write = write;

	//����Э��ͷ0x24�ַ�λ��
	while ((_read != _write) && (*(Comm_Rx.pBuf + _read) != '$')) {
		//ָ����һ�ַ�
		_read++;

		WRAP_AROUND(_read, RX_BUF_SIZE);
	}

	if (_read == _write) //������
			{
		return 0;
	}

	//ת��֡
	while (_read != _write) {
		//�ַ�ת��
		pMsg[i] = *(Comm_Rx.pBuf + _read);

		//����֡β��
		if ((i > 3) && (pMsg[i - 1] == 0x0D) && (pMsg[i] == 0x0A)) {
			//��ȷ�յ�һ������֡
			bMsgReceive = TRUE;

			//ֹͣ������
			break;
		}

		_read++;
		WRAP_AROUND(_read, RX_BUF_SIZE);
		i++;
	}

	//���������꣬û������֡��
	if (!bMsgReceive) {
		return 0;
	}

	//��ȷ��ȡһ������NMEA֡����ָ��������֡
//	Comm_Rx.read = _read;

	//������Ч֡����
	return (i + 1);
}

/**
 * @Func	GPS�����жϴ���ע�������������־λ����������һ��ʱ����޽�������
 * @Param	��
 * @Ret		��
 */
void ISR_GPS_COM(void) {
	u8 u8Data;

	//�����ж�
	if (USART_GetITStatus(GPS_COM, USART_IT_RXNE)) {
		USART_ClearITPendingBit(GPS_COM, USART_IT_RXNE);

		//��ȡ����
		u8Data = (u8) USART_ReceiveData(GPS_COM);

		//�洢����
		*(Comm_Rx.pBuf + Comm_Rx.write++) = u8Data;
		WRAP_AROUND(Comm_Rx.write, RX_BUF_SIZE);
	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_ORE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_ORE);        //��SR

		//��ȡ����
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //��DR

	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_PE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_PE);        //��SR

		//��ȡ����
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //��DR

	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_FE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_FE);        //��SR

		//��ȡ����
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //��DR

	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_NE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_NE);        //��SR

		//��ȡ����
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //��DR

	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_IDLE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_IDLE);        //��SR

		//��ȡ����
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //��DR

	}
	//LBD LIN�Ͽ�����־
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_LBD) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_LBD);        //��SR
	}
	//������ɼ���־
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_TC) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_TC);        //��SR
	}
	//CTS��־
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_CTS) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_CTS);        //��SR
	}

}

/**
 * @Func	GPS��Ϣ��������GPS�����յ�һ֡������NMEA���ݺ󣬵��ô˹��̴���GPS��Ϣ
 * @Param	pcFrame,		[IN] ����ָ֡��
 * 			u32FrameSize,	[IN] ����֡����
 * @Ret		��
 */
void GPS_ParseMsg(u8* pcFrame, u32 u32FrameSize) {
	//������Ч��? ���ݺϷ���(���ַ�������"$"��β�ַ�������"cr")?
	if (!u32FrameSize || !pcFrame || *pcFrame != '$'
			|| *(pcFrame + u32FrameSize - 2) != 0x0D
			|| *(pcFrame + u32FrameSize - 1) != 0x0A) {
		return;
	}

	//GPS��Ӧ��Ϣָ��
	rpcGPSMessage = pcFrame;

	//��ȡGPS��Ӧ����
	switch (GPS_GetResponseCode()) {
	case _GPS_RES_GPTXT_:

		break;

		//GPS���00���Ƽ���λ��Ϣ
		//$GPRMC,UTCʱ��,��λ״̬,γ��,��/��γ,����,��/����,�����ٶ�,�汱����,UTC����,��ƫ��,��ƫ�Ƿ���,ģʽָʾ*����<CR><LF>
		//1)  UTCʱ��:  ��ʽΪ"hhmmss.sss"��"hh"��ʾʱ��ȡֵ��Χ00~23��"mm"��ʾ�֣�ȡֵ��ΧΪ00~59��"ss"��ʾ�룬ȡֵ��ΧΪ00~59��"sss"Ϊ���С�����֡�
		//2)  ��λ״̬: 1���ַ���'A'��ʾ��Ч��λ��'V'��ʾ��Ч��λ��
		//3)  γ��:		��ʽΪ"ddmm.mmmm"��"dd"��ʾ�ȣ�ȡֵ��Χ00~89��"mm"��ʾ�֣�ȡֵ��ΧΪ00~59��"mmmm"Ϊ�ֵ�С�����֡�
		//4)  ��/��γ:  N=��γ(��)��S=��γ(��)
		//5)  ����:		��ʽΪ"dddmm.mmmm"��"ddd"��ʾ�ȣ�ȡֵ��Χ00~179��"mm"��ʾ�֣�ȡֵ��ΧΪ00~59��"mmmm"Ϊ�ֵ�С�����֡�
		//6)  ��/����:	E=����(��)��W=����(��)
		//7)  �����ٶ�: ��λ���ڣ�1��=1����/ʱ=1.852Km/h��
		//8)  �汱����:	��λ���ȡ�
		//9)  UTC����:	��ʽΪ"ddmmyy"������"dd"Ϊ���ڣ�ȡֵ��ΧΪ1~31��"mm"Ϊ�·ݣ�ȡֵ��ΧΪ1~12��"yy"Ϊ�꣬ȡֵ��ΧΪ00~99��
		//10) ��ƫ�Ƿ���:"E"��ʾƫ����"W"��ʾƫ����
		//11) ģʽָʾ:	1���ַ���A=Autonomous, D=DGPS, E=DR
		//����$GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598,,*10
	case _GPS_RES_GPRMC_:

		//������$GPRMC,��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}

		//ȡUTCʱ�䣺"hhmmss.sss"������С������
		if (!GPS_GetUTCTime())
			break;

		//������UTCʱ��,��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//�жϡ���λ״̬���Ƿ���Ч��λ?
		if (*(rpcGPSMessage) != 'A') {
			//����Ч��λ��־
			tGpsInfo.u16GpsInfoValid &= ~_GPSINFO_GPS_VALID;
			tGpsInfo.u16GpsInfoValid |= _GPSINFO_FIRSTTIME;

			//��Ч
			break;
		}

		//��������λ״̬��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//ȡγ�ȣ���dmm.mmmm��
		if (!GPS_GetLatitude())
			break;

		//��������/��γ��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//ȡ���ȣ���dddmm.mmmm��
		if (!GPS_GetLongitude())
			break;

		//��������/������
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}

		//ȡ�����ٶ�
		GPS_GetSpeed();

		//�����������ٶȡ�
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}

		//ȡ����
		GPS_GetHeading();

		//�������汱����
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}

		//UTC����
		GPS_GetUTCDate(6);

		//������UTC���ڡ�
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}

//SIM548 GPS��Ӧ��Ϣ�����޴�ƫ��
#ifndef	_GPS_PRODUCT_WAVECOM_SIM548_
		//��������ƫ�ǡ�
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
#endif

		//��������ƫ�Ƿ���
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}

		//�жϡ�GPSģʽ���Ƿ�������λ(A)���ֶ�λ(D)?
		if ((*(rpcGPSMessage) != 'A') && (*(rpcGPSMessage) != 'D')) {
			//��Ч
//				break;
		}

		if (tGpsInfo.u16GpsInfoValid & _GPSINFO_FIRSTTIME) {
			//����ƽ���ٶ�
			GPS_SpeedAverage(tGpsInfo.u16Speed);

			tGpsInfo.u16GpsInfoValid &= ~_GPSINFO_FIRSTTIME;
			tGpsInfo.u16GpsInfoValid |= _GPSINFO_GPS_VALID;

			//ͬ��ϵͳʱ��
			GPS_SyncSystemTime();
		}

		TRACE_(QS_USER, NULL,
				"[GPRMC] %d-%d-%d:%d-%d-%d, Lon=%x%x%x%x, Lat=%x%x%x%x, Speed=%x%x, total_Sat = %d, viewd_Sat = %d",
				tGpsInfo.u16Year, tGpsInfo.u8Month, tGpsInfo.u8Day,
				tGpsInfo.u8Hour, tGpsInfo.u8Minute, tGpsInfo.u8Second,
				tGpsInfo.Longtitude[0], tGpsInfo.Longtitude[1],
				tGpsInfo.Longtitude[2], tGpsInfo.Longtitude[3],
				tGpsInfo.Latitude[0], tGpsInfo.Latitude[1],
				tGpsInfo.Latitude[2], tGpsInfo.Latitude[3], tGpsInfo.Speed[0],
				tGpsInfo.Speed[1], tGpsInfo.u8VisibleSatellites,
				tGpsInfo.u8ValidSatellites);
		break;

		//GPS���01��GPS��λ��Ϣ
		//$GPGGA,UTCʱ��,γ��,��/��γ,����,��/����,GPS״̬,��������,HDOP,���θ߶�,M,������������Ը߶�,M,���ʱ��,���վID��,*����<CR><LF>
		//1)  UTCʱ�䣺	hhmmss.sss,	��ʽ=122.1 - 222.3������"hh"��ʾʱ��ȡֵ��Χ00~23��"mm"��ʾ�֣�ȡֵ��ΧΪ00~59��"ss"��ʾ�룬ȡֵ��ΧΪ00~59��"DD"Ϊ���С�����֡�
		//2)  γ��:		XXMM.mmmm,	��ʽ=12.1 - 32.4
		//3)  ��/��γ:	N=��γ(��)��S=��γ(��)
		//4)  ����:		XXXMM.mmmm,	��ʽ=12.1 - 32.4
		//5)  ��/����:	E=����(��)��W=����(��)
		//6)  GPS״̬:	"0"��ʾδ��λ��"1"��ʾ�ǲ�ֶ�λ��"2"Ϊ��ֶ�λ��"3"��ʾ������ģʽ��"6"��ʾ���ڹ���
		//7)  ��������:	00 - 12
		//8)  HDOP:		HDOPˮƽ�������ӡ�����ַ���=1.1����ַ���=3.1��ȡֵ��ΧΪ0.5~99.9��
		//9)  ���θ߶�:	��ʽ=1.1 - 5.1����������ʱ���ַ�Ϊ"-"��ȡֵ��ΧΪ-9999.9~99999.9����λΪ1�ס�
		//10) ������������Ը߶�: ����������(WGS-84��׼)��Դ��ˮƽ��ĸ߶ȡ��ַ���Ϊ1~4������Ϊ��
		//11) ���ʱ��:	�ַ���Ϊ1~5���������һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ������Ϊ�ա�
		//12) ���վID��: 4���ַ���ȡֵ��ΧΪ0000~1023��������ǲ�ֶ�λ������Ϊ�ա�
	case _GPS_RES_GPGGA_:

		//������$GPGGA,��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//ȡUTCʱ��
		if (!GPS_GetUTCTime())
			break;

		//������UTCʱ��,��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//ȡγ��
		GPS_GetLatitude();

		//��������/��γ��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//ȡ����
		GPS_GetLongitude();

		//��������/������
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//�жϡ�GPS״̬���Ƿ�ǲ�ֶ�λ(1)���ֶ�λ(2)?
		if ((*(rpcGPSMessage) != '1') && (*(rpcGPSMessage) != '2'))
			//��Ч��λ
			break;

		//������GPS״̬��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//��������
		tGpsInfo.u8ValidSatellites = (u8) GPS_StrToInteger();
		if (tGpsInfo.u8ValidSatellites < 3)
			break;

		//����������������
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//������HDOP��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}

		//���θ߶�
		GPS_GetHeight();

		//�߶�ƽ��
		GPS_AltitudeAverage(tGpsInfo.s16Altitude);

		/*
		 if(tGpsInfo.u16GpsInfoValid & _GPSINFO_FIRSTTIME)
		 {
		 //����ƽ���ٶ�
		 GPS_SpeedAverage(tGpsInfo.u16Speed);

		 tGpsInfo.u16GpsInfoValid &= ~_GPSINFO_FIRSTTIME;
		 tGpsInfo.u16GpsInfoValid |= _GPSINFO_GPS_VALID;

		 //ͬ��ϵͳʱ��
		 GPS_SyncSystemTime();
		 }
		 */
		break;

		/*
		 //GPS���02��GPS��λ������Ϣ
		 //$GPGLL,γ��,��/��γ,����,��/����,UTCʱ��,��λ״̬,ģʽָʾ*����<CR><LF>
		 //1)  γ��:		XXMM.mmmm,	��ʽ=12.1 - 32.4
		 //2)  ��/��γ:	N=��γ(��)��S=��γ(��)
		 //3)  ����:		XXXMM.mmmm,	��ʽ=12.1 - 32.4
		 //4)  ��/����:	E=����(��)��W=����(��)
		 //5)  UTCʱ�䣺	hhmmss.sss,	��ʽ=122.1 - 222.3������"hh"��ʾʱ��ȡֵ��Χ00~23��"mm"��ʾ�֣�ȡֵ��ΧΪ00~59��"ss"��ʾ�룬ȡֵ��ΧΪ00~59��"DD"Ϊ���С�����֡�
		 //6)  ��λ״̬: 1���ַ���"A"��ʾ��Ч��λ��"V"��ʾ��Ч��λ��
		 //7)  ģʽָʾ: 1���ַ���"A"��ʾ������λ��"D"��ʾ��ֶ�λ��"E"��ʾ���㣬"N"��ʾ������Ч��
		 case	_GPS_RES_GPGLL_:

		 break;
		 */

		//GPS���04���ɼ�������Ϣ
		//$GPGSV,GSV����������,����GSV���ı��,�ɼ����ǵ�����,PRN��,��������,���Ƿ�λ��,�����,��PRN��,��������,���Ƿ�λ��,�����*����<CR><LF>
		//1)  GSV����������: �����һ�������԰���4�����ǵ���Ϣ��������������4ʱ���ɼ�������Ϣ���ֳɶ��������1���ַ���ȡֵ��ΧΪ1~9��
		//2)  ����GSV���ı��: 1���ַ���ȡֵ��ΧΪ1~9��
		//3)  �ɼ����ǵ�����: 1~2���ַ���ȡֵ��ΧΪ0~12��
		//4)  PRN��: α��������롣2���ַ����������ڽ���λ�õ����Ǻţ�ȡֵ��ΧΪ01~32��
		//5)  ��������: 1~3���ַ���ȡֵ��ΧΪ0~90 ��
		//6)  ���Ƿ�λ��: 1~3���ַ���ȡֵ��ΧΪ0~359 ��
		//7)  �����: 0~2���ַ���ȡֵ��ΧΪ0~99dB��û�и��ٵ�����ʱΪ�ա�
	case _GPS_RES_GPGSV_: {
		u16 nGSVTotalSentence;
		u16 nGSVSequence;

		//������$GPGSV,��
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//��ȡ�����������
		if ((nGSVTotalSentence = GPS_StrToInteger()) == 0) {
			break;
		}

		//���������������
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//��ȡ������GSV���ı�š�
		nGSVSequence = GPS_StrToInteger();

		//����������GSV���ı�š�
		if (!GPS_JumpToNextSegment()) {
			//���ݴ���
			break;
		}
		//��ȡ���ɼ����ǵ������� ����ʹ�õ�����������00 - 12��
		if ((tGpsInfo.u8VisibleSatellites = GPS_StrToInteger()) == 0) {
			break;
		}

		/*
		 //��δ���յ��κ�GSV���?���߽��յ�һ���µ�GSV���?
		 if( ( m_cGPS_GSV_TotalSentence == 0 ) ||
		 ( m_cGPS_GSV_TotalSentence != nGSVTotalSentence ) ||
		 ( m_cGPS_VisibleSatellites != nGSVTotalSatellites ) )
		 {
		 //�Ƿ��1��?
		 if(nGSVSequence == 1)
		 {
		 //�������
		 m_cGPS_GSV_TotalSentence = nGSVTotalSentence;
		 //��ǰ������к�
		 m_cGPS_GSV_SentenceSequence = nGSVSequence;

		 //��������
		 m_cGPS_VisibleSatellites = nGSVTotalSatellites;
		 //�Ѿ�ȡ�õ�����
		 m_cGPS_GottenVSNumber = 0;

		 //������ݻ�����
		 memset(m_taGPS_VisibleSatellitesInfo, 0, sizeof(_tBWP_SATINFO) * 12);
		 }
		 else
		 {
		 //������
		 m_cGPS_GSV_TotalSentence = 0;
		 break;
		 }
		 }

		 //��ǰ�Ѿ����յ�GSV���ݣ�������˳���Ƿ�����?
		 else if(nGSVSequence == (m_cGPS_GSV_SentenceSequence + 1))
		 {
		 //��ǰ������к�
		 m_cGPS_GSV_SentenceSequence = nGSVSequence;
		 }

		 else
		 {
		 //������
		 m_cGPS_GSV_TotalSentence = 0;
		 m_cGPS_VisibleSatellites = 0;
		 m_cGPS_GottenVSNumber = 0;
		 break;
		 }

		 bErr = FALSE;
		 for(i = 0; i < 4; i++)
		 {
		 //������һ�ֶ�
		 if(!GPS_JumpToNextSegment(rpcGPSMessage))
		 {
		 //���ݴ���
		 bErr = TRUE;
		 break;
		 }

		 //�н���?
		 if((*rpcGPSMessage == 0x0D) || (*rpcGPSMessage == '*'))
		 break;

		 if(m_cGPS_GottenVSNumber < 12)
		 {
		 //��ȡ��PRN��=���Ǻ�
		 m_taGPS_VisibleSatellitesInfo[m_cGPS_GottenVSNumber].cPRN = (u8)GPS_StrToInteger(rpcGPSMessage);

		 //������PRN��
		 if(!GPS_JumpToNextSegment(rpcGPSMessage))
		 {
		 //���ݴ���
		 bErr = TRUE;
		 break;
		 }

		 //��ȡ���������ǡ�����λ=1��
		 m_taGPS_VisibleSatellitesInfo[m_cGPS_GottenVSNumber].cElevation = (u8)GPS_StrToInteger(rpcGPSMessage);

		 //�������������ǡ�
		 if(!GPS_JumpToNextSegment(rpcGPSMessage))
		 {
		 //���ݴ���
		 bErr = TRUE;
		 break;
		 }

		 //��ȡ�����Ƿ�λ�ǡ�����λ=1��
		 m_taGPS_VisibleSatellitesInfo[m_cGPS_GottenVSNumber].wAzimuth = (u16)GPS_StrToInteger(rpcGPSMessage);

		 //���������Ƿ�λ�ǡ�
		 if(!GPS_JumpToNextSegment(rpcGPSMessage))
		 {
		 //���ݴ���
		 bErr = TRUE;
		 break;
		 }

		 //��ȡ������ȡ�����λ=1dB��0~99dB
		 m_taGPS_VisibleSatellitesInfo[m_cGPS_GottenVSNumber].cSN = (u8)GPS_StrToInteger(rpcGPSMessage);

		 //�ѻ�ȡ������+1
		 m_cGPS_GottenVSNumber++;
		 }

		 else
		 {
		 BWNVTRACE( (_BWNV_printf_LEVEL_FORCE_, L"GPS������Ϣ���ɼ�����������12!\n") );
		 }
		 }

		 if(bErr)
		 {
		 //������г���
		 m_cGPS_GSV_TotalSentence = 0;

		 BWNVTRACE( (_BWNV_printf_LEVEL_GPS_, L"GPS��Ϣ��������ϢGSV��������д���\n") );
		 break;
		 }

		 //��������һ�䣬����������ȡ��
		 if((m_cGPS_GSV_SentenceSequence == m_cGPS_GSV_TotalSentence) && (m_cGPS_GottenVSNumber == m_cGPS_VisibleSatellites))
		 {
		 //���뱣��
		 EnterCriticalSection(&m_CS_GPS_Infor);

		 //�Ƿ�������������Ϣ?
		 if(m_cGPS_NeedSatelliteInfo)
		 {
		 //������Ϣ
		 memcpy((u8*)&m_taSYS_VisibleSatellitesInfo, (u8*)m_taGPS_VisibleSatellitesInfo, sizeof(_tBWP_SATINFO) * 12);

		 //������ЧGPS��Ϣ
		 tGPSFSM.u16GpsInfoValid |= _GPSINFO_SATINFO_VALID;
		 }

		 //���GSV�������
		 m_cGPS_GSV_TotalSentence = 0;

		 //�˳�����
		 LeaveCriticalSection(&m_CS_GPS_Infor);
		 }
		 */
//			BWNVTRACE( (_BWNV_printf_LEVEL_GPS_, L"GPS��Ϣ��������ϢGSV��������=%d\n", m_cGPS_GottenVSNumber) );
	}
//		TRACE_(QS_USER, NULL, "[GPGSV] total_Sat = %d, viewd_Sat = %d",
//				tGpsInfo.u8VisibleSatellites, tGpsInfo.u8ValidSatellites);

		break;
		//GPS���03����ǰ������Ϣ
		//$GPGSA,ģʽ,��λ����,PRN��,PRN��,PRN��,PRN��,PRN��,PRN��,PRN��,PRN��,PRN��,PRN��,PRN��,PRN��,PDOPλ�þ�������,HDOPˮƽ��������,VDOP��ֱ��������*����<CR><LF>
		//1)  ģʽ:		1���ַ���"A"��ʾ�Զ��������Զ���2D��3D��ʽ֮���л���"M"��ʾ�ֶ���GPS��ǿ����2D��3D��ʽ������
		//2)  ��λ����: 1���ַ���"1"��ʾû�ж�λ��"2"��ʾ2D��λ��"3"��ʾ3D��λ��
		//3)  PRN��:    α��������롣��Ϊ2���ַ����������ڽ���λ�õ����Ǻţ�ȡֵ��ΧΪ01~32��
		//4)  PDOPλ�þ�������: ����ַ���=1.1����ַ���=3.1��ȡֵ��ΧΪ0.5~99.9��
		//5)  HDOPˮƽ��������: ����ַ���=1.1����ַ���=3.1��ȡֵ��ΧΪ0.5~99.9��
		//6)  VDOP��ֱ��������: ����ַ���=1.1����ַ���=3.1��ȡֵ��ΧΪ0.5~99.9��
	case _GPS_RES_GPGSA_:

		break;
		//GPS���05�������ٶ���Ϣ
		//$GPVTG,�汱����,T,�ű�����,M,�����ٶ�1,N,�����ٶ�2,K,ģʽָʾ*����<CR><LF>
		//1)  ����汱�ĺ���: ��ʽ=1.1 - 3.2��ȡֵ��ΧΪ000.0~359.9 ��
		//2)  ��Դű��ĺ���: ��ʽ=1.1 - 3.2��ȡֵ��ΧΪ000.0~359.9 ��
		//3)  �����ٶ�1: ��ʽ=1.1 - 4.2��ȡֵ��ΧΪ000.0~999.9�ڡ�1��=1����/ʱ=1.852Km/h��
		//4)  �����ٶ�2: ��ʽ=1.1 - 4.2��ȡֵ��ΧΪ0000.0~1851.8Km/h��
		//5)  ģʽָʾ:	1���ַ���"A"��ʾ������λ��"D"��ʾ��ֶ�λ��"E"��ʾ���㣬"N"��ʾ������Ч��
	case _GPS_RES_GPVTG_:

		break;

		//FF: �գ�����Ӧ
	case _GPS_RES_EMPTY_:
		break;
	default:
		break;
	}
}

/***********************************************************************************************************
 * ����������	����GPS��Ӧ����
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	u16,			��Ӧ����
 * ��    �ƣ�	��������2009-02-04
 ***********************************************************************************************************/
u16 GPS_GetResponseCode() {
	u16 nResCode = 0;

	//����GPS��Ӧ����
	nResCode = GPS_GetSegmentResponseCode(_GPS_RES_GPRMC_, _GPS_RES_UNKNOWN_);

	//���ص�һ������Ӧ
	return nResCode;
}

/***********************************************************************************************************
 * ����������	����GPS�ֶ���Ӧ����
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 *				u8StartIndex,	[IN] ��ʼ������Ӧ��
 *				u8EndIndex,		[IN] ����������Ӧ��
 * ��    �ڣ�	u16,			��Ӧ����
 * ��    �ƣ�	��������2009-02-04
 ***********************************************************************************************************/
u16 GPS_GetSegmentResponseCode(u8 u8StartIndex, u8 u8EndIndex) {
	u16 nResCode;
	u16 nLen = 0;

	//����?
	if (*rpcGPSMessage == 0x0D) {
		return _GPS_RES_EMPTY_;
	}

	//�������п��ܵ���Ӧ
	for (nResCode = u8StartIndex; nResCode <= u8EndIndex; nResCode++) {
		//�붨����Ӧ������
		nLen = strlen_(GPS_ResponseString[nResCode]);

		//�Ƚ���Ӧ�ַ����Ƿ���֪���ַ���
		if (memcmp_((char*) rpcGPSMessage, GPS_ResponseString[nResCode],
				nLen)) {
			//�ҵ���Ӧ�ַ���������=ResCode
			break;
		}
	}

	//δ������?
	if (nResCode > u8EndIndex) {
		//δ֪��Ӧ
		return _GPS_RES_UNKNOWN_;
	}

	//ƥ�䵽��Ӧ�󣬽�ָ���ƶ�����Ӧ�ַ�����
	*rpcGPSMessage += nLen;

	return nResCode;

}

/***********************************************************************************************************
 * ����������	��������Ϣ��һ�ֶ�
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	BOOL			��=�ɹ�����=�������ݴ���
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
BOOL GPS_JumpToNextSegment() {
	while (1) {
		//ָ�����ͣ���ڽ�����
		if (*rpcGPSMessage == 0x0D)
			break;

		//�ַ����м䲻�ܳ��֡�$��
		if (*rpcGPSMessage == '$') {
			*rpcGPSMessage = 0x0D;

			return FALSE;
		}

		//�Ƿָ���?
		if (*rpcGPSMessage == ',') {
			//�ҵ��ָ���ʱ��ֹͣ
			rpcGPSMessage++;

			//���������հ�,���¸��ǿհ״�
			while (1) {
				//δ����ʱ������һ�ո�
				if (*rpcGPSMessage != ' ') {
					break;
				}

				//��һ��
				rpcGPSMessage++;
			}

			break;
		}

		//��һ�ַ�
		rpcGPSMessage++;
	}

	return TRUE;
}

/***********************************************************************************************************
 * ����������	ȡUTCʱ��
 * ��    ʽ��	"hhmmss.sss"��"hh"��ʾʱ��ȡֵ��Χ00~23��"mm"��ʾ�֣�ȡֵ��ΧΪ00~59��"ss"��ʾ�룬
 *				ȡֵ��ΧΪ00~59��"sss"Ϊ���С�����֡�
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	BOOL			��=�ɹ�����=�������ݴ���
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
BOOL GPS_GetUTCTime() {
	u16 nPou16Pos;

	//���������������ֳ���
	nPou16Pos = GPS_GetIntegerLen();

	//�����������ֳ���<5�����?
	if (nPou16Pos < 5) {
		//����
		return FALSE;
	}

	//��ʱ(hh)�����ַ�����
	nPou16Pos -= 4;

	//��ȡ��ʱ(hh)��������ʱ��ġ�ʱ�� = UTCʱ��ġ�ʱ�� + 8
	tGpsInfo.u8Hour = GPS_StrToByte(nPou16Pos) + 8;
	//��ȡ����(mm)��
	tGpsInfo.u8Minute = GPS_StrToByte(2);
	//��ȡ����(ss)��
	tGpsInfo.u8Second = GPS_StrToByte(2);
	//��ʱ����
	if (tGpsInfo.u8Hour >= 24) {
		tGpsInfo.u8Hour -= 24;
	}

	return TRUE;
}

/***********************************************************************************************************
 * ����������	ȡUTC����
 * ��    ʽ��	UTC����: 6���ַ�����ʽΪ"DDMMYY"������"DD"Ϊ���ڣ�ȡֵ��ΧΪ1~31��"mm"Ϊ�·ݣ�
 *				ȡֵ��ΧΪ1~12��"YY"Ϊ�꣬ȡֵ��ΧΪ00~99��
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 *				u8Format,		[IN] 6="DDMMYY"��ʽ��8="DDMMYYYY"��ʽ
 * ��    �ڣ�	BOOL			��=�ɹ�����=�������ݴ���
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
void GPS_GetUTCDate(u8 u8Format) {
	//��ȡ����(DD)��
	tGpsInfo.u8Day = GPS_StrToByte(2);
	//��ȡ����(mm)��
	tGpsInfo.u8Month = GPS_StrToByte(2);

	if (u8Format == 6) {
		//��ȡ����(YY)��
		tGpsInfo.u16Year = GPS_StrToWord(2) + 2000;
	}

	else {
		//��ȡ����(YY)��
		tGpsInfo.u16Year = GPS_StrToWord(4);
	}

	return;
}

/***********************************************************************************************************
 * ����������	ȡγ��
 * ��	ʽ��	γ��:��ʽΪ"ddmm.mmmm"��"dd"��ʾ�ȣ�ȡֵ��Χ00~89��"mm"��ʾ�֣�
 ȡֵ��ΧΪ00~59��"mmmm"Ϊ�ֵ�С�����֡�
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	BOOL			��=�ɹ�����=�������ݴ���
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
BOOL GPS_GetLatitude() {
	u16 u8Count;
	u16 nDeg, nMin, ndMin;

	//���������������ֳ���
	u8Count = GPS_GetIntegerLen();

	//�������ֳ���<3�����?
	if (u8Count < 3) {
		//����
		return FALSE;
	}

	/*
	 γ�ȱ�ʾ��Χ��00��00.000�֡���89��59.999�֣�
	 ����ѹ��BCD���룬�����λ�Ƿ���λ����������ʾ����γ������������ʾ����γ����
	 ���֡��ĵ�λ�ǣ�ǧ��֮һ�֡�
	 [����]�� ��γ30��37.901�ֱ�ʾΪ��
	 80H+3H��03H��79H��01H
	 */

	//��
	tGpsInfo.Latitude[0] = (*rpcGPSMessage - 0x30) & 0x0F;
	tGpsInfo.Latitude[1] = (*(rpcGPSMessage + 1) - 0x30) << 4;

	//��
	tGpsInfo.Latitude[1] |= *(rpcGPSMessage + 2) - 0x30;
	tGpsInfo.Latitude[2] = (*(rpcGPSMessage + 3) - 0x30) << 4;

	//��
	if (*(rpcGPSMessage + 4) == '.') {
		tGpsInfo.Latitude[2] |= *(rpcGPSMessage + 5) - 0x30;
		tGpsInfo.Latitude[3] = (*(rpcGPSMessage + 6) - 0x30) << 4;
		tGpsInfo.Latitude[3] |= *(rpcGPSMessage + 7) - 0x30;
	} else {
		tGpsInfo.Latitude[2] &= 0xF0;
		tGpsInfo.Latitude[3] = 0;
	}

	//����(dd)�����ַ�����
	u8Count -= 2;

	//��ȡ����(dd)��
	nDeg = (u16) GPS_StrToByte(u8Count);

	//��ȡ����(mm)��
	nMin = (u16) GPS_StrToByte(2);

	//����С����
	if (*rpcGPSMessage == '.') {
		rpcGPSMessage++;

		//��ȡ����С������(mmmm)��
		ndMin = (u16) GPS_StrToWord(4);
	} else {
		ndMin = 0;
	}

	//����γ�ȣ���λΪ10^-3��
	tGpsInfo.s32Latitude = (nDeg * 60 + nMin) * 1000 + ndMin * 0.1;

	//������γ��,��
	if (!GPS_JumpToNextSegment()) {
		//���ݴ���
		return FALSE;
	}

	//�ж���/��γ
	if (*rpcGPSMessage == 'S') {
		//��γ�����λ��1
		tGpsInfo.Latitude[0] |= 0x80;
	}

	return TRUE;
}

/***********************************************************************************************************
 * ����������	ȡ����
 * ��	ʽ��	����:��ʽΪ"dddmm.mmmm"��"ddd"��ʾ�ȣ�ȡֵ��Χ00~179��"mm"��ʾ�֣�ȡֵ��ΧΪ00~59��
 "mmmm"Ϊ�ֵ�С�����֡���/����: E=����(��)��W=����(��)
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	BOOL			��=�ɹ�����=�������ݴ���
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
BOOL GPS_GetLongitude() {
	u16 u8Count;
	u16 nDeg, nMin, ndMin;

	//���������������ֳ���
	u8Count = GPS_GetIntegerLen();

	//�������ֳ���<3�����?
	if (u8Count < 3) {
		//����
		return FALSE;
	}

	//����:��
	tGpsInfo.Longtitude[0] = (*rpcGPSMessage - 0x30) << 4;
	tGpsInfo.Longtitude[0] |= *(rpcGPSMessage + 1) - 0x30;
	tGpsInfo.Longtitude[1] = (*(rpcGPSMessage + 2) - 0x30) << 4;
	tGpsInfo.Longtitude[1] |= *(rpcGPSMessage + 3) - 0x30;

	//����:��
	tGpsInfo.Longtitude[2] = (*(rpcGPSMessage + 4) - 0x30) << 4;

	//����:��
	if (*(rpcGPSMessage + 5) == '.') {
		tGpsInfo.Longtitude[2] |= *(rpcGPSMessage + 6) - 0x30;
		tGpsInfo.Longtitude[3] = (*(rpcGPSMessage + 7) - 0x30) << 4;
		tGpsInfo.Longtitude[3] |= *(rpcGPSMessage + 8) - 0x30;
	} else {
		tGpsInfo.Longtitude[2] &= 0xF0;
		tGpsInfo.Longtitude[3] = 0;
	}

	//����(dd)�����ַ�����
	u8Count -= 2;

	//��ȡ����(dd)��
	nDeg = (u16) GPS_StrToByte(u8Count);

	//��ȡ����(mm)��
	nMin = (u16) GPS_StrToByte(2);

	//����С����
	if (*rpcGPSMessage == '.') {
		rpcGPSMessage++;

		//��ȡ����С������(mmmm)��
		ndMin = (u16) GPS_StrToWord(4);
	} else {
		ndMin = 0;
	}

	//���㾭�ȣ���λΪ10^-3��
	tGpsInfo.s32Longitude = (nDeg * 60 + nMin) * 1000 + ndMin * 0.1;

	//����������,��
	if (!GPS_JumpToNextSegment()) {
		//���ݴ���
		return FALSE;
	}

	//�ж϶�/����
	if (*rpcGPSMessage == 'W') {
		//����
		tGpsInfo.s32Longitude = -tGpsInfo.s32Longitude;
		//���������λ��1
		tGpsInfo.Longtitude[0] |= 0x80;
	}

	return TRUE;
}

/***********************************************************************************************************
 * ����������	ȡ�ٶ�
 * ��	ʽ��	��λΪ�ڣ�1��=1����/ʱ=1.852Km/h��
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	��
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
void GPS_GetSpeed() {
	u16 u8Count;
	u16 nNodeI = 0, nNodeD = 0;
	u8 u8Speed_Hundred;
	u8 u8Speed_Ten;
	u8 u8Speed_One;
	u8 u8Speed;
	u16 u16Speed;

	//���������������ֳ���
	u8Count = GPS_GetIntegerLen();

	//��ǰλ�ò���С����?
	if (u8Count) {
		//��ȡ�����������֡�
		nNodeI = (u16) GPS_StrToWord(u8Count);
	}

	//����С����
	if (*rpcGPSMessage == '.') {
		rpcGPSMessage++;

		//��ȡ����С�����֡�
		nNodeD = (u16) GPS_StrToWord(3);
	}

	//�����ٶȣ���λΪ0.1����/ʱ
	u16Speed = (nNodeI + nNodeD * 0.001) * 18.52;

	//�����ٶ�ƽ����ʵ���ٶ�ȡƽ�����ֵ
	tGpsInfo.u16Speed = GPS_SpeedAverage(u16Speed);

	//���������ٶ�
	u8Speed = tGpsInfo.u16Speed * 0.1;		//����/ʱ
	u8Speed_Hundred = u8Speed * 0.01;	//��λ
	u8Speed_Ten = (u8Speed - u8Speed_Hundred * 100) * 0.1;	//ʮλ
	u8Speed_One = u8Speed - u8Speed_Hundred * 100 - u8Speed_Ten * 10;	//��λ
			/*
			 �ٶȱ�ʾ������
			 ��ʾ��Χ��0����9999����/Сʱ
			 ����ѹ��BCD���롣
			 [����]�� 120����/Сʱ��ʾΪ��
			 01H��20H
			 */
	tGpsInfo.Speed[0] = u8Speed_Hundred;
	tGpsInfo.Speed[1] = u8Speed_Ten << 4;
	tGpsInfo.Speed[1] |= u8Speed_One;
}

/***********************************************************************************************************
 * ����������	ȡ����
 * ��	ʽ��	�汱����:	��ʽ=1.1 - 3.2��ȡֵ��ΧΪ000.0~359.9 ��
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	��
 * ��    �ƣ�	��  ӱ��2007-12-20
 ***********************************************************************************************************/
void GPS_GetHeading() {
	/*
	 ��λ��ʾ������
	 ��ʾ��Χ��000����359��
	 ����ѹ��BCD���룬����Ϊ0�ȣ�˳ʱ�뷽�������
	 ��λΪ���ȡ�
	 [����]�� 154�ȱ�ʾΪ��
	 01H��54H
	 */
	//����ǵ�λ�ɷָ�Ϊ�ȡ� fixed by GZD,2009-5-21
	u16 u16Head = GPS_StrToFloat();
	if (u16Head < 360) {
		tGpsInfo.u16Heading = u16Head;
	}

	//����
	tGpsInfo.Direction[0] = tGpsInfo.u16Heading / 100;
	tGpsInfo.Direction[1] = ((tGpsInfo.u16Heading / 10) % 10) << 4;
	tGpsInfo.Direction[1] |= tGpsInfo.u16Heading % 10;
}

/***********************************************************************************************************
 * ����������	ȡ�߶�
 * ��	ʽ��	���θ߶�:	��ʽ=1.1 - 5.1����������ʱ���ַ�Ϊ"-"��ȡֵ��ΧΪ-9999.9~99999.9����λΪ1�ס�
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	��
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
void GPS_GetHeight() {
	//��ȡ�߶ȣ���λ=1��
	tGpsInfo.s16Altitude = (short) (GPS_StrToFloat() + 0.5);

	/*	//����
	 tGpsInfo.Altitude[0] = tGpsInfo.s16Altitude / 100;
	 tGpsInfo.Altitude[1] = ((tGpsInfo.s16Altitude / 10) % 10) << 4;
	 tGpsInfo.Altitude[1] |= tGpsInfo.s16Altitude % 10;

	 if(tGpsInfo.s16Altitude < 0)  //���������λ��1
	 {
	 tGpsInfo.Altitude[0] |= 0x80;
	 }*/
}

/***********************************************************************************************************
 * ����������	ASCII�����ַ���ת��������
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	u16,			������
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
u16 GPS_StrToInteger() {
	u16 nResult = 0;			//���=0
	s16 nSign = 1;				//����λ=����

	//�����ո�
	while (*rpcGPSMessage == ' ') {
		rpcGPSMessage++;
	}

	//����?
	if (*rpcGPSMessage == '-') {
		nSign = -1;

		rpcGPSMessage++;
	}

	//����ǰ�ֶ�ת����������
	while (1) {
		//����
		u8 c = *rpcGPSMessage;

		//ת��0-9����
		if ((c >= '0') && (c <= '9')) {
			c -= 0x30;
		} else {
			break;
		}

		nResult = nResult * 10 + (u16) c;

		//��һ��
		rpcGPSMessage++;
	}

	if (nResult) {
		nResult *= nSign;
	}

	//�������������
	return nResult;
}

/***********************************************************************************************************
 * ����������	ASCII�����ַ���ת���ɸ�����
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	u16,			������
 * ��    �ƣ�	��  ӱ��2007-12-20
 ***********************************************************************************************************/
float GPS_StrToFloat() {
	float fResult = 0.0, fE = 10.0;		//���=0
	s16 nSign = 1;						//����λ=����
	u16 nPou16er = 0;

	//�����ո�
	while (*rpcGPSMessage == ' ') {
		rpcGPSMessage++;
	}

	//����?
	if (*rpcGPSMessage == '-') {
		nSign = -1;

		rpcGPSMessage++;
	}

	//����ǰ�ֶ�ת����������
	while (1) {
		//����
		u8 c = *rpcGPSMessage;

		//ת��0-9����
		if ((c >= '0') && (c <= '9')) {
			c -= 0x30;

			if (!nPou16er) {
				fResult = (float) (fResult * 10.0) + (float) c;
			} else {
				fResult += (float) c / fE;
				fE *= 10.0;
			}
		} else if ((nPou16er == 0) && (c == '.')) {
			nPou16er++;
		} else {
			break;
		}

		//��һ��
		rpcGPSMessage++;
	}

	if ((fResult != 0.0) && (nSign < 0)) {
		fResult *= nSign;
	}

	//���ؽ��
	return fResult;
}

/***********************************************************************************************************
 * ����������	ȡ�ֽ�����
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 *				u8Count,			[IN] Ϊ��Ҫת�����ַ�����
 * ��    �ڣ�	u8,			�ֽ�����
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
u8 GPS_StrToByte(u8 u8Count) {
	u8 cResult = 0;

	while (1) {
		//����
		u8 c = *rpcGPSMessage;

		//��0-9���ַ�?
		if ((c >= '0') && (c <= '9')) {
			c -= 0x30;
		} else {
			//�����ַ����˳�
			break;
		}

		cResult = cResult * 10 + c;

		//��һ��
		rpcGPSMessage++;

		//�ַ�����-1
		if (u8Count > 0) {
			u8Count--;

			if (u8Count == 0) {
				break;
			}
		}
	}

	return cResult;
}

/***********************************************************************************************************
 * ����������	ȡ������
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 *				u8Count,			[IN] Ϊ��Ҫת�����ַ�����
 * ��    �ڣ�	u16,			������
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
u16 GPS_StrToWord(u8 u8Count) {
	u16 wResult = 0;

	while (1) {
		u8 c;

		//����
		c = *rpcGPSMessage;

		//ת������
		if ((c >= '0') && (c <= '9')) {
			c -= 0x30;
		} else {
			//�����ַ����˳�
			break;
		}

		//�ۼ�
		wResult = wResult * 10 + (u16) c;

		//��һ��
		rpcGPSMessage++;

		//�ַ�����-1
		if (u8Count > 0) {
			u8Count--;
			if (u8Count == 0) {
				break;
			}
		}
	}

	//�����ַ���ʱ��β����0(ʮ������)
	while (u8Count) {
		wResult *= 10;

		//�ַ�����-1
		u8Count--;
	}

	return wResult;
}

/***********************************************************************************************************
 * ����������	���������������ֳ���
 * ��    �ڣ�	rpcGPSMessage,	[IN] GPS��Ӧ�ִ�ָ������
 * ��    �ڣ�	u16,			С����λ��
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
u16 GPS_GetIntegerLen() {
	u16 nPou16Pos = 0;
	u8* pcMsg = rpcGPSMessage;

	while (1) {
		u8 c = *pcMsg;

		//����?
		if ((c < '0') || (c > '9')) {
			//����0-9�������
			break;
		}

		pcMsg++;

		//�����������ֳ���+1
		nPou16Pos++;
	}

	//���������������ֳ���
	return nPou16Pos;
}

/***********************************************************************************************************
 * ����������	����ƽ���ٶ�
 * ��    �ڣ�	u16Speed,	[IN] ��ǰ�ٶ�(10m/h)
 * ��    �ڣ�	��
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
u16 GPS_SpeedAverage(u16 u16Speed) {
	u16 u16AverageSpeed;

	tSpeedSmooth.aSpeedArray[4] = tSpeedSmooth.aSpeedArray[3];
	tSpeedSmooth.aSpeedArray[3] = tSpeedSmooth.aSpeedArray[2];
	tSpeedSmooth.aSpeedArray[2] = tSpeedSmooth.aSpeedArray[1];
	tSpeedSmooth.aSpeedArray[1] = tSpeedSmooth.aSpeedArray[0];
	tSpeedSmooth.aSpeedArray[0] = u16Speed;

	//�ٶȼ�����+1
	if (tSpeedSmooth.u16SpeedArray_Counter < 5) {
		tSpeedSmooth.u16SpeedArray_Counter++;
	}

	if (tSpeedSmooth.u16SpeedArray_Counter >= 5) {
		//�ٶ�ͻ��(>8Km/h)?
		if (abs(u16Speed, tSpeedSmooth.u16AverageSpeed) > 800) {
			tSpeedSmooth.u16SpeedArray_Counter = 2;
		}
	}

	//�ٶ�ƽ��
	switch (tSpeedSmooth.u16SpeedArray_Counter) {
	//2�㣺ƽ���ٶ� = (�ٶ�i + �ٶ�i-1) / 2
	case 2:
		u16AverageSpeed = (u16) ((tSpeedSmooth.aSpeedArray[0]
				+ tSpeedSmooth.aSpeedArray[1]) / 2);
		break;

		//3�㣺ƽ���ٶ� = 0.3 * �ٶ�i + 0.45 * �ٶ�i-1 + 0.25 * �ٶ�i-2
	case 3:
		u16AverageSpeed = (u16) ((tSpeedSmooth.aSpeedArray[0] * 6
				+ tSpeedSmooth.aSpeedArray[1] * 9
				+ tSpeedSmooth.aSpeedArray[2] * 5) / 20);
		break;

		//4�㣺ƽ���ٶ� = 0.25 * �ٶ�i + 0.4 * �ٶ�i-1 + 0.2 * �ٶ�i-2 + 0.15 * �ٶ�i-3
	case 4:
		u16AverageSpeed = (u16) ((tSpeedSmooth.aSpeedArray[0] * 5
				+ tSpeedSmooth.aSpeedArray[1] * 8
				+ tSpeedSmooth.aSpeedArray[2] * 4
				+ tSpeedSmooth.aSpeedArray[3] * 3) / 20);
		break;

		//5�㣺ƽ���ٶ� = 0.2 * �ٶ�i + 0.375 * �ٶ�i-1 + 0.175 * �ٶ�i-2 + 0.15 * �ٶ�i-3 + 0.1 * �ٶ�i-4
	case 5:
		u16AverageSpeed = (u16) ((tSpeedSmooth.aSpeedArray[0] * 8
				+ tSpeedSmooth.aSpeedArray[1] * 15
				+ tSpeedSmooth.aSpeedArray[2] * 7
				+ tSpeedSmooth.aSpeedArray[3] * 6
				+ tSpeedSmooth.aSpeedArray[4] * 4) / 40);
		break;

		//1�㣺ƽ���ٶ� = �ٶ�i
	case 1:
	default:
		u16AverageSpeed = u16Speed;
		break;
	}

	tSpeedSmooth.u16AverageSpeed = u16AverageSpeed;

	return u16AverageSpeed;
}

/***********************************************************************************************************
 * ����������	�߶�ƽ��
 * ��    �ڣ�	shAltitude,	[IN] ��ǰ�߶�(m)
 * ��    �ڣ�	��
 * ��    �ƣ�	��  ӱ��2005-11-16
 ***********************************************************************************************************/
void GPS_AltitudeAverage(s16 shAltitude) {
	s16 shAverageAltitude;

	tAltitudeSmooth.aAltitudeArray[4] = tAltitudeSmooth.aAltitudeArray[3];
	tAltitudeSmooth.aAltitudeArray[3] = tAltitudeSmooth.aAltitudeArray[2];
	tAltitudeSmooth.aAltitudeArray[2] = tAltitudeSmooth.aAltitudeArray[1];
	tAltitudeSmooth.aAltitudeArray[1] = tAltitudeSmooth.aAltitudeArray[0];
	tAltitudeSmooth.aAltitudeArray[0] = shAltitude;

	//�߶ȼ�����+1
	if (tAltitudeSmooth.u32AltitudeArray_Counter < 5) {
		tAltitudeSmooth.u32AltitudeArray_Counter++;
	}

	if (tAltitudeSmooth.u32AltitudeArray_Counter >= 5) {
		//�߶�ͻ��(>200m)?
		if (abs(shAltitude,tAltitudeSmooth.u16AverageAltitude) > 200) {
			tAltitudeSmooth.u32AltitudeArray_Counter = 2;
		}
	}

	//�ٶȸ߶�
	switch (tAltitudeSmooth.u32AltitudeArray_Counter) {
	//2�㣺ƽ���߶� = (�߶�i + �߶�i-1) / 2
	case 2:
		shAverageAltitude = (s16) ((tAltitudeSmooth.aAltitudeArray[0]
				+ tAltitudeSmooth.aAltitudeArray[1]) / 2);
		break;

		//3�㣺ƽ���߶� = 0.3 * �߶�i + 0.45 * �߶�i-1 + 0.25 * �߶�i-2
	case 3:
		shAverageAltitude = (s16) ((tAltitudeSmooth.aAltitudeArray[0] * 6
				+ tAltitudeSmooth.aAltitudeArray[1] * 9
				+ tSpeedSmooth.aSpeedArray[2] * 5) / 20);
		break;

		//4�㣺ƽ���߶� = 0.25 * �߶�i + 0.4 * �߶�i-1 + 0.2 * �߶�i-2 + 0.15 * �߶�i-3
	case 4:
		shAverageAltitude = (s16) ((tAltitudeSmooth.aAltitudeArray[0] * 5
				+ tAltitudeSmooth.aAltitudeArray[1] * 8
				+ tAltitudeSmooth.aAltitudeArray[2] * 4
				+ tSpeedSmooth.aSpeedArray[3] * 3) / 20);
		break;

		//5�㣺ƽ���߶� = 0.2 * �߶�i + 0.375 * �߶�i-1 + 0.175 * �߶�i-2 + 0.15 * �߶�i-3 + 0.1 * �ٶ�i-4
	case 5:
		shAverageAltitude = (s16) ((tAltitudeSmooth.aAltitudeArray[0] * 8
				+ tAltitudeSmooth.aAltitudeArray[1] * 15
				+ tSpeedSmooth.aSpeedArray[2] * 7
				+ tAltitudeSmooth.aAltitudeArray[3] * 6
				+ tAltitudeSmooth.aAltitudeArray[4] * 4) / 40);
		break;

		//1�㣺ƽ���߶� = �߶�i
	case 1:
	default:
		shAverageAltitude = shAltitude;
		break;
	}

	tAltitudeSmooth.aAltitudeArray[0] = (s16) shAverageAltitude;
	tAltitudeSmooth.u16AverageAltitude = shAverageAltitude;
}

/*��������״̬*/
void GPS_SetAntenaState(u8 state) {
	tGpsInfo.bAntenaState = state;
}

/*��ȡ����״̬*/
u8 GPS_GetAntenaState() {

	return tGpsInfo.bAntenaState;
}

//��ȡ��ǰGPS״̬
_tGPSInfo* GPS_GetInfo() {
	return &tGpsInfo;
}

//�Ƿ�λ�ɹ�
BOOL GPS_IsLocated() {
	if (tGpsInfo.u16GpsInfoValid & _GPSINFO_GPS_VALID)
		return TRUE;

	return FALSE;
}

//ͬ��ϵͳʱ��
void GPS_SyncSystemTime() {
	SYSTIMER timer;

	timer.year = tGpsInfo.u16Year - 2000;
	timer.month = tGpsInfo.u8Month;
	timer.day = tGpsInfo.u8Day;
	timer.hour = tGpsInfo.u8Hour;
	timer.minute = tGpsInfo.u8Minute;
	timer.second = tGpsInfo.u8Second;

	SysTimer_Set(&timer);

	TRACE_(QS_USER, NULL, "[GPS] Sync time: %d-%d-%d:%d-%d-%d", timer.year,
			timer.month, timer.day, timer.hour, timer.minute, timer.second);
}

//�ϱ�GPS��λ��Ϣ
void GPS_ReportGpsInfo() {
	/**
	 TaskEvt	*pe;

	 pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
	 pe->cmd = EVS15_CMD_GPS_AUTOREPORT;
	 pe->sequence = GeneratePacketSequenceNumber();
	 pe->address = 0;
	 pe->dataSize = 0;
	 pe->ret = 0;
	 pe->checksum = 0;
	 QACTIVE_POST(AO_Gprs, (QEvt*)pe, (void*)0);
	 */
}

//��ӡGPS������Ϣ
void TRACE_GPSINFO(/*_tGPSInfo *pGpsInfo*/) {
	TRACE_(QS_USER, NULL,
			"[GPS] %d-%d-%d:%d-%d-%d, Lon=%x.%x%x%x, Lat=%x.%x%x%x, Speed=%x%x, view_S=%x, visib_S=%x",
			tGpsInfo.u16Year, tGpsInfo.u8Month, tGpsInfo.u8Day, tGpsInfo.u8Hour,
			tGpsInfo.u8Minute, tGpsInfo.u8Second, tGpsInfo.Longtitude[0],
			tGpsInfo.Longtitude[1], tGpsInfo.Longtitude[2],
			tGpsInfo.Longtitude[3], tGpsInfo.Latitude[0], tGpsInfo.Latitude[1],
			tGpsInfo.Latitude[2], tGpsInfo.Latitude[3], tGpsInfo.Speed[0],
			tGpsInfo.Speed[1], tGpsInfo.u8ValidSatellites,
			tGpsInfo.u8VisibleSatellites);
}

//�洢GPS���ݣ�
void GPS_DumpInfo() {
//	_tGPSInfo* pGpsInfo;
//	_tGPSSaveInfo tInfo;
//
//	pGpsInfo = GPS_GetInfo();	//ȡ��ǰλ��
//	tInfo.bInfoValid = GPS_IsLocated();
//	tInfo.u8VisibleSatellites = pGpsInfo->u8VisibleSatellites;//�ɼ�������
//	tInfo.u8ValidSatellites = pGpsInfo->u8ValidSatellites;//��Ч������
//	memcpy_(tInfo.Longtitude, pGpsInfo->Longtitude, 4);//����
//	memcpy_(tInfo.Latitude, pGpsInfo->Latitude, 4);
//	tInfo.Direction[0] = pGpsInfo->Altitude[0];//���θ߶�
//	tInfo.Direction[1] = pGpsInfo->Altitude[1];
//	tInfo.Speed[0] = pGpsInfo->Speed[0];//�ٶ�
//	tInfo.Speed[1] = pGpsInfo->Speed[1];
//	tInfo.u16Year = pGpsInfo.u16Year;
//	tInfo.u8Month = pGpsInfo.u8Month;
//	tInfo.u8Day = pGpsInfo.u8Day;
//	tInfo.u8Hour = pGpsInfo.u8Hour;
//	tInfo.u8Minute = pGpsInfo.u8Minute;
//	tInfo.u8Second = pGpsInfo.u8Second;

//	SD_DumpGpsInfo(GPS_GetInfo());	//��λ��Ϣ���浽SD��
}

/**
 * GPSӲ����ʼ��
 *
 */
void GPS_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPS_RST_PIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPS_ANT_DET_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	///���ڳ�ʼ��
	USART_Configuration(COM_GPS, 9600);

}

