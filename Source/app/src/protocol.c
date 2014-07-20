/*
 * protocol.c
 *
 *  Created on: 2013-12-24
 *      Author: gaozhengdong
 */
#include <stm32f2xx_conf.h>
#include "protocol.h"
#include "qevents.h"
#include "config.h"
#include "trace.h"
#include <qassert.h>
#include <parameter.h>
#include <param_formatmsg.h>
#include <qp_port.h>
#include <utility.h>
#include <qactiveobjs.h>
#include <param_area.h>
#include <bsp.h>
#include "qassert.h"
#include <qevt.h>
#include <type.h>

Q_DEFINE_THIS_MODULE("protocol.c")
///������� #include <qevt.h>��������뱨��

static const u32 param_id[] = {
	///������ID����
	PARAM_ALIVE_INTERVAL,	///		0x0001		///�ն��������ͼ������λΪs
	PARAM_TCPACK_TIMEOUT,	///				0x0002		///TCPӦ��ʱ����λΪs
	PARAM_TCP_RESENDTIMES,	///				0x0003		///TCP��Ϣ�ش�����
	PARAM_UDPACK_TIMEOUT,	///				0x0004		///UDPӦ��ʱ����λΪs
	PARAM_UDP_RESENDTIMES,	///				0x0005		///UDP��Ϣ�ش�����
	PARAM_SMSACK_TIMEOUT,	///				0x0006		///SMSӦ��ʱ����λΪs
	PARAM_SMS_RESENDTIMES,	///				0x0007		///SMS��Ϣ�ش�����
	/**0X0008 ----0X000F ����*/
	PARAM_MAINSRV_APN,	///					0x0010		///��������APN������ͨ�Ų��ŷ��ʵ�
	PARAM_MAINSRV_USR,	///					0x0011		///������������ͨ�Ų����û���
	PARAM_MAINSRV_PSWD,	///					0x0012		///������������ͨ�Ų�������
	PARAM_MAINSRV_IP,	///					0x0013		///����������ַ��IP������
	PARAM_VICESRV_APN,	///					0x0014		///���ݷ�����APN������ͨ�Ų��ŷ��ʵ�
	PARAM_VICESRV_USR,	///				0x0015		///���ݷ���������ͨ�Ų����û���
	PARAM_VICESRV_PSWD,	///					0x0016		///���ݷ���������ͨ�Ų�������
	PARAM_VICESRV_IP,	///					0x0017		///���ݷ�������ַ��IP������
	PARAM_TCP_PORT,	///						0x0018		///TCP�˿�
	PARAM_UDP_PORT,	///						0x0019		///UDP�˿�
	/**0X001A ----0X001F ����*/
#ifdef JTT808_Ver_2013	///�°�����Э�鲿��
	PARAM_ICVERIFY_MS_IPOrDNS,	///			0x001A		///��·����֤IC����֤��������IP��ַ������
	PARAM_ICVERIFY_TCPPORT,	///				0x001B		///��·����֤IC����֤��������TCP�˿�
	PARAM_ICVERIFY_UDPPORT,	///				0x001C		///��·����֤IC����֤��������UDP�˿�
	PARAM_ICVERIFY_BS_UDPPORT,	///			0x001D		///��·����֤IC�����ݷ�����IP��ַ���������˿�ͬ��������
#endif///JTT808_Ver_2013

	/**�ն�λ�û㱨���ԣ�0:��ʱ�㱨��1:����㱨��2:��ʱ�Ͷ���㱨*/
	PARAM_POSRPT_STRATEGE,	///			0x0020
	/**�ն˻㱨������0:����ACC״̬��1:���ݵ�¼״̬��ACC״̬�����жϵ�¼״̬��
	 ����¼�ٸ���ACC״̬*/
	PARAM_POSRPT_METHOD,	///					0x0021
	PARAM_DRV_UNREG_RPTPERIOD,	///			0x0022		///��ʻԱδ��¼�㱨ʱ��������λΪ��
	/**0X0023 ----0X0026 ����*/
	PARAM_RPTPERIOD_ONSLEEP,	///				0x0027		///����ʱ�㱨ʱ��������λΪ��
	PARAM_RPTPERIOD_URGENT,	///				0x0028		///��������ʱ�㱨ʱ��������λΪ��
	PARAM_RPTPERIOD_DEFAULT,	///			0x0029		///ȱʡʱ�㱨ʱ��������λΪ��
	/**0X002A ----0X002B ����*/
	PARAM_RPTDIST_DEFAULT,	///				0x002C		///ȱʡʱ����㱨�������λΪ��
	PARAM_RPTDIST_DRV_UNREG,	///				0x002D		///��ʻԱδ��¼����㱨�������λΪ��
	PARAM_RPTDIST_SLEEP,	///					0x002E		///����ʱ����㱨�������λΪ��
	PARAM_RPTDIST_URGENT,	///				0x002F		///��������ʱ����㱨�������λΪ��
	PARAM_INFLECTION_POINT_ANGLE,	///		0x0030		///�յ㲹���Ƕȣ���λΪ��(< 180)
	PARAM_BARRIER_RADIUS,	///				0x0031		///����Χ���뾶(�Ƿ���λ��ֵ)����λΪ��
	/**0X0032----0X003F ����*/
	PARAM_MONITORCENTER_PHONENUM,	///		0x0040		///���ƽ̨�绰����
	PARAM_RESET_PHONENUM,	///		0x0041		///��λ�绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˸�λ
	///�ظ��������õ绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˻ָ���������
	PARAM_RECOVERFACTORY_PHONENUM,	///		0x0042
	PARAM_MONITORCENTER_SMS_NUM,	///			0x0043		///���ƽ̨SMS�绰����
	PARAM_RECVTXTALARM_SMS_NUM,	///			0x0044		///�����ն�SMS�ı������绰����
	///�ն˵绰�������ԣ�0:�Զ���ͨ��1:ACC ONʱ�Զ���ͨ��OFFʱ�ֶ�����
	PARAM_ANSWERCALLING_STRATEGE,	///		0x0045
	///�ն�ÿ��ͨ��ʱ�ͨ��ʱ�䣬��λΪ�룬0Ϊ������ͨ����0xffffffffΪ������
	PARAM_TOUT_PERCALL,	///					0x0046
	///�ն�ÿ���ͨ��ʱ�䣬��λΪ�룬0Ϊ������ͨ����0xffffffffΪ������
	PARAM_TOUT_PERMONTHCALL,	///				0x0047
	PARAM_MONITORLISTEN_PHONENUM,	///		0x0048			///�����绰����
	PARAM_SUPERVISE_SMS_NUM,	///				0x0049			///���ƽ̨��Ȩ���ź���
	/**0X004A----0X004F ����*/

	///���������֣���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����������
	PARAM_ALARM_MASK,	///					0x0050
	///���������ı�SMS���أ���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ�����ı�SMS
	PARAM_SMSCTRL_ONALARM,	///				0x0051
	///�������㿪�أ���λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ����ͷ����
	PARAM_PICCTRL_ONALARM,	///				0x0052
	///��������洢��־����λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����ʱ�ĵ���Ƭ���д洢������ʵʱ�ϴ�
	PARAM_PICCTRL_STORE_ONALARM,	///			0x0053
	///�ؼ���־����λ����Ϣ�㱨��Ϣ�еı�����־λ���Ӧ����ӦλΪ1����Ӧ����Ϊ�ؼ�����
	PARAM_KEYALARM_FLAG,	///		 			0x0054
	PARAM_MAXSPEED,	///						0x0055	   ///����ٶȣ���λΪ����ÿСʱ(km/h)
	PARAM_OVERSPEED_LASTING_TIMEOUT,	///			0x0056	///���ٳ���ʱ�䣬��λΪ��
	PARAM_CONTINUOUS_DRV_TIMEOUT,	///			0x0057	///������ʻʱ�����ޣ���λΪ��
	PARAM_DRV_TOTAL_EACHDAY_TIMEOUT,	///			0x0058	///�����ۼƼ�ʻʱ�䣬��λΪ��
	PARAM_LEAST_REST_TIMEOUT,	///				0x0059	///��С��Ϣʱ�䣬��λΪ��
	PARAM_MAX_STOPPING_TIMEOUT,	///				0x005A	///�ͣ��ʱ�䣬��λΪ��
	/**0X005B----0X006F */
#ifdef JTT808_Ver_2013
	PARAM_OVERSPEED_PREALARM_DIF,	///		0x005B	///���ٱ���Ԥ����ֵ����λ1/10Km/h
	PARAM_FATIGE_PREALARM_DIF,	///			0x005C	///ƣ�ͼ�ʻԤ����ֵ����λΪ�룬>0
	///b7-b0:��ײʱ�䣬��λ4ms��b15-b8:��ײ���ٶȣ���λ0.1g�����÷�Χ0~79��Ĭ��10
	PARAM_COLLISION_SETTING,	///				0x005D
	///�෭�����������ã��෭�Ƕȣ���λ1�ȣ�Ĭ��30��
	PARAM_SIDECOLLAPSE_SETTING,	///			0x005E
	/**0X005F----0X0063 */
	PARAM_PERIODIC_PHOTO_CTRL,	///			0x0064	///��ʱ���տ���
	PARAM_PERDIST_PHOTO_CTRL,	///			0x0065	///�������տ���
	/**0X0066----0X006F */
#endif///JTT808_Ver_2013

	PARAM_VIDEO_QUALITY,	///					0x0070		///ͼƬ/��Ƶ������1-10,1���
	PARAM_BRIGHTNESS,	///					0x0071		///���ȣ�0-255
	PARAM_CONTRASTNESS,	///					0x0072		///�Աȶȣ�0-127
	PARAM_SATURATION,	///					0x0073		///���Ͷȣ�0-127
	PARAM_CHROMA,	///						0x0074		///ɫ�ȣ�0-255
	/**0X0075----0X007F*/
	PARAM_VEHICLE_KILOMETRES,	///			0x0080		///������̱������1/10km
	PARAM_VEHICLE_PROVINCE_ID,	///			0x0081		///�������ڵ�ʡ��ID
	PARAM_VEHICLE_CITY_ID,	///				0x0082		///�������ڵ�����ID
	PARAM_VEHICLE_ID,	///	 				0x0083		///���ƺ���
	PARAM_VEHICLE_ID_COLOR,	///				0x0084		///������ɫ

#ifdef JTT808_Ver_2013
    PARAM_GNSS_MODE,	///			0x0090
	PARAM_GNSS_BOUNDRATE,	///		0x0091
	PARAM_GNSS_DMSG_OUTFREQ,	///		0x0092
	PARAM_GNSS_DMSG_RETRIEVEFREQ,	///		0x0093
	PARAM_GNSS_DMSG_RPTTYPE,	///		0x0094
	PARAM_GNSS_DMSG_RPTSETTING,	///		0x0095
	PARAM_CAN_CH1_RETRIEVEINTERVAL,	///		0x0100
	PARAM_CAN_CH1_UPLOADINTERVAL,	///		0x0101
	PARAM_CAN_CH2_RETRIEVEINTERVAL,	///		0x0102
	PARAM_CAN_CH2_UPLOADINTERVAL,	///		0x0103
	PARAM_CAN_SOLORETRIEVE,	///		0x0110
	PARAM_CAN_SOLORETRIEVE_OTHER,	///		0x0111
#endif

};


#define PARAM_AMOUNT	(sizeof(param_id) >> 2)	///��������
/**���ָ�����**************************************************************************//**///�ն˿����࣬���������ֶΣ���';'����#define GET_TCTRL_SEGMENT(msg, param)							\
	do {														\
		u8 len = get_next_segment(msg, param);					\
		param[len] = '\0';										\
	}while(0)*/

///�����ַ���ת����U16
#define CONVERT_STRING_2_U16(str_buf, str_len, add)				\
	do {														\
		u8 i = 0;												\
		u16 _val = 0;											\
		while(i < str_len)										\
		{														\
			_val = _val * 10 + (str_buf[i] - 0x30);				\
			i++;												\
		}														\
		*(add) = _val;											\
	}while(0)

///�������
#define ERROR_HANDLE(iRet)					\
							do {			\
							if(iRet < 0)	\
							{						\
								ERROR_Handle(iRet);	\
								return;		\
							}				\
							}while(0)

/** ���غ������� ***********************************************************************/
static u16 EnCodeData(u8* const pu8Data, u16 u16Length, u8* pu8DestData);
static u16 DeCodeData(const u8* pu8Data, u16 u16Length, u8* pu8DestData);
static u8 CalculateXorChecksum(u8* pData, u16 u16Length);
static u16 GenerateCirculateSeqId(void);
static BOOL CheckFrame(u8 *pMsg, u16 u16Len);
static int map_param_id_2_index(u8 *pArr, u8 len, u32 id);
static u32 map_index_2_param_id(u8 row, u8 column);
static u8 get_next_segment(char *msg, char *seg_buf);
static u16 Protocol_FormSysParams(u8* pCursor, u16 seq);
static void Protocol_ServerGeneralAckProc(tMsg_S_GeneralAck* pMsgBlock);
static void Protocol_T_SendGeneralAck(u16 u16ResMsgId, u16 u16ResSeqId, u8 ret);
static void ERROR_Handle(int iRet);
static u16 Protocol_GetPositionInfo(u8 *pBuf);
/*********************************************************************************/

static u16 __inline__ GET_TCTRL_SEGMENT(u8 *msg, u8 *param) {
	u8 len = get_next_segment((char*)msg, (char*)param);
	param[len] = '\0';
	return len + 1;
}

/**��ϢͷС��ת���ɴ��*/
void __inline__ ENDIAN_MSGHEAD_LITTLE2BIG(tMSGHEAD *pMsg) {
	ENDIAN_U16(pMsg->u16MsgId);
	ENDIAN_U16(*((u16* )(&pMsg->tMsgProperty)));
	ENDIAN_U16(pMsg->u16MsgCircularId);
}

/**��Ϣͷ���ת����С�ˣ�ͬС�˵����ת����ͬ*/
#define ENDIAN_MSGHEAD_BIG2LITTLE	ENDIAN_MSGHEAD_LITTLE2BIG



#if 0
/**�ն˷���ͨ������Ӧ����Ϣ*/
#define Protocol_T_SendGeneralAck(u16ResMsgId, u16ResSeqId, ret)	\
	do {											\
		tMsg_S_GeneralAck tAck;						\
		tAck.u16RespSeqId = u16ResMsgId;				\
		tAck.u16RespMsgId = u16ResSeqId;				\
		tAck.eResult = ret;							\
													\
		Protocol_ServerGeneralAckProc(&tAck);		\
	}while(0)
#endif

/**
 * �ն��������ͨ��Э�����ݴ�������������·�����
 *
 * @param pMsg		Э������
 * @param msgLen	Э�����ݳ���
 */
