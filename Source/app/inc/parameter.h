#ifndef	_PARAMETER_H_
#define	_PARAMETER_H_

#include "stm32f2xx_conf.h"
#include "protocol.h"
#include "config.h"
#include <fram.h>


#pragma pack(1)


/**������ID����*/
enum {
	ePARAM_ALIVE_INTERVAL = 0,	///			=	0x0001,		///�ն��������ͼ������λΪs
	ePARAM_TCPACK_TIMEOUT,	///			=	0x0002,		///TCPӦ��ʱ����λΪs
	ePARAM_TCP_RESENDTIMES,	///			=	0x0003,		///TCP��Ϣ�ش�����
	ePARAM_UDPACK_TIMEOUT,	///			=	0x0004,		///UDPӦ��ʱ����λΪs
	ePARAM_UDP_RESENDTIMES,	///			=	0x0005,		///UDP��Ϣ�ش�����
	ePARAM_SMSACK_TIMEOUT,	///			=	0x0006,		///SMSӦ��ʱ����λΪs
	ePARAM_SMS_RESENDTIMES,	///			=	0x0007,		///SMS��Ϣ�ش�����
	/**0X0008 ----0X000F ����*/
	ePARAM_MAINSRV_APN = 7,	///				=	0x0010,		///��������APN������ͨ�Ų��ŷ��ʵ�
	ePARAM_MAINSRV_USR,	///				=	0x0011,		///������������ͨ�Ų����û���
	ePARAM_MAINSRV_PSWD,	///				=	0x0012,		///������������ͨ�Ų�������
	ePARAM_MAINSRV_IP,	///				=	0x0013,		///����������ַ��IP������
	ePARAM_VICESRV_APN,	///				=	0x0014,		///���ݷ�����APN������ͨ�Ų��ŷ��ʵ�
	ePARAM_VICESRV_USR,	///				=	0x0015,		///���ݷ���������ͨ�Ų����û���
	ePARAM_VICESRV_PSWD,	///				=	0x0016,		///���ݷ���������ͨ�Ų�������
	ePARAM_VICESRV_IP,	///				=	0x0017,		///���ݷ�������ַ��IP������
	ePARAM_TCP_PORT,	///				=	0x0018,		///TCP�˿�
	ePARAM_UDP_PORT,	///					=	0x0019,		///UDP�˿�
	/**0X001A ----0X001F ����*/
#ifdef JTT808_Ver_2013	///�°�����Э�鲿��
	ePARAM_ICVERIFY_MS_IPOrDNS = 17,	///		=	0x001A,		///��·����֤IC����֤��������IP��ַ������
	ePARAM_ICVERIFY_TCPPORT,	///			=	0x001B,		///��·����֤IC����֤��������TCP�˿�
	ePARAM_ICVERIFY_UDPPORT,	///			=	0x001C,		///��·����֤IC����֤��������UDP�˿�
	ePARAM_ICVERIFY_BS_UDPPORT,	///		=	0x001D,		///��·����֤IC�����ݷ�����IP��ַ���������˿�ͬ��������
#endif///JTT808_Ver_2013

	ePARAM_POSRPT_STRATEGE = 21,	///	�ն�λ�û㱨����
				/**�ն˻㱨������0:����ACC״̬��1:���ݵ�¼״̬��ACC״̬�����жϵ�¼״̬��
				 	 	 	 	 	 	 	 	 	 	 	 	����¼�ٸ���ACC״̬*/
	ePARAM_POSRPT_METHOD = 22,	///				=	0x0021,
	ePARAM_DRV_UNREG_RPTPERIOD = 23,	///		=	0x0022,		///��ʻԱδ��¼�㱨ʱ��������λΪ��
	/**0X0023 ----0X0026 ����*/
	ePARAM_RPTPERIOD_ONSLEEP,	///			=	0x0027,		///����ʱ�㱨ʱ��������λΪ��
	ePARAM_RPTPERIOD_URGENT,	///			=	0x0028,		///��������ʱ�㱨ʱ��������λΪ��
	ePARAM_RPTPERIOD_DEFAULT,	///			=	0x0029,		///ȱʡʱ�㱨ʱ��������λΪ��
	/**0X002A ----0X002B ����*/
	ePARAM_RPTDIST_DEFAULT,	///			=	0x002C,		///ȱʡʱ����㱨�������λΪ��
	ePARAM_RPTDIST_DRV_UNREG,	///			=	0x002D,		///��ʻԱδ��¼����㱨�������λΪ��
	ePARAM_RPTDIST_SLEEP,	///				=	0x002E,		///����ʱ����㱨�������λΪ��
	ePARAM_RPTDIST_URGENT,	///			=	0x002F,		///��������ʱ����㱨�������λΪ��
	ePARAM_INFLECTION_POINT_ANGLE,	///	=	0x0030,		///�յ㲹���Ƕȣ���λΪ��(< 180)
	ePARAM_BARRIER_RADIUS,	///			=	0x0031,		///����Χ���뾶(�Ƿ���λ��ֵ)����λΪ��
	/**0X0032----0X003F ����*/
	ePARAM_MONITORCENTER_PHONENUM = 33,	///	=	0x0040,		///���ƽ̨�绰����
	ePARAM_RESET_PHONENUM,	///			=	0x0041,		///��λ�绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˸�λ
				  ///�ظ��������õ绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˻ָ���������
	ePARAM_RECOVERFACTORY_PHONENUM = 34,	///	=	0x0042,
	ePARAM_MONITORCENTER_SMS_NUM,	///		=	0x0043,		///���ƽ̨SMS�绰����
	ePARAM_RECVTXTALARM_SMS_NUM,	///		=	0x0044,		///�����ն�SMS�ı������绰����
					///�ն˵绰�������ԣ�0:�Զ���ͨ��1:ACC ONʱ�Զ���ͨ��OFFʱ�ֶ�����
	ePARAM_ANSWERCALLING_STRATEGE,	///	=	0x0045,
			 ///�ն�ÿ��ͨ��ʱ�ͨ��ʱ�䣬��λΪ�룬0Ϊ������ͨ����0xffffffffΪ������
	ePARAM_TOUT_PERCALL,	///				=	0x0046,
					///�ն�ÿ���ͨ��ʱ�䣬��λΪ�룬0Ϊ������ͨ����0xffffffffΪ������
	ePARAM_TOUT_PERMONTHCALL,	///			=	0x0047,
	ePARAM_MONITORLISTEN_PHONENUM,	///	=	0x0048,			///�����绰����
	ePARAM_SUPERVISE_SMS_NUM = 41,	///			=	0x0049,			///���ƽ̨��Ȩ���ź���
	/**0X004A----0X004F ����*/

