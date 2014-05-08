/**
 * MSGID.h
 *
 *  Created on: 2013-12-24
 *      Author: gaozhengdong
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stm32f2xx.h>


/**
 * ˵��:
 * 1)��˸�ʽ
 * 2)����ʹ���¾ɹ������
 */

/**���뿪�أ������¾�Э��֧�֣�����ͬʱֻ�ܴ�һ��*/
///#define JTT808_Ver_2011	///���뿪�أ���ʹ�ܾɹ���Э��
#define JTT808_Ver_2013	///�¹���Э��

#pragma pack(1)


/**************************** Э������������ *********************************/
#define	MSGID_T_GeneralAck				0x0001///�ն�ͨ��Ӧ����Ϣ
#define	MSGID_S_GeneralAck				0x8001///ƽ̨ͨ��Ӧ����Ϣ
#ifdef JTT808_Ver_2013
#define	MSGID_S_ReloadSubPack			0x8003///Ver2013,///�����ְ�����
#endif///JTT808_Ver_2013
#define	MSGID_T_Heatbeat				0x0002///�ն�����
#define	MSGID_T_Register				0x0100///�ն�ע��
#define	MSGID_S_RegisterAck				0x8100///�ն�ע��Ӧ��
#define	MSGID_T_Unregister				0x0003///�ն�ע��
#define	MSGID_T_Authentication			0x0102///�ն˼�Ȩ
#define	MSGID_S_SetParam				0x8103///�����ն˲���
#define	MSGID_S_QueryParam				0x8104///��ѯ�ն˲���
#ifdef JTT808_Ver_2013
#define	MSGID_T_QuerySpecifyParam		0x8106///��ѯָ���ն˲���
#endif///JTT808_Ver_2013
#define	MSGID_T_QueryParamAck			0x0104///��ѯ�ն˲���Ӧ��
#define	MSGID_S_Control					0x8105///�ն˿���
#ifdef JTT808_Ver_2013
#define	MSGID_S_QueryProperty			0x8107///��ѯ�ն�����
#define	MSGID_T_QueryPropertyAck		0x0107///��ѯ�ն�����Ӧ��
#define	MSGID_T_DispathUpgPack			0x8108///�·��ն�������
#define	MSGID_T_UpgrageResult			0x0108///�ն��������֪ͨ
#endif///JTT808_Ver_2013
#define	MSGID_T_RptPositionInfo			0x0200///λ����Ϣ�㱨
#define	MSGID_S_QueryPosition			0x8201///λ����Ϣ��ѯ
#define	MSGID_T_QueryPositionAck		0x0201///λ����Ϣ��ѯӦ��
#define	MSGID_S_Tracking				0x8202///��ʱλ�ø��ٿ���
#ifdef JTT808_Ver_2013
#define	MSGID_S_ConfirmWarningInfo		0x8203///�˹�ȷ�ϱ�����Ϣ
#endif///JTT808_Ver_2013
#define	MSGID_S_DownloadTxtInfo			0x8300///�ı���Ϣ�·�
#define	MSGID_S_SetEvent				0x8301///�¼�����
#define	MSGID_T_RptEvent				0x0301///�¼�����
#define	MSGID_S_AskQuestion				0x8302///�����·�
#define	MSGID_T_QuestionAck				0x0302///����Ӧ��
#define	MSGID_S_SetInfoOnDemandMenu		0x8303///��Ϣ�㲥�˵�����
#define	MSGID_T_OrderInfoCtrl			0x0303///��Ϣ�㲥/ȡ��
#define	MSGID_S_InfoService				0x8304///��Ϣ����
#define	MSGID_S_Redial					0x8400///�绰�ز�
#define	MSGID_S_SetPhoneBook			0x8401///���õ绰��
#define	MSGID_S_VechicleCtrl			0x8500///��������
#define	MSGID_T_VechicleCtrlAck			0x0500///��������Ӧ��
#define	MSGID_S_SetRoundErea			0x8600///����Բ������
#define	MSGID_S_DelRoundErea			0x8601///ɾ��Բ������
#define	MSGID_S_SetRectangleErea		0x8602///���þ�������
#define	MSGID_S_DelRectangleErea		0x8603///ɾ����������
#define	MSGID_S_SetPolygonErea			0x8604///���ö��������
#define	MSGID_S_DelPolygonErea			0x8605///ɾ�����������
#define	MSGID_S_SetPathline				0x8606///����·��
#define	MSGID_S_DelPathline				0x8607///ɾ��·��
#define	MSGID_S_RecorderCapture			0x8700///��ʻ��¼�����ݲɼ�����
#define	MSGID_T_RecorderDataUpload		0x0700///��ʻ��¼�������ϴ�
#define	MSGID_S_RecorderParamDownload	0x8701///��ʻ��¼�����´�����
#define	MSGID_T_EBillRpt				0x0701///�����˵��ϱ�
#define	MSGID_S_RequestDriverInfo		0x8702///�ϱ���ʻ����Ϣ
#define	MSGID_T_DriverInfoRpt			0x0702///��ʻԱ�����Ϣ�ɼ��ϱ�
#ifdef JTT808_Ver_2013
#define	MSGID_T_PositionInfoBatchRpt	0x0704///��λ���������ϴ�
#define	MSGID_T_CANDataUpload			0x0705///CAN���������ϴ�
#endif///JTT808_Ver_2013
#define	MSGID_T_MediaEventInfoRpt		0x0800///��ý���¼���Ϣ�ϴ�
#define	MSGID_T_MediaDataRpt			0x0801///��ý�������ϴ�
#define	MSGID_S_MediaDataRptAck			0x8800///��ý�������ϴ�Ӧ��
#define	MSGID_S_TakePhoto				0x8801///����ͷ������������
#ifdef JTT808_Ver_2013
#define	MSGID_S_TakePhotoAck			0x0805///����ͷ������������Ӧ��
#endif///JTT808_Ver_2013
#define	MSGID_S_StoredMediaQuery		0x8802///�洢��ý�����ݼ�������
#define	MSGID_T_StoredMediaQueryAck		0x0802///�洢��ý�����ݼ���Ӧ������
#define	MSGID_S_StoredMediaRpt			0x8803///�洢��ý�������ϴ�����
#define	MSGID_S_StartAudioRecord		0x8804///¼����ʼ����
#define MSGID_S_SingleMediaRetrieve		0x8805///8.60 �����洢��ý�����ݼ����ϴ�����
#define	MSGID_S_TransparentTransfer		0x8900///��������͸������
#define	MSGID_T_TransparentTransfer		0x0900///��������͸������
#define	MSGID_T_DataCompressRpt			0x0901///����ѹ���ϱ�
#define	MSGID_S_PlatformRASKey			0x8A00///ƽ̨RAS��Կ
#define	MSGID_T_RASKey					0x0A00///�ն�RAS��Կ

///��Ϣ��ʽ
///typedef struct
///{
///	u8	flag;
///
///	u8	flag;
///
///};

///��Ϣ��ʶλ����
#define		MSG_FLAG_BYTE		0x7E
#define		MSG_TRANS_BYTE		0x7D

#define ENCRYPT_FLAG	0	///���ܱ�־��1=���ݼ���, 0=���ݲ�����


///��֯��Ϣ���Ը�ʽ
#define	FORM_MSG_PROPERTY(len, rsa, multipack) 	(((len) & 0x3FF) |		    \
												((rsa == 0) ? 0 : 0x0400) |	\
												((multipack == 0) ? 0 : 0x2000))
/**��Ϣ������*/
typedef struct
{
	u16 msg_len			:	10;		///��Ϣ�峤��
	u16 msg_encrypt		:	3;		///��Ϣ�����
	u16 msg_subpack		:	1;		///��Ϣ��ְ�
	u16 msg_reserved	:	2;

} MSG_PROPERTY;


/**ȡ��Ϣͷ�ְ���־*/
//#define GET_MULTIPACK_FLAG(prop)	(prop & 0x2000)

///��Ϣͷ����
typedef	struct
{
	u8	u8Flag;					///��ʶ
	u16	u16MsgId;				///��ϢID
//	u16	u16MsgProperty;			///��Ϣ����
	MSG_PROPERTY tMsgProperty;	///��Ϣ����
	u8 	u8BcdPhoneNum[6];		///�ն��ֻ���
	u16 u16MsgCircularId;		///��Ϣ��ˮ��
}tMSGHEAD;

/////�зְ���Ϣͷ����
//typedef		struct
//{
//	u16	u16MsgId;	///��ϢID
//	u16	u16MsgProperty;		///��Ϣ����
//	u8 	u8BcdTerminalPhoneNum[6];	///�ն��ֻ���
//	u16 u16MsgCircularId;	///��Ϣ��ˮ��
//	u16 u16MsgTotalPackageNum;	///��Ϣ�ܰ���
//	u16 u16MsgPackageSerial;	///��Ϣ����� ��*1*��ʼ��ע�⣡����
//}tMSGHEAD_PACKED;

//�зְ�ʱ�ĸ�����
typedef struct
{
	u16 u16MsgTotalPackageNum;	///��Ϣ�ܰ���
	u16 u16MsgPackageSerial;	///��Ϣ����� ��*1*��ʼ��ע�⣡����
} tMSGHEAD_PACKED;

typedef		enum
{
	RET_SUCCESS	= 0,		///�ɹ�/ȷ��
	RET_FAIL = 1,			///ʧ��
	RET_ERROR = 2,			///��Ϣ����
	RET_NOTSUPPORT = 3		///��֧��
}eRESULT;

typedef		enum
{
	REG_SUCCESS			= 0,	///�ɹ�/ȷ��
	REG_VECHICLEREGED	= 1,	///�����Ѿ�ע��
	REG_NOVEHICLE		= 2,	///���ݿ����޴˳���
	REG_TERMINALREGED	= 3,	///�ն��Ѿ�ע��
	REG_NOTERMINAL		= 4,	///���ݿ����޴��ն�
} eREGRESULT;

/** 8.1�ն�ͨ��Ӧ��, ID=0x0001 ***********************************************/
typedef	struct
{
	u16	u16ResponsePackId;	///��Ӧ��ƽ̨��Ϣ����ˮ��
	u16	u16CmdId;	///��Ӧ��ƽ̨��Ϣ��ID
	eRESULT	eResult;	///���
} tMsg_T_GeneralAck;

/** 8.2 ƽ̨ͨ��Ӧ��,ID=0x8001 ***********************************************/
typedef	struct
{
	u16	u16RespSeqId;	///��Ӧ���ն���Ϣ����ˮ��
	u16	u16RespMsgId;	///��Ӧ���ն���Ϣ��ID
	eRESULT	eResult;	///���
} tMsg_S_GeneralAck;

/** 8.3 �ն�����,��ϢID��0x0002 **********************************************/
/**�ն�����������Ϣ��Ϊ�ա�*/


#ifdef JTT808_Ver_2013
/** 8.4 �����ְ�����,��ϢID��0x8003��******************************************/
typedef struct
{
	u16 u16OriginalMsgSeq;	///ԭʼ��Ϣ��ˮ��
	u8	u8ReloadPackCnt;	///�ش�������
	u8* pu8PackIds;			///�ش���ID�б�
} tMsg_S_ReloadSubPack;
#endif///JTT808_Ver_2013

#define PARAM_VIN_PLATE_MAXLEN		40	/**VIN�������󳤶�*/

/**�ն˻�����Ϣ*/
typedef struct
{
	u16	u16ProviceId;		///ʡ��ID
	u16	u16CountyId;		///������ID
	u8	au8FactoryId[5];	///������ID
#ifdef JTT808_Ver_2011
	u8	aType[8];	///�ն��ͺţ�8���ֽڣ����ն��ͺ������������ж��壬λ����	��ʱ�����ո�
	u8	aTId[7];		///�ն�ID,7 ���ֽڣ��ɴ�д��ĸ��������ɣ����ն�ID �����������ж��壬λ������ʱ���󲹡�0X00����
	u8	u8PlateColor;		///������ɫ������JT/T415-2006 ��5.4.12
#endif
#ifdef JTT808_Ver_2013
	u8	aType[20];///�ն��ͺ�20 ���ֽڣ����ն��ͺ������������ж��壬λ������ʱ���󲹡�0X00����
	u8	aTId[7];		///�ն�ID
	u8	u8PlateColor;		///������ɫ������JT/T415-2006 ��5.4.12��δ����ʱ��ȡֵΪ0��
#endif///JTT808_Ver_2013
} tTerminalInfo_Base;

/** 8.5 �ն�ע�� ��ϢID��0x0100��**************************************************/
///�ն�ע����Ϣ�ṹ�嶨��
typedef	struct
{
	tTerminalInfo_Base	tBase;
	u8	aPlate[0];		/**V2011�� ����������;
						  V2013�����Ƴ�����ɫΪ0 ʱ����ʾ����VIN�����򣬱�ʾ����
						  ��ͨ�����Ű䷢�Ļ��������ơ�*/
} tMsg_T_Register;