void Protocol_TCPIPData_Process(u8 *pMsg, u16 msgLen) {
	tMSGHEAD *pHead;
	u8 *pData;
	u8 *pMsgBlock;  ///��Ϣ��
	u16 u16MsgBlockLen = 0; ///��Ϣ�峤��
	u16 u16TransMsgLen = 0;	///ת�����Ϣ����
	int iRet = 0;
	TaskEvt *pe = NULL;

	static u8 aTransMsg[CONFIG_TCP_RXBUF_MAXLEN];	///ת����Ϣ����

	if ((pMsg == NULL) || (msgLen == 0))
		return;

	/**ת�崦��,������Ϣʱ��ת�廹ԭ����>��֤У���롪��>������Ϣ��*/
	/**��ת�廹ԭ*/
	u16TransMsgLen = DeCodeData(pMsg, msgLen, aTransMsg);

	/**������У��*/
	if (FALSE == CheckFrame(aTransMsg, u16TransMsgLen)) {
		TRACE_(QS_USER, NULL, " ERR! Msg check failed! ");
		return;
	}

	/**���½�����Ϣ����*/
	pHead = (tMSGHEAD *) pMsg;

	/**��Ϣͷ�ֶδ�С��ת��*/
	ENDIAN_U16(pHead->u16MsgId);
	ENDIAN_U16(*((u16* )(&pHead->tMsgProperty)));
	ENDIAN_U16(pHead->u16MsgCircularId);

	u16MsgBlockLen = pHead->tMsgProperty.msg_len; ///��Ϣ�峤��
	pMsgBlock = pMsg + sizeof(tMSGHEAD); ///ָ����Ϣ������

	/**�����Ϣ�Ƿ�ְ�������������������4B��Ϣ�ְ�����ͷ��������Ϣ��*/
	if (pHead->tMsgProperty.msg_subpack) {
		pMsgBlock += 4;
	}

	TRACE_(QS_USER, NULL, "[GET MSG]: ");
	TRACE_RAW_BUFFER(pMsg, msgLen);	///��ӡԭʼ����

	switch (pHead->u16MsgId) {
	/**�ն�ͨ��Ӧ����Ϣ*/
//	case	MSGID_T_GeneralAck: 				//0x0001
//
//		///�ն������Ϸ���Ϣ����������ڴ˴�������
//		break;
	/**ƽ̨ͨ��Ӧ����Ϣ*/
	case MSGID_S_GeneralAck: 				//0x8001
		Protocol_ServerGeneralAckProc((tMsg_S_GeneralAck*) pMsgBlock);
		break;

#ifdef JTT808_Ver_2013
		//�����ְ�����
	case MSGID_S_ReloadSubPack: 				//0x8003

		break;
#endif

		//�ն�����
	case MSGID_T_Heatbeat: 				//0x0002

		break;

		//�ն�ע��
//	case	MSGID_T_Register: 				//0x0100
//		break;

		//�ն�ע��Ӧ��
	case MSGID_S_RegisterAck: 				//0x8100
	{
		tMsg_S_Ack_Register *pInfo = (tMsg_S_Ack_Register *) pMsgBlock;
		switch (pInfo->eRet) {
		case REG_SUCCESS: 				///	= 0,	///�ɹ�/ȷ��
			/**������Ȩ��*/
			memcpy_(ptParam_Runtime->sAuthorCode, pMsgBlock + sizeof(tMsg_S_Ack_Register),
					strlen_((char*) (pMsgBlock + sizeof(tMsg_S_Ack_Register)))); ///STRING�����Ƿ���'\0'��β��

			PARAM_SaveSysParam(PARAM_RUNTIME); ///�������
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, SACK_TERMREG_OK_SIG), (void *)0); ///����ע��ɹ���Ϣ
			break;
		case REG_VECHICLEREGED: ///	= 1,	///�����Ѿ�ע��
		case REG_NOVEHICLE: ///		= 2,	///���ݿ����޴˳���
		case REG_TERMINALREGED: ///	= 3,	///�ն��Ѿ�ע��
		case REG_NOTERMINAL: ///		= 4,	///���ݿ����޴��ն�
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, SACK_TERMREG_ERR_SIG), (void *)0); ///����ע��ʧ����Ϣ
			break;
		default:
			break;
		}
	}
		break;

//	//�ն�ע��
//	case	MSGID_T_Unregister: 				//0x0003
//
//		break;
//
//	//�ն˼�Ȩ
//	case	MSGID_T_Authentication: 				//0x0102
//
//		break;

		//�����ն˲���
	case MSGID_S_SetParam: 				//0x8103
	{
		u8 i = 0;
		u8 u8ParamAmt = 0;
		tPARAMITEMHEAD *pItem;
		tMsg_SCMD_SetParam *pParam;
		TaskEvt *pe;

		TRACE_(QS_USER, NULL, "[CMD]: SetParam ");

		pParam = (tMsg_SCMD_SetParam*) pMsgBlock;

		/**ѭ�����ò�����*/
		u8ParamAmt = ((tMsg_SCMD_SetParam*) pMsgBlock)->u8Amount;
		pMsgBlock += 1;	///�������������ֽڣ�ָ��������б�
		pItem = (tPARAMITEMHEAD*) pMsgBlock; ///ָ�������
		while (i < pParam->u8Amount)	///pParam->u8Amount: ��������
		{
			pMsgBlock += 5;	///��������5���ֽڣ�ָ���������
			ENDIAN_U32(pItem->u32Id);	///IDת��С��
			iRet = Set_SystemParam(pItem->u32Id, pItem->u8Len, pMsgBlock); ///д����
			if (iRet < 0) {
				TRACE_(QS_USER, NULL, "## param setting err! ");
				Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, RET_ERROR);///�ϱ�������Ϣ
				PARAM_LoadSysParam(PARAM_SYSTEM); ///��Ƭ��洢�������������
				return;
			}
			pMsgBlock += pItem->u8Len;
			pItem = (tPARAMITEMHEAD*) pMsgBlock; ///ָ�������
			i++;
		}

		PARAM_SaveSysParam(PARAM_SYSTEM);	///����д��Ƭ��洢��

		/**��������Ӧ�����¼�������*/
		pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);
		pe->cmd = MSGID_T_GeneralAck;
		pe->resCmd = MSGID_S_SetParam;
		pe->sequence = GenerateCirculateSeqId();
		pe->resSeqence = pHead->u16MsgCircularId;
		pe->ret = ((iRet == 0) ? RET_SUCCESS : RET_FAIL);

		QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
	}
		break;

		//��ѯ�ն˲���
	case MSGID_S_QueryParam: 				//0x8104
	{
		TRACE_(QS_USER, NULL, "[CMD]: QueryParam ");

		/**��������Ӧ�����¼�������*/
		pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);
		pe->cmd = MSGID_T_QueryParamAck;
		pe->resCmd = MSGID_S_QueryParam;
		pe->sequence = GenerateCirculateSeqId();
		pe->resSeqence = pHead->u16MsgCircularId;

		QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
	}
		break;

#ifdef JTT808_Ver_2013
		//��ѯָ���ն˲���
	case MSGID_T_QuerySpecifyParam: 			//0x8106
	{
		/**��ѯָ���ն˲�����Ϣ�����ݸ�ʽ����15���ն˲���0x0104ָ��Ӧ��*/
		/**0 �������� BYTE ��������Ϊn
		 1 ����ID �б� BYTE[4*n]����˳�����У��硰����ID1 ����ID2......����IDn����*/
		u8 u8ParamCnt = 0;
		u8 i = 0;

		TRACE_(QS_USER, NULL, "[CMD]: QuerySpecifyParam ");

		TaskEvt *pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);

		/**����ձ������*/
		memset_(pe->U.arr, 0, sizeof(pe->U.arr));

		u8ParamCnt = *pMsgBlock++;	///��������,1B
		Q_ASSERT((u8ParamCnt * 4) == (pMsg - pMsgBlock));	///�������������ֽ���һ��

		/**��ȡ����ָ��������ת���ɱ������*/
		while (i < u8ParamCnt) {
			u32 id = *(u32*) pMsgBlock;
			ENDIAN_U32(id);
			map_param_id_2_index(pe->U.arr, sizeof(pe->U.arr), id);	///ӳ�����ID->��־λ
			pMsgBlock += 4;
			i++;
		}

		pe->cmd = MSGID_T_QueryParamAck;
		pe->resCmd = MSGID_T_QuerySpecifyParam;
		pe->sequence = GenerateCirculateSeqId();
		pe->resSeqence = pHead->u16MsgCircularId;

		QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
	}

		break;

#endif
		//��ѯ�ն˲���Ӧ��
	case MSGID_T_QueryParamAck: 				//0x0104
		break;

		//�ն˿���
	case MSGID_S_Control: 				//0x8105
	{
		TRACE_(QS_USER, NULL, "[CMD]: Control ");

		u8 u8CmdType = *pMsgBlock++;	///������
		Q_ASSERT((u8CmdType <= 7) && (u8CmdType >= 1));
		if (!((u8CmdType <= 7) && (u8CmdType >= 1)))
			TRACE_(QS_USER, NULL, "ret = ERR, reason: param invalid.");

		u16MsgBlockLen--;

		switch (u8CmdType) {
		case CMD_WIRELESS_UPG:	///			= 1,	///��������
			/**	��������������֮����ð�Ƿֺŷָ���ָ�����£���URL ��ַ;���ŵ�
			 ����;�����û���;��������;��ַ;TCP �˿�;UDP �˿�;������ID; Ӳ��
			 �汾;�̼��汾; ���ӵ�ָ��������ʱ�ޡ�����ĳ��������ֵ����ſ� */
		{
			u8 seg_buf[100];
			u8 seg_len = 0;

			//URL��ַ
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sURL);
			pMsgBlock++; ///����';'

			//���ŵ�����
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sPPP_name);
			pMsgBlock++; ///����';'

			//�����û���;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sPPP_user);
			pMsgBlock++; ///����';'

			//��������;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sPPP_password);
			pMsgBlock++; ///����';'

			//��ַ;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sIP);
			pMsgBlock++; ///����';'

			//TCP �˿�;
			seg_len = get_next_segment(pMsgBlock, (u8*)seg_buf);
			if (seg_len > 0) {
				CONVERT_STRING_2_U16(seg_buf, seg_len, &ptParam_Runtime->u16TCP_port);
			}
			pMsgBlock++; ///����';'

			//UDP �˿�;
			seg_len = get_next_segment(pMsgBlock, (u8*)seg_buf);
			if (seg_len > 0) {
				CONVERT_STRING_2_U16(seg_buf, seg_len, &ptParam_Runtime->u16UDP_port);
			}
			pMsgBlock++; ///����';'

			//������ID;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sManufacture_ID);
			pMsgBlock++; ///����';'

			//Ӳ���汾;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sHardware_Version);
			pMsgBlock++; ///����';'

			//�̼��汾;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sFirmware_Version);
			pMsgBlock++; ///����';'

			//���ӵ�ָ��������ʱ��
			CONVERT_STRING_2_U16(pMsgBlock, (pMsg + sizeof(tMSGHEAD) + u16MsgBlockLen - pMsgBlock), ///ʣ���ֽڳ���
					&ptParam_Runtime->u16TOUT_ServerConnect);
			ptParam_Runtime->u16TOUT_ServerConnect *= 60; ///��ת������

			TRACE_(QS_USER, NULL,
					"URL:%s; PPP_name:%s, PPP_user:%s, PPP_password:%s, IP:%s, TCP_port:%d, 	\
				UDP_port:%d,ManufuctureId:%s, Hardware_Version:%s,	Firmware_Version:%s, 	\
				Link_tout:%d",
					ptParam_Runtime->sURL, ptParam_Runtime->sPPP_name, ptParam_Runtime->sPPP_user,
					ptParam_Runtime->sPPP_password, ptParam_Runtime->sIP, ptParam_Runtime->u16TCP_port,
					ptParam_Runtime->u16UDP_port, ptParam_Runtime->sManufacture_ID, ptParam_Runtime->sHardware_Version,
					ptParam_Runtime->sFirmware_Version, ptParam_Runtime->u16TOUT_ServerConnect);

			iRet = PARAM_SaveSysParam(PARAM_RUNTIME);	//���浽���в���
			if (iRet < 0) {
				TRACE_(QS_USER, NULL, " ERR! Save_Params failed! ");
				return;
			}
		}
			break;
		case CMD_CONNECT_ALLOCATE_SERVER: /// = 2,	///�����ն�����ָ��������
			/**�����ն�����ָ��������������֮����ð�Ƿֺŷָ�������ָ�����£�
			 �����ӿ���;���ƽ̨��Ȩ��;���ŵ�����;�����û���;��������;��ַ;
			 TCP �˿�;UDP �˿�;���ӵ�ָ��������ʱ�ޡ�����ĳ��������ֵ����
			 �ſգ������ӿ���ֵΪ1�����޺�̲���*/
		{
			u8 seg_buf[100];
			u8 seg_len = 0;

			//���ӿ���;
			u8 ctrl = *pMsgBlock++;
			Q_ASSERT((ctrl == 0) || (ctrl == 1)); ///���ӿ���

			/**	0���л���ָ�����ƽ̨�����������ӵ��÷������󼴽���Ӧ��״̬��
			 ��״̬�½����·�����ָ��ļ��ƽ̨�ɷ��Ͱ����������ڵĿ���ָ��
			 1���л���ԭȱʡ���ƽ̨�����������ָ�����״̬
			 */
			if (ctrl == 1) {
				QACTIVE_POST(AO_Gprs, Q_NEW(QEvent, GPRS_CONNECTDEFAULTSERVER), (void *)0);
				return;
			}

			///����ctrl=0ʱ��������������Ч
			ptParam_Runtime->u8ConnectionCtrl = ctrl;
			pMsgBlock++; ///����';'

			//���ƽ̨��Ȩ��;
			GET_TCTRL_SEGMENT(pMsgBlock, ptParam_Runtime->sAuthorCode);
			pMsgBlock++; ///����';'

			//���ŵ�����;
			GET_TCTRL_SEGMENT((char*) pMsgBlock, (u8*)ptParam_Runtime->sPPP_name);
			pMsgBlock++; ///����';'

			//�����û���;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sPPP_user);
			pMsgBlock++; ///����';'

			//��������;
			GET_TCTRL_SEGMENT(pMsgBlock, ptParam_Runtime->sPPP_password);
			pMsgBlock++; ///����';'

			//��ַ;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sIP);
			pMsgBlock++; ///����';'

			//TCP �˿�;
			seg_len = get_next_segment((char*)pMsgBlock, (char*)seg_buf);
			if (seg_len > 0) {
				CONVERT_STRING_2_U16(seg_buf, seg_len, &ptParam_Runtime->u16TCP_port);
			}
			pMsgBlock++; ///����';'

			//UDP �˿�;
			seg_len = get_next_segment((char*)pMsgBlock, (char*)seg_buf);
			if (seg_len > 0) {
				CONVERT_STRING_2_U16(seg_buf, seg_len, &ptParam_Runtime->u16UDP_port);
			}
			pMsgBlock++; ///����';'

			//���ӵ�ָ��������ʱ��
			CONVERT_STRING_2_U16(pMsgBlock, (pMsg + sizeof(tMSGHEAD) + u16MsgBlockLen - pMsgBlock), ///ʣ���ֽڳ���
					&ptParam_Runtime->u16TOUT_ServerConnect);
			ptParam_Runtime->u16TOUT_ServerConnect *= 60; ///��ת������

			TRACE_(QS_USER, NULL,
					"Link_ctrl:%d, AuthorCode:%s, PPP_name:%s, PPP_user:%s, PPP_password:%s, 	\
					IP:%s, TCP_port:%d,	UDP_port:%d, Link_tout:%d",
					ptParam_Runtime->u8ConnectionCtrl, ptParam_Runtime->sAuthorCode, ptParam_Runtime->sPPP_name,
					ptParam_Runtime->sPPP_user, ptParam_Runtime->sPPP_password, ptParam_Runtime->sIP,
					ptParam_Runtime->u16TCP_port, ptParam_Runtime->u16UDP_port, ptParam_Runtime->u16TOUT_ServerConnect);

			iRet = PARAM_SaveSysParam(PARAM_RUNTIME);	//���浽���в���
			if (iRet < 0) {
				TRACE_(QS_USER, NULL, " ERR! Save_Params failed! ");
				return;
			}
		}
			break;
		case CMD_POWER_OFF: ///�����ն˹ػ�
			TRACE_(QS_USER, NULL, "3, CMD_POWER_OFF");
			///�㲥�ػ���Ϣ����ȴ�����״̬�����������ִ��
			QF_publish(Q_NEW(QEvent, SYS_POWEROFF), NULL);
			break;
		case CMD_RESET: ///�����ն˸�λ
			TRACE_(QS_USER, NULL, "4, CMD_RESET");
			///�㲥��λ��Ϣ����ȴ�����״̬�����������ִ��
			QF_publish(Q_NEW(QEvent, SYS_RESET), NULL);
			break;
		case CMD_RESUME_FACTORY_SET: ///�ն˻ָ���������
			TRACE_(QS_USER, NULL, "5, CMD_RESUME_FACTORY_SET");
			QF_publish(Q_NEW(QEvent, SYS_RECOVERFACTORYSETTING), NULL);
			break;
		case CMD_CLOSE_DATA_LINK: ///�ر�����ͨ��
			TRACE_(QS_USER, NULL, "6, CMD_CLOSE_DATA_LINK");
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvent, GPRS_SHUTDATACOMM), (void *)0);
			break;
		case CMD_CLOSE_ALL_LINK: ///�ر���������ͨ��
			TRACE_(QS_USER, NULL, "7, CMD_CLOSE_ALL_LINK");
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvent, GPRS_SHUTALLCOMM), (void *)0);
			break;
		default:
			break;
		}
	}
		break;