	///���������֣���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����������
	ePARAM_ALARM_MASK = 42,	///				=	0x0050,
	///���������ı�SMS���أ���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ�����ı�SMS
	ePARAM_SMSCTRL_ONALARM,	///			=	0x0051,
	///�������㿪�أ���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ����ͷ����
	ePARAM_PICCTRL_ONALARM,	///			=	0x0052,
	///��������洢��־����λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ�ĵ���Ƭ���д洢������ʵʱ�ϴ�
	ePARAM_PICCTRL_STORE_ONALARM,	///		=	0x0053,
	///�ؼ���־����λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����Ϊ�ؼ�����
	ePARAM_KEYALARM_FLAG,	///	 			=	0x0054,
	ePARAM_MAXSPEED,	///					=	0x0055,	   	///����ٶȣ���λΪ����ÿСʱ(km/h)
	ePARAM_OVERSPEED_LASTING_TIMEOUT,	///	=	0x0056,		///���ٳ���ʱ�䣬��λΪ��
	ePARAM_CONTINUOUS_DRV_TIMEOUT,	///	=	0x0057,		///������ʻʱ�����ޣ���λΪ��
	ePARAM_DRV_TOTAL_EACHDAY_TIMEOUT,	///	=	0x0058,		///�����ۼƼ�ʻʱ�䣬��λΪ��
	ePARAM_LEAST_REST_TIMEOUT,	///		=	0x0059,		///��С��Ϣʱ�䣬��λΪ��
	ePARAM_MAX_STOPPING_TIMEOUT = 52,	///		=	0x005A,		///�ͣ��ʱ�䣬��λΪ��
	/**0X005B----0X006F */
#ifdef JTT808_Ver_2013
	ePARAM_OVERSPEED_PREALARM_DIF,	///	=	0x005B,		///���ٱ���Ԥ����ֵ����λ1/10Km/h
	ePARAM_FATIGE_PREALARM_DIF,	///		=	0x005C,		///ƣ�ͼ�ʻԤ����ֵ����λΪ�룬>0
		  ///b7-b0:��ײʱ�䣬��λ4ms��b15-b8:��ײ���ٶȣ���λ0.1g�����÷�Χ0~79��Ĭ��10
	ePARAM_COLLISION_SETTING,	///			=	0x005D,
									   ///�෭�����������ã��෭�Ƕȣ���λ1�ȣ�Ĭ��30��
	ePARAM_SIDECOLLAPSE_SETTING,	///		=	0x005E,
	/**0X005F----0X0063 */
	ePARAM_PERIODIC_PHOTO_CTRL,	///		=	0x0064,		///��ʱ���տ���
	ePARAM_PERDIST_PHOTO_CTRL = 58,	///		=	0x0065,		///�������տ���
	/**0X0066----0X006F */
#endif///JTT808_Ver_2013

	ePARAM_VIDEO_QUALITY = 59,	///		=	0x0070,		///ͼƬ/��Ƶ������1-10,1���
	ePARAM_BRIGHTNESS,	///				=	0x0071,		///���ȣ�0-255
	ePARAM_CONTRASTNESS,	///				=	0x0072,		///�Աȶȣ�0-127
	ePARAM_SATURATION,	///				=	0x0073,		///���Ͷȣ�0-127
	ePARAM_CHROMA,	///					=	0x0074,		///ɫ�ȣ�0-255
	/**0X0075----0X007F*/
	ePARAM_VEHICLE_KILOMETRES,	///		=	0x0080,		///������̱������1/10km
	ePARAM_VEHICLE_PROVINCE_ID,	///		=	0x0081,		///�������ڵ�ʡ��ID
	ePARAM_VEHICLE_CITY_ID,	///			=	0x0082,		///�������ڵ�����ID
	ePARAM_VEHICLE_ID,	/// 				=	0x0083,		///���ƺ���
	ePARAM_VEHICLE_ID_COLOR,	///			=	0x0084,		///������ɫ

#ifdef JTT808_Ver_2013
/**����Ϊ��Э�����ӵĲ�����*/
	ePARAM_GNSS_MODE = 69,	///		0x0090
	ePARAM_GNSS_BOUNDRATE,	///	0x0091
	ePARAM_GNSS_DMSG_OUTFREQ,	///	0x0092
	ePARAM_GNSS_DMSG_RETRIEVEFREQ,	///	0x0093
	ePARAM_GNSS_DMSG_RPTTYPE,	///	0x0094
	ePARAM_GNSS_DMSG_RPTSETTING,	///	0x0095
	ePARAM_CAN_CH1_RETRIEVEINTERVAL,	///	0x0100
	ePARAM_CAN_CH1_UPLOADINTERVAL,	///	0x0101
	ePARAM_CAN_CH2_RETRIEVEINTERVAL,	///	0x0102
	ePARAM_CAN_CH2_UPLOADINTERVAL,	///	0x0103
	ePARAM_CAN_SOLORETRIEVE,	///	0x0110
	ePARAM_CAN_SOLORETRIEVE_OTHER = 80,	///	0x0111
#endif

	ePARAM_END,

};