/** 8.6 �ն�ע��Ӧ����Ϣ�ṹ�嶨�� ID=8100��************************************/
typedef	struct
{
	u16	u16SAckSeqNum;		///��Ӧ��ƽ̨��Ϣ����ˮ��
	eREGRESULT	eRet;	///���
	u8	aAuthorCode[0];	///��Ȩ��
} tMsg_S_Ack_Register;

/** 8.7 �ն�ע�� ID=0003��*****************************************************/
/**�ն�ע����Ϣ��Ϊ�ա�*/

/** 8.8 �ն˼�Ȩ  ID=0102��****************************************************/
/**�ն��������ϱ���Ȩ��*/
typedef	struct
{
	u8	aAuthorCode[0];	///��Ȩ��
} tMsg_T_RptAuthorCode;

/**���ڸ�ʽ*/
typedef	struct
{
	u8	u8BcdYear;
	u8	u8BcdMonth;
	u8	u8BcdDay;
	u8	u8BcdHour;
	u8	u8BcdMinute;
	u8	u8BcdSecond;
}tBCDTime;


#if 1
///������ID����
#define	PARAM_ALIVE_INTERVAL			0x0001		///�ն��������ͼ������λΪs
#define	PARAM_TCPACK_TIMEOUT			0x0002		///TCPӦ��ʱ����λΪs
#define	PARAM_TCP_RESENDTIMES			0x0003		///TCP��Ϣ�ش�����
#define	PARAM_UDPACK_TIMEOUT			0x0004		///UDPӦ��ʱ����λΪs
#define	PARAM_UDP_RESENDTIMES			0x0005		///UDP��Ϣ�ش�����
#define	PARAM_SMSACK_TIMEOUT			0x0006		///SMSӦ��ʱ����λΪs
#define	PARAM_SMS_RESENDTIMES			0x0007		///SMS��Ϣ�ش�����
/**0X0008 ----0X000F ����*/
#define	PARAM_MAINSRV_APN				0x0010		///��������APN������ͨ�Ų��ŷ��ʵ�
#define	PARAM_MAINSRV_USR				0x0011		///������������ͨ�Ų����û���
#define	PARAM_MAINSRV_PSWD				0x0012		///������������ͨ�Ų�������
#define	PARAM_MAINSRV_IP				0x0013		///����������ַ��IP������
#define	PARAM_VICESRV_APN				0x0014		///���ݷ�����APN������ͨ�Ų��ŷ��ʵ�
#define	PARAM_VICESRV_USR				0x0015		///���ݷ���������ͨ�Ų����û���
#define	PARAM_VICESRV_PSWD				0x0016		///���ݷ���������ͨ�Ų�������
#define	PARAM_VICESRV_IP				0x0017		///���ݷ�������ַ��IP������
#define	PARAM_TCP_PORT					0x0018		///TCP�˿�
#define	PARAM_UDP_PORT					0x0019		///UDP�˿�
/**0X001A ----0X001F ����*/
#ifdef JTT808_Ver_2013	///�°�����Э�鲿��
#define	PARAM_ICVERIFY_MS_IPOrDNS		0x001A		///��·����֤IC����֤��������IP��ַ������
#define	PARAM_ICVERIFY_TCPPORT			0x001B		///��·����֤IC����֤��������TCP�˿�
#define	PARAM_ICVERIFY_UDPPORT			0x001C		///��·����֤IC����֤��������UDP�˿�
#define	PARAM_ICVERIFY_BS_UDPPORT		0x001D		///��·����֤IC�����ݷ�����IP��ַ���������˿�ͬ��������
#endif///JTT808_Ver_2013

					 /**�ն�λ�û㱨���ԣ�0:��ʱ�㱨��1:����㱨��2:��ʱ�Ͷ���㱨*/
#define	PARAM_POSRPT_STRATEGE		0x0020
			/**�ն˻㱨������0:����ACC״̬��1:���ݵ�¼״̬��ACC״̬�����жϵ�¼״̬��
			 	 	 	 	 	 	 	 	 	 	 	 	����¼�ٸ���ACC״̬*/
#define	PARAM_POSRPT_METHOD				0x0021
#define	PARAM_DRV_UNREG_RPTPERIOD		0x0022		///��ʻԱδ��¼�㱨ʱ��������λΪ��
/**0X0023 ----0X0026 ����*/
#define	PARAM_RPTPERIOD_ONSLEEP			0x0027		///����ʱ�㱨ʱ��������λΪ��
#define	PARAM_RPTPERIOD_URGENT			0x0028		///��������ʱ�㱨ʱ��������λΪ��
#define	PARAM_RPTPERIOD_DEFAULT			0x0029		///ȱʡʱ�㱨ʱ��������λΪ��
/**0X002A ----0X002B ����*/
#define	PARAM_RPTDIST_DEFAULT			0x002C		///ȱʡʱ����㱨�������λΪ��
#define	PARAM_RPTDIST_DRV_UNREG			0x002D		///��ʻԱδ��¼����㱨�������λΪ��
#define	PARAM_RPTDIST_SLEEP				0x002E		///����ʱ����㱨�������λΪ��
#define	PARAM_RPTDIST_URGENT			0x002F		///��������ʱ����㱨�������λΪ��
#define	PARAM_INFLECTION_POINT_ANGLE	0x0030		///�յ㲹���Ƕȣ���λΪ��(< 180)
#define	PARAM_BARRIER_RADIUS			0x0031		///����Χ���뾶(�Ƿ���λ��ֵ)����λΪ��
/**0X0032----0X003F ����*/
#define	PARAM_MONITORCENTER_PHONENUM	0x0040		///���ƽ̨�绰����
#define	PARAM_RESET_PHONENUM	0x0041		///��λ�绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˸�λ
			  ///�ظ��������õ绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˻ָ���������
#define	PARAM_RECOVERFACTORY_PHONENUM	0x0042
#define	PARAM_MONITORCENTER_SMS_NUM		0x0043		///���ƽ̨SMS�绰����
#define	PARAM_RECVTXTALARM_SMS_NUM		0x0044		///�����ն�SMS�ı������绰����
				///�ն˵绰�������ԣ�0:�Զ���ͨ��1:ACC ONʱ�Զ���ͨ��OFFʱ�ֶ�����
#define	PARAM_ANSWERCALLING_STRATEGE	0x0045
		 ///�ն�ÿ��ͨ��ʱ�ͨ��ʱ�䣬��λΪ�룬0Ϊ������ͨ����0xffffffffΪ������
#define	PARAM_TOUT_PERCALL				0x0046
				///�ն�ÿ���ͨ��ʱ�䣬��λΪ�룬0Ϊ������ͨ����0xffffffffΪ������
#define	PARAM_TOUT_PERMONTHCALL			0x0047
#define	PARAM_MONITORLISTEN_PHONENUM	0x0048			///�����绰����
#define	PARAM_SUPERVISE_SMS_NUM			0x0049			///���ƽ̨��Ȩ���ź���
/**0X004A----0X004F ����*/

///���������֣���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����������
#define	PARAM_ALARM_MASK				0x0050
///���������ı�SMS���أ���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ�����ı�SMS
#define	PARAM_SMSCTRL_ONALARM			0x0051
///�������㿪�أ���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ����ͷ����
#define	PARAM_PICCTRL_ONALARM			0x0052
///��������洢��־����λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ�ĵ���Ƭ���д洢������ʵʱ�ϴ�
#define	PARAM_PICCTRL_STORE_ONALARM		0x0053
///�ؼ���־����λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����Ϊ�ؼ�����
#define	PARAM_KEYALARM_FLAG	 			0x0054
#define	PARAM_MAXSPEED					0x0055	   ///����ٶȣ���λΪ����ÿСʱ(km/h)
#define	PARAM_OVERSPEED_LASTING_TIMEOUT		0x0056	///���ٳ���ʱ�䣬��λΪ��
#define	PARAM_CONTINUOUS_DRV_TIMEOUT		0x0057	///������ʻʱ�����ޣ���λΪ��
#define	PARAM_DRV_TOTAL_EACHDAY_TIMEOUT		0x0058	///�����ۼƼ�ʻʱ�䣬��λΪ��
#define	PARAM_LEAST_REST_TIMEOUT			0x0059	///��С��Ϣʱ�䣬��λΪ��
#define	PARAM_MAX_STOPPING_TIMEOUT			0x005A	///�ͣ��ʱ�䣬��λΪ��
/**0X005B----0X006F */
#ifdef JTT808_Ver_2013
#define	PARAM_OVERSPEED_PREALARM_DIF	0x005B	///���ٱ���Ԥ����ֵ����λ1/10Km/h
#define	PARAM_FATIGE_PREALARM_DIF		0x005C	///ƣ�ͼ�ʻԤ����ֵ����λΪ�룬>0
	  ///b7-b0:��ײʱ�䣬��λ4ms��b15-b8:��ײ���ٶȣ���λ0.1g�����÷�Χ0~79��Ĭ��10
#define	PARAM_COLLISION_SETTING			0x005D
								   ///�෭�����������ã��෭�Ƕȣ���λ1�ȣ�Ĭ��30��
#define	PARAM_SIDECOLLAPSE_SETTING		0x005E
/**0X005F----0X0063 */
#define	PARAM_PERIODIC_PHOTO_CTRL		0x0064	///��ʱ���տ���
#define	PARAM_PERDIST_PHOTO_CTRL		0x0065	///�������տ���
/**0X0066----0X006F */
#endif///JTT808_Ver_2013

#define	PARAM_VIDEO_QUALITY				0x0070		///ͼƬ/��Ƶ������1-10,1���
#define	PARAM_BRIGHTNESS				0x0071		///���ȣ�0-255
#define	PARAM_CONTRASTNESS				0x0072		///�Աȶȣ�0-127
#define	PARAM_SATURATION				0x0073		///���Ͷȣ�0-127
#define	PARAM_CHROMA					0x0074		///ɫ�ȣ�0-255
/**0X0075----0X007F*/
#define	PARAM_VEHICLE_KILOMETRES		0x0080		///������̱������1/10km
#define	PARAM_VEHICLE_PROVINCE_ID		0x0081		///�������ڵ�ʡ��ID
#define	PARAM_VEHICLE_CITY_ID			0x0082		///�������ڵ�����ID
#define	PARAM_VEHICLE_ID 				0x0083		///���ƺ���
#define	PARAM_VEHICLE_ID_COLOR			0x0084		///������ɫ
#endif

#ifdef JTT808_Ver_2013
/**����Ϊ��Э�����ӵĲ�����*/
/**
GNSS ��λģʽ���������£�
bit0��0������GPS ��λ�� 1������GPS ��λ��
bit1��0�����ñ�����λ�� 1�����ñ�����λ��
bit2��0������GLONASS ��λ�� 1������GLONASS ��λ��
bit3��0������Galileo ��λ�� 1������Galileo ��λ��
*/
#define PARAM_GNSS_MODE		0x0090
/**
GNSS �����ʣ��������£�
0x00��4800��0x01��9600��
0x02��19200��0x03��38400��
0x04��57600��0x05��115200��
*/
#define PARAM_GNSS_BOUNDRATE	0x0091
/**
GNSS ģ����ϸ��λ�������Ƶ�ʣ��������£�
0x00��500ms��0x01��1000ms��Ĭ��ֵ����
0x02��2000ms��0x03��3000ms��
0x04��4000ms��
*/
#define PARAM_GNSS_DMSG_OUTFREQ	0x0092
/**0x0093 DWORD GNSS ģ����ϸ��λ���ݲɼ�Ƶ�ʣ���λΪ�룬Ĭ��Ϊ1��*/
#define PARAM_GNSS_DMSG_RETRIEVEFREQ	0x0093
/**
GNSS ģ����ϸ��λ�����ϴ���ʽ��
0x00�����ش洢�����ϴ���Ĭ��ֵ����
0x01����ʱ�����ϴ���
0x02�����������ϴ���
0x0B�����ۼ�ʱ���ϴ����ﵽ����ʱ����Զ�ֹͣ�ϴ���
0x0C�����ۼƾ����ϴ����ﵽ������Զ�ֹͣ�ϴ���
0x0D�����ۼ������ϴ����ﵽ�ϴ��������Զ�ֹͣ�ϴ���
*/
#define PARAM_GNSS_DMSG_RPTTYPE	0x0094
/**
GNSS ģ����ϸ��λ�����ϴ����ã�
�ϴ���ʽΪ0x01 ʱ����λΪ�룻
�ϴ���ʽΪ0x02 ʱ����λΪ�ף�
�ϴ���ʽΪ0x0B ʱ����λΪ�룻
�ϴ���ʽΪ0x0C ʱ����λΪ�ף�
�ϴ���ʽΪ0x0D ʱ����λΪ����
*/
#define PARAM_GNSS_DMSG_RPTSETTING	0x0095

/**0x0100 DWORD CAN ����ͨ��1 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�*/
#define PARAM_CAN_CH1_RETRIEVEINTERVAL	0x0100
/**0x0101 WORD CAN ����ͨ��1 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�*/
#define PARAM_CAN_CH1_UPLOADINTERVAL	0x0101