#ifdef JTT808_Ver_2013
		//��ѯ�ն�����
	case MSGID_S_QueryProperty:			//0x8107
	{
		TaskEvt *pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);

		pe->cmd = MSGID_T_QueryPropertyAck;
		pe->resCmd = MSGID_S_QueryProperty;
		pe->sequence = GenerateCirculateSeqId();
		pe->resSeqence = pHead->u16MsgCircularId;

		QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
	}
		break;

		//��ѯ�ն�����Ӧ��
//	case	MSGID_T_QueryPropertyAck:			//0x0107
//		break;

		//�·��ն�������
	case MSGID_T_DispathUpgPack:			//0x8108
	{
		/**
		 ��ʼ�ֽ� 	�ֶ�  			��������     	����
		 0 		�������� 			BYTE 		0���նˣ�12����·����֤IC ����������52���������Ƕ�λģ��
		 1 		������			ID BYTE[5] 	�����̱��
		 6 		�汾�ų��� 		BYTE 		n
		 7 		�汾�� 			STRING
		 7+n 	�������ݰ����� 	DWORD 		��λΪBYTE
		 11+n 	�������ݰ�
		 */
//		ptUpgInfo->upg_type = *pMsgBlock++;		///��������
//		memcpy_(ptUpgInfo->factoryId, pMsgBlock, 5); ///������
//		pMsgBlock += 5;
//		ptUpgInfo->version_len = *pMsgBlock++; ///�汾�ų���
//		memcpy_(ptUpgInfo->version, pMsgBlock, ptUpgInfo->version_len); ///�汾��
//		pMsgBlock += ptUpgInfo->version_len;
//		ptUpgInfo->u32UpgPack_len = *((*u32)(pMsgBlock)); ///�������ݰ�����
//		ENDIAN_U32(ptUpgInfo->u32UpgPack_len);
//
//		///��������δ����������
//		QACTIVE_POST(AO_Upgrade, Q_NEW(QEvent, UPG_REQUEST_SIG), (void *)0);
	}
		break;

		//�ն��������֪ͨ
//	case	MSGID_T_UpgrageResult:			//0x0108
//		break;

#endif
		//λ����Ϣ�㱨
//	case	MSGID_T_RptPositionInfo:			//0x0200
//		break;

		//λ����Ϣ��ѯ
	case MSGID_S_QueryPosition:			//0x8201
	{
		TaskEvt *pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);

		pe->cmd = MSGID_T_QueryPositionAck;
		pe->resCmd = MSGID_S_QueryPosition;
		pe->sequence = GenerateCirculateSeqId();
		pe->resSeqence = pHead->u16MsgCircularId;

		QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
	}
		break;

		//λ����Ϣ��ѯӦ��
//	case	MSGID_T_QueryPositionAck:			//0x0201
//		break;

		//��ʱλ�ø��ٿ���
	case MSGID_S_Tracking:					//0x8202
	{
		tMsg_SCMD_TemproryTrack *pTrackInfo;
		TrackEvt *pe;

		pTrackInfo = (tMsg_SCMD_TemproryTrack*) pMsgBlock;
		ENDIAN_U16(pTrackInfo->u16TimeInterval);
		ENDIAN_U32(pTrackInfo->u32TrackingTimeout);

		pe = Q_NEW(TrackEvt, GPS_TRACK_SIG);
		Q_ASSERT(pe);
		pe->u16Period = pTrackInfo->u16TimeInterval;
		pe->u32Timetill = pTrackInfo->u32TrackingTimeout;

		QACTIVE_POST(AO_Gps, (QEvt* )pe, (void *)0);
	}
		break;

#ifdef JTT808_Ver_2013
		//�˹�ȷ�ϱ�����Ϣ
	case MSGID_S_ConfirmWarningInfo:		//0x8203
	{
		tMsg_SCMD_MannalVerifyWarning *pWarnInfo;
		ManualVerifyEvt *pe;

		pWarnInfo = (tMsg_SCMD_MannalVerifyWarning *) pMsgBlock;
		ENDIAN_U16(pWarnInfo->u16SeqNum);
		ENDIAN_U32(pWarnInfo->u32WarnType);

		pe = Q_NEW(ManualVerifyEvt, SYS_MANUALVERIFYWARN_SIG);
		Q_ASSERT(pe);
		pe->u16VerifyWarnSeq = pWarnInfo->u16SeqNum;
		memcpy_((u8*) &pe->tWarnInfo, (u8*) pWarnInfo->u32WarnType, 4);

		QACTIVE_POST(AO_Gps, (QEvt* )pe, (void *)0); ///����Ϣ���ĸ�״̬�������������������������
	}
		break;