/**
 * Э��涨���豣������
 *
1�������ն˷��͵Ĺؼ�������Ϣ�����ﵽ�ش���������δ�յ�Ӧ����Ӧ������б��档�Ժ��ڷ���������
ϢǰҪ�ȷ��ͱ���Ĺؼ�������Ϣ��
2��ƽ̨ͨ�������¼�������Ϣ�����¼��б����ն˴洢����ʻԱ��������Ӧ�¼���ɽ�
���¼��б�������ѡ��ѡ����ն���ƽ̨�����¼�������Ϣ��
3��ƽ̨ͨ��������Ϣ�㲥�˵�������Ϣ������Ϣ�㲥���б����ն˴洢����ʻԱ����ͨ
���˵�ѡ��㲥/ȡ����Ӧ����Ϣ����ѡ����ն���ƽ̨������Ϣ�㲥/ȡ����Ϣ��
4��ƽ̨ͨ���������õ绰����Ϣ�����ն����õ绰��������Ϣ��Ҫ�ն˻ظ��ն�ͨ��Ӧ��
��Ϣ��
5��ƽ̨ͨ����������Բ���������þ����������ö������������·�ߵ���Ϣ������
�˽����������·����
6����ʻ��¼����
7����ý������
8������ͷͼƬ/��Ƶ
9����Ƶ����
*/

/**��������*/
typedef enum
{
///	PARAM_FACTORY_DEFAULT 	= 0,	////��������
	PARAM_TERMINALINFO 		= 1,	////�豸��Ϣ
	PARAM_SYSTEM			= 2,	////ϵͳ��Ϣ
	PARAM_RUNTIME			= 3,	////������Ϣ
	PARAM_RECTANGLEAREA		= 4,	////��������
	PARAM_ROUNDAREA			= 5,	////Բ������
	PARAM_POLYGONAREA		= 6,	////���������
	PARAM_ROUTE				= 7,	////·��
	PARAM_ONDEMANDMSGINFO	= 8,	////�㲥��Ϣ
	PARAM_EVENT,					///�¼�

	PARAM_TYPEEND,

} ePARAMTYPE;

/**��������У��ǰ׺�룬��������Ϊ�˱�����ⲿ�洢����������������ʱ�����ݺ�У����
 * ȫΪ0��������ͨ��У�飬������Ч���������*/
#define PARAM_PREFIX  0x12345678


/*

///ϵͳ�洢�ĵ绰��
#define		PHONE_NUM_MAX_LENGTH	16	///�绰���볤��
#define		PHONE_NAME_MAX_LENGTH	16	///��������
#define		PHONE_NUM_MAX_CNT		50	///�绰������Ŀ

///�绰������
typedef      struct
{
	u8		u8CodeLength;									///�绰���볤��
	u8		au8CodeNum[PHONE_NUM_MAX_LENGTH];				///�绰��������
	u8		u8NameLength;									///��������
	u8		au8Name[PHONE_NAME_MAX_LENGTH];					///��������
}tPHONENUMITEM;


///ͨ����¼��
typedef 	struct
{
	u8				u8OutDialingCnt;								///�Ѳ�������Ŀ
	tPHONENUMITEM	tOutDialPhoneNumItem[PHONE_NUM_MAX_CNT];		///�Ѳ��绰��������
	u8				u8UnListenedCnt;								///δ��������Ŀ
	tPHONENUMITEM	tUnListenedPhoneNumItem[PHONE_NUM_MAX_CNT];		///δ�ӵ绰��������
	u8				u8ListenedCnt;									///�ѽ�������Ŀ
	tPHONENUMITEM	tListenedPhoneNumItem[PHONE_NUM_MAX_CNT];		///�ѽӵ绰��������
	u8				u8CRC;											///У���
}tPHONELOGINFOType;

typedef	struct
{
	u8 	u8PhoneTag;					///�绰��־1:���룻2:������3:����/����
	tPHONENUMITEM tPhoneNumItem;
}tPHONEBOOKITEM;
///�绰���벾
typedef 	struct
{
	u8				u8Amount;										///�绰������Ŀ
	tPHONEBOOKITEM	tPhoneNumItem[PHONE_NUM_MAX_CNT];				///�绰����
	u8				u8CRC;											///У���
}tPHONEBOOKINFOType;


typedef	struct	
{
	u8 		u8PhoneNumLength;				///�绰���볤��
	u8		au8PhoneNum[20];				///�绰����
}tPHONENUMITEM;
*/


#define PLATE_SIZE	30	///�������鳤��


#define PARAM_FIX_ITEM_CNT	10	///����ϵͳ��������
#define PARAM_STR_ITEM_CNT	10	///�䳤ϵͳ�����������ַ���



/////������ͷ��
//typedef 	struct
//{
//	u32		u32Id;		///����ID
//	u8		u8Len;		///��������
//	u8		aValue[0];	///��������
//} tPARAMITEMHEAD;

/**����ϵͳ��������Ϣ�壬u8/u16/u32*/
typedef struct
{
	u32		u32Id;		///����ID
	u8		u8Len;		///��������
	union{
		u8	u8Val;
		u16	u16Val;
		u32	u32Val;
	} val;

} tPARAMITEM_FIXED;

/**�䳤��������Ϣ�壬�ַ���*/
//typedef struct
//{
//	u32		u32Id;		///����ID
//	u8		u8Len;		///��������
//	u8		str[STR_SIZE];	///�ַ���
//
//} tPARAMITEM_STR;