/**0x0102 DWORD CAN ����ͨ��2 �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�*/
#define PARAM_CAN_CH2_RETRIEVEINTERVAL	0x0102
/**0x0103 WORD CAN ����ͨ��2 �ϴ�ʱ����(s)��0 ��ʾ���ϴ�*/
#define PARAM_CAN_CH2_UPLOADINTERVAL	0x0103


/**
		0x0110 BYTE[8]
CAN ����ID �����ɼ����ã�
bit63-bit32 ��ʾ��ID �ɼ�ʱ����(ms)��0 ��ʾ���ɼ���
bit31 ��ʾCAN ͨ���ţ�0��CAN1��1��CAN2��
bit30 ��ʾ֡���ͣ�0����׼֡��1����չ֡��
bit29 ��ʾ���ݲɼ���ʽ��0��ԭʼ���ݣ�1���ɼ�����ļ���ֵ��
bit28-bit0 ��ʾCAN ����ID��
*/
#define PARAM_CAN_SOLORETRIEVE	0x0110

/**0x0111-0x01FF BYTE[8] ��������CAN ����ID �����ɼ�����*/
#define PARAM_CAN_SOLORETRIEVE_OTHER	0x0111

/**0xF000-0xFFFF �û��Զ���*/

/**

�� 13 ��ʱ���տ���λ����
λ ���� ������Ҫ��
0 ����ͨ��1 ��ʱ���տ��ر�־ 0�������� 1������
1 ����ͨ��2 ��ʱ���տ��ر�־ 0�������� 1������
2 ����ͨ��3 ��ʱ���տ��ر�־ 0�������� 1������
3 ����ͨ��4 ��ʱ���տ��ر�־ 0�������� 1������
4 ����ͨ��5 ��ʱ���տ��ر�־ 0�������� 1������
5-7 ����
8 ����ͨ��1 ��ʱ���մ洢��־ 0���洢�� 1���ϴ�
9 ����ͨ��2 ��ʱ���մ洢��־ 0���洢�� 1���ϴ�
10 ����ͨ��3 ��ʱ���մ洢��־ 0���洢�� 1���ϴ�
11 ����ͨ��4 ��ʱ���մ洢��־ 0���洢�� 1���ϴ�
12 ����ͨ��5 ��ʱ���մ洢��־ 0���洢�� 1���ϴ�
13-15 ����
16 ��ʱʱ�䵥λ
0���룬����ֵС��5 ��ʱ���ն˰�5 �봦��
1���֡�
17-31 ��ʱʱ���� �յ��������û�����������ִ��
*/
typedef struct
{
	u32	bit_ch1				:	1;
	u32	bit_ch2				:	1;
	u32	bit_ch3				:	1;
	u32	bit_ch4				:	1;
	u32	bit_ch5				:	1;
	u32	reserved5_7			:	3;
	u32	bit_ch1_save		:	1;
	u32	bit_ch2_save		:	1;
	u32	bit_ch3_save		:	1;
	u32	bit_ch4_save		:	1;
	u32	bit_ch5_save		:	1;
	u32	reserved13_15		:	3;
	u32	bit_timeUnit		:	1;
	u32	bit_timeInterval;
} tParam_PeriodicTimeVideoCapture;

/**

��14 �������տ���λ����
λ ���� ������Ҫ��
0 ����ͨ��1 �������տ��ر�־ 0�������� 1������
1 ����ͨ��2 �������տ��ر�־ 0�������� 1������
2 ����ͨ��3 �������տ��ر�־ 0�������� 1������
3 ����ͨ��4 �������տ��ر�־ 0�������� 1������
4 ����ͨ��5 �������տ��ر�־ 0�������� 1������
5-7 ����
8 ����ͨ��1 �������մ洢��־ 0���洢�� 1���ϴ�
9 ����ͨ��2 �������մ洢��־ 0���洢�� 1���ϴ�
10 ����ͨ��3 �������մ洢��־ 0���洢�� 1���ϴ�
11 ����ͨ��4 �������մ洢��־ 0���洢�� 1���ϴ�
12 ����ͨ��5 �������մ洢��־ 0���洢�� 1���ϴ�
13-15 ����
16 ������뵥λ
0���ף�����ֵС��100 ��ʱ���ն˰�100 �״���
1�����
17-31 ��������� �յ��������û�����������ִ��
*/
typedef struct
{
	u32	bit_ch1		:	1;
	u32	bit_ch2		:	1;
	u32	bit_ch3		:	1;
	u32	bit_ch4		:	1;
	u32	bit_ch5		:	1;
	u32	reserved5_7		:	3;
	u32	bit_ch1_save		:	1;
	u32	bit_ch2_save		:	1;
	u32	bit_ch3_save		:	1;
	u32	bit_ch4_save		:	1;
	u32	bit_ch5_save		:	1;
	u32	reserved13_15		:	3;
	u32	bit_distUnit		:	1;
	u32	bit_distInterval;
} tParam_PeriodicDistVideoCapture;

#endif///JTT808_Ver_2013

///������ͷ��
typedef 	struct
{
	u32		u32Id;		///����ID
	u8		u8Len;		///��������
	u8		aValue[0];	///��������
} tPARAMITEMHEAD;

///������ͷ��
typedef 	struct
{
	u32		u32Id;		///����ID
	u8		u8Len;		///��������
	u32		u32Value;	///DWORD
} tPARAMITEM_U32;

///������ͷ��
typedef 	struct
{
	u32		u32Id;		///����ID
	u8		u8Len;		///��������
	u16		u16Value;	///WORD
} tPARAMITEM_U16;

///������ͷ��
typedef 	struct
{
	u32		u32Id;		///����ID
	u8		u8Len;		///��������
	u8		u8Value;	///BYTE
} tPARAMITEM_U8;

///������ͷ��
typedef 	struct
{
	u32		u32Id;		///����ID
	u8		u8Len;		///��������
	u8		str[0];		///BYTE
} tPARAMITEM_STR;



/** 8.9 �����ն˲���,��ϢID��0x8103��*****************************************/
typedef	struct
{
	u8		u8Amount;			///��������
///	u8		u8PackageParamNum;	///����������
	tPARAMITEMHEAD	atItems[0];		///������
} tMsg_SCMD_SetParam;


/** 8.10 ��ѯ�ն˲��� ��ϢID��0x8104��*****************************************/
/**��ѯ�ն˲�����Ϣ��Ϊ�ա�*/

/** 8.11 ��ѯָ���ն˲��� ��ϢID��0x8106��**************************************/
/**�ն˲���0x0104ָ��Ӧ��*/
typedef struct
{
	u8	u8Amount;	///��������,��������Ϊn
	u32	aIds[0];    ///����ID �б� BYTE[4*n],����˳�����У���"����ID1 ����ID2......����IDn"
} tMSG_SCMD_QuerySpecicialParam;

/** 8.12 ��ѯ�ն˲���Ӧ�� ��ϢID��0x0104��**************************************/
typedef	struct
{
	u16	u16SequenceNumber;	///��Ӧ��ƽ̨��Ϣ����ˮ��
	u8 u8ParamAmount;		///��������
	tPARAMITEMHEAD atItems[0];	///������
} tMSG_TACK_QueryParams;

///�ն˿���������
typedef enum
{
	CMD_WIRELESS_UPG			= 1,	///��������
	CMD_CONNECT_ALLOCATE_SERVER = 2,	///�����ն�����ָ��������
	CMD_POWER_OFF 				= 3,	///�����ն˹ػ�
	CMD_RESET 					= 4,	///�����ն˸�λ
	CMD_RESUME_FACTORY_SET 		= 5,	///�ն˻ָ���������
	CMD_CLOSE_DATA_LINK 		= 6,	///�ر�����ͨ��
	CMD_CLOSE_ALL_LINK 			= 7,	///�ر���������ͨ��
}eTERMINALCTRL;

/** 8.13 �ն˿���,��ϢID��0x8105��********************************************/
/**
0 ������ 	BYTE 	�ն˿���������˵������18
1 ������� STRING 	���������ʽ���������������ÿ���ֶ�֮����ð�ǡ�;���ָ���ÿ��
					STRING �ֶ��Ȱ�GBK ���봦����������Ϣ

������                    �������                         ������Ҫ��
1 ���������ʽ����19      ��������������֮����ð�Ƿֺŷָ���ָ�����£���URL ��ַ;���ŵ�
                                                                   ����;�����û���;��������;��ַ;TCP �˿�;UDP �˿�;������ID; Ӳ��
                                                                  �汾;�̼��汾; ���ӵ�ָ��������ʱ�ޡ�����ĳ��������ֵ����ſ�
2 ���������ʽ����19 		�����ն�����ָ��������������֮����ð�Ƿֺŷָ�������ָ�����£�
						�����ӿ���;���ƽ̨��Ȩ��;���ŵ�����;�����û���;��������;��
						ַ;TCP �˿�;UDP �˿�;���ӵ�ָ��������ʱ�ޡ�����ĳ��������ֵ����
						�ſգ������ӿ���ֵΪ1�����޺�̲���
3 �� �ն˹ػ�
4 �� �ն˸�λ
5 �� �ն˻ָ���������
6 �� �ر�����ͨ��
7 �� �ر���������ͨ��

�� 19 ���������ʽ
�ֶ�          		 �������� 	������Ҫ��
���ӿ���   	BYTE    	0���л���ָ�����ƽ̨�����������ӵ��÷������󼴽���Ӧ��״̬����״̬�½����·�����ָ
						��ļ��ƽ̨�ɷ��Ͱ����������ڵĿ���ָ�
						1���л���ԭȱʡ���ƽ̨�����������ָ�����״̬��
						���ŵ����� STRING һ��Ϊ������APN������ͨ�Ų��ŷ��ʵ㣬��������ʽΪCDMA�����ֵ
						ΪPPP ���Ӳ��ź���
�����û��� 	STRING 		����������ͨ�Ų����û���
�������� 		STRING 		����������ͨ�Ų�������
��ַ 		STRING 		��������ַ,IP ������
TCP�˿� 		WORD 		������TCP �˿�
UDP�˿� 		WORD 		������UDP �˿�
������ID 	BYTE[5] 	�ն������̱���
���ƽ̨��Ȩ�� STRING 	���ƽ̨�·��ļ�Ȩ�룬�������ն����ӵ����ƽ̨֮��ļ�Ȩ���ն�
						���ӻ�ԭ���ƽ̨����ԭ��Ȩ��
Ӳ���汾 		STRING 		�ն˵�Ӳ���汾�ţ����������Զ�
�̼��汾 		STRING 		�ն˵Ĺ̼��汾�ţ����������Զ�
URL��ַ 		STRING 		����URL ��ַ
���ӵ�ָ��������ʱ��
			WORD 		��λ���֣�min����ֵ��0 ��ʾ���ն˽��յ�����������ָ��������ָ��
						�����Ч�ڽ�ֹǰ���ն�Ӧ����ԭ��ַ����ֵΪ0�����ʾһֱ����ָ
						��������
*/

#ifdef JTT808_Ver_2013
/** 8.14 ��ѯ�ն�����,��ϢID��0x8107��*****************************************/
/**��ѯ�ն�������Ϣ��Ϊ�ա�*/
#endif ///JTT808_Ver_2013

/** 8.15 ��ѯ�ն�����Ӧ��,��ϢID��0x0107��*************************************/
#ifdef JTT808_Ver_2013
/**�ն�����*/
typedef struct
{
	u16 bit_PassengerTransFittable	:	1;	///bit0��0�������ÿ��˳�����1�����ÿ��˳�����
	u16 bit_DangerousFittable		:	1;	///bit1��0��������Σ��Ʒ������1������Σ��Ʒ������
	u16 bit_FreightTransFittable	:	1;	///bit2��0����������ͨ���˳�����1��������ͨ���˳�����
	u16 bit_TaxiFittable			:	1;	///bit3��0�������ó��⳵����1�����ó��⳵����
	u16 bit_reserved_4_5	:	2;
	u16 bit_SupportVideoRecord		:	1;	///bit6��0����֧��Ӳ��¼��1��֧��Ӳ��¼��
	u16 bit_Integration				:	1;		///bit7��0��һ�����1���������
	u16 bit_reserved_8_15	:	8;
} tTerminalType;
#endif///JTT808_Ver_2013

/**GNSS ģ������*/
typedef struct
{
	u16 bit_Support_GPS		:	1;	///bit0��0����֧��GPS ��λ�� 1��֧��GPS ��λ��
	u16 bit_Support_Beidou	:	1;	///bit1��0����֧�ֱ�����λ�� 1��֧�ֱ�����λ��
	u16 bit_Support_GLONASS	:	1;	///bit2��0����֧��GLONASS ��λ�� 1��֧��GLONASS ��λ��
	u16 bit_Support_GALILEO	:	1;	///bit3��0����֧��Galileo ��λ�� 1��֧��Galileo ��λ��
	u16 bit_Property_reserved_4_15	: 	12;
} tGNSSModule_Property;