#endif
		//�ı���Ϣ�·�
	case MSGID_S_DownloadTxtInfo:			//0x8300
	{
		tMsg_SCMD_DispatchTxtInfo *pInfo;
		u8 *msg_buf;
		u16 msg_len;

		pInfo = (tMsg_SCMD_DispatchTxtInfo*) pMsgBlock;
		msg_buf = pInfo->msg;
		msg_len = u16MsgBlockLen - 1;

		if (pInfo->flag_Emergency) ///����
		{
			//????��ô������������
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
			return;
		}

		if (pInfo->flag_Display)	///����ʾ
		{
			DisplayMsgEvt *pe;

			pe = Q_NEW(DisplayMsgEvt, UI_SHOWMSG_SIG);
			Q_ASSERT(pe);

			memcpy_((u8*) &pe->msg, (u8*) &pInfo->msg, u16MsgBlockLen - 1);
			pe->msg[u16MsgBlockLen - 1] = '\0';

			QACTIVE_POST(AO_LCD, (QEvt* )pe, (void *)0);

			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
			return;
		}

		if (pInfo->flag_Advertise)	///��棬��֧��
		{
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
			return;
		}

		if (pInfo->flag_TTSRead)	///�ʶ�
		{
			TTSMsgEvt *pe;
			pe = Q_NEW(TTSMsgEvt, TTS_READMSG_SIG);
			Q_ASSERT(pe);

			memcpy_((u8*) &pe->msg, (u8*) &pInfo->msg, u16MsgBlockLen - 1);
			pe->msg[u16MsgBlockLen - 1] = '\0';

			QACTIVE_POST(AO_TTS, (QEvt* )pe, (void *)0);

			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
			return;

		}

#ifdef JTT808_Ver_2013
		if (0 == pInfo->flag_MsgInfo)	{  ///5 0�����ĵ�����Ϣ��1��CAN ��������Ϣ
		}
		else {
		}

		///�ն˷��ͳɹ�Ӧ��
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
		return;
//	}
#endif
	}
		break;

		//�¼�����
		/**
		 7.4.2 �¼����ü�����
		 ƽ̨ͨ�������¼�������Ϣ�����¼��б����ն˴洢����ʻԱ��������Ӧ�¼���ɽ����¼��б�������ѡ��
		 ѡ����ն���ƽ̨�����¼�������Ϣ��
		 �¼�������Ϣ����Ҫ�ն˻ظ��ն�ͨ��Ӧ����Ϣ��
		 �¼�������Ϣ����Ҫƽ̨�ظ�ƽ̨ͨ��Ӧ����Ϣ��
		 */
	case MSGID_S_SetEvent:					//0x8301
	{
		u8 u8EvtType;
		u8 u8EvtAmount;
		u8 i = 0;
		u8 u8EvtId;
		u8 u8EvtLen;
		u8 *pEvtMsg;

		u8EvtType = *pMsgBlock++;
		u8EvtAmount = *pMsgBlock++;

		TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_SetEvent], Type[%d], Amount[%s], ", MSGID_S_SetEvent,
				u8EvtType, u8EvtAmount);

		if (u8EvtType == eDEL_ALL_EVENT) ///ɾ���ն����������¼�����������к����ֽ�
				{
			TRACE_(QS_USER, NULL, "Action[eDEL_ALL_EVENT]");

			PARAM_EraseFMTParam(PARAM_EVENT);	///�ڴ�������
			ERROR_HANDLE(iRet);
		}
		else if (u8EvtType == eUPDATE_EVENT)			// �����¼�	���޸��¼���ʲô���𣿣���������
				{
			///����ʵ�ַ�ʽ��ɾ�����������¼����滻���¼�
			TRACE_(QS_USER, NULL, "Action[eUPDATE_EVENT]");

			///ɾ��ԭ�¼�
			iRet = PARAM_EraseFMTParam(PARAM_EVENT);		///����ڴ�
			ERROR_HANDLE(iRet);

			///�滻���¼�
			i = 0;
			while (i < u8EvtAmount) {
				u8EvtId = *pMsgBlock;
				u8EvtLen = *(pMsgBlock + 1) + 2; ///�¼����ܳ���
				pEvtMsg = pMsgBlock;

				///�����¼���
				iRet = PARAM_FormatMsg_ApendItem(PARAM_EVENT, u8EvtLen, pEvtMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Add ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8EvtId, u8EvtLen,
						(char*) pEvtMsg);

				pMsgBlock += u8EvtLen;
				i++;
			}
		}
		else if (u8EvtType == eAPPEND_EVENT) ///׷���¼�
				{
			TRACE_(QS_USER, NULL, "Action[eAPPEND_EVENT]");

			i = 0;
			while (i < u8EvtAmount) {
				u8EvtId = *pMsgBlock;
				u8EvtLen = *(pMsgBlock + 1) + 2; ///�¼����ܳ���
				pEvtMsg = pMsgBlock;

				///�����¼���
				iRet = PARAM_FormatMsg_ApendItem(PARAM_EVENT, u8EvtLen, pEvtMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Append ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8EvtId, u8EvtLen,
						(char*) pEvtMsg);

				pMsgBlock += u8EvtLen;
				i++;
			}
		}
		else if (u8EvtType == eCHANGE_EVENT) ///�޸��¼�
				{
			TRACE_(QS_USER, NULL, "Action[eCHANGE_EVENT]");

			i = 0;
			while (i < u8EvtAmount) {
				u8EvtId = *pMsgBlock;
				u8EvtLen = *(pMsgBlock + 1) + 2; ///�¼����ܳ���
				pEvtMsg = pMsgBlock;

				///�����¼���
				iRet = PARAM_FormatMsg_ReplaceItem(PARAM_EVENT, u8EvtId, u8EvtLen, pEvtMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Change ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8EvtId, u8EvtLen,
						(char*) pEvtMsg);

				pMsgBlock += u8EvtLen;
				i++;
			}
		}
		else {
			///���󣬻ش���ͨ����Ӧ����Ϣ
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
			return;
		}

		///дEEPROM����ͷ
		PARAM_SaveFMTParam(PARAM_EVENT);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);

		///֪ͨ��������¼��б�
		QACTIVE_POST(AO_LCD, Q_NEW(QEvt, UI_REFRESH_EVENTS_SIG), (void *)0);
	}
		break;

		//�¼�����
	case MSGID_T_RptEvent:					//0x0301
		break;

		//�����·�
	case MSGID_S_AskQuestion:				//0x8302
	{
		///�����·���־
		u8 u8QuestionType;
		u8 u8QuestionLen;

		u8QuestionType = *pMsgBlock++;
		u8QuestionLen = *pMsgBlock++;

		if (u8QuestionType & QUESTION_TYPE_ERGENT) {
			///��������ô��������
		}

		if (u8QuestionType & QUESTION_TYPE_TTS_READ)	///����
		{
			TTSMsgEvt *pe;
			pe = Q_NEW(TTSMsgEvt, TTS_READMSG_SIG);
			Q_ASSERT(pe);

			///����ԭʼ��Ϣ��ע��������⼰��ѡӦ�����к�ѡӦ��ĳ����ֶ�û��ת����С��
			memcpy_((u8*) &pe->msg, pMsgBlock, u16MsgBlockLen - 2);
			QACTIVE_POST(AO_TTS, (QEvt* )pe, (void *)0);
		}

		if (u8QuestionType & QUESTION_TYPE_DISPLAY) ///�������ʾ����֧��
		{
			return;
		}
	}
		break;

		//����Ӧ��
	case MSGID_T_QuestionAck:				//0x0302
		break;

		//��Ϣ�㲥�˵�����
	case MSGID_S_SetInfoOnDemandMenu:		//0x8303
	{
		u8 u8Type, u8ItemAmount;
		u8 i, u8ItemId, u8ItemLen;
		u8 *pItemMsg;

		u8Type = *pMsgBlock++;	///����
		u8ItemAmount = *pMsgBlock++;	///����

		TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_SetInfoOnDemandMenu], Type[%d], Amount[%s], ",
		MSGID_S_SetInfoOnDemandMenu, u8Type, u8ItemAmount);

		if (u8Type == MSGOD_MENU_DELETE) ///ɾ���ն�������Ϣ��
		{
			TRACE_(QS_USER, NULL, "Action[ DELETE ]");
			iRet = PARAM_EraseFMTParam(PARAM_MSGODMENU);
			ERROR_HANDLE(iRet);
		}
		else if (u8Type == MSGOD_MENU_UPDATE) ///���²˵�
		{
			TRACE_(QS_USER, NULL, "Action[ UPDATE ]");
			iRet = PARAM_EraseFMTParam(PARAM_MSGODMENU);  ///����ڴ�
			ERROR_HANDLE(iRet);

			i = 0;
			while (i < u8ItemAmount) {
				u8ItemId = *pMsgBlock++;	///�˵�ID
				u8ItemLen = *(pMsgBlock + 1) + 2; ///�˵����
				pItemMsg = pMsgBlock;

				///�����¼���
				iRet = PARAM_FormatMsg_ApendItem(PARAM_MSGODMENU, u8ItemLen, pItemMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "UPDATE ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else if (u8Type == MSGOD_MENU_APPEND) ///׷�Ӳ˵�
		{
			TRACE_(QS_USER, NULL, "Action[ APPEND ]");

			i = 0;
			while (i < u8ItemAmount) {
				u8ItemId = *pMsgBlock++;	///�˵�ID
				u8ItemLen = *(pMsgBlock + 1) + 2; ///�˵����
				pItemMsg = pMsgBlock;

				///�����¼���
				iRet = PARAM_FormatMsg_ApendItem(PARAM_MSGODMENU, u8ItemLen, pItemMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Append ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else if (u8Type == MSGOD_MENU_CHANGE) ///�޸Ĳ˵�
		{
			TRACE_(QS_USER, NULL, "Action[ CHANGE ]");

			i = 0;
			while (i < u8ItemAmount) {
				u8ItemId = *pMsgBlock++;	///�˵�ID
				u8ItemLen = *(pMsgBlock + 1) + 2; ///�˵����
				pItemMsg = pMsgBlock;

				///�����¼���
				iRet = PARAM_FormatMsg_ReplaceItem(PARAM_MSGODMENU, u8ItemId, u8ItemLen, pItemMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Change ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else {
			///���󣬻ش���ͨ����Ӧ����Ϣ
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
			return;
		}

		///дEEPROM����ͷ
		PARAM_SaveFMTParam(PARAM_MSGODMENU);
		ERROR_HANDLE(iRet);

		///֪ͨ��������¼��б�
		QACTIVE_POST(AO_LCD, Q_NEW(QEvt, UI_REFRESH_EVENTS_SIG), (void *)0);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//��Ϣ�㲥/ȡ�����ն��������𣬷�����Ӧ��
	case MSGID_T_OrderInfoCtrl:			//0x0303
		break;

		//��Ϣ����
	case MSGID_S_InfoService:				//0x8304
	{
		u16 u16ItemLen = 0;
		tMsg_SCMD_InfoService *pInfo;
		pInfo = (tMsg_SCMD_InfoService *) pMsgBlock;

		ENDIAN_U16(pInfo->u16InfoTotalLength);
		u16ItemLen = pInfo->u16InfoTotalLength + 2;

		///����һ������Ϣ
		PARAM_FormatMsg_ApendItem(PARAM_MSGODINFO, u16ItemLen, pMsgBlock);

		///��������ͷ
		PARAM_SaveFMTParam(PARAM_MSGODINFO);

		///֪ͨUI�����µ㲥��Ϣ
		QACTIVE_POST(AO_LCD, Q_NEW(QEvt, UI_REFRESH_MSGODMSG_SIG), (void *)0);
	}
		break;

		//�绰�ز�
	case MSGID_S_Redial:					//0x8400
	{
		tMsg_SCMD_ReverseDial *pDial;
		DialEvt *pe;

		pDial = (tMsg_SCMD_ReverseDial *) pMsgBlock;
		pMsgBlock++;

		TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_Redial], type[%d], phone[%s]", MSGID_S_Redial,
				(u8) pDial->u8Flag, (char*) pMsgBlock);

		if (pDial->u8Flag == 0) {	///��ͨͨ��
			pe = Q_NEW(DialEvt, GPRS_DIAL_NORMAL_SIG);
			Q_ASSERT(pe);
			strcpy_(pe->phone, (char*) pMsgBlock);
			QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
		}
		else if (pDial->u8Flag == 1) {	///����
			pe = Q_NEW(DialEvt, GPRS_DIAL_MONITOR_SIG);
			Q_ASSERT(pe);
			strcpy_(pe->phone, (char*) pMsgBlock);
			QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
		}
		else {
			///�ش���ͨ����Ӧ����Ϣ
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
		}
	}
		break;

		/**���õ绰����ע�⣬�Ե绰���Ĳ�������ϵ��Ϊ����������ϵ��Ϊ�䳤���ݣ���˲�������ϵ��Ϊ������
		 �����ȡ��ͨ����������ϵ��CRCУ�����Ϊ��������ͬ��ϵ��CRCһ����ͬ����˿�������*/
	case MSGID_S_SetPhoneBook:	//0x8401 ���õ绰��
	{
		u8 u8ItemAmount;
		u32 u32Address;
		u8 i, u8ItemId, u8ItemLen;
		u8 *pItemMsg;

		T_SCMD_SetPhoneBook *pBook;
		pBook = (T_SCMD_SetPhoneBook *) pMsgBlock;

		if (pBook->u8ConnPersonCount > PHONE_ITEM_SIZE)	///��Ŀ����Խ����
			return;

		u8ItemAmount = pBook->u8ConnPersonCount;

		TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_SetPhoneBook], Type[%d], Amount[%s], ", MSGID_S_SetPhoneBook,
				pBook->u8SetType, u8ItemAmount);

		pMsgBlock += 2; ///ָ���һ��

		if (pBook->u8SetType == ePHONEBOOK_DELETE) ///0,ɾ���ն������д洢����ϵ��
				{
			TRACE_(QS_USER, NULL, "Action[ DELETE ]");
			iRet = PARAM_EraseFMTParam(PARAM_PHONEBOOK); ///����ڴ�
			ERROR_HANDLE(iRet);
		}
		else if (pBook->u8SetType == ePHONEBOOK_UPDATE) ///1,���µ绰��
				{
			///ɾ���ն������е�ȫ����ϵ�˲�׷����Ϣ�е���ϵ��
			TRACE_(QS_USER, NULL, "Action[ UPDATE ]");
			iRet = PARAM_EraseFMTParam(PARAM_PHONEBOOK); ///����ڴ�
			ERROR_HANDLE(iRet);

			i = 0;
			while (i < u8ItemAmount) {
				///ȡ���Ե绰������ϵ��CRC��ΪID����������ϵ��������Ϊ����
				u8 u8NameLen = *(pMsgBlock + *(pMsgBlock + 1) + 2);	///���볤��
				u8 *pName = pMsgBlock + *(pMsgBlock + 1) + 2;
				u8ItemId = CalculateCRC8(pName, u8NameLen);
				u8ItemLen = *(pMsgBlock + 1) + 2 + u8NameLen;

				///������
				iRet = PARAM_FormatMsg_ApendItem(PARAM_PHONEBOOK, u8ItemLen, pMsgBlock);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "UPDATE ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else if (pBook->u8SetType == ePHONEBOOK_APPEND)	///2,׷�ӵ绰��
				{
			i = 0;
			while (i < u8ItemAmount) {
				///�绰������ϵ��CRC��ΪID
				u8 u8NameLen = *(pMsgBlock + *(pMsgBlock + 1) + 2);	///���볤��
				u8 *pName = pMsgBlock + *(pMsgBlock + 1) + 2;
				u8ItemId = CalculateCRC8(pName, u8NameLen);
				u8ItemLen = *(pMsgBlock + 1) + 2 + u8NameLen;

				///������
				iRet = PARAM_FormatMsg_ApendItem(PARAM_PHONEBOOK, u8ItemLen, pMsgBlock);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Append ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else if (pBook->u8SetType == ePHONEBOOK_CHANGE)	///3,�޸ĵ绰��(����ϵ��Ϊ����)
				{
			i = 0;
			while (i < u8ItemAmount) {
				///�绰������ϵ��CRC��ΪID
				u8 u8NameLen = *(pMsgBlock + *(pMsgBlock + 1) + 2);	///���볤��
				u8 *pName = pMsgBlock + *(pMsgBlock + 1) + 2;
				u8ItemId = CalculateCRC8(pName, u8NameLen);
				u8ItemLen = *(pMsgBlock + 1) + 2 + u8NameLen;

				///������
				iRet = PARAM_FormatMsg_ReplaceItem(PARAM_PHONEBOOK, u8ItemId, u8ItemLen, pMsgBlock);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Replace ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else {
			///���󣬻ش���ͨ����Ӧ����Ϣ
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
			return;
		}

		///дEEPROM����ͷ
		PARAM_SaveFMTParam(PARAM_PHONEBOOK);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);

		///֪ͨUI�����µ绰��������Ҫ
		QACTIVE_POST(AO_LCD, Q_NEW(QEvt, UI_REFRESH_PHONEBOOK_SIG), (void *)0);
	}
		break;

		//��������
	case MSGID_S_VechicleCtrl:				//0x8500
	{
		TaskEvt *pe;
		u8 u8Ctrl;

		///0�����Ž�����1�����ż���
		u8Ctrl = *pMsgBlock;

		if (u8Ctrl & 0x01) {
			BSP_Vechical_Lock();	///����
			QACTIVE_POST(AO_LCD, Q_NEW(QEvt, VECHICAL_CTRL_LOCK_SIG), (void *)0);
			TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_VechicleCtrl], ctrl[%s]", (u8) MSGID_S_VechicleCtrl,
					"LOCK");
		}
		else {
			BSP_Vechical_UnLock();	///����
			QACTIVE_POST(AO_LCD, Q_NEW(QEvt, VECHICAL_CTRL_UNLOCK_SIG), (void *)0);
			TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_VechicleCtrl], ctrl[%s]", (u8) MSGID_S_VechicleCtrl,
					"UNLOCK");
		}

		///���Ϳ�������Ӧ��
		pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);
		pe->cmd = MSGID_T_VechicleCtrlAck;
		pe->resCmd = MSGID_S_VechicleCtrl;
		pe->sequence = GenerateCirculateSeqId();
		pe->resSeqence = pHead->u16MsgCircularId;

		QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
	}
		break;

		//��������Ӧ��
	case MSGID_T_VechicleCtrlAck:			//0x0500
		break;

		//����Բ������
	case MSGID_S_SetRoundErea:			//0x8600
	{
		u8 u8Type = 0;
		u8 u8Amount = 0;
		u8 i;
		tItem_RoundArea *pRound;

		u8Type = *pMsgBlock++;
		u8Amount = *pMsgBlock++;

		if (u8Type == eAREA_UPDATE) {	///0,��������
			PARAM_Area_EraseAll(PARAM_AREA_ROUND); ///�����ԭ��������Ϣ

			i = 0;
			while (i < u8Amount) {
				pRound = (tItem_RoundArea *) pMsgBlock;

				///�ֶ�ת����С��
				ENDIAN_U32(pRound->u32AreaId);
				ENDIAN_U32(pRound->u32CenterPtLatitude);
				ENDIAN_U32(pRound->u32CenterPtLongitude);
				ENDIAN_U32(pRound->u32Radius);
				ENDIAN_U16(pRound->u16SpeedMax);

				iRet = PARAM_Area_AppendItem(PARAM_AREA_ROUND,
						pRound->u32AreaId, (u8*) pRound, sizeof(tItem_RoundArea));
				ERROR_HANDLE(iRet);

				pMsgBlock += sizeof(tItem_RoundArea);
				i++;
			}
		}
		if (u8Type == eAREA_APPEND) { 	///1,׷������
			i = 0;
			while (i < u8Amount) {
				pRound = (tItem_RoundArea *) pMsgBlock;

				///�ֶ�ת����С��
				ENDIAN_U32(pRound->u32AreaId);
				ENDIAN_U32(pRound->u32CenterPtLatitude);
				ENDIAN_U32(pRound->u32CenterPtLongitude);
				ENDIAN_U32(pRound->u32Radius);
				ENDIAN_U16(pRound->u16SpeedMax);

				iRet = PARAM_Area_AppendItem(PARAM_AREA_ROUND, pRound->u32AreaId, (u8*) pRound,
						sizeof(tItem_RoundArea));
				ERROR_HANDLE(iRet);

				pMsgBlock += sizeof(tItem_RoundArea);
				i++;
			}
		}
		if (u8Type == eAREA_CHANGE) {	///2,�޸�����
			i = 0;
			while (i < u8Amount) {
				pRound = (tItem_RoundArea *) pMsgBlock;

				///�ֶ�ת����С��
				ENDIAN_U32(pRound->u32AreaId);
				ENDIAN_U32(pRound->u32CenterPtLatitude);
				ENDIAN_U32(pRound->u32CenterPtLongitude);
				ENDIAN_U32(pRound->u32Radius);
				ENDIAN_U16(pRound->u16SpeedMax);

				iRet = PARAM_Area_ReplaceItem(PARAM_AREA_ROUND, pRound->u32AreaId, (u8*) pRound,
						sizeof(tItem_RoundArea));
				ERROR_HANDLE(iRet);

				pMsgBlock += sizeof(tItem_RoundArea);
				i++;
			}
		}

		///дEEPROM
//		PARAM_Area_Save(PARAM_AREA_ROUND);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//ɾ��Բ������
	case MSGID_S_DelRoundErea:			//0x8601
	{
		u8 u8Amount = 0;
		u8 i;
		u32 u32Id;

		u8Amount = *pMsgBlock++;

		i = 0;
		while (i < u8Amount) {
			u32Id = *((u32*) pMsgBlock);
			iRet = PARAM_Area_DeleteItem(PARAM_AREA_ROUND, u32Id);
			ERROR_HANDLE(iRet);
			pMsgBlock += 4;
			i++;
		}

		///дEEPROM
//		PARAM_Area_Save(PARAM_AREA_ROUND);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//���þ�������
	case MSGID_S_SetRectangleErea:			//0x8602
	{
		u8 u8Type = 0;
		u8 u8Amount = 0;
		u8 i;
		tItem_RectangleArea *pRect;

		u8Type = *pMsgBlock++;
		u8Amount = *pMsgBlock++;

		if (u8Type == eAREA_UPDATE) ///0,��������
				{
			PARAM_Area_EraseAll(PARAM_AREA_RECTANGLE); ///�����ԭ��������Ϣ

			i = 0;
			while (i < u8Amount) {
				pRect = (tItem_RectangleArea *) pMsgBlock;

				///�ֶ�ת����С��
				ENDIAN_U16(pRect->u16AreaProperty);
				ENDIAN_U16(pRect->u16SpeedMax);
				ENDIAN_U32(pRect->u32AreaId);
				ENDIAN_U32(pRect->u32LT_Lat);
				ENDIAN_U32(pRect->u32LT_Long);
				ENDIAN_U32(pRect->u32RB_Lat);
				ENDIAN_U32(pRect->u32RB_Long);

				iRet = PARAM_Area_AppendItem(PARAM_AREA_RECTANGLE, pRect->u32AreaId, (u8*) pRect,
						sizeof(tItem_RectangleArea));
				ERROR_HANDLE(iRet);

				pMsgBlock += sizeof(tItem_RectangleArea);
				i++;
			}
		}
		if (u8Type == eAREA_APPEND) ///1,׷������
				{
			i = 0;
			while (i < u8Amount) {
				pRect = (tItem_RectangleArea *) pMsgBlock;

				///�ֶ�ת����С��
				ENDIAN_U16(pRect->u16AreaProperty);
				ENDIAN_U16(pRect->u16SpeedMax);
				ENDIAN_U32(pRect->u32AreaId);
				ENDIAN_U32(pRect->u32LT_Lat);
				ENDIAN_U32(pRect->u32LT_Long);
				ENDIAN_U32(pRect->u32RB_Lat);
				ENDIAN_U32(pRect->u32RB_Long);

				iRet = PARAM_Area_AppendItem(PARAM_AREA_RECTANGLE, pRect->u32AreaId, (u8*) pRect,
						sizeof(tItem_RoundArea));
				ERROR_HANDLE(iRet);

				pMsgBlock += sizeof(tItem_RectangleArea);
				i++;
			}
		}
		if (u8Type == eAREA_CHANGE) ///2,�޸�����
				{
			i = 0;
			while (i < u8Amount) {
				pRect = (tItem_RectangleArea *) pMsgBlock;

				///�ֶ�ת����С��
				ENDIAN_U16(pRect->u16AreaProperty);
				ENDIAN_U16(pRect->u16SpeedMax);
				ENDIAN_U32(pRect->u32AreaId);
				ENDIAN_U32(pRect->u32LT_Lat);
				ENDIAN_U32(pRect->u32LT_Long);
				ENDIAN_U32(pRect->u32RB_Lat);
				ENDIAN_U32(pRect->u32RB_Long);

				iRet = PARAM_Area_ReplaceItem(PARAM_AREA_RECTANGLE, pRect->u32AreaId, (u8*) pRect,
						sizeof(tItem_RoundArea));
				ERROR_HANDLE(iRet);

				pMsgBlock += sizeof(tItem_RectangleArea);
				i++;
			}
		}

		///дEEPROM
//		PARAM_Area_Save(PARAM_AREA_RECTANGLE);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//ɾ����������
	case MSGID_S_DelRectangleErea:			//0x8603
	{
		u8 u8Amount = 0;
		u8 i;
		u32 u32Id;

		u8Amount = *pMsgBlock++;

		i = 0;
		while (i < u8Amount) {
			u32Id = *((u32*) pMsgBlock);
			iRet = PARAM_Area_DeleteItem(PARAM_AREA_RECTANGLE, u32Id);
			ERROR_HANDLE(iRet);
			pMsgBlock += 4;
			i++;
		}

		///дEEPROM
//		PARAM_Area_Save(PARAM_AREA_RECTANGLE);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//���ö��������
	case MSGID_S_SetPolygonErea:			//0x8604
	{
		u8 i;
		u16 u16PointSize;
		tPOINT *pPoint;
		tItem_PolygonArea *pPolygon;

		pPolygon = (tItem_PolygonArea *) pMsgBlock;

		ENDIAN_U16(pPolygon->u16AreaTotalVertexCount);
		ENDIAN_U16(pPolygon->u16SpeedMax);
		ENDIAN_U32(pPolygon->u32AreaId);

		pMsgBlock += sizeof(tItem_PolygonArea);

		///����ת��С��
		while (i < pPolygon->u16AreaTotalVertexCount) {
			pPoint = (tPOINT*) pMsgBlock;
			ENDIAN_U32(pPoint->u32Lat);
			ENDIAN_U32(pPoint->u32Long);
			pMsgBlock += 8;
			i++;
		}

		u16PointSize = sizeof(tItem_PolygonArea) + (pPolygon->u16AreaTotalVertexCount << 3);

		iRet = PARAM_Area_AppendItem(PARAM_AREA_POLYGON, pPolygon->u32AreaId, (u8*) pPolygon, u16PointSize);
		ERROR_HANDLE(iRet);

		///дEEPROM
//		PARAM_Area_Save(PARAM_AREA_POLYGON);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//ɾ�����������
	case MSGID_S_DelPolygonErea:			//0x8605
	{
		u8 u8Amount = 0;
		u8 i;
		u32 u32Id;

		u8Amount = *pMsgBlock++;	///������

		i = 0;
		while (i < u8Amount) {
			u32Id = *((u32*) pMsgBlock);
			iRet = PARAM_Area_DeleteItem(PARAM_AREA_POLYGON, u32Id);
			ERROR_HANDLE(iRet);
			pMsgBlock += 4;
			i++;
		}

		///дEEPROM
//		PARAM_Area_Save(PARAM_AREA_POLYGON);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//����·��
	case MSGID_S_SetPathline:			//0x8606
	{
		u8 i;
		tCORNERPOINT *pPoint;
		tITEM_CORNERPOINT *pCornerPt;

		pCornerPt = (tITEM_CORNERPOINT *) pMsgBlock;

		ENDIAN_U16(pCornerPt->u16InflectionAmount);
		ENDIAN_U16(pCornerPt->u16LineProperty);
		ENDIAN_U32(pCornerPt->u32LineId);

		pMsgBlock += sizeof(tITEM_CORNERPOINT);

		///����ת��С��
		while (i < pCornerPt->u16InflectionAmount) {
			pPoint = (tCORNERPOINT*) pMsgBlock;

			ENDIAN_U16(pPoint->u16SpeedMax);
			ENDIAN_U16(pPoint->u16TimeNotEnoughThreshold);
			ENDIAN_U16(pPoint->u16TimeOverThreshold);
			ENDIAN_U32(pPoint->u32CornerId);
			ENDIAN_U32(pPoint->u32Latitude);
			ENDIAN_U32(pPoint->u32Longitude);
			ENDIAN_U32(pPoint->u32RoadSectionId);

			pMsgBlock += sizeof(tCORNERPOINT);
			i++;
		}

		iRet = PARAM_Area_AppendItem(PARAM_AREA_ROUTE, pCornerPt->u32LineId, (u8*) pCornerPt, u16MsgBlockLen);
		ERROR_HANDLE(iRet);

		///дEEPROM
//		PARAM_Area_Save(PARAM_AREA_ROUTE);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//ɾ��·��
	case MSGID_S_DelPathline:			//0x8607
	{
		u8 u8Amount = 0;
		u8 i;
		u32 u32Id;

		u8Amount = *pMsgBlock++;	///������

		i = 0;
		while (i < u8Amount) {
			u32Id = *((u32*) pMsgBlock);
			iRet = PARAM_Area_DeleteItem(PARAM_AREA_ROUTE, u32Id);
			ERROR_HANDLE(iRet);
			pMsgBlock += 4;
			i++;
		}

		///дEEPROM
//		PARAM_Area_Save(PARAM_AREA_ROUTE);

		///�ش���ͨ�ɹ�Ӧ����Ϣ
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);

	}
		break;

		//��ʻ��¼�����ݲɼ�����
	case MSGID_S_RecorderCapture:			//0x8700
	{
		RecorderStartEvt *pe;

		pe = Q_NEW(RecorderStartEvt, RECORDER_START_SIG);
		Q_ASSERT(pe);

		pe->u8Cmd = *pMsgBlock++;			///����
		pe->u16CmdSeq = pHead->u16MsgCircularId; ///��ˮ��
		memcpy_(pe->param, pMsgBlock, u16MsgBlockLen);	///����

		QACTIVE_POST(AO_Recorder, (QEvt * )pe, (void *)0);
	}
		break;

		//��ʻ��¼�������ϴ�
	case MSGID_T_RecorderDataUpload:			//0x0700
		break;

		//��ʻ��¼�����´�����
	case MSGID_S_RecorderParamDownload:			//0x8701
	{
		///����ɡ�
	}
		break;

		///8.47 �ϱ���ʻԱ�����Ϣ����, 0x8702
	case MSGID_S_RequestDriverInfo: {
		TaskEvt *pe;

		/**��������Ӧ�����¼�������*/
		pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);
		pe->cmd = MSGID_T_DriverInfoRpt;
		pe->resCmd = MSGID_S_RequestDriverInfo;
		pe->sequence = GenerateCirculateSeqId();
		pe->resSeqence = pHead->u16MsgCircularId;

		QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
	}
		break;

		//�����˵��ϱ�
	case MSGID_T_EBillRpt:			//0x0701
		break;

		//��ʻԱ�����Ϣ�ɼ��ϱ�
	case MSGID_T_DriverInfoRpt:			//0x0702
		break;

#ifdef JTT808_Ver_2013
		//��λ���������ϴ�
	case MSGID_T_PositionInfoBatchRpt:			//0x0704
		break;

		//CAN���������ϴ�
	case MSGID_T_CANDataUpload:			//0x0705
		break;

#endif
		//��ý���¼���Ϣ�ϴ�
	case MSGID_T_MediaEventInfoRpt:			//0x0800
		break;

		//��ý�������ϴ�
	case MSGID_T_MediaDataRpt:			//0x0801
		break;

		//��ý�������ϴ�Ӧ��
	case MSGID_S_MediaDataRptAck:			//0x8800
	{
		tMsg_SACK_Media_AckData *pData;
		MultimediaDataAckEvt *pe;

		pData = (tMsg_SACK_Media_AckData*) pMsgBlock;

		pMsgBlock += 3;

		pe = Q_NEW(MultimediaDataAckEvt, MULTIMEDIA_UPLOADPACK_ACK_SIG);
		Q_ASSERT(pe);

		pe->u32Id = pData->u32MediaDataId;			///����
		ENDIAN_U32(pe->u32Id);
		if (pe->u32Id > 0) {
			u8 i;
			pe->u8Amount = pData->u8ReSendPackAmount;
			while (i < pe->u8Amount) {
				pe->IDList[i] = *((u16*) pMsgBlock);
				ENDIAN_U16(pe->IDList[i]);
				pMsgBlock += 2;
				i++;
			}
		}

		///���Ǹ�״̬����������
		QACTIVE_POST(AO_Video, (QEvt* )pe, (void *)0);
	}
		break;

		//����ͷ������������
	case MSGID_S_TakePhoto:			//0x8801
	{
		tMsg_SCMD_InstantPhoto *pCmd;
		InstantPhotoEvt *pe;

		pCmd = (tMsg_SCMD_InstantPhoto*) pMsgBlock;

		ENDIAN_U16(pCmd->u16Cmd);
		ENDIAN_U16(pCmd->u16PicIntervalOrRecordTime);

		pe = Q_NEW(InstantPhotoEvt, CAMERA_TAKEPHOTO_SIG);

		pe->u16CmdSeq = pHead->u16MsgCircularId;

		pe->tParam.u16Cmd = pCmd->u16Cmd;
		pe->tParam.u16PicIntervalOrRecordTime = pCmd->u16PicIntervalOrRecordTime;
		pe->tParam.u8AVQuality = pCmd->u8AVQuality;
		pe->tParam.u8Brightness = pCmd->u8Brightness;
		pe->tParam.u8ChannelId = pCmd->u8ChannelId;
		pe->tParam.u8Chrome = pCmd->u8Chrome;
		pe->tParam.u8Contrastness = pCmd->u8Contrastness;
		pe->tParam.u8Resolve = pCmd->u8Resolve;
		pe->tParam.u8Saturation = pCmd->u8Saturation;
		pe->tParam.u8SaveTag = pCmd->u8SaveTag;

		QACTIVE_POST(AO_Video, (QEvt* )pe, (void *)0);
	}
		break;

#ifdef JTT808_Ver_2013
		//����ͷ������������Ӧ��
	case MSGID_S_TakePhotoAck:			//0x0805
		break;

#endif
		//�洢��ý�����ݼ�������
	case MSGID_S_StoredMediaQuery:			//0x8802
	{
		tMsg_SCMD_MediaDataRetrieve *pCmd;
		MediaDataRetrieveEvt *pe;

		pCmd = (tMsg_SCMD_MediaDataRetrieve*) pMsgBlock;

		pe = Q_NEW(MediaDataRetrieveEvt, MEDIADATA_RETRIEVE_SIG);

		pe->u16CmdSeq = pHead->u16MsgCircularId;

		pe->tParam.u8MediaType = pCmd->u8MediaType;
		pe->tParam.u8EventCode = pCmd->u8EventCode;
		pe->tParam.u8ChannelId = pCmd->u8ChannelId;
		memcpy_((u8*) &pe->tParam.tBcdStartTime, (u8*) &pCmd->tBcdStartTime, sizeof(tBCDTime));
		memcpy_((u8*) &pe->tParam.tBcdEndTime, (u8*) &pCmd->tBcdEndTime, sizeof(tBCDTime));

		QACTIVE_POST(AO_Video, (QEvt* )pe, (void *)0);
	}
		break;

		//�洢��ý�����ݼ���Ӧ������
	case MSGID_T_StoredMediaQueryAck:			//0x0802
		break;

		//�洢��ý�������ϴ�����
	case MSGID_S_StoredMediaRpt:			//0x8803
	{
		tMsg_SCMD_MediaDataUpload *pCmd;
		MediaDataUploadEvt *pe;

		pCmd = (tMsg_SCMD_MediaDataUpload*) pMsgBlock;

		pe = Q_NEW(MediaDataUploadEvt, MEDIADATA_UPLOAD_SIG);

		pe->u16CmdSeq = pHead->u16MsgCircularId;

		pe->tParam.u8MediaType = pCmd->u8MediaType;
		pe->tParam.u8EventCode = pCmd->u8EventCode;
		pe->tParam.u8ChannelId = pCmd->u8ChannelId;
		pe->tParam.u8DeleteTag = pCmd->u8DeleteTag;
		memcpy_((u8*) &pe->tParam.tBcdStartTime, (u8*) &pCmd->tBcdStartTime, sizeof(tBCDTime));
		memcpy_((u8*) &pe->tParam.tBcdEndTime, (u8*) &pCmd->tBcdEndTime, sizeof(tBCDTime));

		QACTIVE_POST(AO_Video, (QEvt* )pe, (void *)0);
	}
		break;

		//¼����ʼ����
	case MSGID_S_StartAudioRecord:			//0x8804
	{
		tMsg_SCMD_RecordAudio *pCmd;
		AudioRecordEvt *pe;

		pCmd = (tMsg_SCMD_RecordAudio*) pMsgBlock;

		ENDIAN_U16(pCmd->u16RecordTime);

		pe = Q_NEW(AudioRecordEvt, AUDIO_RECORD_START_SIG);

		pe->u16CmdSeq = pHead->u16MsgCircularId;

		pe->tParam.u16RecordTime = pCmd->u16RecordTime;
		pe->tParam.u8RecordCmd = pCmd->u8RecordCmd;
		pe->tParam.u8SampleRate = pCmd->u8SampleRate;
		pe->tParam.u8SaveTag = pCmd->u8SaveTag;

		QACTIVE_POST(AO_Audio, (QEvt* )pe, (void *)0);
	}
		break;

		///	8.60 �����洢��ý�����ݼ����ϴ�����	��ϢID��0x8805
	case MSGID_S_SingleMediaRetrieve: {
		tMsg_SCMD_SingleStoredMediaUpload *pCmd;
		SingleMediaDataUploadEvt *pe;

		pCmd = (tMsg_SCMD_SingleStoredMediaUpload*) pMsgBlock;

		ENDIAN_U32(pCmd->u32MediaId);

		pe = Q_NEW(SingleMediaDataUploadEvt, MEDIADATA_SINGAL_UPLOAD_SIG);

		pe->u16CmdSeq = pHead->u16MsgCircularId;

		pe->tParam.u32MediaId = pCmd->u32MediaId;
		pe->tParam.u8DeleteFlag = pCmd->u8DeleteFlag;

		QACTIVE_POST(AO_Audio, (QEvt * )pe, (void *)0);
	}
		break;

		//��������͸������
	case MSGID_S_TransparentTransfer:			//0x8900
	{
		TransparentUploadEvt *pe;

		pe = Q_NEW(TransparentUploadEvt, TRANSPARENT_UPLOAD_SIG);

		pe->u8Type = *pMsgBlock++;
		memcpy_(pe->data, pMsgBlock, u16MsgBlockLen - 1);

		QACTIVE_POST(AO_Audio, (QEvt* )pe, (void *)0);
	}
		break;

		//��������͸������
	case MSGID_T_TransparentTransfer:			//0x0900
		break;

		//����ѹ���ϱ�
	case MSGID_T_DataCompressRpt:			//0x0901
		break;

		//ƽ̨RAS��Կ
	case MSGID_S_PlatformRASKey:			//0x8A00
		break;

		//�ն�RAS��Կ
	case MSGID_T_RASKey:			//0x0A00
		break;

	default:

		break;
	} ///switch
}

/**
 * ��Ϣת�壬����
 *
 * ����0x7e ��ʾ����У���롢��Ϣͷ�Լ���Ϣ���г���0x7e����Ҫ����ת�崦��ת����������£�
 * 0x7e <��������> 0x7d �����һ��0x02��
 * 0x7d <��������> 0x7d �����һ��0x01��
 * ת�崦��������£�
 * ������Ϣʱ����Ϣ��װ����>���㲢���У���롪��>ת�壻
 * ������Ϣʱ��ת�廹ԭ����>��֤У���롪��>������Ϣ��
 * ʾ��������һ������Ϊ0x30 0x7e 0x08 0x7d 0x55 �����ݰ����򾭹���װ���£�
 * 	0x7e 0x30 7d 0x02 0x08 0x7d	0x01 0x55 0x7e��
 *
 * @param pu8Data		[in]	�����͵������׵�ַ��
 * @param u16Length		[in]	�����͵����ݳ���
 * @param pu8DestData	[in]	ת�������׵�ַ
 * @return	u16��ת�����ݳ���
 * @note
 */
u16 EnCodeData(u8* const pu8Data, u16 u16Length, u8* pu8DestData) {
	u16 u16ResultLength = 0;
	u16 u16i = 0;
	u8* pu8SourceAddr = NULL;
	u8* pu8DestAddr = NULL;

	pu8SourceAddr = pu8Data;
	pu8DestAddr = pu8DestData;

	for (u16i = 0; u16i < u16Length; u16i++) {
		if (*pu8SourceAddr == MSG_FLAG_BYTE) {
			if (u16i == 0 || u16i == u16Length - 1) {
				*pu8DestAddr++ = MSG_FLAG_BYTE;
				u16ResultLength++;
			}
			else {
				*pu8DestAddr++ = MSG_TRANS_BYTE;
				*pu8DestAddr++ = 0x02;

				u16ResultLength += 2;
			}
		}
		else {
			if (*pu8SourceAddr == MSG_TRANS_BYTE) {
				*pu8DestAddr++ = MSG_TRANS_BYTE;
				*pu8DestAddr++ = 0X01;

				u16ResultLength += 2;
			}
			else {
				*pu8DestAddr++ = *pu8SourceAddr;
				u16ResultLength += 1;
			}
		}
		pu8SourceAddr++;
	}

	return u16ResultLength;
}

/**
 * ���յ�����Ϣת��
 *
 * @param pu8Data		[in]	���յ������׵�ַ��
 * @param u16Length		[in]	���յ����ݳ���
 * @param pu8DestData	[in]	ת����ɴ洢�������׵�ַ
 * @return	u16��ת����ɺ����ݳ���
 */
u16 DeCodeData(const u8* pu8Data, u16 u16Length, u8* pu8DestData) {
	u16 u16ResultLength = 0;
	u16 u16i = 0;
	u8* pu8SourceAddr = NULL;
	u8* pu8DestAddr = NULL;

	pu8SourceAddr = pu8Data;
	pu8DestAddr = pu8DestData;

	for (u16i = 0; u16i < u16Length; u16i++) {
		if ((*pu8SourceAddr == MSG_TRANS_BYTE) && (u16i < (u16Length - 1)) && (*(pu8SourceAddr + 1) == 0x02)) {
			*pu8DestAddr++ = MSG_FLAG_BYTE;
			u16ResultLength += 1;
			pu8SourceAddr += 2;
		}
		else if ((*pu8SourceAddr == MSG_TRANS_BYTE) && (u16i < (u16Length - 1)) && (*(pu8SourceAddr + 1) == 0x01)) {
			*pu8DestAddr++ = MSG_TRANS_BYTE;
			u16ResultLength += 1;
			pu8SourceAddr += 2;
		}
		else {
			*pu8DestAddr++ = *pu8SourceAddr;
			u16ResultLength += 1;
			pu8SourceAddr++;
		}
	}

	return u16ResultLength;
}

/**
 * �������У���
 *
 * @param pData		[IN]	����
 * @param u16Length	[IN]	���ݳ���
 * @return	u8,	У����
 */
u8 CalculateXorChecksum(u8* pData, u16 u16Length) {
	u8 u8Checksum = 0;

	if (pData == NULL || u16Length == 0) {
		return u8Checksum;
	}

	u8Checksum = *pData++;
	u16Length--;
	while (u16Length) {
		u8Checksum ^= *pData++;
		u16Length--;
	}

	return u8Checksum;
}

/**
 * ��֯���������ݰ�����ʵ�ʷ���ǰ����
 *
 * @param pTaskEvt	 �����¼�
 * @param pBuf		�����ͻ���
 * @param bufSize	�����ͻ�������
 * @return	ʵ�ʴ������ֽ���
 */
u16 Protocol_FormRawFrame(TaskEvt *pTaskEvt, u8 *pBuf, u16 bufSize) {
	tMSGHEAD *pHead;
	u8 *pData, *pCursor, *pMsg;
	u16 frameLen = 0;
	int ret = 0;

	if ((pTaskEvt == NULL) || (pBuf == NULL) || (bufSize == 0))
		return 0;

	ZeroMem(pBuf, bufSize);

	pHead = (tMSGHEAD *) pBuf;

	/**�����Ϣͷ�ֶ�*/
	pHead->u8Flag = MSG_FLAG_BYTE;	///��ʶλ
	pHead->u16MsgId = pTaskEvt->cmd;	///��ϢID
//	pHead->u16MsgProperty = 0; 	///��Ϣ���ԣ��������������
	memcpy_(pHead->u8BcdPhoneNum, ptParam_Runtime->aPhoneNum, 6);	///�ն��ֻ���
	pHead->u16MsgCircularId = pTaskEvt->sequence;	///��Ϣ��ˮ�����¼�����ʱ������

	pCursor = pBuf + sizeof(tMSGHEAD);
	pMsg = pCursor; ///������Ϣ����ʼλ�ã����������Ϣ����

	TRACE_(QS_USER, NULL, "\n[SEND MSG]");

	switch (pTaskEvt->cmd) {
	//�ն�ͨ��Ӧ����Ϣ
	case MSGID_T_GeneralAck: 			///0x0001
	{
		tMsg_T_GeneralAck *pMsgBlock;

		/**��Ϣ��*/
		pMsgBlock = (tMsg_T_GeneralAck*) pCursor;
		pMsgBlock->u16CmdId = pTaskEvt->resCmd;	///��Ӧ��ƽ̨��Ϣ��ID
		pMsgBlock->u16ResponsePackId = pTaskEvt->resSeqence;	///��Ӧ��ƽ̨��Ϣ����ˮ��
		pMsgBlock->eResult = pTaskEvt->ret; ///0���ɹ�/ȷ�ϣ�1��ʧ�ܣ�2����Ϣ����3����֧��
		/**У����*/
		pCursor += sizeof(tMsg_T_GeneralAck);

		TRACE_(QS_USER, NULL, "[T-SEND] type:GeneralAck, \n id:%x, \n u16CmdId=%x, \n u16ResponsePackId=%x, \n ret=%x",
		MSGID_T_GeneralAck, pMsgBlock->u16CmdId, pMsgBlock->u16ResponsePackId, pMsgBlock->eResult);
	}
		break;

//	//ƽ̨ͨ��Ӧ����Ϣ
//	case	MSGID_S_GeneralAck: 				//0x8001
//
//		break;

#ifdef JTT808_Ver_2013
		//�����ְ�����
//	case MSGID_S_ReloadSubPack: 				//0x8003
//		break;
#endif

		//�ն�����
	case MSGID_T_Heatbeat: 				//0x0002
		/**�ն�����������Ϣ��Ϊ�ա�*/
		break;

		//�ն�ע��
	case MSGID_T_Register: 				//0x0100
	{
		tMsg_T_Register *pRegInfo = (tMsg_T_Register*)pCursor;
		pRegInfo->u16ProviceId = ptParam_Device->u16ProviceId;		///ʡ��ID
		pRegInfo->u16CountyId = ptParam_Device->u16CountyId = 0x0;		///������ID
		memcpy_(pRegInfo->au8FactoryId, ptParam_Device->au8FactoryId, 5);	///������ID
	#ifdef JTT808_Ver_2011
		memcpy_(pRegInfo->aType, ptParam_Device->aType, 8);	///�ն��ͺţ�8���ֽڣ����ն��ͺ������������ж��壬λ����	��ʱ�����ո�
		memcpy_(pRegInfo->aTId, ptParam_Device->aTId, 7);///�ն�ID,7 ���ֽڣ��ɴ�д��ĸ��������ɣ����ն�ID �����������ж��壬λ������ʱ���󲹡�0X00����
		pRegInfo->u8PlateColor = ptParam_Device->u8PlateColor;///������ɫ������JT/T415-2006 ��5.4.12
	#endif
	#ifdef JTT808_Ver_2013
		memcpy_(pRegInfo->aType, ptParam_Device->aType, 20);	///�ն��ͺ�20 ���ֽڣ����ն��ͺ������������ж��壬λ������ʱ���󲹡�0X00����
		memcpy_(pRegInfo->aTId, ptParam_Device->aTId, 7);		///�ն�ID
		pRegInfo->u8PlateColor = ptParam_Device->u8PlateColor;	///������ɫ������JT/T415-2006 ��5.4.12��δ����ʱ��ȡֵΪ0��
	#endif///JTT808_Ver_2013

		memcpy_(pRegInfo->aPlate, ptParam_Device->aPlate, strlen_(ptParam_Device->aPlate)); /**V2011�� ����������;
											 V2013�����Ƴ�����ɫΪ0 ʱ����ʾ����VIN�����򣬱�ʾ���� ��ͨ�����Ű䷢�Ļ��������ơ�*/

		pCursor += sizeof(tMsg_T_Register) + strlen_((char*) ptParam_Device->aPlate);
	}
		break;

//	//�ն�ע��Ӧ��
//	case	MSGID_S_RegisterAck: 				//0x8100
//
//		break;

		//�ն�ע��
	case MSGID_T_Unregister: 				//0x0003
		/*�ն�ע����Ϣ��Ϊ��*/
		break;

		//�ն˼�Ȩ
	case MSGID_T_Authentication: 				//0x0102
		/**�ն��������ϱ���Ȩ��*/
		strcpy_((char*) pCursor, (char*) ptParam_Runtime->sAuthorCode);
		pCursor += strlen_((char*) ptParam_Runtime->sAuthorCode);
		break;

		//�����ն˲���
//	case	MSGID_S_SetParam: 				//0x8103
//		break;

		//��ѯ�ն˲���
//	case	MSGID_S_QueryParam: 				//0x8104
//		break;

#ifdef JTT808_Ver_2013
		//��ѯָ���ն˲���
//	case	MSGID_T_QuerySpecifyParam: 			//0x8106
//		break;

#endif
		//��ѯ�ն˲���Ӧ��
	case MSGID_T_QueryParamAck: 				//0x0104
	{
		u16 u16Len = 0;

		/**��֯Ӧ�����*/
		u16Len = Protocol_FormSysParams(pCursor, pTaskEvt->resSeqence);
		pCursor += u16Len;
	}
		break;

		//�ն˿���
//	case	MSGID_S_Control	: 				//0x8105
//		break;

#ifdef JTT808_Ver_2013
		//��ѯ�ն�����
//	case	MSGID_S_QueryProperty:			//0x8107
//	break;

		//��ѯ�ն�����Ӧ��
	case MSGID_T_QueryPropertyAck:			//0x0107
	{
		tMsg_TACK_QueryTerminalProperty *pProperty;

		pProperty = (tMsg_TACK_QueryTerminalProperty*)pCursor;

		pProperty->tType = ptParam_Device->tType;	///�ն����� WORD
		ENDIAN_U16(pProperty->tType);
		memcpy_(pProperty->factoryId, ptParam_Device->au8FactoryId, 5);	///������ID BYTE[5] 5 ���ֽڣ��ն������̱��롣
		memcpy_(pProperty->class, ptParam_Device->aType, 20);	///�ն��ͺ� ���ն��ͺ������������ж��壬λ������ʱ���󲹡�0X00����
		memcpy_(pProperty->tid, ptParam_Device->aTId, 7);	///�ն�ID �ɴ�д��ĸ��������ɣ����ն�ID �����������ж��壬λ������ʱ���󲹡�0X00����
		memcpy_(pProperty->ICCID, ptParam_Runtime->ICCID, 10);	///�ն�SIM ��ICCID BCD[10] �ն�SIM ��ICCID ��
		pProperty->ver_hardware_len = strlen_((char*)ptParam_Device->ver_hardware);	///�ն�Ӳ���汾�ų���
		pCursor += 42;
		memcpy_(pCursor, ptParam_Device->ver_hardware, pProperty->ver_hardware_len);///�ն�Ӳ���汾�� STRING
		pCursor += pProperty->ver_hardware_len;
		*pCursor++ = strlen_((char*)ptParam_Device->ver_software);	///�ն�����汾�ų���
		memcpy_(pCursor, ptParam_Device->ver_software, pProperty->ver_software_len);///�ն�����汾�� STRING
		pCursor++;
		*pCursor++ = ptParam_Device->tGNSSModuleProperty; ///GNSS ģ������ BYTE
		*pCursor = ptParam_Device->tCommModuleProperty; ///ͨ��ģ������ BYTE

		pCursor += 42 + pProperty->ver_hardware_len + 1 + pProperty->ver_software_len + 2;
	}
		break;

		//�·��ն�������
//	case	MSGID_T_DispathUpgPack:			//0x8108
//	break;

		//�ն��������֪ͨ
	case MSGID_T_UpgrageResult:			//0x0108
	{
		tMsg_TACK_UpgResult *pUpgRet = (tMsg_TACK_UpgResult *)pCursor;
		pUpgRet->upg_result = pTaskEvt->ret;	///�������
		pUpgRet->upg_type = pTaskEvt->U.arr[0];	///��������

		pCursor += sizeof(tMsg_TACK_UpgResult);
	}
		break;
#endif

		//λ����Ϣ�㱨
	case MSGID_T_RptPositionInfo:			//0x0200
	{
		u16 u16Len = 0;

		u16Len = Protocol_GetPositionInfo(pCursor);	///ȡλ����Ϣ
		pCursor += u16Len;
	}
		break;

		//λ����Ϣ��ѯ
//	case	MSGID_S_QueryPosition:			//0x8201
//		break;

		//λ����Ϣ��ѯӦ��
	case MSGID_T_QueryPositionAck:			//0x0201
	{
		tMsg_TACK_QueryPositionInfoAck *pAck;
		u16 u16Len = 0;

		pAck = (tMsg_TACK_QueryPositionInfoAck *)pCursor;

		pAck->u16SeqNum = pTaskEvt->resSeqence;
		ENDIAN_U16(pAck->u16SeqNum);

		u16Len = Protocol_GetPositionInfo((u8*)&pAck->tInfo);	///ȡλ����Ϣ

		pCursor += (u16Len + 1);
	}
		break;

		//��ʱλ�ø��ٿ���
//	case	MSGID_S_Tracking:					//0x8202
//	break;

#ifdef JTT808_Ver_2013
		//�˹�ȷ�ϱ�����Ϣ
//	case	MSGID_S_ConfirmWarningInfo:		//0x8203
//	break;

#endif
		//�ı���Ϣ�·�
//	case	MSGID_S_DownloadTxtInfo:			//0x8300
//	break;

		//�¼�����
//	case	MSGID_S_SetEvent:					//0x8301
//	break;

		//�¼�����
	case MSGID_T_RptEvent:					//0x0301

		*pCursor++ = pTaskEvt->ret;	///�¼�ID������ret�ֶ�
		break;

		//�����·�
//	case	MSGID_S_AskQuestion:				//0x8302
//	break;

		//����Ӧ��
	case MSGID_T_QuestionAck:				//0x0302
	{
		tMsg_TACK_AnswerQuestion *pAnswer;
		pAnswer = (tMsg_TACK_AnswerQuestion *)pCursor;

		pAnswer->u8AnswerId = pTaskEvt->ret;	///�����ID������ret�ֶ�
		pAnswer->u16SeqNum = pTaskEvt->resSeqence;
		ENDIAN_U16(pAnswer->u16SeqNum);

		pCursor += sizeof(tMsg_TACK_AnswerQuestion);
	}
		break;

		//��Ϣ�㲥�˵�����
//	case	MSGID_S_SetInfoOnDemandMenu:		//0x8303
//	break;

		//��Ϣ�㲥/ȡ��
	case MSGID_T_OrderInfoCtrl:			//0x0303
	{
		tMsg_T_InfoOrderCtrl *pCtrl;
		pCtrl = (tMsg_T_InfoOrderCtrl *)pCursor;

		pCtrl->u8InfoOrderSetType = pTaskEvt->U.arr[0];
		pCtrl->u8OrderOrCancel = pTaskEvt->U.arr[1];

		pCursor += sizeof(tMsg_T_InfoOrderCtrl);
	}
		break;

		//��Ϣ����
//	case	MSGID_S_InfoService:				//0x8304
//	break;

		//�绰�ز�
//	case	MSGID_S_Redial:					//0x8400
//	break;

		//���õ绰��
//	case	MSGID_S_SetPhoneBook:				//0x8401
//	break;

		//��������
//	case	MSGID_S_VechicleCtrl:				//0x8500
//	break;

		//��������Ӧ��
	case MSGID_T_VechicleCtrlAck:			//0x0500
	{
		tMsg_TACK_VechicalCtrl *pAck;
		u16 u16Len = 0;

		pAck = (tMsg_TACK_VechicalCtrl *)pCursor;

		pAck->u16SeqNum = pTaskEvt->resSeqence;
		ENDIAN_U16(pAck->u16SeqNum);

		u16Len = Protocol_GetPositionInfo((u8*)&pAck->tBasicPos);	///ȡλ����Ϣ

		pCursor += (u16Len + 2);
	}
		break;

		//����Բ������
//	case	MSGID_S_SetRoundErea:			//0x8600
//	break;

		//ɾ��Բ������
//	case	MSGID_S_DelRoundErea:			//0x8601
//	break;

		//���þ�������
//	case MSGID_S_SetRectangleErea:			//0x8602
//
//		break;

		//ɾ����������
//	case MSGID_S_DelRectangleErea:			//0x8603
//
//		break;

		//���ö��������
//	case	MSGID_S_SetPolygonErea:			//0x8604
//	break;

		//ɾ�����������
//	case	MSGID_S_DelPolygonErea:			//0x8605
//	break;

		//����·��
//	case	MSGID_S_SetPathline:			//0x8606
//	break;

		//ɾ��·��
//	case	MSGID_S_DelPathline:			//0x8607
//	break;

		//��ʻ��¼�����ݲɼ�����
//	case	MSGID_S_RecorderCapture:			//0x8700
//	break;

		//��ʻ��¼�������ϴ�
	case MSGID_T_RecorderDataUpload:			//0x0700

		break;

		//��ʻ��¼�����´�����
//	case	MSGID_S_RecorderParamDownload:			//0x8701
//	break;

		//�����˵��ϱ�
	case MSGID_T_EBillRpt:			//0x0701
	{
		tETICKET *pTicket = (tETICKET *)pCursor;

		pTicket->u32ETicketLen = ptParam_Runtime->tEticket.u32ETicketLen;
		ENDIAN_U32(pTicket->u32ETicketLen);

		memcpy_(pTicket->aData, ptParam_Runtime->tEticket.aData, ptParam_Runtime->tEticket.u32ETicketLen);

		pCursor += (4 + ptParam_Runtime->tEticket.u32ETicketLen);
	}
		break;

		//��ʻԱ�����Ϣ�ɼ��ϱ�
	case MSGID_T_DriverInfoRpt:			//0x0702
	{
#ifdef JTT808_Ver_2011
	*pCursor++ = ptParam_Runtime.tDriverInfo.u8DriverNameLen;///��ʻԱ��������
	memcpy_(pCursor, ptParam_Runtime.tDriverInfo.aDriverName,
			ptParam_Runtime.tDriverInfo.u8DriverNameLen);///��ʻԱ����������n
	pCursor += ptParam_Runtime.tDriverInfo.u8DriverNameLen;
	memcpy_(pCursor, ptParam_Runtime.tDriverInfo.aDrvIdCode, 20);///��ʻԱ���֤����
	pCursor += 20;
	memcpy_(pCursor, ptParam_Runtime.tDriverInfo.aQulCode, 40);///��ҵ�ʸ�֤����
	pCursor += 40;
	*pCursor++ = ptParam_Runtime.tDriverInfo.u8GovNameLength;///��֤�������Ƴ���
	memcpy_(pCursor, ptParam_Runtime.tDriverInfo.aGovName,
			ptParam_Runtime.tDriverInfo.u8GovNameLength);///��֤��������
	pCursor += ptParam_Runtime.tDriverInfo.u8GovNameLength;
#endif

#ifdef JTT808_Ver_2013
	*pCursor++ = ptParam_Runtime->tDriverInfo.state;
	memcpy_(pCursor, ptParam_Runtime->tDriverInfo.time_BCD, 6);
	pCursor += 6;
	*pCursor++ = ptParam_Runtime->tDriverInfo.IC_read_ret;
	*pCursor++ = ptParam_Runtime->tDriverInfo.u8DriverNameLen;
	memcpy_(pCursor, ptParam_Runtime->tDriverInfo.aDriverName,
			ptParam_Runtime->tDriverInfo.u8DriverNameLen);
	pCursor += ptParam_Runtime->tDriverInfo.u8DriverNameLen;
	memcpy_(pCursor, ptParam_Runtime->tDriverInfo.aQulCode, 20);///��ҵ�ʸ�֤����
	pCursor += 20;
	*pCursor++ = ptParam_Runtime->tDriverInfo.u8GovNameLength;///��֤�������Ƴ���
	memcpy_(pCursor, ptParam_Runtime->tDriverInfo.aGovName,
			ptParam_Runtime->tDriverInfo.u8GovNameLength);///��֤��������
	pCursor += ptParam_Runtime->tDriverInfo.u8GovNameLength;
	memcpy_(pCursor, ptParam_Runtime->tDriverInfo.LicieceValidation, 4);
	pCursor += 4;
#endif
	}
		break;

#ifdef JTT808_Ver_2013
		//��λ���������ϴ�
	case MSGID_T_PositionInfoBatchRpt:			//0x0704
	{
//		tMsg_T_PositionInfoBatchRpt *pMsg = (tMsg_T_PositionInfoBatchRpt *)pCursor;
	}
		break;

		//CAN���������ϴ�
	case MSGID_T_CANDataUpload:			//0x0705

		break;

#endif
		//��ý���¼���Ϣ�ϴ�
	case MSGID_T_MediaEventInfoRpt:			//0x0800

		break;

		//��ý�������ϴ�
	case MSGID_T_MediaDataRpt:			//0x0801

		break;

		//��ý�������ϴ�Ӧ��
//	case	MSGID_S_MediaDataRptAck:			//0x8800
//	break;

		//����ͷ������������
//	case	MSGID_S_TakePhoto:			//0x8801
//	break;

#ifdef JTT808_Ver_2013
		//����ͷ������������Ӧ��
	case MSGID_S_TakePhotoAck:			//0x0805

		break;

#endif
		//�洢��ý�����ݼ�������
//	case	MSGID_S_StoredMediaQuery:			//0x8802
//	break;

		//�洢��ý�����ݼ���Ӧ������
	case MSGID_T_StoredMediaQueryAck:			//0x0802

		break;

		//�洢��ý�������ϴ�����
//	case	MSGID_S_StoredMediaRpt:			//0x8803
//	break;

		//¼����ʼ����
//	case	MSGID_S_StartAudioRecord:			//0x8804
//	break;

		//��������͸������
//	case	MSGID_S_TransparentTransfer:			//0x8900
//	break;

		//��������͸������
	case MSGID_T_TransparentTransfer:			//0x0900

		break;

		//����ѹ���ϱ�
	case MSGID_T_DataCompressRpt:			//0x0901

		break;

		//ƽ̨RAS��Կ
//	case	MSGID_S_PlatformRASKey:			//0x8A00
//	break;

		//�ն�RAS��Կ
	case MSGID_T_RASKey:			//0x0A00

		break;

	default:

		break;
	}

	/**������Ϣͷ���ԣ�δ���ְ����������ǲ���Ҫ�ְ���������......*/
	pHead->tMsgProperty.msg_encrypt = 0;
	pHead->tMsgProperty.msg_subpack = 0;
	pHead->tMsgProperty.msg_len = (u16)(pCursor - pMsg) + 1;

	*pCursor = CalculateXorChecksum(pBuf, (u16)(pCursor - pBuf)); ///У����
	pCursor++;  ///������־
	*pCursor = MSG_FLAG_BYTE;

	ENDIAN_MSGHEAD_LITTLE2BIG(pHead);	///��ϢͷС��ת���ɴ��

	/**�����ת�崦����ע��ת����̷��ڷ���ǰת�壬�������ڴ˴�������Ļ���������һ����ʱ����*/
	///EncodeData();
	return (pCursor - pBuf + 1); 	///����ʵ��֡����
}

/**
 * ������ͨ��Ӧ����Ϣ����
 *
 * @param pSAck		Ӧ����Ϣ
 */
void Protocol_ServerGeneralAckProc(tMsg_S_GeneralAck *pSAck) {
//	SAckEvt *pe;
//	Q_ASSERT(pSAck);
//
//	/**���������Ӧ���¼�������*/
//	pe = Q_NEW(SAckEvt, GPRS_SACK_SIG);
//	Q_ASSERT(pe);
//	pe->u16RespMsgId = pSAck->u16RespMsgId;
//	pe->u16RespSeqId = pSAck->u16RespSeqId;
//	pe->ret = (u8) pSAck->eResult;
//
//	QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
}

/**
 * �ն˷���ͨ������Ӧ����Ϣ
 *
 * @param u16ResMsgId		Ӧ����ϢID
 * @param u16ResSeqId		Ӧ����ˮ��
 * @param ret				�������
 */
void Protocol_T_SendGeneralAck(u16 u16ResMsgId, u16 u16ResSeqId, u8 ret) {
	TaskEvt *pe;

	/**��������Ӧ�����¼�������*/
	pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
	Q_ASSERT(pe);
	pe->cmd = MSGID_T_GeneralAck;	///0x0001
	pe->resCmd = u16ResMsgId;		///Ӧ����ϢID
	pe->sequence = GenerateCirculateSeqId();	///��ˮ��
	pe->resSeqence = u16ResSeqId;	///Ӧ����ϢID
	pe->ret = ret;		///���

	QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
}

/**
 * ѭ��������Ϣ��ˮ��
 *
 * @return	��ˮ��
 */
u16 GenerateCirculateSeqId() {
	static u16 seq = 0;
	return seq++;
}

/**
 * ��Ϣ����У��
 *
 * ÿ����Ϣ�ɱ�ʶλ����Ϣͷ����Ϣ���У�������
 *
 * @param pMsg		[IN]	��Ϣ����
 * @param u16Len	[IN]	��Ϣ����
 * @return	TRUE=��ȷ��FALSE=����
 */
BOOL CheckFrame(u8 *pMsg, u16 u16Len) {
	if (*(pMsg + u16Len - 2) == CalculateXorChecksum(pMsg, u16Len - 2))
		return TRUE;

	return FALSE;
}

/**
 * ��֯Ӧ�����
 *
 * @param pCursor	������ַ
 * @param seq		Ӧ����Ϣ��ˮ��
 * @return	��������
 */
u16 Protocol_FormSysParams(u8* pCursor, u16 seq) {
	u16 u16Len = 0;
	tMSG_TACK_QueryParams *pBlock;

	pBlock = (tMSG_TACK_QueryParams*) pCursor;

	pBlock->u8ParamAmount = 0;
	pBlock->u16SequenceNumber = seq;
	ENDIAN_U16(pBlock->u16SequenceNumber);

	pCursor += 3;
	u16Len += 3;

	/**	ѭ���������������*/
	u16Len += GetItem_AllSystemParam(pCursor);

	return u16Len;
}

/**
 * ӳ�����ID->��־λ�����ڡ���ѯָ�������������ã���ָ������ת���ɱ�־���飬��ʡ�ռ�
 * ӳ���ϵ��                        7      6      5      4      3      2      1     0
 * 			 ��������������������������������������������������������������������������������������������������������������
 *           |  7   |  6   |  5   |  4   |  3   |   2  |  1  |  0  |		pArr[0]
 * 			 ��������������������������������������������������������������������������������������������������������������
 *           |  15  |  14  |  13  |  12  |  11  |  10  |  9  |  8  |		pArr[1]
 * 			 ��������������������������������������������������������������������������������������������������������������
 *			 |   ...                                               |
 * 			 ����������������������������������������������������������������������������������������������������������������
 *           |  79  |  78  |  77  |  76  |  75  |  74  |  73 |  72 |		pArr[9]
 * 			 ����������������������������������������������������������������������������������������������������������������
 *
 * @param pArr	��־����
 * @param len	��־���鳤��
 * @param id	����ID
 * @return	0=�ɹ���-1=����
 */
int map_param_id_2_index(u8 *pArr, u8 len, u32 id) {
	u8 i;

	for (i = 0; i < PARAM_AMOUNT; i++) {
		if (param_id[i] == id)
			break;
	}

	if (i >= PARAM_AMOUNT)
		return -1;

	pArr[(i >> 3)] |= 0x01 << (i % 7);
	return 0;
}

/**
 * ���������ӳ��Ϊ����ID
 *
 * @param pArr	��־����
 * @param len	��־���鳤��
 * @param id	����ID
 * @return	0=�ɹ���-1=����
 */
u32 map_index_2_param_id(u8 row, u8 column) {
	return (param_id[(row << 3) + column]);
}

/**
 * ȡ�¸��ֶΣ��ԡ�;���ָ�
 * @param msg
 * @param seg_buf
 * @return
 */
u8 get_next_segment(char *msg, char *seg_buf) {
	u8 len = 0;

	while (*msg != ';') {
		seg_buf[len++] = *msg++;
	}

	return len;
}

/**
 * ȡλ�����ݣ�����������Ϣ�͸�����Ϣ
 *
 * @param pBuf		�ڴ滺��
 * @return		λ�����ݳ���
 */
u16 Protocol_GetPositionInfo(u8 *pBuf) {
	u16 u16Len = 0;
	tMsg_T_PositionInfo *pInfo = NULL;

	pInfo = (tMsg_T_PositionInfo *)pBuf;

	memcpy_((u8*)&pInfo->tBaseInfo.tSysState, (u8*)&ptParam_Runtime->tSysState, sizeof(tSYSTEMSTATE));
	memcpy_((u8*)&pInfo->tBaseInfo.tWarningState, (u8*)&ptParam_Runtime->tWarningState, sizeof(tWARNINGSTATE));
	memcpy_((u8*)&pInfo->tBaseInfo.tGpsInfo, (u8*)&ptParam_Runtime->tGpsInfo, sizeof(tGPSINFO));

	///���λ�ø�����Ϣ��δ����
	if(0 == ptParam_Runtime->bOverSpeed) {
		tMsg_Position_AppendInfo_NonOverSpeed * pApendInfo = &pInfo->tAppendInfo.tAppendInfo_NonOverSpeed;
		pApendInfo->ID_1 = 0x01;
		pApendInfo->ID_1_len = 4;
		pApendInfo->u32Kilemeters = ptParam_Runtime->u32Kilemeters;
		ENDIAN_U32(pApendInfo->u32Kilemeters);

		pApendInfo->ID_2 = 0x02;
		pApendInfo->ID_2_len = 2;
		pApendInfo->u16Oil = ptParam_Runtime->u16Oil; 	///0x02 2 ������WORD��1/10L����Ӧ�������������
		ENDIAN_U16(pApendInfo->u16Oil);

		pApendInfo->ID_3 = 0X03;
		pApendInfo->ID_3_len = 2;
		pApendInfo->u16Speed = ptParam_Runtime->u16Speed;	///0x03 2 ��ʻ��¼���ܻ�ȡ���ٶȣ�WORD��1/10km/h
		ENDIAN_U16(pApendInfo->u16Speed);

	#ifdef JTT808_Ver_2013
		pApendInfo->ID_4 = 0x04;
		pApendInfo->ID_4_len = 2;
		pApendInfo->u16WarningEvtId = ptParam_Runtime->u16WarningEvtId; ///0x04 2 ��Ҫ�˹�ȷ�ϱ����¼���ID��WORD����1 ��ʼ����
		ENDIAN_U16(pApendInfo->u16WarningEvtId);
	#endif
		/**0x05-0x10 ����*/
		pApendInfo->ID_11 = 0x11;
		pApendInfo->ID_11_len = 1;
		pApendInfo->tNonOverspeedWarn.u8PosType = 0; ///1 ��5 ���ٱ���������Ϣ����28

		pApendInfo->ID_12 = 0x12;
		pApendInfo->ID_12_len = sizeof(pApendInfo->tBarrierWarn);
		///6 ��������/·�߱���������Ϣ����29
		pApendInfo->tBarrierWarn.u8PosType = ptParam_Runtime->tBarrierWarn.u8PosType;
		pApendInfo->tBarrierWarn.u8EnterOrLeave = ptParam_Runtime->tBarrierWarn.u8EnterOrLeave;
		pApendInfo->tBarrierWarn.u32AreaOrLineId = ptParam_Runtime->tBarrierWarn.u32AreaOrLineId;
		ENDIAN_U32(pApendInfo->tBarrierWarn.u32AreaOrLineId);

		///0x13 7 ·����ʻʱ�䲻��/��������������Ϣ����30
		pApendInfo->ID_13 = 0x13;
		pApendInfo->ID_13_len = sizeof(pApendInfo->tDrvTimeWarn);
		pApendInfo->tDrvTimeWarn.u8Result = ptParam_Runtime->tDrvTimeWarn.u8Result;
		pApendInfo->tDrvTimeWarn.u16DriveTime = ptParam_Runtime->tDrvTimeWarn.u16DriveTime;
		pApendInfo->tDrvTimeWarn.u32LineId = ptParam_Runtime->tDrvTimeWarn.u32LineId;
		ENDIAN_U16(pApendInfo->tDrvTimeWarn.u16DriveTime);
		ENDIAN_U32(pApendInfo->tDrvTimeWarn.u32LineId);

		/**0x14-0x24 ����*/
	#ifdef JTT808_Ver_2013
		pApendInfo->ID_25 = 0x25;
		pApendInfo->ID_25_len = 4;
		pApendInfo->u32ExtSignalState = ptParam_Runtime->u32ExtSignalState;	///0x25 4 ��չ�����ź�״̬λ���������31
		ENDIAN_U16(pApendInfo->u32ExtSignalState);

		pApendInfo->ID_2A = 0x2A;
		pApendInfo->ID_2A_len = 2;
		pApendInfo->u16IOState = ptParam_Runtime->u16IOState;	///0x2A 2 IO״̬λ���������32
		ENDIAN_U16(pApendInfo->u16IOState);

		pApendInfo->ID_2B = 0x2B;
		pApendInfo->ID_2B_len = 4;
		pApendInfo->u32ADValue = ptParam_Runtime->u32ADValue;	///0x2B 4 ģ������bit0-15��AD0��bit16-31��AD1��
		ENDIAN_U32(pApendInfo->u32ADValue);

		pApendInfo->ID_30 = 0x30;
		pApendInfo->ID_30_len = 1;
		pApendInfo->u8Csq = ptParam_Runtime->u8Csq;	///0x30 1 BYTE������ͨ�������ź�ǿ��

		pApendInfo->ID_31 = 0x31;
		pApendInfo->ID_31_len = 1;
		pApendInfo->u8GNSS_Satellites = ptParam_Runtime->u8GNSS_Satellites; ///0x31 1 BYTE��GNSS ��λ������

		pApendInfo->ID_E0;	///0xE0 ������Ϣ���� �����Զ�����Ϣ����
		pApendInfo->ID_E0_len = 1;
		pApendInfo->u8ExtraInfoLen = 0;
	#endif

		u16Len += sizeof(tMsg_T_PositionInfo) - 4;
	}
	else {
		tMsg_Position_AppendInfo_OverSpeed * pApendInfo = &pInfo->tAppendInfo.tAppendInfo_OverSpeed;
		pApendInfo->ID_1 = 0x01;
		pApendInfo->ID_1_len = 4;
		pApendInfo->u32Kilemeters = ptParam_Runtime->u32Kilemeters;
		ENDIAN_U32(pApendInfo->u32Kilemeters);

		pApendInfo->ID_2 = 0x02;
		pApendInfo->ID_2_len = 2;
		pApendInfo->u16Oil = ptParam_Runtime->u16Oil; 	///0x02 2 ������WORD��1/10L����Ӧ�������������
		ENDIAN_U16(pApendInfo->u16Oil);

		pApendInfo->ID_3 = 0X03;
		pApendInfo->ID_3_len = 2;
		pApendInfo->u16Speed = ptParam_Runtime->u16Speed;	///0x03 2 ��ʻ��¼���ܻ�ȡ���ٶȣ�WORD��1/10km/h
		ENDIAN_U16(pApendInfo->u16Speed);

	#ifdef JTT808_Ver_2013
		pApendInfo->ID_4 = 0x04;
		pApendInfo->ID_4_len = 2;
		pApendInfo->u16WarningEvtId = ptParam_Runtime->u16WarningEvtId; ///0x04 2 ��Ҫ�˹�ȷ�ϱ����¼���ID��WORD����1 ��ʼ����
		ENDIAN_U16(pApendInfo->u16WarningEvtId);
	#endif
		/**0x05-0x10 ����*/
		pApendInfo->ID_11 = 0x11;
		pApendInfo->ID_11_len = 1;
		///1 ��5 ���ٱ���������Ϣ����28
		pApendInfo->tOverspeedWarn.u8PosType = ptParam_Runtime->tOverspeedWarn.u8PosType;
		pApendInfo->tOverspeedWarn.u32AreaOrLineId = ptParam_Runtime->tOverspeedWarn.u32AreaOrLineId;
		ENDIAN_U32(pApendInfo->tOverspeedWarn.u32AreaOrLineId);

		pApendInfo->ID_12 = 0x12;
		pApendInfo->ID_12_len = sizeof(pApendInfo->tBarrierWarn);
		///6 ��������/·�߱���������Ϣ����29
		pApendInfo->tBarrierWarn.u8PosType = ptParam_Runtime->tBarrierWarn.u8PosType;
		pApendInfo->tBarrierWarn.u8EnterOrLeave = ptParam_Runtime->tBarrierWarn.u8EnterOrLeave;
		pApendInfo->tBarrierWarn.u32AreaOrLineId = ptParam_Runtime->tBarrierWarn.u32AreaOrLineId;
		ENDIAN_U32(pApendInfo->tBarrierWarn.u32AreaOrLineId);

		///0x13 7 ·����ʻʱ�䲻��/��������������Ϣ����30
		pApendInfo->ID_13 = 0x13;
		pApendInfo->ID_13_len = sizeof(pApendInfo->tDrvTimeWarn);
		pApendInfo->tDrvTimeWarn.u8Result = ptParam_Runtime->tDrvTimeWarn.u8Result;
		pApendInfo->tDrvTimeWarn.u16DriveTime = ptParam_Runtime->tDrvTimeWarn.u16DriveTime;
		pApendInfo->tDrvTimeWarn.u32LineId = ptParam_Runtime->tDrvTimeWarn.u32LineId;
		ENDIAN_U16(pApendInfo->tDrvTimeWarn.u16DriveTime);
		ENDIAN_U32(pApendInfo->tDrvTimeWarn.u32LineId);

		/**0x14-0x24 ����*/
	#ifdef JTT808_Ver_2013
		pApendInfo->ID_25 = 0x25;
		pApendInfo->ID_25_len = 4;
		pApendInfo->u32ExtSignalState = ptParam_Runtime->u32ExtSignalState;	///0x25 4 ��չ�����ź�״̬λ���������31
		ENDIAN_U32(pApendInfo->u32ExtSignalState);

		pApendInfo->ID_2A = 0x2A;
		pApendInfo->ID_2A_len = 2;
		pApendInfo->u16IOState = ptParam_Runtime->u16IOState;	///0x2A 2 IO״̬λ���������32
		ENDIAN_U16(pApendInfo->u16IOState);

		pApendInfo->ID_2B = 0x2B;
		pApendInfo->ID_2B_len = 4;
		pApendInfo->u32ADValue = ptParam_Runtime->u32ADValue;	///0x2B 4 ģ������bit0-15��AD0��bit16-31��AD1��
		ENDIAN_U32(pApendInfo->u32ADValue);

		pApendInfo->ID_30 = 0x30;
		pApendInfo->ID_30_len = 1;
		pApendInfo->u8Csq = ptParam_Runtime->u8Csq;	///0x30 1 BYTE������ͨ�������ź�ǿ��

		pApendInfo->ID_31 = 0x31;
		pApendInfo->ID_31_len = 1;
		pApendInfo->u8GNSS_Satellites = ptParam_Runtime->u8GNSS_Satellites; ///0x31 1 BYTE��GNSS ��λ������

		pApendInfo->ID_E0;	///0xE0 ������Ϣ���� �����Զ�����Ϣ����
		pApendInfo->ID_E0_len = 1;
		pApendInfo->u8ExtraInfoLen = 0;
	#endif

		u16Len += sizeof(tMsg_T_PositionInfo);
	}

	return u16Len;
}

/**
 * ���������������
 *
 */
void ERROR_Handle( iRet) {

}