#if 1
/**ϵͳ���� *******************************************************************/
typedef	struct
{
/**ͨ�Ų���*/
	u32 u32LinkAliveTimeOut;	///�ն����������ͼ������λΪ��(s)
	u32 u32TCPAckTimeout;		///TCPӦ��ʱʱ�䣬��λΪ��(s)
	u32 u32TCPReSendTimes;		///TCP��Ϣ�ش�����
	u32 u32UDPAckTimeout;		///UDPӦ��ʱʱ�䣬��λΪ��(s)
	u32 u32UDPReSendTimes;		///UDP��Ϣ�ش�����
	u32 u32SMSAckTimeout;		///SMSӦ��ʱʱ�䣬��λΪ��(s)
	u32 u32SMSReSendTimes;		///SMS��Ϣ�ش�����

///������������
	u8 au8MainServerApn[STR_SIZE];	///��������APN
	u8 au8MainServerUsr[STR_SIZE];	///������������ͨ���û���
	u8 au8MainServerPswd[STR_SIZE];	///������������ͨ������
	u8 au8MainServerIP[STR_SIZE];		///��������IP��ַ,IP������
///���ݷ���������
	u8 au8ViceServerApn[STR_SIZE];	///���ݷ�����APN
	u8 au8ViceServerUsr[STR_SIZE];	///���ݷ���������ͨ���û���
	u8 au8ViceServerPswd[STR_SIZE];	///���ݷ���������ͨ������
	u8 au8ViceServerIP[STR_SIZE];		///���ݷ�����IP��ַ
	u32 u32TCPPort;	///������TCP�˿�
	u32 u32UDPPort;	///������UDP�˿�
#ifdef JTT808_Ver_2013
	u8	IC_Verify_MS_IPOrDNS[STR_SIZE];	///��·����֤IC����֤��������IP��ַ������
	u32 IC_Verify_TCP_Port;		///��·����֤IC����֤��������TCP�˿�
	u32 IC_Verify_UDP_Port;		///��·����֤IC����֤��������UDP�˿�
	u8	IC_Verify_BS_IPOrDNS[STR_SIZE];	///��·����֤IC�����ݷ�����IP��ַ���������˿�ͬ��������
#endif

/**λ�û㱨����*/
	u32	u32PosRptStrategy;	///λ�û㱨����
	u32	u32PosRptMethod;	///λ�û㱨����
	u32	u32RptIntervalDrvUnReg;	///��ʻԱδ��¼�㱨ʱ��������λ Ϊ��(s)
	u32	u32RptIntervalSleep;	///���߻㱨ʱ��������λ Ϊ��(s)
	u32	u32RptIntervalEmergency;	///�����㱨ʱ��������λ Ϊ��(s)
	u32	u32RptIntervalDefault;		///ȱʡ�㱨ʱ��������λ Ϊ��(s)
	u32	u32RptDistanceDefault;	///ȱʡ�㱨����������λ Ϊ��(m)
	u32	u32RptDistanceDrvUnReg;		///��ʻԱδ��¼�㱨����������λ Ϊ��(m)
	u32	u32RptDistanceSleep;	///���߻㱨����������λ Ϊ��(m)
	u32	u32RptDistanceEmergency;	///�����㱨����������λ Ϊ��(m)
	u32	u32InflectionSuppleAngle;	///�յ㲹���Ƕȣ���λΪ��, < 180
	u16 u16BarrierRadius;		///����Χ���뾶(�Ƿ���λ��ֵ)����λΪ��

/**�绰����*/
	u8 aMonitorPlatformPhone[STR_SIZE];	///���ƽ̨��������
	u8 aResetPhone[STR_SIZE];		///��λ�绰��������
	u8 aRecoverFactorySettingPhone[STR_SIZE];		///�ָ��������õ绰��������
	u8 aCenterSMSPhone[STR_SIZE];		///���ƽ̨��������
	u8 aRcvTerminalSMSAlarmPhone[STR_SIZE];	///�����ն�SMS������������

	u32 u32PhoneListenStratagy;		///�ն˵绰��������
	u32 u32PhoneConnTimeoutEachTime;	///ÿ��ͨ���ʱ�䣬��λΪ��(s)
	u32 u32PhoneConnTimeoutCurMonth;	///����ͨ���ʱ�䣬��λΪ��(s)

	u8 aVoiceMonitorPhone[STR_SIZE];		///�����绰��������
	u8 aMonitorPlatformPriviligPhone[STR_SIZE];	///���ƽ̨��Ȩ�绰��������

/**��������*/
	u32 u32AlarmMask;	///���������֣���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1����Ӧ����������
	u32 u32SendSMSOnAlarmCtrl;	///���������ı�SMS���أ���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ�����ı�SMS
	u32 u32CaptureOnAlarmCtrl;	///�������㿪�أ���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ����ͷ����
	u32 u32AlarmPictureStorageCtrl;		///��������洢��־����λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ�ĵ���Ƭ���д洢������ʵʱ�ϴ�
	u32 u32KeyAlarmMask;		///�ؼ���־����λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����Ϊ�ؼ�����

/**���ټ�����*/
	u32 u32SpeedMax;		///����ٶȣ���λΪkm/h
	u32 u32OverSpeedContinousTime;		///���ٳ���ʱ�䣬��λΪ��(s)
	u32 u32ContinousDriveTimeThreshold;		///������ʻʱ�����ޣ���λΪ��(s)
	u32 u32DriveTimesTodayThreshold;		///�����ۼƼ�ʻʱ�����ޣ���λΪ��(s)
	u32 u32MinRestTime;			///��С��Ϣʱ�䣬��λΪ��(s)
	u32 u32MaxStopTime;			///�ͣ��ʱ�䣬��λΪ��(s)
#ifdef JTT808_Ver_2013
	u16 u16OverSpeed_PreAlarm_Dif;	///���ٱ���Ԥ����ֵ����λ1/10Km/h
	u16 u16Fatige_PreAlarm_Dif;		///ƣ�ͼ�ʻԤ����ֵ����λΪ�룬>0
	u16 u16CollisionSetting;	/**��ײ�����������ã�b7-b0�� ��ײʱ�䣬��λ4ms��
				b15-b8����ײ���ٶȣ���λ0.1g�����÷�Χ�ڣ�0-79 ֮�䣬Ĭ��Ϊ10*/
	u16 u16SideCollapseSetting;	///�෭�����������ã��෭�Ƕȣ���λ1 �ȣ�Ĭ��Ϊ30 �ȡ�
/**0X005F----0X0063 */
	tParam_PeriodicTimeVideoCapture tPhotoCtrl_Periodic;	///��ʱ���տ���
	tParam_PeriodicDistVideoCapture tPhotoCtrl_Dist;	///�������տ���
#endif

///����ͷ��ز�������
	u32 u32PictureQuality;		///ͼƬ/��Ƶ������1-10,1���
	u32 u32PictureBrightness;	///���ȣ�0 -255
	u32 u32PictureContrastness;		///�Աȶȣ�0 -255
	u32 u32PictureSaturation;	///���Ͷȣ�0 -255
	u32 u32PictureChroma;	///ɫ�ȣ�0 -255

///����������ز�������
	u32	u32Kilometers;		///������ʻ���
	u16	u16ProviceId;		///ʡ��ID
	u16	u16CountyId;		///������ID
	u8	au8Plate[PLATE_SIZE];		///����
	u8	u8Color;		///������ɫ

#ifdef JTT808_Ver_2013
/**����Ϊ��Э�����ӵĲ�����*/
/**
GNSS ��λģʽ���������£�
bit0��0������GPS ��λ�� 1������GPS ��λ��
bit1��0�����ñ�����λ�� 1�����ñ�����λ��
bit2��0������GLONASS ��λ�� 1������GLONASS ��λ��
bit3��0������Galileo ��λ�� 1������Galileo ��λ��
*/
	u8 u8GNSS_Mode;	///	0x0090
/**
GNSS �����ʣ��������£�
0x00��4800��0x01��9600��
0x02��19200��0x03��38400��
0x04��57600��0x05��115200��
*/
	u8 u8GNSS_Baund;	///PARAM_GNSS_BOUNDRATE	0x0091
/**
GNSS ģ����ϸ��λ�������Ƶ�ʣ��������£�
0x00��500ms��0x01��1000ms��Ĭ��ֵ����
0x02��2000ms��0x03��3000ms��
0x04��4000ms��
*/
	u8 u8GNSS_DetailMsg_OutFreq;	///0x0092
/**0x0093 DWORD GNSS ģ����ϸ��λ���ݲɼ�Ƶ�ʣ���λΪ�룬Ĭ��Ϊ1��*/
	u32 u32GNSS_DetailMsg_RetrieveFreq;	///0x0093
/**
GNSS ģ����ϸ��λ�����ϴ���ʽ��
0x00�����ش洢�����ϴ���Ĭ��ֵ����
0x01����ʱ�����ϴ���
0x02�����������ϴ���
0x0B�����ۼ�ʱ���ϴ����ﵽ����ʱ����Զ�ֹͣ�ϴ���
0x0C�����ۼƾ����ϴ����ﵽ������Զ�ֹͣ�ϴ���
0x0D�����ۼ������ϴ����ﵽ�ϴ��������Զ�ֹͣ�ϴ���
*/
	u8 u8GNSS_DetailMsg_RptType;	///0x0094
/**
GNSS ģ����ϸ��λ�����ϴ����ã�
�ϴ���ʽΪ0x01 ʱ����λΪ�룻
�ϴ���ʽΪ0x02 ʱ����λΪ�ף�
�ϴ���ʽΪ0x0B ʱ����λΪ�룻
�ϴ���ʽΪ0x0C ʱ����λΪ�ף�
�ϴ���ʽΪ0x0D ʱ����λΪ����
*/
	u32 u32GNSS_DetailMsg_RptSetting;	///0x0095

/**0x0100 DWORD CAN ����ͨ��1 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�*/
	u32 u32CAN_Ch1_RetrieveInterval;	///0x0100
/**0x0101 WORD CAN ����ͨ��1 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�*/
	u16 u16CAN_Ch1_UploadInterval;	///0x0101

/**0x0102 DWORD CAN ����ͨ��2 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�*/
	u32 u32CAN_Ch2_RetrieveInterval;	///0x0102
/**0x0103 WORD CAN ����ͨ��2 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�*/
	u16 u16CAN_Ch2_UploadInterval;		///0x0103

/**
		0x0110 BYTE[8]
CAN ����ID �����ɼ����ã�
bit63-bit32 ��ʾ��ID �ɼ�ʱ����(ms)��0 ��ʾ���ɼ���
bit31 ��ʾCAN ͨ���ţ�0��CAN1��1��CAN2��
bit30 ��ʾ֡���ͣ�0����׼֡��1����չ֡��
bit29 ��ʾ���ݲɼ���ʽ��0��ԭʼ���ݣ�1���ɼ�����ļ���ֵ��
bit28-bit0 ��ʾCAN ����ID��
*/
	u8 u8CAN_SingleRetrieveSetting[8];	///0x0110

/**0x0111-0x01FF BYTE[8] ��������CAN ����ID �����ɼ�����*/
	u8 u8CAN_SingleRetrieveSetting_Others[8]; ///0x0111-0x01FF

/**0xF000-0xFFFF �û��Զ���*/

#endif

} tPARAM_SYSTEM;
#endif