/**ͨ��ģ������ BYTE*/
typedef struct
{
	u8 bit_SupportGPRS		:	1;	///bit0��0����֧��GPRSͨ�ţ� 1��֧��GPRSͨ�ţ�
	u8 bit_SupportCDMA		:	1;	///bit1��0����֧��CDMAͨ�ţ� 1��֧��CDMAͨ�ţ�
	u8 bit_SupportTDCDMA	:	1;	///bit2��0����֧��TD-SCDMAͨ�ţ� 1��֧��TD-SCDMAͨ�ţ�
	u8 bit_SupportWCDMA		:	1;	///bit3��0����֧��WCDMAͨ�ţ� 1��֧��WCDMAͨ�ţ�
	u8 bit_SupportCDMA2000	:	1;	///bit4��0����֧��CDMA2000ͨ�ţ� 1��֧��CDMA2000ͨ�š�
	u8 bit_SupportTDLTE		:	1;	///bit5��0����֧��TD-LTEͨ�ţ� 1��֧��TD-LTEͨ�ţ�
	u8 bit_CommProperty_reserved_6	:	1;
	u8 bit_SupportOther		:	1;	///bit7��0����֧������ͨ�ŷ�ʽ�� 1��֧������ͨ�ŷ�ʽ��
} tCommModule_Property;
//#endif

#ifdef JTT808_Ver_2013
typedef struct
{
	tTerminalType tType;	///�ն����� WORD
	u8	factoryId[5];	///������ID BYTE[5] 5 ���ֽڣ��ն������̱��롣
	u8  class[20];		///�ն��ͺ� ���ն��ͺ������������ж��壬λ������ʱ���󲹡�0X00����
	u8	tid[7];			///�ն�ID �ɴ�д��ĸ��������ɣ����ն�ID �����������ж��壬λ������ʱ���󲹡�0X00����
	u8	ICCID[10];		///�ն�SIM ��ICCID BCD[10] �ն�SIM ��ICCID ��
	u8	ver_hardware_len;	///�ն�Ӳ���汾�ų���
	u8*	ver_hardware;	///�ն�Ӳ���汾�� STRING
	u8	ver_software_len;	///�ն�Ӳ����汾�ų���
	u8*	ver_software;	///�ն�����汾�� STRING
	tGNSSModule_Property tGNSSProperty; ///GNSS ģ������ BYTE
	tCommModule_Property tCommProperty; ///ͨ��ģ������ BYTE

} tMsg_TACK_QueryTerminalProperty;

#endif	///JTT808_Ver_2013

#ifdef JTT808_Ver_2013
/** 8.16 �·��ն�������,��ϢID��0x8108��***************************************/
/**�·��ն���������Ϣ�����ݸ�ʽ����21���Ը������ն�ʹ��ͨ��Ӧ��ȷ���Ƿ���ȷ��
�����������ݡ�
*/
typedef struct
{
	u8 upg_type;	///�������� 0���նˣ�12����·����֤IC ����������52���������Ƕ�λģ��
	u8 factoryId[5];	///������ID BYTE[5] �����̱��
	u8 version_len;		///�汾�ų��� BYTE n
	u8 version;		///�汾�� STRING
	u32 upgPack_len;	///7+n �������ݰ����� DWORD ��λΪBYTE
	u8 *pUpgPack;		///11+n �������ݰ�
} tMsg_SCMD_dispatchUpgPackage;

#endif///JTT808_Ver_2013

#ifdef JTT808_Ver_2013
/** 8.17 �ն��������֪ͨ,��ϢID��0x0108��*************************************/
/**�ն���������ɲ��������Ӻ�ʹ�ø�����֪ͨ������ġ��ն��������֪ͨ��Ϣ����
�ݸ�ʽ����22��
*/
typedef struct
{
	u8 upg_type;	///��������  0���նˣ�12����·����֤IC ����������52���������Ƕ�λģ��
	u8 upg_result;	/// �������  0���ɹ���1��ʧ�ܣ�2��ȡ��
} tMsg_TACK_UpgResult;

#endif///JTT808_Ver_2013



/**������־λ����*/
#define WARNING_Emergency		((u32)0x00000001)	///1�����������������������غ󴥷� �յ�Ӧ�������
#define WARNING_Overspeed		(((u32)0x00000001) << 1)	///1�����ٱ��� ��־ά���������������
#define WARNING_Fatigure		(((u32)0x00000001) << 2)	///2 1��ƣ�ͼ�ʻ ��־ά���������������
#define WARNING_PreDangerous	(((u32)0x00000001) << 3)	///3 1��Σ��Ԥ�� �յ�Ӧ�������
#define WARNING_GNSS_Module_Fault		(((u32)0x00000001) << 4)	///4 1��GNSS ģ�鷢������ ��־ά���������������
#define WARNING_GNSS_Antena_Breakdown	(((u32)0x00000001) << 5)	///5 1��GNSS ����δ�ӻ򱻼��� ��־ά���������������
#define WARNING_GNSS_Antena_ShortCircuit	(((u32)0x00000001) << 6)	///6 1��GNSS ���߶�· ��־ά���������������
#define WARNING_MPwr_LowVoltage		(((u32)0x00000001) << 7)	///7 1���ն�����ԴǷѹ ��־ά���������������
#define WARNING_MPwr_poweroff		(((u32)0x00000001) << 8)	///8 1���ն�����Դ���� ��־ά���������������
#define WARNING_LCD_Fault		(((u32)0x00000001) << 9)	///9 1���ն�LCD ����ʾ������ ��־ά���������������
#define WARNING_TTS_Fault		(((u32)0x00000001) << 10)	///10 1��TTS ģ����� ��־ά���������������
#define WARNING_Video_Fault		(((u32)0x00000001) << 11)	///11 1������ͷ���� ��־ά���������������
#ifdef JTT808_Ver_2013
#define WARNING_IC_Fault		(((u32)0x00000001) << 12)	///12 1����·����֤IC ��ģ����� ��־ά���������������
#define WARNING_PreOverspeed	(((u32)0x00000001) << 13)	///13 1������Ԥ�� ��־ά���������������
#define WARNING_PreFatigure		(((u32)0x00000001) << 14)	///14 1��ƣ�ͼ�ʻԤ�� ��־ά���������������
#endif///JTT808_Ver_2013
#define WARNING_TOUT_Driving_day	(((u32)0x00000001) << 18)	///18 1�������ۼƼ�ʻ��ʱ ��־ά���������������
#define WARNING_TOUT_Stopping		(((u32)0x00000001) << 19)	///19 1����ʱͣ�� ��־ά���������������
#define WARNING_CrossBarrier		(((u32)0x00000001) << 20)	///20 1���������� �յ�Ӧ�������
#define WARNING_CrossPath		(((u32)0x00000001) << 21)	///21 1������·�� �յ�Ӧ�������
#define WARNING_DriveTime_NotEnough	(((u32)0x00000001) << 22)	///22 1��·����ʻʱ�䲻��/���� �յ�Ӧ�������
#define WARNING_Path_Deviate		(((u32)0x00000001) << 23)	///23 1��·��ƫ�뱨�� ��־ά���������������
#define WARNING_Vechicle_VSS_Fault		(((u32)0x00000001) << 24)	///24 1������VSS ���� ��־ά���������������
#define WARNING_Vechicle_Oil_Fault		(((u32)0x00000001) << 25)	///25 1�����������쳣 ��־ά���������������
#define WARNING_Vechicle_Stolen		(((u32)0x00000001) << 26)	///26 1����������(ͨ������������) ��־ά���������������
#define WARNING_Vechicle_Illegical_Launch	(((u32)0x00000001) << 27)	///27 1�������Ƿ���� �յ�Ӧ�������
#define WARNING_Vechicle_Illegical_Moving	(((u32)0x00000001) << 28)	///28 1�������Ƿ�λ�� �յ�Ӧ�������
#define WARNING_Vechicle_PreCollision	(((u32)0x00000001) << 29)	///29 1����ײԤ�� ��־ά���������������
///#define warning_reserved_30_31	: 	2;	///30-31����
#ifdef JTT808_Ver_2013
#define WARNING_Vechicle_PreSidestroke		(((u32)0x00000001) << 30)	///30 1���෭Ԥ�� ��־ά���������������
#define WARNING_Vechicle_Illegical_DoorOpen		(((u32)0x00000001) << 31)	///31 1���Ƿ����ű������ն�δ��������ʱ�����жϷǷ����ţ��յ�Ӧ�������
#endif ///JTT808_Ver_2013

/**������־λ����*/
typedef struct
{
	u32 warning_Emergency		:	1;	///1�����������������������غ󴥷� �յ�Ӧ�������
	u32 warning_Overspeed		:	1;	///1�����ٱ���, ��־ά���������������
	u32 warning_Fatigure		:	1;	///2 1��ƣ�ͼ�ʻ, ��־ά���������������
	u32 warning_PreDangerous	:	1;	///3 1��Σ��Ԥ��, �յ�Ӧ�������
	u32 warning_GNSS_Module_Fault		:	1;	///4 1��GNSS ģ�鷢�����ϱ�־,ά���������������
	u32 warning_GNSS_Antena_Breakdown	:	1;	///5 1��GNSS ����δ�ӻ򱻼��ϱ�־, ά���������������
	u32 warning_GNSS_Antena_ShortCircuit	:	1;	///6 1��GNSS ���߶�·, ��־ά���������������
	u32 warning_MPwr_LowVoltage		:	1;	///7 1���ն�����ԴǷѹ, ��־ά���������������
	u32 warning_MPwr_poweroff		:	1;	///8 1���ն�����Դ����, ��־ά���������������
	u32 warning_LCD_Fault		:	1;	///9 1���ն�LCD ����ʾ������, ��־ά���������������
	u32 warning_TTS_Fault		:	1;	///10 1��TTS ģ�����, ��־ά���������������
	u32 warning_Video_Fault		:	1;	///11 1������ͷ����, ��־ά���������������
#ifdef JTT808_Ver_2011
	u32 warning_reserved_12_17	: 	6;	///12-17����
#endif
#ifdef JTT808_Ver_2013
	u32 warning_IC_Fault		:	1;	///12 1����·����֤IC ��ģ����� ��־ά���������������
	u32 warning_PreOverspeed	:	1;	///13 1������Ԥ�� ��־ά���������������
	u32 warning_PreFatigure		:	1;	///14 1��ƣ�ͼ�ʻԤ�� ��־ά���������������
	u32 warning_reserved_15_17	:	3;	///15-17 ����
#endif///JTT808_Ver_2013
	u32 warning_TOUT_Driving_day	:	1;	///18 1�������ۼƼ�ʻ��ʱ ��־ά���������������
	u32 warning_TOUT_Stopping		:	1;	///19 1����ʱͣ�� ��־ά���������������
	u32 warning_CrossBarrier		:	1;	///20 1���������� �յ�Ӧ�������
	u32 warning_CrossPath		:	1;	///21 1������·�� �յ�Ӧ�������
	u32 warning_DriveTime_NotEnough	:	1;	///22 1��·����ʻʱ�䲻��/���� �յ�Ӧ�������
	u32 warning_Path_Deviate		:	1;	///23 1��·��ƫ�뱨�� ��־ά���������������
	u32 warning_Vechicle_VSS_Fault		:	1;	///24 1������VSS ���� ��־ά���������������
	u32 warning_Vechicle_Oil_Fault		:	1;	///25 1�����������쳣 ��־ά���������������
	u32 warning_Vechicle_Stolen		:	1;	///26 1����������(ͨ������������) ��־ά���������������
	u32 warning_Vechicle_Illegical_Launch	:	1;	///27 1�������Ƿ���� �յ�Ӧ�������
	u32 warning_Vechicle_Illegical_Moving	:	1;	///28 1�������Ƿ�λ�� �յ�Ӧ�������
	u32 warning_Vechicle_PreCollision	:	1;	///29 1����ײԤ�� ��־ά���������������
#ifdef JTT808_Ver_2011
	u32 warning_reserved_30_31	: 	2;	///30-31����
#endif
#ifdef JTT808_Ver_2013
	u32 warning_Vechicle_PreSidestroke		:	1;	///30 1���෭Ԥ�� ��־ά���������������
	u32 warning_Vechicle_Illegical_DoorOpen		:	1;	///31 1���Ƿ����ű������ն�δ��������ʱ�����жϷǷ����ţ��յ�Ӧ�������
#endif///JTT808_Ver_2013
} tWARNINGSTATE;

/**״̬λ����*/
typedef struct
{
	/**״̬λ*/
	u32	state_ACC			:	1;	///0:ACC�أ�1:ACC��
	u32	state_Location		:	1;	///0:δ��λ��1:��λ
	u32	state_Lat			:	1;	///0:��γ��1:��γ
	u32	state_Long			:	1;	///0:������1:����
	u32	state_OnService		:	1;	///0:��Ӫ״̬��1:ͣ��״̬
	u32	state_Encrypt		:	1;	///0:��γ��δ�����ܲ�����ܣ�1:��γ���Ѿ����ܲ������
#ifdef JTT808_Ver_2011
	u32 state_reserved_6_9	:	4; 	///����
#endif
#ifdef JTT808_Ver_2013
	u32 state_reserved_6_7	:	2;	///����
	u32 state_Loading		:	2;	///00���ճ���01�����أ�10��������11������
		///	�������ڿͳ��Ŀա��س��������Ŀ��ء�����״̬��ʾ���˹�����򴫸�����ȡ��
#endif///JTT808_Ver_2013

	u32 state_Oil			:	1;	///10 0��������·������1��������·�Ͽ�
	u32 state_ElectricCircuit	:	1;	///11 0��������·������1��������·�Ͽ�
	u32 state_DoorLock			:	1;	///12 0�����Ž�����1�����ż���
#ifdef JTT808_Ver_2011
	u32 state_Reserved_13_31	:	19;	/// ����
#endif

#ifdef JTT808_Ver_2013
	u32 state_Door_1			:	1;	///13 0����1 �أ�1����1 ����ǰ�ţ�
	u32 state_Door_2			:	1;	///14 0����2 �أ�1����2 �������ţ�
	u32 state_Door_3			:	1;	///15 0����3 �أ�1����3 �������ţ�
	u32 state_Door_4			:	1;	///16 0����4 �أ�1����4 ������ʻϯ�ţ�
	u32 state_Door_5			:	1;	///17 0����5 �أ�1����5 �����Զ��壩
	u32 state_GPS_Used			:	1;	///18 0��δʹ��GPS ���ǽ��ж�λ��1��ʹ��GPS ���ǽ��ж�λ
	u32 state_Beidou_Used		:	1;	///19 0��δʹ�ñ������ǽ��ж�λ��1��ʹ�ñ������ǽ��ж�λ
	u32 state_GLONASS_Used		:	1;	///20 0��δʹ��GLONASS ���ǽ��ж�λ��1��ʹ��GLONASS ���ǽ��ж�λ
	u32 state_Galileo_Used		:	1;	///21 0��δʹ��Galileo ���ǽ��ж�λ��1��ʹ��Galileo ���ǽ��ж�λ
	u32 state_Reserved_22_31	:	10;	/// ����
#endif///JTT808_Ver_2013

} tSYSTEMSTATE;

/**λ�û�����Ϣ���ݸ�ʽ*/
typedef struct
{
	tWARNINGSTATE 	tWarningState;	/**u32 ������־λ����*/
	tSYSTEMSTATE 	tSysState;	/**״̬λ*/

	u32 u32Lat;		///8 γ�� DWORD �Զ�Ϊ��λ��γ��ֵ����10 ��6 �η�����ȷ�������֮һ��
	u32 u32Long;	///12 ���� DWORD �Զ�Ϊ��λ�ľ���ֵ����10 ��6 �η�����ȷ�������֮һ��
	u16 u16Height;	///16 �߳� WORD ���θ߶ȣ���λΪ�ף�m��
	u16 u16Speed;	///18 �ٶ� WORD 1/10km/h
	u16 u16Direct;	///20 ���� WORD 0-359������Ϊ0��˳ʱ��
	u8	time[6];	///21 ʱ�� BCD[6] YY-MM-DD-hh-mm-ss��GMT+8 ʱ�䣬����׼��֮���漰��ʱ������ô�ʱ����
} tMsg_Position_BaseInfo;

///���ٻ�����/·�߱���ʱ�����Ͷ���
typedef	enum
{
	eNORMAL	 		 	= 0,	///���ض�λ��
	eROUND_AREA 		= 1,	///Բ������
	eRECT_AREA			= 2,	///��������
	eMULTIMEDIA_AREA 	= 3,	///���������
	eLINE 				= 4		///·��
}ePOSType;

///�޳��ٱ���������Ϣ��Ϣ�����ݸ�ʽ
typedef 	struct
{
	ePOSType   	ePosType;				///λ������
}tAppendInfo_NonOverspeedWarning;

///���ٱ���������Ϣ��Ϣ�����ݸ�ʽ
typedef 	struct
{
	ePOSType   	ePosType;				///λ������
	u32			u32AreaOrLineId;		///�������·��ID
}tAppendInfo_OverspeedWarning;

///������������
#define		DIR_ENTER_AREA				0
#define		DIR_LEAVE_AREA				1

///��������/·�߱���������Ϣ��Ϣ�����ݸ�ʽ
typedef		struct
{
	ePOSType   	ePosType;				///λ������
	u32			u32AreaOrLineId;		///�����·��ID
	u8			u8EnterOrLeave;			///����
}tAppendInfo_BarrierAndPathWarning;

///·����ʻʱ�䲻��/��������������Ϣ��Ϣ�����ݸ�ʽ
typedef		struct
{
	u32			u32LineId;				///·��ID
	u16			u16DriveTime;			///��ʻʱ�䣬��λΪ��
	u8			u8Result;				///�����0:���㣬1:����
}tAppendInfo_DrivingTimeInadequetWarning;

/**��չ�����ź�״̬λ*/
#define VECHICLE_EXTSIG_CloseLight	((u32)0x00000001)	///0 1��������ź�
#define VECHICLE_EXTSIG_FarLight	(((u32)0x00000001) << 1)	///1 1��Զ����ź�
#define VECHICLE_EXTSIG_RightLight	(((u32)0x00000001) << 2)	///2 1����ת����ź�
#define VECHICLE_EXTSIG_LeftLight	(((u32)0x00000001) << 3)	///3 1����ת����ź�
#define VECHICLE_EXTSIG_StopLight	(((u32)0x00000001) << 4)	///4 1���ƶ��ź�
#define VECHICLE_EXTSIG_ReverseGearLight	(((u32)0x00000001) << 5)	///5 1�������ź�
#define VECHICLE_EXTSIG_FogLight	(((u32)0x00000001) << 6)	///6 1������ź�
#define VECHICLE_EXTSIG_WideLight	(((u32)0x00000001) << 7)	///7 1��ʾ����
#define VECHICLE_EXTSIG_SpeakerLight	(((u32)0x00000001) << 8)	///8 1�������ź�
#define VECHICLE_EXTSIG_AirConditionLight	(((u32)0x00000001) << 9)	///9 1���յ�״̬
#define VECHICLE_EXTSIG_NullGearLight	(((u32)0x00000001) << 10)	///10 1���յ��ź�
#define VECHICLE_EXTSIG_RetardWorking	(((u32)0x00000001) << 11)	///11 1������������
#define VECHICLE_EXTSIG_ABSWorking	(((u32)0x00000001) << 12)	///12 1��ABS ����
#define VECHICLE_EXTSIG_HeatWorking	(((u32)0x00000001) << 13)	///13 1������������
#define VECHICLE_EXTSIG_ClutchState	(((u32)0x00000001) << 14)	///14 1�������״̬
/**15-31 ����*/

/**IO ״̬λ*/
#define VECHICLE_IOSTATE_DeepSleep	((u16)0x00000001)	///0 1���������״̬
#define VECHICLE_IOSTATE_Sleep		(((u16)0x00000001) << 1)	///1 1������״̬
/**2-15 ����*/

/**λ�ø�����Ϣ��-δ����*/
typedef struct
{
	u8	msgId;	///������ϢID BYTE 1-255
	u8	msgLen;	///������Ϣ���� BYTE
	u8  tag_1;
	u32	kilemeters;	///0x01 4 ��̣�DWORD��1/10km����Ӧ������̱����
	u8  tag_2;
	u16 oil; 	///0x02 2 ������WORD��1/10L����Ӧ�������������
	u8  tag_3;
	u16 speed;	///0x03 2 ��ʻ��¼���ܻ�ȡ���ٶȣ�WORD��1/10km/h
	u8  tag_4;
	u16 warningEvtId; 	///0x04 2 ��Ҫ�˹�ȷ�ϱ����¼���ID��WORD����1 ��ʼ����
	/**0x05-0x10 ����*/
	u8  tag_11;
	tAppendInfo_NonOverspeedWarning tNonOverspeedWarn; ///1 ��5 ���ٱ���������Ϣ����28
	u8  tag_12;
	tAppendInfo_BarrierAndPathWarning tBarrierWarn;///6 ��������/·�߱���������Ϣ����29
	u8  tag_13;
	tAppendInfo_DrivingTimeInadequetWarning tDrvTimeWarn; ///0x13 7 ·����ʻʱ�䲻��/��������������Ϣ����30
	/**0x14-0x24 ����*/
	u8  tag_25;
	u32 vechicalExtraSignal;	///0x25 4 ��չ�����ź�״̬λ���������31
	u8  tag_2A;
	u16 IOState;	///0x2A 2 IO״̬λ���������32
	u8  tag_2B;
	u32 ADValue;	///0x2B 4 ģ������bit0-15��AD0��bit16-31��AD1��
	u8  tag_30;
	u8 	csq;	///0x30 1 BYTE������ͨ�������ź�ǿ��
	u8  tag_31;
	u8	GNSS_Satellites; ///0x31 1 BYTE��GNSS ��λ������
	u8  tag_E0;	///0xE0 ������Ϣ���� �����Զ�����Ϣ����
	u8	extraInfoLen;
	/**0xE1-0xFF �Զ�������*/
} tMsg_Position_AppendInfo_NonOverSpeed;

/**λ�ø�����Ϣ��-����*/
typedef struct
{
	u8	msgId;	///������ϢID BYTE 1-255
	u8	msgLen;	///������Ϣ���� BYTE
	u8  tag_1;
	u32	kilemeters;	///0x01 4 ��̣�DWORD��1/10km����Ӧ������̱����
	u8  tag_2;
	u16 oil; 	///0x02 2 ������WORD��1/10L����Ӧ�������������
	u8  tag_3;
	u16 speed;	///0x03 2 ��ʻ��¼���ܻ�ȡ���ٶȣ�WORD��1/10km/h
	u8  tag_4;
	u16 warningEvtId; 	///0x04 2 ��Ҫ�˹�ȷ�ϱ����¼���ID��WORD����1 ��ʼ����
	/**0x05-0x10 ����*/
	u8  tag_11;
	tAppendInfo_NonOverspeedWarning tNonOverspeedWarn; ///1 ��5 ���ٱ���������Ϣ����28
	u8  tag_12;
	tAppendInfo_BarrierAndPathWarning tBarrierWarn;///6 ��������/·�߱���������Ϣ����29
	u8  tag_13;
	tAppendInfo_DrivingTimeInadequetWarning tDrvTimeWarn; ///0x13 7 ·����ʻʱ�䲻��/��������������Ϣ����30
	/**0x14-0x24 ����*/
	u8  tag_25;
	u32 vechicalExtraSignal;	///0x25 4 ��չ�����ź�״̬λ���������31
	u8  tag_2A;
	u16 IOState;	///0x2A 2 IO״̬λ���������32
	u8  tag_2B;
	u32 ADValue;	///0x2B 4 ģ������bit0-15��AD0��bit16-31��AD1��
	u8  tag_30;
	u8 	csq;	///0x30 1 BYTE������ͨ�������ź�ǿ��
	u8  tag_31;
	u8	GNSS_Satellites; ///0x31 1 BYTE��GNSS ��λ������
	u8  tag_E0;	///0xE0 ������Ϣ���� �����Զ�����Ϣ����
	u8	extraInfoLen;
	/**0xE1-0xFF �Զ�������*/
} tMsg_Position_AppendInfo_OverSpeed;

typedef union
{
	tMsg_Position_AppendInfo_NonOverSpeed	tAppendInfo_NonOverSpeed;
	tMsg_Position_AppendInfo_OverSpeed 	tAppendInfo_OverSpeed;
} U_AppendInfo;

/** 8.18 λ����Ϣ�㱨,��ϢID��0x0200��****************************************/
typedef struct
{
	tMsg_Position_BaseInfo 	tBaseInfo;		///������Ϣ
	U_AppendInfo			tAppendInfo;	///������Ϣ
} tMsg_T_PositionInfo;


/**8.19 λ����Ϣ��ѯ ��ϢID��0x8201��******************************************/
/**λ����Ϣ��ѯ��Ϣ��Ϊ�ա�*/

/**8.20 λ����Ϣ��ѯӦ�� ��ϢID��0x0201��**************************************/
/**λ����Ϣ��ѯӦ����Ϣ�����ݸ�ʽ����33��
��33 λ����Ϣ��ѯӦ����Ϣ�����ݸ�ʽ
��ʼ�ֽ� �ֶ� �������� ������Ҫ��
0 Ӧ����ˮ�� WORD ��Ӧ��λ����Ϣ��ѯ��Ϣ����ˮ��
2 λ����Ϣ�㱨 λ����Ϣ�㱨��8.12
���������� ע����Э����Ҳ�ǡ�8.12��������Э�飬8.12��λ�û㱨����Э����Ӧ���Ǳ��󣡣�����
*/
typedef struct
{
	u16	u16SeqNum;	///Ӧ����ˮ��
	tMsg_T_PositionInfo tInfo;	///λ����Ϣ
} tMsg_TACK_QueryPositionInfoAck;