/** ϵͳ�����洢�ṹ�� */
typedef struct
{
	u32	prefix;	///ǰ׺
	tPARAM_SYSTEM tSystemParam;	/**ϵͳ����*/
	u8 u8CRC;
} tPARAM_SYSTEM_STORAGE;

#define PARAM_AUTHORCODE_LEN	100  	/**�ն˼�Ȩ�볤��*/


/** �豸���� *****************************************************************/
typedef	struct
{
	tTerminalInfo_Base	tBase; /**������Ϣ*/
	u8	aPlate[PARAM_VIN_PLATE_MAXLEN];		/**V2011�� ����������;
						  V2013�����Ƴ�����ɫΪ0 ʱ����ʾ����VIN�����򣬱�ʾ����
						  ��ͨ�����Ű䷢�Ļ��������ơ�*/

#ifdef JTT808_Ver_2013
	tTerminalType tType;	/**�ն�����*/
#endif
	tGNSSModule_Property	tGNSSModuleProperty; 	/**GNSSģ������*/
	tCommModule_Property	tCommModuleProperty;	/**ͨ��ģ������*/

	u8	ver_hardware[50];	///�ն�Ӳ���汾�� STRING
	u8	ver_software[50];	///�ն�����汾�� STRING

} tPARAM_DEVICE;

/**�豸�����洢�ṹ��*/
typedef	struct
{
	u32	prefix;	///ǰ׺
	tPARAM_DEVICE	tDeviceParam; ///�豸����
	u8 u8CRC; ///У����
} tPARAM_DEVICE_STORAGE;

/** ���в��� *****************************************************************/
typedef	struct
{
	u8	u8CommMode;		///ͨѶģʽ��ȱʡΪTCP
	u8	aServerIP[4];	///������IP
	u16	u16Server_Port;		///�������˿ں�

	u8   u8ConnectionCtrl;	///���ӿ���
	char sURL[100];	///URL��ַ
	char sPPP_name[10];///���ŵ�����;
	char sPPP_user[10];///�����û���;
	char sPPP_password[10]; ///��������;
	char sIP[30];///��ַ;IP��DNS
	u16  u16TCP_port;///TCP �˿�;
	u16  u16UDP_port;///UDP �˿�;
	char sManufacture_ID[20]; ///������ID;
	char sHardware_Version[30];///Ӳ���汾;
	char sFirmware_Version[30];///�̼��汾;
	u16  u16TOUT_ServerConnect; ///���ӵ�ָ��������ʱ��,��λ��

	u8	sAuthorCode[PARAM_AUTHORCODE_LEN];	/**�ն˼�Ȩ��*/
	u8	ICCID[10];		///�ն�SIM ��ICCID BCD[10]
	u8 	u8BcdPhoneNum[6];	///�ն��ֻ���,BCD��

	tWARNINGSTATE 	tWarningState;	/**������־λ*/
	tSYSTEMSTATE 	tSysState;	/**״̬λ*/

	u16	u16UpReportInterval;	///�����ϱ�-�ϱ����ڣ���λ��
	u16	u16KeepOnTime;		///�����ϱ�-����ʱ������λ��

	u8 u8VechicalCtrlState;	///��������״̬��0=������1=����

} tPARAM_RUNTIME;

/**���в����洢�ṹ��*/
typedef	struct
{
	u32	prefix;	///ǰ׺
	tPARAM_RUNTIME	tRuntimeParam; ///���в���
	u8 u8CRC; ///У����
} tPARAM_RUNTIME_STORAGE;


#ifdef JTT808_Ver_2013
/**��������*/
typedef enum
{
	UPG_TERMINAL = 0, ///�ն�
	UPG_IC_Reader = 12, ///��·����֤IC ��������
	UPG_GNSS = 52, ///�������Ƕ�λģ��
} eUPG_TYPE;

/** ������Ϣ *****************************************************************/
typedef struct
{
	u8 upg_type;	///�������� 0���նˣ�12����·����֤IC ����������52���������Ƕ�λģ��
	u8 factoryId[5];	///������ID BYTE[5] �����̱��
	u8 version_len;		///�汾�ų��� BYTE n
	u8 version[50];		///�汾�� STRING
	u32 u32UpgPack_len;	///7+n �������ݰ����� DWORD ��λΪBYTE
	u8 result;     ///�������  0���ɹ���1��ʧ�ܣ�2��ȡ��
} tUPGRADEINFO;

/**������Ϣ�洢�ṹ��*/
typedef	struct
{
	u32	prefix;	///ǰ׺
	tUPGRADEINFO	tUpgradeInfo; ///���в���
	u8 u8CRC; ///У����
} tUPGRADEINFO_STORAGE;
#endif

/** �¼�/�㲥�˵�/�㲥��Ϣ/�绰������  ***********************************************************/

enum {
	PARAM_FORMATMSG_EVT = 0,
	PARAM_FORMATMSG_MSGOD_MENU,
	PARAM_FORMATMSG_MSGOD_MSG,
	PARAM_FORMATMSG_PHONEBOOK,

	PARAM_FORMATMSG_END,
};

///������Ϣ��EEPROM�еĴ洢�׵�ַ
#define EEPROM_ADDR_EVENT		0x3333	///�¼���Ϣ��EEPROM���׵�ַ
#define EEPROM_ADDR_MSGOD_MENU	0x3333	///�㲥�˵���Ϣ��EEPROM���׵�ַ
#define EEPROM_ADDR_MSGOD_MSG	0x3333	///�㲥��Ϣ��EEPROM���׵�ַ
#define EEPROM_ADDR_PHONEBOOK	0x3333	///�绰����EEPROM���׵�ַ

#define EEPROM_ADDR_ROUND		0x3333	///Բ��������EEPROM���׵�ַ
#define EEPROM_ADDR_RECTANGLE	0x3333	///����������EEPROM���׵�ַ
#define EEPROM_ADDR_POLYGON		0x3333	///�����������EEPROM���׵�ַ

#define EVT_MSG_SIZE			50 	///�¼�������󳤶�
#define EVT_ITEM_SIZE			50	///�¼�������
#define MSGOD_MENU_MSG_SIZE		50 	///�㲥�˵����ݳ���
#define MSGOD_MENU_ITEM_SIZE	50 	///�㲥�˵�����
#define MSGOD_MSG_MSG_SIZE		50	///�㲥��Ϣ���ݳ���
#define MSGOD_MSG_ITEM_SIZE		50	///�㲥��Ϣ����
#define PHONEBOOK_MSG_SIZE		50	///�绰��������¼����
#define PHONEBOOK_ITEM_SIZE		50	///�绰����¼������

#define ROUND_AREA_ITEM_SIZE	50	///Բ������������
#define RECT_AREA_ITEM_SIZE		50	///��������������
#define POLYGON_AREA_ITEM_SIZE	50	///���������������

///�¼�������
typedef struct
{
	u8	u8Id;	///�¼�ID,���ն�����ͬID���¼����򱻸���
	u8	u8Len;	///�¼���Ϣʵ�ʳ���
	u16 u16Offset;	///�¼���Ϣƫ�Ƶ�ַ�����������ͷ
	u8  u8CRC;		///�¼�����CRC
} tITEMINDEX;