/** 8.21 ��ʱλ�ø��ٿ���,��ϢID��0x8202��*************************************/
typedef struct
{
	u16	u16TimeInterval;///ʱ����,��λΪ�루s����0 ��ֹͣ���١�ֹͣ�������������ֶ�
	/**�ն��ڽ��յ�λ�ø��ٿ�����Ϣ������Ч �ڽ�ֹʱ��֮ǰ��������Ϣ�е�ʱ��������λ�û㱨*/
	u32 u32TrackingTimeout;	///λ�ø�����Ч�� 	��λΪ�루s��
} tMsg_SCMD_TemproryTrack;

#ifdef JTT808_Ver_2013
/**8.22 �˹�ȷ�ϱ�����Ϣ ��ϢID��0x8203****************************************/

typedef struct
{
	u32	bit_Warning_ConfirmEmergency			:	1;	///0 1��ȷ�Ͻ���������
	u32	bit_Warning_Reserved_1_2				:	2;	///1-2 ����
	u32	bit_Warning_ConfirmDanger				:	1;	///3 1��ȷ��Σ��Ԥ����
	u32	bit_Warning_Reserved_4_19				:	15;	///4-19 ����
	u32	bit_Warning_Confirm_CrossArea			:	1;	///20 1��ȷ�Ͻ������򱨾���
	u32	bit_Warning_Confirm_CrossPath			:	1;	///21 1��ȷ�Ͻ���·�߱�����
	u32	bit_Warning_ConfirmDrvTimeInadequet		:	1;	///22 1��ȷ��·����ʻʱ�䲻��/����������
	u32	bit_Warning_Reserved_23_26				:	4;	///23-26 ����
	u32	bit_Warning_ConfirmIllegicalLaunch		:	1;	///27 1��ȷ�ϳ����Ƿ���𱨾���
	u32	bit_Warning_ConfirmIllegicalMoving		:	1;	///28 1��ȷ�ϳ����Ƿ�λ�Ʊ�����
	u32	bit_Warning_Reserved_29_31				:	3;	///29-31 ����
} tManualVerifyWarnType;

/**�˹�ȷ�ϱ�����Ϣ�����ݸ�ʽ����35��*/
typedef struct
{
	u16	u16SeqNum;	///������Ϣ��ˮ��
	u32 u32WarnType;///��������
} tMsg_SCMD_MannalVerifyWarning;

#endif///JTT808_Ver_2013

/**8.23 �ı���Ϣ�·� ��ϢID��0x8300��******************************************/
typedef struct
{
	u8	flag_Emergency		:	1;	/// 0 1������
	u8  flag_Reserved_1		:	1;	///1 ����
	u8  flag_Display		:	1;	///2 1���ն���ʾ����ʾ
	u8  flag_TTSRead		:	1;	///3 1���ն�TTS ����
	u8  flag_Advertise		:	1;	///4 1���������ʾ
#ifdef JTT808_Ver_2013
	u8  flag_MsgInfo		:	1;	///5 0�����ĵ�����Ϣ��1��CAN ��������Ϣ
	u8  flag_Reserved_6_7	:	2;	///6-7 ����
#endif///JTT808_Ver_2013
#ifdef JTT808_Ver_2011
	u8  flag_Reserved_5_7	:	3;	///5-7 ����
#endif
	u8	msg[0];	///�ı���Ϣ STRING �Ϊ1024 �ֽڣ���GBK ����
} tMsg_SCMD_DispatchTxtInfo;

///�ն��¼���������
typedef	enum
{
	eDEL_ALL_EVENT 		= 0,	///ɾ���ն����������¼�����������к����ֽ�
	eUPDATE_EVENT 		= 1,	///�����¼�
	eAPPEND_EVENT 		= 2,	///׷���¼�
	eCHANGE_EVENT 		= 3,	///�޸��¼�
	eDEL_SPECIFIC_EVENT = 4,	///ɾ���ض������¼���֮���¼����������ʵ������
}eEVENTSETType;

///�¼����
typedef	struct
{
	u8	u8EventId;			///�¼�ID,���ն�����ͬID���¼����򱻸���
	u8	u8EventLength;		///�¼�����
	u8 *sEventMsg;			///�¼�����

}tEventItem;

/** 8.24 �¼�����,��ϢID��0x8301��********************************************/
typedef	struct
{
	eEVENTSETType	eEventSetType;	///�����¼�����
	u8		u8SetCount;				///��������
	tEventItem*	ptEventItem;		///�¼�������ָ��
} tMsg_SCMD_SetEvent;

/**8.25 �¼����� ��ϢID��0x0301��**********************************************/
typedef struct
{
	u8	u8EventId;	///�¼�ID
} tMsg_T_EventRptInfo;

/**8.26 �����·� *************************************************************/
///�ı���Ϣ���嶨��
#define	QUESTION_TYPE_ERGENT			0X01	///����
#define	QUESTION_TYPE_TTS_READ			0X08	///�ն�TTS����
#define	QUESTION_TYPE_DISPLAY			0X10	///�������ʾ

///�����·���ѡ����Ϣ�����ݸ�ʽ
typedef	struct
{
	u8	u8AnswerId;			///��ID
	u16	u16AnswerLength;	///�𰸳���
	u8*	pu8AnswerContent;	///������
}tQUESTIONANSWERITEM;

/** 8.27 ����Ӧ��,��ϢID��0x0302��********************************************/
typedef	struct
{
	u8	u8QuestionTag;		///�����·���־λ����
	u8	u8QuestionContentLength;	///�����ֶ��ֽڳ���
	u8*	pu8QuestionContent;			///�����ı�����GBK���룬����ΪN
	tQUESTIONANSWERITEM	*ptQuestionAnswer;///���ʺ�ѡ��ָ��
} tMsg_SCMD_AskQuestion;

///����Ӧ��ṹ�嶨��
typedef	struct
{
	u16	u16SeqNum;	///Ӧ�����ˮ�ţ���Ӧ�������·���Ϣ����ˮ��
	u8	u8AnswerId;		///��ID,�����·��и����Ĵ�ID
} tMsg_TACK_AnswerQuestion;

/** 8.28 ��Ϣ�㲥�˵�����  ID��0x8303 *****************************************/
///��Ϣ�㲥�˵�����
///��Ϣ�㲥�˵���������
#define	MSGOD_MENU_DELETE	0	///ɾ���ն�������Ϣ��
#define	MSGOD_MENU_UPDATE	1	///���²˵�
#define	MSGOD_MENU_APPEND	2	///׷�Ӳ˵�
#define	MSGOD_MENU_CHANGE	3	///�޸Ĳ˵�

///��Ϣ�㲥��Ϣ���
typedef	struct
{
	u8	u8InfoType;				///��Ϣ����
	u16	u16InfoNameLength;		///��Ϣ���Ƴ���
	u8	aInfoName[0];			///��Ϣ����
} tMSGODMENUITEM;

typedef	struct
{
	u8	u8InfoOrderSetType;		///��Ϣ�㲥�˵���������
	u8	u8SetCount;				///��������
	tMSGODMENUITEM	tInfoOrderItem[0];	///��Ϣ�㲥�˵�
} T_SCMD_InfoOrder;

/** 8.29 ��Ϣ�㲥/ȡ�� ID��0x0303 *********************************************/
typedef	struct
{
	u8	u8InfoOrderSetType;		///��Ϣ����
	u8	u8OrderOrCancel;		///�㲥/ȡ����־��0:ȡ����1:�㲥
}T_SCMD_InfoOrderCtrl;

/** 8.30 ��Ϣ���� ��ϢID��0x8304 **********************************************/
typedef	struct
{
	u8	u8InfoType;				///��Ϣ����
	u16	u16InfoTotalLength;		///��Ϣ�ܳ���
	u8	aInfo[0];			///��Ϣ���ݣ���GBK����
} T_SCMD_InfoService;

/** 8.31 �绰�ز� ��ϢID��0x8400 **********************************************/
typedef	struct
{
	u8	u8Flag;			///��־��0:��ͨͨ��;1:����
	u8	aPhoneNum[0];		///�绰���롣�20�ֽ�
} T_SCMD_ReverseDial;

/** 8.32 ���õ绰�� ��ϢID��0x8401 ********************************************/
///�绰����������
typedef	enum
{
	ePHONEBOOK_DELETE	= 0,	///ɾ���ն������д洢����ϵ��
	ePHONEBOOK_UPDATE 	= 1,	///���µ绰��(ɾ���ն������е�ȫ����ϵ�˲�׷����Ϣ�е���ϵ��)
	ePHONEBOOK_APPEND	= 2,	///׷�ӵ绰��
	ePHONEBOOK_CHANGE	= 3		///�޸ĵ绰��(����ϵ��Ϊ����)
}ePHONESETTYPE;

///�绰����ϵ�����
typedef	struct
{
	u8	u8PhoneNumTag;		///��־��1:����,2:����,3:����/����
	u8	u8PhoneNumLength;	///���볤��
	u8	aPhoneNum[0];		///�绰����
	u8	u8NameLength;		///��ϵ�˳���
	u8	aName[0];			///��ϵ��
}tPHONEBOOKITEM;

///���õ绰��
typedef	struct
{
	u8	u8SetType;						///��������
	u8	u8ConnPersonCount;				///��ϵ������
	tPHONEBOOKITEM	tPhoneBookItem[0];	///��ϵ����
} T_SCMD_SetPhoneBook;

/*************8.33 �������� ��ϢID��0x8500*************/
#define	VEHICLE_CTRL_UNLOCK		0	///���Ž���
#define	VEHICLE_CTRL_LOCK		1	///���ż���
///����������Ϣ��
typedef struct
{
	u8	bit_DoorLock			:	1;	/// 0�����Ž�����1�����ż���
	u8	bit_vechical_reserved	:	7;
} tMsg_SCMD_VechicalCtrl;

/*************8.34 ��������Ӧ�� ��ϢID��0x0500��*************/
///��������Ӧ��ṹ�嶨��
typedef	struct
{
	u16	u16SeqNum;		///Ӧ�����ˮ�ţ���Ӧ�������·���Ϣ����ˮ��
	tMsg_T_PositionInfo	tBasicPos;		///λ����Ϣ�㱨
} tMsg_TACK_VechicalCtrl;

/*************8.35 ����Բ������ ��ϢID��0x8600��*************/
///������������
typedef	enum
{
	eAREA_UPDATE	= 0,	///��������
	eAREA_APPEND 	= 1,	///׷������
	eAREA_CHANGE	= 2		///�޸�����
}eAREATYPE;

/**������������Զ���*/
typedef struct
{
	u16	Property_BaseOnTime					:	1;	///0 1������ʱ��
	u16	Property_LimitSpeed					:	1;	///1 1������
	u16	Property_Alarm2DriverOnEnterErea	:	1;	///2 1�������򱨾�����ʻԱ
	u16	Property_Alarm2ServerOnEnterErea	:	1;	///3 1�������򱨾���ƽ̨
	u16	Property_Alarm2DriverOnLeftErea		:	1;	///4 1�������򱨾�����ʻԱ
	u16	Property_Alarm2ServerOnLeftErea		:	1;	///5 1�������򱨾���ƽ̨
	u16	Property_LatFlag					:	1;	///6 0����γ��1����γ
	u16	Property_LongFlag					:	1;	///7 0��������1������
#ifdef JTT808_Ver_2011
	u16	Property_rerserved_8_15				:	8;	///9-13 ����
#endif
#ifdef JTT808_Ver_2013
	u16	Property_DoorOpenEnable				:	1;	///8 0�������ţ�1����ֹ����
	u16	Property_Rerserved_9_13				:	5;	///9-13 ����
	u16	Property_CommModuleEnableOnEnterArea	:	1;	///14 0����������ͨ��ģ�飻1��������ر�ͨ��ģ��
	u16	Property_GNSSDetailMsgEnableOnEnterArea	:	1;	///15 0�������򲻲ɼ�GNSS ��ϸ��λ���ݣ�1��������ɼ�GNSS ��ϸ��λ����
#endif///JTT808_Ver_2013
} tAreaProperty;

/**Բ��������ṹ�嶨��*/
typedef	struct
{
	u32	u32AreaId;		///����ID
	tAreaProperty	tAreaProperty;	///��������
	u32	u32CenterPtLatitude;	///���ĵ�γ��
	u32	u32CenterPtLongitude;	///���ĵ㾭��
	u32	u32Radius;		///�뾶����λΪm;·��Ϊ�ùյ㵽��һ�յ�
	tBCDTime tBcdStartTime;	///��ʼʱ�䣬����������0λΪ0��û�и��ֶ�
	tBCDTime tBcdEndTime;	///����ʱ�䣬����������0λΪ0��û�и��ֶ�
	u16	u16SpeedMax;	///����ٶȣ�km/h������������1λΪ0��û�и��ֶ�
	u8	u8OverSpeedContinuousTime;	///���ٳ���ʱ�䣬��λΪ�룬����������1λΪ0��û�и��ֶ�
} tItem_RoundArea;