///�¼���Ϣ�洢ͷ
typedef struct
{
	u8 u8Amount;	///�¼�����
	tITEMINDEX index[EVT_ITEM_SIZE];	///�¼���������
} tEVT_HEADINFO;

///�¼���Ϣ�洢��ʽ
typedef struct
{
	u32	prefix;	///ǰ׺
	tEVT_HEADINFO	headInfo;
	u8 u8CRC; ///У����
} tPARAM_EVT_HEADINFO_STORAGE;

///�㲥�˵��洢ͷ
typedef struct
{
	u8 u8Amount;	///�¼�����
	tITEMINDEX index[MSGOD_MENU_ITEM_SIZE];	///�¼���������
} tMSGODMENU_HEADINFO;

///�㲥�˵��洢��ʽ
typedef struct
{
	u32	prefix;	///ǰ׺
	tMSGODMENU_HEADINFO	headInfo;
	u8 u8CRC; ///У����
} tPARAM_MSGOD_MENU_HEADINFO_STORAGE;

///�㲥��Ϣ�洢ͷ
typedef struct
{
	u8 u8Amount;	///�¼�����
	tITEMINDEX index[MSGOD_MSG_ITEM_SIZE];	///�¼���������
} tMSGODMSG_HEADINFO;

///�㲥��Ϣ�洢��ʽ
typedef struct
{
	u32	prefix;	///ǰ׺
	tMSGODMSG_HEADINFO	headInfo;
	u8 u8CRC; ///У����
} tPARAM_MSGOD_MSG_HEADINFO_STORAGE;

///�绰���洢ͷ
typedef struct
{
	u8 u8Amount;	///�绰������
	tITEMINDEX index[MSGOD_MSG_ITEM_SIZE];	///��������
} tPHONEBOOK_HEADINFO;

///�绰���洢��ʽ
typedef struct
{
	u32	prefix;	///ǰ׺
	tMSGODMSG_HEADINFO	headInfo;
	u8 u8CRC; ///У����
} tPARAM_PHONEBOOK_HEADINFO_STORAGE;

/** �绰�� **/
#define		PHONE_NUM_SIZE		16		///�绰���볤��
#define		PHONE_NAME_SIZE		0xFF	///��������
#define		PHONE_ITEM_SIZE		50		///�绰������Ŀ

///�绰������
typedef struct
{
	u8	u8Flag;		///1�����룻2��������3������/����
	u8	u8PhoneLen;		///�绰���볤��
	u8	aPhoneNum[PHONE_NUM_SIZE];	///�绰��������
	u8	u8NameLen;		///��������
	u8	aName[PHONE_NAME_SIZE];		///��������
}tPHONEBOOK_ITEM;


/** Բ������ ****************************************************************/
#define PARAM_ROUNDAREA_MAXITEMS	24 ////Բ�������������
/**Բ����������չ�ṹ�壬�洢��ʽ����������Ч��־*/
typedef	struct
{
	u8  u8IsValid;	///��Ч��־��0����Ч��1����Ч��Ĭ����Ч�����Ӵ��ֶ�������������Ƿ�ɾ��
	tItem_RoundArea tItem;
} tItem_RoundArea_Ext;

typedef	struct
{
//	u8	u8AreaProperty;		///Բ����������,eAREATYPE
	u8	u8Amount;	///��������
	tItem_RoundArea_Ext atItems[PARAM_ROUNDAREA_MAXITEMS];	///������
} tPARAM_ROUNDAREA;

/**Բ������洢��ʽ*/
typedef struct
{
	u32	prefix;	///ǰ׺
	tPARAM_ROUNDAREA	tRoundArea;
	u8 u8CRC; ///У����
} tPARAM_ROUNDAREA_STORAGE;

/** �������� ****************************************************************/
#define PARAM_RECTAREA_MAXITEMS	24 ///�����Ŀ����

/**��������չ�ṹ�壬�洢��ʽ����������Ч��־*/
typedef	struct
{
	u8  u8IsValid;	///��Ч��־��0����Ч��1����Ч��Ĭ����Ч�����Ӵ��ֶ�������������Ƿ�ɾ��
	tItem_RectangleArea tItem;
} tItem_RectArea_Ext;

typedef	struct
{
//	u8	u8AreaProperty;		///���þ�����������
	u8	u8AreaTotalCount;	///��������
	tItem_RectArea_Ext	atItems[PARAM_RECTAREA_MAXITEMS];
} tPARAM_RECTAREA;

/***��������洢��ʽ*/
typedef struct
{
	u32	prefix;	///ǰ׺
	tPARAM_RECTAREA	tRectArea;
	u8 u8CRC; ///У����
} tPARAM_RECTAREA_STORAGE;

/** ��������� **************************************************************/
#define PARAM_POLYGONAREA_ITEMS_MAX	24 ///�����Ŀ����
#define PARAM_POLYGONAREA_POINT_MAX	40 ///����ζ�����

/**����չ�ṹ�壬�洢��ʽ����������Ч��־*/
typedef	struct
{
	u8  u8IsValid;	///��Ч��־��0����Ч��1����Ч��Ĭ����Ч�����Ӵ��ֶ�������������Ƿ�ɾ��
	tItem_PolygonArea tHead;
	tPOINT	points[PARAM_POLYGONAREA_POINT_MAX];	///������
} tItem_Polygon_Ex;

typedef struct
{
	u8	u8Amount;
	tItem_Polygon_Ex	atItems[PARAM_POLYGONAREA_ITEMS_MAX];
} tPARAM_POLYGONAREA;

/**���������洢��ʽ*/
typedef struct
{
	u32	prefix;	///ǰ׺
	tPARAM_POLYGONAREA	tPolygonArea;
	u8 u8CRC; ///У����
} tPARAM_POLYGONAREA_STORAGE;

/** ·�� **************************************************************/
#define PARAM_ROUTE_POINT_MAX	40
#define PARAM_ROUTE_ITEM_MAX	24