///����Բ��������Ϣ�����ݸ�ʽ
typedef	struct
{
	u8	u8AreaProperty;		///Բ����������,eAREATYPE
	u8	u8AreaAmount;	///��������
///	u8	u8PackageAreaCount;		///���������
	tItem_RoundArea	atItems[0];	///������
} tMsg_SCMD_SetRoundArea;


/** 8.36 ɾ��Բ������ ��ϢID��0x8601��*****************************************/
typedef	struct
{
	/**���������������Ϣ�а��������������������125��������125�������ö�����Ϣ��0Ϊɾ����������*/
	u8	u8AreaCount;	/**��ɾ����������*/
	u32 au32AreaIdList[0];	///����ID1,...
} tMsg_SCMD_DeleteRoundArea;

/** 8.37 ���þ������� ��ϢID��0x8602��*****************************************/
/**����������ṹ�嶨��*/
typedef	struct
{
	u32	u32AreaId;			///����ID
	u16	u16AreaProperty;	///��������

	u32	u32LT_Lat;	///���ϵ�γ�� 	�Զ�Ϊ��λ��γ��ֵ����10 ��6 �η�����ȷ�������֮һ��
	u32	u32LT_Long;	/// ���ϵ㾭�� DWORD	�Զ�Ϊ��λ��γ��ֵ����10 ��6 �η�����ȷ�������֮һ��
	u32	u32RB_Lat;	/// 14 ���µ�γ�� DWORD�Զ�Ϊ��λ��γ��ֵ����10 ��6 �η�����ȷ�������֮һ��
	u32	u32RB_Long;	/// 18 ���µ㾭�� DWORD�Զ�Ϊ��λ��γ��ֵ����10 ��6 �η�����ȷ�������֮һ��

	tBCDTime tBcdStartTime;	///��ʼʱ�䣬����������0λΪ0��û�и��ֶ�
	tBCDTime tBcdEndTime;	///����ʱ�䣬����������0λΪ0��û�и��ֶ�
	u16	u16SpeedMax;		///����ٶȣ�km/h������������1λΪ0��û�и��ֶ�
	u8	u8KeepingOverSpeedTimeout;///���ٳ���ʱ�䣬��λΪ�룬����������1λΪ0��û�и��ֶ�
} tItem_RectangleArea;

///���þ���������Ϣ�����ݸ�ʽ
typedef	struct
{
	u8	u8AreaProperty;			///���þ�����������
	u8	u8AreaTotalCount;		///��������
	u8	u8PackageAreaCount;		///���������
	tItem_RectangleArea	atItems[0];	///������
} tMsg_SCMD_SetRectangleArea;

/** 8.38 ɾ���������� ��ϢID��0x8603��*****************************************/
///ɾ������������Ϣ�����ݸ�ʽ
typedef	struct
{
	u8	u8Amount;	///���������������Ϣ�а��������������������125����
	u32	au32Ids[0];		///����ID1,...,����IDn
} tMsg_SCMD_DeleteRectangleArea;

/** 8.39 ���ö�������� ��ϢID��0x8604��***************************************/
///����
typedef	struct
{
	u32	u32Lat;	///γ��
	u32	u32Long;///����
} tPOINT;

///#define	MAX_REGION_VERTEX_NUMBER	30

///�������·������ö����������Ϣ��
typedef struct
{
	u32	u32AreaId;	///����ID
	tAreaProperty tProperty;	///��������
	tBCDTime tBcdStartTime;		///��ʼʱ�䣬����������0λΪ0��û�и��ֶ�
	tBCDTime tBcdEndTime;	///����ʱ�䣬����������0λΪ0��û�и��ֶ�
	u16	u16SpeedMax;	///����ٶȣ�km/h������������1λΪ0��û�и��ֶ�
	u8	u8OverSpeedContinuousTime;///���ٳ���ʱ�䣬��λΪ�룬����������1λΪ0��û�и��ֶ�
	u16	u16AreaTotalVertexCount;	///�����ܶ�����
} tItem_PolygonArea;

typedef struct
{
	tItem_PolygonArea tHead;
	tPOINT	atPoints[0];	///������
} tMsg_SCMD_SetPolygonArea;

/** 8.40 ɾ����������� ��ϢID��0x8604��***************************************/
///ɾ�������������Ϣ�����ݸ�ʽ
typedef	struct
{
	u8	u8Amount;  /**���������������Ϣ�а��������������������125��������125��
												�����ö�����Ϣ��0Ϊɾ����������*/
	u32 au32Ids[0];	///����ID1
} tMsg_SCMD_DeletePolygonArea;

/** 8.41 ����·�� ��ϢID��0x8606��*********************************************/
///·�����Զ���
#define	ALARM_TIME					0x0001	///����ʱ��
#define	ALARM_2DRIVER_IN_LINE		0x0004	///��·�߱�������ʻԱ
#define	ALARM_2PLATFORM_IN_LINE		0x0008	///��·�߱�����ƽ̨
#define	ALARM_2DRIVER_OUT_LINE		0x0010	///��·�߱�������ʻԱ
#define	ALARM_2PLATFORM_OUT_LINE	0x0020	///��·�߱�����ƽ̨

///·�����Զ���
#define	ROADSECTION_DIRVE_TIME			0x01	///��ʻʱ��
#define	ROADSECTION_RESTRICT_SPEED		0x02	///����
#define	ROADSECTION_NORTH_LATITUDE		0x04	///0:��γ,1:��γ
#define	ROADSECTION_EAST_LONGITUDE		0x08	///0:����,1:����

///#define	MAX_INFLECTION_PT_NUMBER	20

///�յ����
typedef	struct
{
	u32 u32CornerId;	///�յ�ID
	u32	u32RoadSectionId;	///·��ID
	u32	u32Latitude;		///�յ�γ��
	u32	u32Longitude;		///�յ㾭��
    u8	u8RoadSectionWidth;		///·�ο�ȣ���λΪ�ף�·��Ϊ�ùյ㵽��һ�յ�
	u8	u8RoadSectionProperty;		///·������
	u16	u16TimeOverThreshold;	///·����ʻ������ֵ����λΪ�룬��·������0λΪ0��û�и��ֶ�
	u16	u16TimeNotEnoughThreshold;	///·����ʻ������ֵ����λΪ�룬��·������0λΪ0��û�и��ֶ�
	u16	u16SpeedMax;		///·������ٶȣ�km/h����·������1λΪ0��û�и��ֶ�
	u8	u8OverSpeedKeptTime;	///·�γ��ٳ���ʱ�䣬��λΪ�룬��·������1λΪ0��û�и��ֶ�
}tCORNERPOINT;

/** 8.42 ɾ��·�� ��ϢID��0x8607��*********************************************/
///ɾ��·����Ϣ�����ݸ�ʽ
typedef	struct
{
	u8	u8LineCount;	    /**·�߸�����������Ϣ�а��������������������125��������
											125�������ö�����Ϣ��0Ϊɾ����������*/
	u32	u32LineId[0];	///·��ID1
} tMsg_SCMD_DeleteLine;

/** 8.43 ��ʻ��¼���ݲɼ����� ��ϢID��0x8700��**********************************/
typedef	struct
{
	u8	u8Cmd;	///������
	u8*	pu8Data;	/**���ݿ� ���ݿ����ݸ�ʽ��GB/T 19056 ��������ݣ�����
										GB/T 19056 Ҫ����������ݰ�����Ϊ�ա�*/
} tMsg_SCMD_Recorder_CaptureData;


/** 8.44 ��ʻ��¼�����ϴ� ��ϢID��0x0700��*************************************/
typedef	struct
{
	u16	u16SeqNum;	///Ӧ����ˮ�� WORD ��Ӧ����ʻ��¼���ݲɼ�������Ϣ����ˮ��
	u8	u8Cmd;	///������
	u8*	pu8Data;	/**���ݿ� ���ݿ����ݸ�ʽ��GB/T 19056 ��������ݣ�����
										GB/T 19056 Ҫ����������ݰ�*/
} tMsg_TACK_Recorder_CaptureData;

/** 8.45 ��ʻ��¼�����´����� ��ϢID��0x8701��**********************************/
typedef	struct
{
	u8	u8Cmd;	///������
	u8*	pu8Data;	/**���ݿ� ���ݿ����ݸ�ʽ��GB/T 19056 ��������ݣ�����
										GB/T 19056 Ҫ����������ݰ�����Ϊ�ա�*/
} tMsg_SCMD_Recorder_DispatchParams;

/** 8.46 �����˵��ϱ� ��ϢID��0x0701��*****************************************/
typedef	struct
{
	u32	u32ETicketLen;	///�����˵�����
	u8*	pu8Data;	///�����˵����ݰ�
}tMsg_T_ETicketRpt;

/** 8.47 �ϱ���ʻԱ�����Ϣ���� ��ϢID��0x8702��********************************/  ///----------????????
///�ϱ���ʻԱ�����Ϣ������Ϣ��Ϊ�ա�


/** 8.48 ��ʻԱ�����Ϣ�ɼ��ϱ� ��ϢID��0x0702��********************************/
#ifdef JTT808_Ver_2011
///��ʻԱ�����Ϣ�ɼ��ϱ�
typedef	struct
{
	u8	u8DriverNameLen;	///��ʻԱ��������
	u8*	pau8DriverName;		///��ʻԱ����������n
	u8	au8DrvIdCode[20];	///��ʻԱ���֤����
	u8	au8QulCode[40];		///��ҵ�ʸ�֤����
	u8	u8GovNameLength;	///��֤�������Ƴ���
	u8*	pau8GovName;		///��֤��������
} tMsg_T_DriverInfoRpt;
#endif

#ifdef JTT808_Ver_2013
typedef	struct
{
	u8 state;	/** ״̬ BYTE 0x01����ҵ�ʸ�֤IC �����루��ʻԱ�ϰࣩ��
							0x02����ҵ�ʸ�֤IC ���γ�����ʻԱ�°ࣩ��*/
	u8 time_BCD[6];	///1 ʱ�� BCD[6] �忨/�ο�ʱ�䣬YY-MM-DD-hh-mm-ss��
/**�����ֶ���״̬Ϊ0x01 ʱ����Ч������䡣*/
	u8 IC_read_ret;	///IC ����ȡ��� BYTE
///						0x00��IC �������ɹ���
///						0x01������ʧ�ܣ�ԭ��Ϊ��Ƭ��Կ��֤δͨ����
///						0x02������ʧ�ܣ�ԭ��Ϊ��Ƭ�ѱ�������
///						0x03������ʧ�ܣ�ԭ��Ϊ��Ƭ���γ���
///						0x04������ʧ�ܣ�ԭ��Ϊ����У�����

/**�����ֶ���IC ����ȡ�������0x00 ʱ����Ч��*/
	u8	u8DriverNameLen;	///��ʻԱ��������
	u8*	pau8DriverName;		///��ʻԱ����������n
	u8	au8QulCode[20];		///��ҵ�ʸ�֤����,���㲹0
	u8	u8GovNameLength;	///��֤�������Ƴ���
	u8*	pau8GovName;		///��֤��������
	u8	LicieceValidation[4];	///֤����Ч�� BCD[4] YYYYMMDD
} tMsg_T_DriverInfoRpt;
#endif///JTT808_Ver_2013

/** 8.49 ��λ���������ϴ� ��ϢID��0x0704��*************************************/
typedef struct
{
	u16	u16ItemAmount;	///0 ��������� WORD ������λ�û㱨�����������>0
	u8	u8Type;		///1 λ���������� BYTE 0������λ�������㱨��1��ä������
	u16 u16ItemBlockLen;	/// λ�û㱨�����峤�� WORD λ�û㱨�����峤�ȣ�n
	tMsg_T_PositionInfo tPositionBlock;	///t2 λ�û㱨������ BYTE[n] �����8.12 λ����Ϣ�㱨
} tMsg_T_PositionInfoBatch;

#ifdef JTT808_Ver_2013
/** 8.50 CAN ���������ϴ� ��ϢID��0x0705��*************************************/
///CAN �������������ݸ�ʽ
typedef struct
{
	/**bit31 ��ʾCAN ͨ���ţ�0��CAN1��1��CAN2��
	bit30 ��ʾ֡���ͣ�0����׼֡��1����չ֡��
	bit29 ��ʾ���ݲɼ���ʽ��0��ԭʼ���ݣ�1����
	�������ƽ��ֵ��
	bit28-bit0 ��ʾCAN ����ID��
	*/
	u8 CAN_ID[4];	///CAN ID
	u8 CAN_DATA[8];	///CAN ����
} tCANItem;