typedef	struct
{
	u32	u32LineId;		///·��ID
	u16	u16LineProperty;	///·������
	tBCDTime tBcdStartTime;	///��ʼʱ�䣬����������0λΪ0��û�и��ֶ�
	tBCDTime tBcdEndTime;	///����ʱ�䣬����������0λΪ0��û�и��ֶ�
	u16	u16InflectionAmount;	///·���ܹյ���
	tCORNERPOINT ptItem[PARAM_ROUTE_POINT_MAX]; 	///�յ���
}tITEM_CORNERPOINT;

/**����չ�ṹ�壬�洢��ʽ����������Ч��־*/
typedef	struct
{
	u8  u8IsValid;	///��Ч��־��0����Ч��1����Ч��Ĭ����Ч�����Ӵ��ֶ�������������Ƿ�ɾ��
	tITEM_CORNERPOINT	point;	///������
} tITEM_CORNERPOINT_Ex;

typedef struct
{
	u8	u8Amount;
	tITEM_CORNERPOINT_Ex	atItems[PARAM_ROUTE_ITEM_MAX];
} tPARAM_ROUTE;

/**���������洢��ʽ*/
typedef struct
{
	u32	prefix;	///ǰ׺
	tPARAM_ROUTE	tRoute;
	u8 u8CRC; ///У����
} tPARAM_ROUTE_STORAGE;


/** ��ʻԱ��Ϣ **********************************************************************/
///������Ӧ�ļ�ʻԱ��Ϣ
typedef	struct
{
	u8 au8DriverId[8];			///��ʻԱ���룬��IC���ж�ȡ�����ߴ�ƽ�·�����
	u8 u8DriverNameLength;		///��ʻԱ��������
	u8 au8DriveName[20];		///��ʻԱ����
	u8 au8DriverIdCode[20];		///��ʻԱ���֤����
	u8 au8QulifiedCode[40];		///��ʻԱ��ҵ�ʸ�֤����
	u8 u8GovNameLength;			///��֤�������Ƴ���
	u8 au8GovName[20];			///��֤��������
}tDRIVERINFO;


///һ������Ӧ������ʻԱ��Ŀ
#define	ONE_VEHICLE_DRIVER_NUNBER_MAX		6
typedef	struct
{
	u8 	u8CurDriver;		///��ǰ��ʻԱ 0xFF��ʾ��Ч
	u8 	u8DriverCnt;		///��Ч��ʻԱ����
	tDRIVERINFO	 tDriverInfo[ONE_VEHICLE_DRIVER_NUNBER_MAX]; 	///��ʻԱ�б�
	u8 	u8CRC;
}tVEHICLEDRIVERType;

/**
 * �ָ�����������recovery system default setting from EEPROM
 *
 * @return 	int��0=�ɹ���-1=ʧ��
 */
int RecoverSystemDefaultParam(void);

/**
 * @brief	��������ϵͳ����
 * @return	int��0=�ɹ���-1=ʧ��
 */
int Load_CommonParams(void);

/**
 * @brief	��������ϵͳ����
 * @return	int��0=�ɹ���-1=ʧ��
 */
int Save_CommonParams(void);

/**
 * @brief	��ȡָ�����Ͳ���
 * @param 	eType	��������
 * @return	int��0=�ɹ���-1=ʧ��
 */
int Load_Params(ePARAMTYPE eType);

/**
 * @brief	����ָ�����Ͳ���
 * @param 	eType ��������
 * @return	int��0=�ɹ���-1=ʧ��
 */
int Save_Params(ePARAMTYPE eType);

/**
 * ����ϵͳ����
 * @param id	��������
 * @param len	��������
 * @param pVal	����ֵ
 * @return	0=�ɹ���-1=ʧ��
 */
int Set_SystemParam(u32 id, u8 len, u8 *pVal);

/**
 * ��ȡ�㲥��Ϣ����EEPROM����
 *
 * @return
 */
int PARAM_LoadMsgOD();

/**
 * ����㲥��Ϣ��д��EEPROM
 *
 * @return
 */
int PARAM_SaveMsgOD();


extern tPARAM_SYSTEM *ptParam_System;
extern tPARAM_DEVICE *ptParam_Device;
extern tPARAM_RUNTIME *ptParam_Runtime;
extern tPARAM_RECTAREA *ptParam_Rect;
extern tPARAM_ROUNDAREA *ptParam_Round;
extern tPARAM_SYSTEM *ptParam_System;
//extern tPARAM_EVT_HEADINFO *ptParam_EvtInfo;
//extern tPARAM_MSGOD_MENUINDEX *ptParam_MsgOD_MenuIndex;
//extern tPARAM_MSGOD_MSGINDEX *ptParam_MsgOD_MsgIndex;


/** �¼�/�㲥�˵�/�㲥��Ϣ/�绰���ⲿ�ӿ� *******************************************/
int PARAM_FormatMsg_GetHead(u8 type);
int PARAM_FormatMsg_SaveHead(u8 type);
int PARAM_FormatMsg_GetAmount(u8 type);
int PARAM_FormatMsg_GetItem(u8 type, u8 u8ItemIndex, u8* pu8ItemId, u8 *pu8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_ApendItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_ReplaceItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_EraseAll(u8 type);

/** Բ��/����/�����/�ⲿ�ӿ� ******************************************************/
int PARAM_Area_Load(u8 type);
int PARAM_Area_EraseAll(u8 type);
int PARAM_Area_Save(u8 type);
int PARAM_Area_AppendItem(u8 type, u32 u32Id, u8 *pItem, u16 u16ItemLen);
int PARAM_Area_ReplaceItem(u8 type, u32 u32Id, u8 *pItem, u16 u16ItemLen);
int PARAM_Area_DeleteItem(u8 type, u32 u32Id);

u16 GetItem_AllSystemParam(u8 *pData);



#pragma pack()
#endif   ///_GPS_PARAMETER_H_