///CAN ���������ϴ�
typedef struct
{
	u16 u16ItemAmount;	///0 ��������� WORD ������CAN ���������������>0
	u8	u8RevTime[5];	///2 CAN �������ݽ���ʱ�� BCD[5] ��1 ��CAN �������ݵĽ���ʱ�䣬hh-mm-ss-msms
	tCANItem tItem[0];	///CAN������
} tMsg_T_CANInfoUpload;
#endif///JTT808_Ver_2013


/** 8.51 ��ý���¼���Ϣ�ϴ� ��ϢID��0x0800��***********************************/
///��ý������
#define	MEDIA_TYPE_PHOTO		0	///ͼ��
#define	MEDIA_TYPE_AUDIO		1	///��Ƶ
#define	MEDIA_TYPE_VIDEO		2	///��Ƶ

///��ý���ʽ����
#define	MEDIA_STRUCT_JPEG		0	///JPEG
#define	MEDIA_STRUCT_TIF		1	///TIF
#define	MEDIA_STRUCT_MP3		2	///MP3
#define	MEDIA_STRUCT_WAV		3	///WAV
#define	MEDIA_STRUCT_WMV		4	///WMV

///�¼������
#define	MEDIA_EVT_PLATFORM			0	///ƽ̨�·�ָ��
#define	MEDIA_EVT_TIME				1	///��ʱ����
#define	MEDIA_EVT_ROBBER			2	///���ٱ�������
#define	MEDIA_EVT_CRUSH				3	///��ײ�෭��������
#ifdef JTT808_Ver_2013
#define	MEDIA_EVT_DOOROPEN_PIC		4	///�ſ����գ�
#define	MEDIA_EVT_DOOCLOSE_PIC		5	///�Ź����գ�
#define	MEDIA_EVT_DOOROPEN2CLOSE 	6	///�����ɿ���أ�ʱ�ٴӣ�20 ���ﵽ����20 ���
#define	MEDIA_EVT_DIST_PIC			7	///�������գ���������
#endif///JTT808_Ver_2013

typedef	struct
{
	u32	u32MediaDataId;		///��ý������ID
	u8	u8MediaType;			///��ý������
	u8	u8MediaFormatCode;		///��ý���ʽ����
	u8	u8EventCode;		///�¼������
	u8	u8ChannelId;		///ͨ��ID
} tMsg_T_Media_EvtUpload;

/** 8.52 ��ý�������ϴ� ��ϢID��0x0801��***************************************/
typedef	struct
{
	u32	u32MediaDataId;	///��ý������ID
	u8	u8MediaType;		///��ý������
	u8	u8MediaFormatCode;		///��ý���ʽ����
	u8	u8EventCode;	///�¼������
	u8	u8ChannelId;	///ͨ��ID
#ifdef JTT808_Ver_2013
	tMsg_Position_BaseInfo tPositionBaseInfo;	///����λ����Ϣ
#endif///JTT808_Ver_2013
	u8	Data[0];	///��ý�����ݰ�
} tMsg_T_Media_DataUpload;

/** 8.53 ��ý�������ϴ�Ӧ�� ��ϢID��0x8800��***********************************/
#ifdef JTT808_Ver_2011
///��ý�������ϴ�Ӧ��
typedef	struct
{
///	u16	u16ResponsePackId;	///��Ӧ��ƽ̨��Ϣ����ˮ��
	u32	u32MediaDataId;	///��ý������ID
	u8	u8ReSendPackAmount;	///�ش�������
	u8	IDList[0];		///�ش���ID�б�������125��޸��ֶ���������յ�ȫ�����ݰ�
} tMsg_SACK_Media_AckData;
#endif

#ifdef JTT808_Ver_2013
/**
��ý�������ϴ�Ӧ��
ע���Դ���Ϣ��Ӧ��Ӧ����0x0801 ��Ϣ���ش���ID �б��еķְ��ط�һ�Σ���ԭʼ�ְ�
��Ϣ��ȫһ�¡�
*/
typedef	struct
{
	u32	u32MediaDataId;	///��ý��ID DWORD >0�����յ�ȫ�����ݰ���û�к����ֶ�
	u8	u8ReSendPackAmount;	///�ش������� BYTE n
	u8	IDList[0];	///�ش���ID �б� BYTE[2*n]	�ش������˳�����У��硰��ID1 ��ID2......��IDn��
} tMsg_SACK_Media_AckData;
#endif///JTT808_Ver_2013

/** 8.54 ����ͷ������������ ��ϢID��0x8801��***********************************/
///�ֱ��ʶ���
#define	PIC_RESOLVE_320_240		0x01
#define	PIC_RESOLVE_640_480		0x02
#define	PIC_RESOLVE_800_600		0x03
#define	PIC_RESOLVE_1024_768	0x04
#define	PIC_RESOLVE_176_144		0x05
#define	PIC_RESOLVE_352_288		0x06
#define	PIC_RESOLVE_704_288		0x07
#define	PIC_RESOLVE_704_576		0x08

typedef	struct
{
	u8	u8ChannelId;	///ͨ��ID
	u16	u16Cmd;		///�������0:��ʾֹͣ����;0xfffff��ʾ¼��������ʾ��������
	u16	u16PicIntervalOrRecordTime;		///���ռ������¼��ʱ��
	u8	u8SaveTag;		///�����־��1:���棬0:ʵʱ�ϴ�
	u8	u8Resolve;		///�ֱ���
	u8	u8AVQuality;	///ͼ��/��Ƶ������1����������ʧ��С��10��ʾѹ�������
	u8	u8Brightness;	///����
	u8	u8Contrastness;		///�Աȶ�
	u8	u8Saturation;	///���Ͷ�
	u8	u8Chrome;	///ɫ��
} tMsg_SCMD_InstantPhoto;
/**�ն�����֧��ϵͳҪ��ķֱ��ʣ���ȡ��ӽ��ķֱ������㲢�ϴ�*/

#ifdef JTT808_Ver_2013
/** 8.55 ����ͷ������������Ӧ�� ��ϢID��0x0805��********************************/
///����������Ӧ���������·�������ͷ������������0x8801��
typedef struct
{

	u16 u16SeqNum;	///0 Ӧ����ˮ�� WORD ��Ӧƽ̨����ͷ���������������Ϣ��ˮ��
	u8	u8Result;	///2 ��� BYTE 0���ɹ���1��ʧ�ܣ�2��ͨ����֧�֡�
/**�����ֶ��ڽ��=0 ʱ����Ч��*/
	u16 u16MediaIdAmount;	///3 ��ý��ID ���� WORD n������ɹ��Ķ�ý�����
	u32 u32MediaId[0];	///4 ��ý��ID �б� BYTE[4*n]
} tMsg_TACK_InstantlyTakePhone;
#endif///JTT808_Ver_2013

/** 8.56 �洢��ý�����ݼ��� ��ϢID��0x8802��***********************************/
///ע������ʱ�䷶Χ����ʼʱ��/����ʱ�䶼��Ϊ00-00-00-00-00-00��
typedef	struct
{
	u8 u8MediaType;	///��ý������0��ͼ��1����Ƶ��2����Ƶ
	u8 u8ChannelId;	///ͨ��ID��ʾ������ý�����͵�����ͨ��
	u8 u8EventCode;	/**�¼������0��ƽ̨�·�ָ�1����ʱ������2�����ٱ�����
											����3����ײ�෭������������������*/
	tBCDTime tBcdStartTime;	///��ʼʱ��,yy-mm-dd-hh-mm-ss
	tBCDTime tBcdEndTime;	///����ʱ��,yy-mm-dd-hh-mm-ss
} tMsg_SCMD_MediaDataRetrieve;

/** 8.57 �洢��ý�����ݼ���Ӧ�� ��ϢID��0x0802��********************************/
///��ý����������ݸ�ʽ
typedef		struct
{
	u8 u8MediaType;		///��ý������
	u8 u8ChannelId;		///ͨ��ID
	u8 u8EventCode;		///�¼������
	tMsg_T_PositionInfo	tPosInfo;	///λ����Ϣ�㱨����ʾ�����¼�Ƶ���ʼʱ�̵Ļ㱨��Ϣ
} tMediaIndexItem;

typedef	struct
{
	u16	u16SeqNum;		///��Ӧ��ƽ̨��Ϣ����ˮ��
	u16	u16MediaItemAmount;		///��ý�����ݰ�������
	tMediaIndexItem	IndexItem;	///������
} tMsg_TACK_StoredMediaDataRetrieve;

/** 8.58 �洢��ý�������ϴ����� ��ϢID��0x8803 *********************************/
///�洢��ý�������ϴ�����
typedef	struct
{
	u8 u8MediaType;		///��ý������
	u8 u8ChannelId;		///ͨ��ID
	u8 u8EventCode;		///�¼������
	tBCDTime tBcdStartTime;		///��ʼʱ��,yy-mm-dd-hh-mm-ss
	tBCDTime tBcdEndTime;	///����ʱ��,yy-mm-dd-hh-mm-ss
	u8 u8DeleteTag;		///ɾ����־��0:������1:ɾ��
} tMsg_SCMD_MediaDataUpload;

/** 8.59 ¼����ʼ���� ��ϢID��0x8804*******************************************/
typedef	struct
{
	u8 u8RecordCmd;		///0:ֹͣ¼����1:��ʼ¼��
	u16	u16RecordTime;	///¼��ʱ�䣬��λΪ�룬0��ʾһֱ¼��
	u8 u8SaveTag;	///�����־��0:ʵʱ�ϴ���1:����
	u8 u8SampleRate;	///��Ƶ�����ʣ�0:8K; 1: 11K;2: 23K; 3:32K;��������
} tMsg_SCMD_RecordAudio;


#ifdef JTT808_Ver_2013
/** 8.60 �����洢��ý�����ݼ����ϴ����� ��ϢID��0x8805**************************/
typedef struct
{
	u32 u32MediaId;		///��ý��ID DWORD >0
	u8	u8DeleteFlag;	///ɾ����־ BYTE 0��������1��ɾ����
} tMsg_SCMD_SingleStoredMediaUpload;
#endif///JTT808_Ver_2013


/** 8.61 ��������͸�� ��ϢID�� 0x8900��*****************************************/		/////////////////////////????????????δ����͸�����ݸ�ʽ
#ifdef JTT808_Ver_2013
/**��������͸����Ϣ�����ݸ�ʽ*/
#define TRANSPARENTMSGTYPE_GNSS		0x00	///GNSS ģ����ϸ��λ���� 0x00 GNSS ģ����ϸ��λ����
#define TRANSPARENTMSGTYPE_IC		0x0B	/**��·����֤IC ����Ϣ 0x0B ��·����֤IC ����Ϣ�ϴ���ϢΪ64Byte��
												�´���ϢΪ24Byte����·����֤IC����֤͸����ʱʱ��Ϊ30s����ʱ�󣬲��ط���*/
#define TRANSPARENTMSG_UART1		0x41	///0x41 ����1 ͸����Ϣ
#define TRANSPARENTMSG_UART2		0x42	///0x42 ����2 ͸����Ϣ
///�û��Զ���͸�� 0xF0-0xFF �û��Զ���͸����Ϣ
#endif///JTT808_Ver_2013

typedef	struct
{
	u8	u8MsgType;	///͸����Ϣ����
	u8	Data[0];	///͸����Ϣ����
} tMsg_SCMD_DownloadTransparentData;


/** 8.62 ��������͸�� ��ϢID�� 0x0900��*****************************************/	/////////////////////////????????????δ����͸�����ݸ�ʽ
typedef	struct
{
	u8	u8MsgType;	///͸����Ϣ����
	u8	Data[0];	///͸����Ϣ����
} tMsg_T_UploadTransparentData;

/** 8.63 ����ѹ���ϱ� ��ϢID��0x0901��*****************************************/
///ѹ����Ϣ��Ϊ��Ҫѹ������Ϣ����GZIP ѹ���㷨�����Ϣ
typedef	struct
{
	u32	u32CompressMsgLength;	///ѹ����Ϣ����
	u8	Data[0];	///ѹ����Ϣ��
} tMsg_T_UploadCompressedMsg;

/** 8.64 ƽ̨RSA ��Կ ��ϢID��0x8A00��*****************************************/
///ƽ̨RAS��Կ
typedef	struct
{
	u32	u32RAS_e;	///ƽ̨RAS��Կe,n�е�e
	u8	RAS_n[128];	///ƽ̨RAS��Կe,n�е�n
} tMsg_SCMD_RAS_PlatformPublicKey;

/** 8.65 �ն�RSA ��Կ ��ϢID��0x0A00��*****************************************/
///�ն�RAS��Կ
typedef	struct
{
	u32	u32E;	///�ն�RAS��Կe,n�е�e
	u8	au8Data[128];	///�ն�RAS��Կe,n�е�n
} tMsg_T_RAS_TerminalPublicKey;

/** �������� ******************************************************************/
void MSGID_TCPIPData_Process(u8 *pMsg, u16 msgLen);
void MSGID_ServerAckProc(tMsg_S_GeneralAck *pSAck);


#pragma pack()

#endif /** PROTOCOL_H_ */
