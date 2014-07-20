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
///必须加上 #include <qevt.h>，否则编译报错

static const u32 param_id[] = {
	///参数项ID定义
	PARAM_ALIVE_INTERVAL,	///		0x0001		///终端心跳发送间隔，单位为s
	PARAM_TCPACK_TIMEOUT,	///				0x0002		///TCP应答超时，单位为s
	PARAM_TCP_RESENDTIMES,	///				0x0003		///TCP消息重传次数
	PARAM_UDPACK_TIMEOUT,	///				0x0004		///UDP应答超时，单位为s
	PARAM_UDP_RESENDTIMES,	///				0x0005		///UDP消息重传次数
	PARAM_SMSACK_TIMEOUT,	///				0x0006		///SMS应答超时，单位为s
	PARAM_SMS_RESENDTIMES,	///				0x0007		///SMS消息重传次数
	/**0X0008 ----0X000F 保留*/
	PARAM_MAINSRV_APN,	///					0x0010		///主服务器APN，无线通信拨号访问点
	PARAM_MAINSRV_USR,	///					0x0011		///主服务器无线通信拨号用户名
	PARAM_MAINSRV_PSWD,	///					0x0012		///主服务器无线通信拨号密码
	PARAM_MAINSRV_IP,	///					0x0013		///主服务器地址，IP或域名
	PARAM_VICESRV_APN,	///					0x0014		///备份服务器APN，无线通信拨号访问点
	PARAM_VICESRV_USR,	///				0x0015		///备份服务器无线通信拨号用户名
	PARAM_VICESRV_PSWD,	///					0x0016		///备份服务器无线通信拨号密码
	PARAM_VICESRV_IP,	///					0x0017		///备份服务器地址，IP或域名
	PARAM_TCP_PORT,	///						0x0018		///TCP端口
	PARAM_UDP_PORT,	///						0x0019		///UDP端口
	/**0X001A ----0X001F 保留*/
#ifdef JTT808_Ver_2013	///新版增加协议部分
	PARAM_ICVERIFY_MS_IPOrDNS,	///			0x001A		///道路运输证IC卡认证主服务器IP地址或域名
	PARAM_ICVERIFY_TCPPORT,	///				0x001B		///道路运输证IC卡认证主服务器TCP端口
	PARAM_ICVERIFY_UDPPORT,	///				0x001C		///道路运输证IC卡认证主服务器UDP端口
	PARAM_ICVERIFY_BS_UDPPORT,	///			0x001D		///道路运输证IC卡备份服务器IP地址或域名，端口同主服务器
#endif///JTT808_Ver_2013

	/**终端位置汇报策略，0:定时汇报，1:定距汇报，2:定时和定距汇报*/
	PARAM_POSRPT_STRATEGE,	///			0x0020
	/**终端汇报方案，0:根据ACC状态，1:根据登录状态和ACC状态，先判断登录状态，
	 若登录再根据ACC状态*/
	PARAM_POSRPT_METHOD,	///					0x0021
	PARAM_DRV_UNREG_RPTPERIOD,	///			0x0022		///驾驶员未登录汇报时间间隔，单位为秒
	/**0X0023 ----0X0026 保留*/
	PARAM_RPTPERIOD_ONSLEEP,	///				0x0027		///休眠时汇报时间间隔，单位为秒
	PARAM_RPTPERIOD_URGENT,	///				0x0028		///紧急报警时汇报时间间隔，单位为秒
	PARAM_RPTPERIOD_DEFAULT,	///			0x0029		///缺省时汇报时间间隔，单位为秒
	/**0X002A ----0X002B 保留*/
	PARAM_RPTDIST_DEFAULT,	///				0x002C		///缺省时距离汇报间隔，单位为米
	PARAM_RPTDIST_DRV_UNREG,	///				0x002D		///驾驶员未登录距离汇报间隔，单位为米
	PARAM_RPTDIST_SLEEP,	///					0x002E		///休眠时距离汇报间隔，单位为米
	PARAM_RPTDIST_URGENT,	///				0x002F		///紧急报警时距离汇报间隔，单位为米
	PARAM_INFLECTION_POINT_ANGLE,	///		0x0030		///拐点补传角度，单位为度(< 180)
	PARAM_BARRIER_RADIUS,	///				0x0031		///电子围栏半径(非法移位阈值)，单位为米
	/**0X0032----0X003F 保留*/
	PARAM_MONITORCENTER_PHONENUM,	///		0x0040		///监控平台电话号码
	PARAM_RESET_PHONENUM,	///		0x0041		///复位电话号码，可采用此电话号码拨打终端电话让终端复位
	///回复出厂设置电话号码，可采用此电话号码拨打终端电话让终端恢复出厂设置
	PARAM_RECOVERFACTORY_PHONENUM,	///		0x0042
	PARAM_MONITORCENTER_SMS_NUM,	///			0x0043		///监控平台SMS电话号码
	PARAM_RECVTXTALARM_SMS_NUM,	///			0x0044		///接收终端SMS文本报警电话号码
	///终端电话接听策略，0:自动接通，1:ACC ON时自动接通，OFF时手动接听
	PARAM_ANSWERCALLING_STRATEGE,	///		0x0045
	///终端每次通话时最长通话时间，单位为秒，0为不允许通话，0xffffffff为不限制
	PARAM_TOUT_PERCALL,	///					0x0046
	///终端每月最长通话时间，单位为秒，0为不允许通话，0xffffffff为不限制
	PARAM_TOUT_PERMONTHCALL,	///				0x0047
	PARAM_MONITORLISTEN_PHONENUM,	///		0x0048			///监听电话号码
	PARAM_SUPERVISE_SMS_NUM,	///				0x0049			///监管平台特权短信号码
	/**0X004A----0X004F 保留*/

	///报警屏蔽字，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警被屏蔽
	PARAM_ALARM_MASK,	///					0x0050
	///报警发送文本SMS开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时发送文本SMS
	PARAM_SMSCTRL_ONALARM,	///				0x0051
	///报警拍摄开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时摄像头拍摄
	PARAM_PICCTRL_ONALARM,	///				0x0052
	///报警拍摄存储标志，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时拍的照片进行存储，否则实时上传
	PARAM_PICCTRL_STORE_ONALARM,	///			0x0053
	///关键标志，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警为关键报警
	PARAM_KEYALARM_FLAG,	///		 			0x0054
	PARAM_MAXSPEED,	///						0x0055	   ///最高速度，单位为公里每小时(km/h)
	PARAM_OVERSPEED_LASTING_TIMEOUT,	///			0x0056	///超速持续时间，单位为秒
	PARAM_CONTINUOUS_DRV_TIMEOUT,	///			0x0057	///连续驾驶时间门限，单位为秒
	PARAM_DRV_TOTAL_EACHDAY_TIMEOUT,	///			0x0058	///当天累计驾驶时间，单位为秒
	PARAM_LEAST_REST_TIMEOUT,	///				0x0059	///最小休息时间，单位为秒
	PARAM_MAX_STOPPING_TIMEOUT,	///				0x005A	///最长停车时间，单位为秒
	/**0X005B----0X006F */
#ifdef JTT808_Ver_2013
	PARAM_OVERSPEED_PREALARM_DIF,	///		0x005B	///超速报警预警差值，单位1/10Km/h
	PARAM_FATIGE_PREALARM_DIF,	///			0x005C	///疲劳驾驶预警差值，单位为秒，>0
	///b7-b0:碰撞时间，单位4ms；b15-b8:碰撞加速度，单位0.1g，设置范围0~79，默认10
	PARAM_COLLISION_SETTING,	///				0x005D
	///侧翻报警参数设置，侧翻角度，单位1度，默认30度
	PARAM_SIDECOLLAPSE_SETTING,	///			0x005E
	/**0X005F----0X0063 */
	PARAM_PERIODIC_PHOTO_CTRL,	///			0x0064	///定时拍照控制
	PARAM_PERDIST_PHOTO_CTRL,	///			0x0065	///定距拍照控制
	/**0X0066----0X006F */
#endif///JTT808_Ver_2013

	PARAM_VIDEO_QUALITY,	///					0x0070		///图片/视频质量，1-10,1最好
	PARAM_BRIGHTNESS,	///					0x0071		///亮度，0-255
	PARAM_CONTRASTNESS,	///					0x0072		///对比度，0-127
	PARAM_SATURATION,	///					0x0073		///饱和度，0-127
	PARAM_CHROMA,	///						0x0074		///色度，0-255
	/**0X0075----0X007F*/
	PARAM_VEHICLE_KILOMETRES,	///			0x0080		///车辆里程表读数，1/10km
	PARAM_VEHICLE_PROVINCE_ID,	///			0x0081		///车辆所在地省域ID
	PARAM_VEHICLE_CITY_ID,	///				0x0082		///车辆所在地市域ID
	PARAM_VEHICLE_ID,	///	 				0x0083		///车牌号码
	PARAM_VEHICLE_ID_COLOR,	///				0x0084		///车牌颜色

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


#define PARAM_AMOUNT	(sizeof(param_id) >> 2)	///参数个数
/**各种辅助宏**************************************************************************//**///终端控制类，解析参数字段，以';'隔开#define GET_TCTRL_SEGMENT(msg, param)							\
	do {														\
		u8 len = get_next_segment(msg, param);					\
		param[len] = '\0';										\
	}while(0)*/

///数字字符串转换成U16
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

///错误处理宏
#define ERROR_HANDLE(iRet)					\
							do {			\
							if(iRet < 0)	\
							{						\
								ERROR_Handle(iRet);	\
								return;		\
							}				\
							}while(0)

/** 本地函数声明 ***********************************************************************/
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

/**消息头小端转换成大端*/
void __inline__ ENDIAN_MSGHEAD_LITTLE2BIG(tMSGHEAD *pMsg) {
	ENDIAN_U16(pMsg->u16MsgId);
	ENDIAN_U16(*((u16* )(&pMsg->tMsgProperty)));
	ENDIAN_U16(pMsg->u16MsgCircularId);
}

/**消息头大端转换成小端，同小端到大端转换相同*/
#define ENDIAN_MSGHEAD_BIG2LITTLE	ENDIAN_MSGHEAD_LITTLE2BIG



#if 0
/**终端发送通用命令应答消息*/
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
 * 终端与服务器通信协议数据处理，处理服务器下发命令
 *
 * @param pMsg		协议数据
 * @param msgLen	协议数据长度
 */
void Protocol_TCPIPData_Process(u8 *pMsg, u16 msgLen) {
	tMSGHEAD *pHead;
	u8 *pData;
	u8 *pMsgBlock;  ///消息体
	u16 u16MsgBlockLen = 0; ///消息体长度
	u16 u16TransMsgLen = 0;	///转义后消息长度
	int iRet = 0;
	TaskEvt *pe = NULL;

	static u8 aTransMsg[CONFIG_TCP_RXBUF_MAXLEN];	///转义消息缓区

	if ((pMsg == NULL) || (msgLen == 0))
		return;

	/**转义处理,接收消息时：转义还原——>验证校验码——>解析消息。*/
	/**先转义还原*/
	u16TransMsgLen = DeCodeData(pMsg, msgLen, aTransMsg);

	/**再内容校验*/
	if (FALSE == CheckFrame(aTransMsg, u16TransMsgLen)) {
		TRACE_(QS_USER, NULL, " ERR! Msg check failed! ");
		return;
	}

	/**以下解析消息内容*/
	pHead = (tMSGHEAD *) pMsg;

	/**消息头字段大小端转换*/
	ENDIAN_U16(pHead->u16MsgId);
	ENDIAN_U16(*((u16* )(&pHead->tMsgProperty)));
	ENDIAN_U16(pHead->u16MsgCircularId);

	u16MsgBlockLen = pHead->tMsgProperty.msg_len; ///消息体长度
	pMsgBlock = pMsg + sizeof(tMSGHEAD); ///指向消息体内容

	/**检查消息是否分包，是则需跳过后续的4B消息分包附加头，才是消息体*/
	if (pHead->tMsgProperty.msg_subpack) {
		pMsgBlock += 4;
	}

	TRACE_(QS_USER, NULL, "[GET MSG]: ");
	TRACE_RAW_BUFFER(pMsg, msgLen);	///打印原始数据

	switch (pHead->u16MsgId) {
	/**终端通用应答消息*/
//	case	MSGID_T_GeneralAck: 				//0x0001
//
//		///终端主动上发消息，不会出现在此处，忽略
//		break;
	/**平台通用应答消息*/
	case MSGID_S_GeneralAck: 				//0x8001
		Protocol_ServerGeneralAckProc((tMsg_S_GeneralAck*) pMsgBlock);
		break;

#ifdef JTT808_Ver_2013
		//补传分包请求
	case MSGID_S_ReloadSubPack: 				//0x8003

		break;
#endif

		//终端心跳
	case MSGID_T_Heatbeat: 				//0x0002

		break;

		//终端注册
//	case	MSGID_T_Register: 				//0x0100
//		break;

		//终端注册应答
	case MSGID_S_RegisterAck: 				//0x8100
	{
		tMsg_S_Ack_Register *pInfo = (tMsg_S_Ack_Register *) pMsgBlock;
		switch (pInfo->eRet) {
		case REG_SUCCESS: 				///	= 0,	///成功/确认
			/**保存授权码*/
			memcpy_(ptParam_Runtime->sAuthorCode, pMsgBlock + sizeof(tMsg_S_Ack_Register),
					strlen_((char*) (pMsgBlock + sizeof(tMsg_S_Ack_Register)))); ///STRING类型是否以'\0'结尾？

			PARAM_SaveSysParam(PARAM_RUNTIME); ///保存参数
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, SACK_TERMREG_OK_SIG), (void *)0); ///发送注册成功消息
			break;
		case REG_VECHICLEREGED: ///	= 1,	///车辆已经注册
		case REG_NOVEHICLE: ///		= 2,	///数据库中无此车辆
		case REG_TERMINALREGED: ///	= 3,	///终端已经注册
		case REG_NOTERMINAL: ///		= 4,	///数据库中无此终端
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, SACK_TERMREG_ERR_SIG), (void *)0); ///发送注册失败消息
			break;
		default:
			break;
		}
	}
		break;

//	//终端注销
//	case	MSGID_T_Unregister: 				//0x0003
//
//		break;
//
//	//终端鉴权
//	case	MSGID_T_Authentication: 				//0x0102
//
//		break;

		//设置终端参数
	case MSGID_S_SetParam: 				//0x8103
	{
		u8 i = 0;
		u8 u8ParamAmt = 0;
		tPARAMITEMHEAD *pItem;
		tMsg_SCMD_SetParam *pParam;
		TaskEvt *pe;

		TRACE_(QS_USER, NULL, "[CMD]: SetParam ");

		pParam = (tMsg_SCMD_SetParam*) pMsgBlock;

		/**循环设置参数项*/
		u8ParamAmt = ((tMsg_SCMD_SetParam*) pMsgBlock)->u8Amount;
		pMsgBlock += 1;	///跳过参数总数字节，指向参数项列表
		pItem = (tPARAMITEMHEAD*) pMsgBlock; ///指向参数项
		while (i < pParam->u8Amount)	///pParam->u8Amount: 参数总数
		{
			pMsgBlock += 5;	///跳过上面5个字节，指向参数内容
			ENDIAN_U32(pItem->u32Id);	///ID转成小端
			iRet = Set_SystemParam(pItem->u32Id, pItem->u8Len, pMsgBlock); ///写参数
			if (iRet < 0) {
				TRACE_(QS_USER, NULL, "## param setting err! ");
				Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, RET_ERROR);///上报错误消息
				PARAM_LoadSysParam(PARAM_SYSTEM); ///从片外存储器重新载入参数
				return;
			}
			pMsgBlock += pItem->u8Len;
			pItem = (tPARAMITEMHEAD*) pMsgBlock; ///指向参数项
			i++;
		}

		PARAM_SaveSysParam(PARAM_SYSTEM);	///参数写入片外存储器

		/**创建命令应答发送事件并发送*/
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

		//查询终端参数
	case MSGID_S_QueryParam: 				//0x8104
	{
		TRACE_(QS_USER, NULL, "[CMD]: QueryParam ");

		/**创建命令应答发送事件并发送*/
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
		//查询指定终端参数
	case MSGID_T_QuerySpecifyParam: 			//0x8106
	{
		/**查询指定终端参数消息体数据格式见表15，终端采用0x0104指令应答。*/
		/**0 参数总数 BYTE 参数总数为n
		 1 参数ID 列表 BYTE[4*n]参数顺序排列，如“参数ID1 参数ID2......参数IDn”。*/
		u8 u8ParamCnt = 0;
		u8 i = 0;

		TRACE_(QS_USER, NULL, "[CMD]: QuerySpecifyParam ");

		TaskEvt *pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);

		/**先清空标记数组*/
		memset_(pe->U.arr, 0, sizeof(pe->U.arr));

		u8ParamCnt = *pMsgBlock++;	///参数总数,1B
		Q_ASSERT((u8ParamCnt * 4) == (pMsg - pMsgBlock));	///参数个数和总字节数一致

		/**提取所有指定参数，转换成标记数组*/
		while (i < u8ParamCnt) {
			u32 id = *(u32*) pMsgBlock;
			ENDIAN_U32(id);
			map_param_id_2_index(pe->U.arr, sizeof(pe->U.arr), id);	///映射参数ID->标志位
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
		//查询终端参数应答
	case MSGID_T_QueryParamAck: 				//0x0104
		break;

		//终端控制
	case MSGID_S_Control: 				//0x8105
	{
		TRACE_(QS_USER, NULL, "[CMD]: Control ");

		u8 u8CmdType = *pMsgBlock++;	///命令字
		Q_ASSERT((u8CmdType <= 7) && (u8CmdType >= 1));
		if (!((u8CmdType <= 7) && (u8CmdType >= 1)))
			TRACE_(QS_USER, NULL, "ret = ERR, reason: param invalid.");

		u16MsgBlockLen--;

		switch (u8CmdType) {
		case CMD_WIRELESS_UPG:	///			= 1,	///无线升级
			/**	无线升级。参数之间采用半角分号分隔。指令如下：“URL 地址;拨号点
			 名称;拨号用户名;拨号密码;地址;TCP 端口;UDP 端口;制造商ID; 硬件
			 版本;固件版本; 连接到指定服务器时限”，若某个参数无值，则放空 */
		{
			u8 seg_buf[100];
			u8 seg_len = 0;

			//URL地址
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sURL);
			pMsgBlock++; ///跳过';'

			//拨号点名称
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sPPP_name);
			pMsgBlock++; ///跳过';'

			//拨号用户名;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sPPP_user);
			pMsgBlock++; ///跳过';'

			//拨号密码;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sPPP_password);
			pMsgBlock++; ///跳过';'

			//地址;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sIP);
			pMsgBlock++; ///跳过';'

			//TCP 端口;
			seg_len = get_next_segment(pMsgBlock, (u8*)seg_buf);
			if (seg_len > 0) {
				CONVERT_STRING_2_U16(seg_buf, seg_len, &ptParam_Runtime->u16TCP_port);
			}
			pMsgBlock++; ///跳过';'

			//UDP 端口;
			seg_len = get_next_segment(pMsgBlock, (u8*)seg_buf);
			if (seg_len > 0) {
				CONVERT_STRING_2_U16(seg_buf, seg_len, &ptParam_Runtime->u16UDP_port);
			}
			pMsgBlock++; ///跳过';'

			//制造商ID;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sManufacture_ID);
			pMsgBlock++; ///跳过';'

			//硬件版本;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sHardware_Version);
			pMsgBlock++; ///跳过';'

			//固件版本;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sFirmware_Version);
			pMsgBlock++; ///跳过';'

			//连接到指定服务器时限
			CONVERT_STRING_2_U16(pMsgBlock, (pMsg + sizeof(tMSGHEAD) + u16MsgBlockLen - pMsgBlock), ///剩余字节长度
					&ptParam_Runtime->u16TOUT_ServerConnect);
			ptParam_Runtime->u16TOUT_ServerConnect *= 60; ///分转换成秒

			TRACE_(QS_USER, NULL,
					"URL:%s; PPP_name:%s, PPP_user:%s, PPP_password:%s, IP:%s, TCP_port:%d, 	\
				UDP_port:%d,ManufuctureId:%s, Hardware_Version:%s,	Firmware_Version:%s, 	\
				Link_tout:%d",
					ptParam_Runtime->sURL, ptParam_Runtime->sPPP_name, ptParam_Runtime->sPPP_user,
					ptParam_Runtime->sPPP_password, ptParam_Runtime->sIP, ptParam_Runtime->u16TCP_port,
					ptParam_Runtime->u16UDP_port, ptParam_Runtime->sManufacture_ID, ptParam_Runtime->sHardware_Version,
					ptParam_Runtime->sFirmware_Version, ptParam_Runtime->u16TOUT_ServerConnect);

			iRet = PARAM_SaveSysParam(PARAM_RUNTIME);	//保存到运行参数
			if (iRet < 0) {
				TRACE_(QS_USER, NULL, " ERR! Save_Params failed! ");
				return;
			}
		}
			break;
		case CMD_CONNECT_ALLOCATE_SERVER: /// = 2,	///控制终端连接指定服务器
			/**控制终端连接指定服务器。参数之间采用半角分号分隔。控制指令如下：
			 “连接控制;监管平台鉴权码;拨号点名称;拨号用户名;拨号密码;地址;
			 TCP 端口;UDP 端口;连接到指定服务器时限”，若某个参数无值，则
			 放空，若连接控制值为1，则无后继参数*/
		{
			u8 seg_buf[100];
			u8 seg_len = 0;

			//连接控制;
			u8 ctrl = *pMsgBlock++;
			Q_ASSERT((ctrl == 0) || (ctrl == 1)); ///连接控制

			/**	0：切换到指定监管平台服务器，连接到该服务器后即进入应急状态，
			 此状态下仅有下发控制指令的监管平台可发送包括短信在内的控制指令
			 1：切换回原缺省监控平台服务器，并恢复正常状态
			 */
			if (ctrl == 1) {
				QACTIVE_POST(AO_Gprs, Q_NEW(QEvent, GPRS_CONNECTDEFAULTSERVER), (void *)0);
				return;
			}

			///仅当ctrl=0时，后续参数才有效
			ptParam_Runtime->u8ConnectionCtrl = ctrl;
			pMsgBlock++; ///跳过';'

			//监管平台鉴权码;
			GET_TCTRL_SEGMENT(pMsgBlock, ptParam_Runtime->sAuthorCode);
			pMsgBlock++; ///跳过';'

			//拨号点名称;
			GET_TCTRL_SEGMENT((char*) pMsgBlock, (u8*)ptParam_Runtime->sPPP_name);
			pMsgBlock++; ///跳过';'

			//拨号用户名;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sPPP_user);
			pMsgBlock++; ///跳过';'

			//拨号密码;
			GET_TCTRL_SEGMENT(pMsgBlock, ptParam_Runtime->sPPP_password);
			pMsgBlock++; ///跳过';'

			//地址;
			GET_TCTRL_SEGMENT(pMsgBlock, (u8*)ptParam_Runtime->sIP);
			pMsgBlock++; ///跳过';'

			//TCP 端口;
			seg_len = get_next_segment((char*)pMsgBlock, (char*)seg_buf);
			if (seg_len > 0) {
				CONVERT_STRING_2_U16(seg_buf, seg_len, &ptParam_Runtime->u16TCP_port);
			}
			pMsgBlock++; ///跳过';'

			//UDP 端口;
			seg_len = get_next_segment((char*)pMsgBlock, (char*)seg_buf);
			if (seg_len > 0) {
				CONVERT_STRING_2_U16(seg_buf, seg_len, &ptParam_Runtime->u16UDP_port);
			}
			pMsgBlock++; ///跳过';'

			//连接到指定服务器时限
			CONVERT_STRING_2_U16(pMsgBlock, (pMsg + sizeof(tMSGHEAD) + u16MsgBlockLen - pMsgBlock), ///剩余字节长度
					&ptParam_Runtime->u16TOUT_ServerConnect);
			ptParam_Runtime->u16TOUT_ServerConnect *= 60; ///分转换成秒

			TRACE_(QS_USER, NULL,
					"Link_ctrl:%d, AuthorCode:%s, PPP_name:%s, PPP_user:%s, PPP_password:%s, 	\
					IP:%s, TCP_port:%d,	UDP_port:%d, Link_tout:%d",
					ptParam_Runtime->u8ConnectionCtrl, ptParam_Runtime->sAuthorCode, ptParam_Runtime->sPPP_name,
					ptParam_Runtime->sPPP_user, ptParam_Runtime->sPPP_password, ptParam_Runtime->sIP,
					ptParam_Runtime->u16TCP_port, ptParam_Runtime->u16UDP_port, ptParam_Runtime->u16TOUT_ServerConnect);

			iRet = PARAM_SaveSysParam(PARAM_RUNTIME);	//保存到运行参数
			if (iRet < 0) {
				TRACE_(QS_USER, NULL, " ERR! Save_Params failed! ");
				return;
			}
		}
			break;
		case CMD_POWER_OFF: ///控制终端关机
			TRACE_(QS_USER, NULL, "3, CMD_POWER_OFF");
			///广播关机消息，须等待其他状态机合理处理后再执行
			QF_publish(Q_NEW(QEvent, SYS_POWEROFF), NULL);
			break;
		case CMD_RESET: ///控制终端复位
			TRACE_(QS_USER, NULL, "4, CMD_RESET");
			///广播复位消息，须等待其他状态机合理处理后再执行
			QF_publish(Q_NEW(QEvent, SYS_RESET), NULL);
			break;
		case CMD_RESUME_FACTORY_SET: ///终端恢复出厂设置
			TRACE_(QS_USER, NULL, "5, CMD_RESUME_FACTORY_SET");
			QF_publish(Q_NEW(QEvent, SYS_RECOVERFACTORYSETTING), NULL);
			break;
		case CMD_CLOSE_DATA_LINK: ///关闭数据通信
			TRACE_(QS_USER, NULL, "6, CMD_CLOSE_DATA_LINK");
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvent, GPRS_SHUTDATACOMM), (void *)0);
			break;
		case CMD_CLOSE_ALL_LINK: ///关闭所有无线通信
			TRACE_(QS_USER, NULL, "7, CMD_CLOSE_ALL_LINK");
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvent, GPRS_SHUTALLCOMM), (void *)0);
			break;
		default:
			break;
		}
	}
		break;

#ifdef JTT808_Ver_2013
		//查询终端属性
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

		//查询终端属性应答
//	case	MSGID_T_QueryPropertyAck:			//0x0107
//		break;

		//下发终端升级包
	case MSGID_T_DispathUpgPack:			//0x8108
	{
		/**
		 起始字节 	字段  			数据类型     	描述
		 0 		升级类型 			BYTE 		0：终端，12：道路运输证IC 卡读卡器，52：北斗卫星定位模块
		 1 		制造商			ID BYTE[5] 	制造商编号
		 6 		版本号长度 		BYTE 		n
		 7 		版本号 			STRING
		 7+n 	升级数据包长度 	DWORD 		单位为BYTE
		 11+n 	升级数据包
		 */
//		ptUpgInfo->upg_type = *pMsgBlock++;		///升级类型
//		memcpy_(ptUpgInfo->factoryId, pMsgBlock, 5); ///制造商
//		pMsgBlock += 5;
//		ptUpgInfo->version_len = *pMsgBlock++; ///版本号长度
//		memcpy_(ptUpgInfo->version, pMsgBlock, ptUpgInfo->version_len); ///版本号
//		pMsgBlock += ptUpgInfo->version_len;
//		ptUpgInfo->u32UpgPack_len = *((*u32)(pMsgBlock)); ///升级数据包长度
//		ENDIAN_U32(ptUpgInfo->u32UpgPack_len);
//
//		///升级过程未处理？？？？
//		QACTIVE_POST(AO_Upgrade, Q_NEW(QEvent, UPG_REQUEST_SIG), (void *)0);
	}
		break;

		//终端升级结果通知
//	case	MSGID_T_UpgrageResult:			//0x0108
//		break;

#endif
		//位置信息汇报
//	case	MSGID_T_RptPositionInfo:			//0x0200
//		break;

		//位置信息查询
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

		//位置信息查询应答
//	case	MSGID_T_QueryPositionAck:			//0x0201
//		break;

		//临时位置跟踪控制
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
		//人工确认报警消息
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

		QACTIVE_POST(AO_Gps, (QEvt* )pe, (void *)0); ///该消息在哪个状态机处理待定？？？？？？？？
	}
		break;

#endif
		//文本信息下发
	case MSGID_S_DownloadTxtInfo:			//0x8300
	{
		tMsg_SCMD_DispatchTxtInfo *pInfo;
		u8 *msg_buf;
		u16 msg_len;

		pInfo = (tMsg_SCMD_DispatchTxtInfo*) pMsgBlock;
		msg_buf = pInfo->msg;
		msg_len = u16MsgBlockLen - 1;

		if (pInfo->flag_Emergency) ///紧急
		{
			//????怎么处理？？？？？
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
			return;
		}

		if (pInfo->flag_Display)	///屏显示
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

		if (pInfo->flag_Advertise)	///广告，不支持
		{
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
			return;
		}

		if (pInfo->flag_TTSRead)	///朗读
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
		if (0 == pInfo->flag_MsgInfo)	{  ///5 0：中心导航信息，1：CAN 故障码信息
		}
		else {
		}

		///终端发送成功应答
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
		return;
//	}
#endif
	}
		break;

		//事件设置
		/**
		 7.4.2 事件设置及报告
		 平台通过发送事件设置消息，将事件列表发到终端存储，驾驶员在遇到相应事件后可进入事件列表界面进行选择，
		 选择后终端向平台发出事件报告消息。
		 事件设置消息，需要终端回复终端通用应答消息。
		 事件报告消息，需要平台回复平台通用应答消息。
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

		if (u8EvtType == eDEL_ALL_EVENT) ///删除终端现有所有事件，该命令不带有后续字节
				{
			TRACE_(QS_USER, NULL, "Action[eDEL_ALL_EVENT]");

			PARAM_EraseFMTParam(PARAM_EVENT);	///内存参数清空
			ERROR_HANDLE(iRet);
		}
		else if (u8EvtType == eUPDATE_EVENT)			// 更新事件	和修改事件有什么区别？？？？？？
				{
			///现在实现方式：删除现有所有事件，替换新事件
			TRACE_(QS_USER, NULL, "Action[eUPDATE_EVENT]");

			///删除原事件
			iRet = PARAM_EraseFMTParam(PARAM_EVENT);		///清空内存
			ERROR_HANDLE(iRet);

			///替换新事件
			i = 0;
			while (i < u8EvtAmount) {
				u8EvtId = *pMsgBlock;
				u8EvtLen = *(pMsgBlock + 1) + 2; ///事件项总长度
				pEvtMsg = pMsgBlock;

				///加入事件项
				iRet = PARAM_FormatMsg_ApendItem(PARAM_EVENT, u8EvtLen, pEvtMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Add ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8EvtId, u8EvtLen,
						(char*) pEvtMsg);

				pMsgBlock += u8EvtLen;
				i++;
			}
		}
		else if (u8EvtType == eAPPEND_EVENT) ///追加事件
				{
			TRACE_(QS_USER, NULL, "Action[eAPPEND_EVENT]");

			i = 0;
			while (i < u8EvtAmount) {
				u8EvtId = *pMsgBlock;
				u8EvtLen = *(pMsgBlock + 1) + 2; ///事件项总长度
				pEvtMsg = pMsgBlock;

				///加入事件项
				iRet = PARAM_FormatMsg_ApendItem(PARAM_EVENT, u8EvtLen, pEvtMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Append ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8EvtId, u8EvtLen,
						(char*) pEvtMsg);

				pMsgBlock += u8EvtLen;
				i++;
			}
		}
		else if (u8EvtType == eCHANGE_EVENT) ///修改事件
				{
			TRACE_(QS_USER, NULL, "Action[eCHANGE_EVENT]");

			i = 0;
			while (i < u8EvtAmount) {
				u8EvtId = *pMsgBlock;
				u8EvtLen = *(pMsgBlock + 1) + 2; ///事件项总长度
				pEvtMsg = pMsgBlock;

				///加入事件项
				iRet = PARAM_FormatMsg_ReplaceItem(PARAM_EVENT, u8EvtId, u8EvtLen, pEvtMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Change ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8EvtId, u8EvtLen,
						(char*) pEvtMsg);

				pMsgBlock += u8EvtLen;
				i++;
			}
		}
		else {
			///错误，回传普通错误应答消息
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
			return;
		}

		///写EEPROM索引头
		PARAM_SaveFMTParam(PARAM_EVENT);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);

		///通知界面清空事件列表
		QACTIVE_POST(AO_LCD, Q_NEW(QEvt, UI_REFRESH_EVENTS_SIG), (void *)0);
	}
		break;

		//事件报告
	case MSGID_T_RptEvent:					//0x0301
		break;

		//提问下发
	case MSGID_S_AskQuestion:				//0x8302
	{
		///提问下发标志
		u8 u8QuestionType;
		u8 u8QuestionLen;

		u8QuestionType = *pMsgBlock++;
		u8QuestionLen = *pMsgBlock++;

		if (u8QuestionType & QUESTION_TYPE_ERGENT) {
			///紧急，怎么处理？？？
		}

		if (u8QuestionType & QUESTION_TYPE_TTS_READ)	///播报
		{
			TTSMsgEvt *pe;
			pe = Q_NEW(TTSMsgEvt, TTS_READMSG_SIG);
			Q_ASSERT(pe);

			///拷贝原始消息，注意包括问题及候选应答，其中候选应答的长度字段没有转换成小端
			memcpy_((u8*) &pe->msg, pMsgBlock, u16MsgBlockLen - 2);
			QACTIVE_POST(AO_TTS, (QEvt* )pe, (void *)0);
		}

		if (u8QuestionType & QUESTION_TYPE_DISPLAY) ///广告屏显示，不支持
		{
			return;
		}
	}
		break;

		//提问应答
	case MSGID_T_QuestionAck:				//0x0302
		break;

		//信息点播菜单设置
	case MSGID_S_SetInfoOnDemandMenu:		//0x8303
	{
		u8 u8Type, u8ItemAmount;
		u8 i, u8ItemId, u8ItemLen;
		u8 *pItemMsg;

		u8Type = *pMsgBlock++;	///类型
		u8ItemAmount = *pMsgBlock++;	///总数

		TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_SetInfoOnDemandMenu], Type[%d], Amount[%s], ",
		MSGID_S_SetInfoOnDemandMenu, u8Type, u8ItemAmount);

		if (u8Type == MSGOD_MENU_DELETE) ///删除终端现有信息项
		{
			TRACE_(QS_USER, NULL, "Action[ DELETE ]");
			iRet = PARAM_EraseFMTParam(PARAM_MSGODMENU);
			ERROR_HANDLE(iRet);
		}
		else if (u8Type == MSGOD_MENU_UPDATE) ///更新菜单
		{
			TRACE_(QS_USER, NULL, "Action[ UPDATE ]");
			iRet = PARAM_EraseFMTParam(PARAM_MSGODMENU);  ///清空内存
			ERROR_HANDLE(iRet);

			i = 0;
			while (i < u8ItemAmount) {
				u8ItemId = *pMsgBlock++;	///菜单ID
				u8ItemLen = *(pMsgBlock + 1) + 2; ///菜单项长度
				pItemMsg = pMsgBlock;

				///加入事件项
				iRet = PARAM_FormatMsg_ApendItem(PARAM_MSGODMENU, u8ItemLen, pItemMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "UPDATE ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else if (u8Type == MSGOD_MENU_APPEND) ///追加菜单
		{
			TRACE_(QS_USER, NULL, "Action[ APPEND ]");

			i = 0;
			while (i < u8ItemAmount) {
				u8ItemId = *pMsgBlock++;	///菜单ID
				u8ItemLen = *(pMsgBlock + 1) + 2; ///菜单项长度
				pItemMsg = pMsgBlock;

				///加入事件项
				iRet = PARAM_FormatMsg_ApendItem(PARAM_MSGODMENU, u8ItemLen, pItemMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Append ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else if (u8Type == MSGOD_MENU_CHANGE) ///修改菜单
		{
			TRACE_(QS_USER, NULL, "Action[ CHANGE ]");

			i = 0;
			while (i < u8ItemAmount) {
				u8ItemId = *pMsgBlock++;	///菜单ID
				u8ItemLen = *(pMsgBlock + 1) + 2; ///菜单项长度
				pItemMsg = pMsgBlock;

				///加入事件项
				iRet = PARAM_FormatMsg_ReplaceItem(PARAM_MSGODMENU, u8ItemId, u8ItemLen, pItemMsg);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Change ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else {
			///错误，回传普通错误应答消息
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
			return;
		}

		///写EEPROM索引头
		PARAM_SaveFMTParam(PARAM_MSGODMENU);
		ERROR_HANDLE(iRet);

		///通知界面清空事件列表
		QACTIVE_POST(AO_LCD, Q_NEW(QEvt, UI_REFRESH_EVENTS_SIG), (void *)0);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//信息点播/取消，终端主动发起，服务器应答
	case MSGID_T_OrderInfoCtrl:			//0x0303
		break;

		//信息服务
	case MSGID_S_InfoService:				//0x8304
	{
		u16 u16ItemLen = 0;
		tMsg_SCMD_InfoService *pInfo;
		pInfo = (tMsg_SCMD_InfoService *) pMsgBlock;

		ENDIAN_U16(pInfo->u16InfoTotalLength);
		u16ItemLen = pInfo->u16InfoTotalLength + 2;

		///保存一个新信息
		PARAM_FormatMsg_ApendItem(PARAM_MSGODINFO, u16ItemLen, pMsgBlock);

		///保存索引头
		PARAM_SaveFMTParam(PARAM_MSGODINFO);

		///通知UI，更新点播信息
		QACTIVE_POST(AO_LCD, Q_NEW(QEvt, UI_REFRESH_MSGODMSG_SIG), (void *)0);
	}
		break;

		//电话回拨
	case MSGID_S_Redial:					//0x8400
	{
		tMsg_SCMD_ReverseDial *pDial;
		DialEvt *pe;

		pDial = (tMsg_SCMD_ReverseDial *) pMsgBlock;
		pMsgBlock++;

		TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_Redial], type[%d], phone[%s]", MSGID_S_Redial,
				(u8) pDial->u8Flag, (char*) pMsgBlock);

		if (pDial->u8Flag == 0) {	///普通通话
			pe = Q_NEW(DialEvt, GPRS_DIAL_NORMAL_SIG);
			Q_ASSERT(pe);
			strcpy_(pe->phone, (char*) pMsgBlock);
			QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
		}
		else if (pDial->u8Flag == 1) {	///监听
			pe = Q_NEW(DialEvt, GPRS_DIAL_MONITOR_SIG);
			Q_ASSERT(pe);
			strcpy_(pe->phone, (char*) pMsgBlock);
			QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
		}
		else {
			///回传普通错误应答消息
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
		}
	}
		break;

		/**设置电话本，注意，对电话本的操作以联系人为索引，而联系人为变长数据，因此不能以联系人为索引，
		 这里采取变通做法，以联系人CRC校验和作为索引，因不同联系人CRC一定不同，因此可以区分*/
	case MSGID_S_SetPhoneBook:	//0x8401 设置电话本
	{
		u8 u8ItemAmount;
		u32 u32Address;
		u8 i, u8ItemId, u8ItemLen;
		u8 *pItemMsg;

		T_SCMD_SetPhoneBook *pBook;
		pBook = (T_SCMD_SetPhoneBook *) pMsgBlock;

		if (pBook->u8ConnPersonCount > PHONE_ITEM_SIZE)	///条目总数越界检查
			return;

		u8ItemAmount = pBook->u8ConnPersonCount;

		TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_SetPhoneBook], Type[%d], Amount[%s], ", MSGID_S_SetPhoneBook,
				pBook->u8SetType, u8ItemAmount);

		pMsgBlock += 2; ///指向第一项

		if (pBook->u8SetType == ePHONEBOOK_DELETE) ///0,删除终端上所有存储的联系人
				{
			TRACE_(QS_USER, NULL, "Action[ DELETE ]");
			iRet = PARAM_EraseFMTParam(PARAM_PHONEBOOK); ///清空内存
			ERROR_HANDLE(iRet);
		}
		else if (pBook->u8SetType == ePHONEBOOK_UPDATE) ///1,更新电话本
				{
			///删除终端中已有的全部联系人并追加消息中的联系人
			TRACE_(QS_USER, NULL, "Action[ UPDATE ]");
			iRet = PARAM_EraseFMTParam(PARAM_PHONEBOOK); ///清空内存
			ERROR_HANDLE(iRet);

			i = 0;
			while (i < u8ItemAmount) {
				///取消以电话本的联系人CRC作为ID，还是以联系人名称作为索引
				u8 u8NameLen = *(pMsgBlock + *(pMsgBlock + 1) + 2);	///号码长度
				u8 *pName = pMsgBlock + *(pMsgBlock + 1) + 2;
				u8ItemId = CalculateCRC8(pName, u8NameLen);
				u8ItemLen = *(pMsgBlock + 1) + 2 + u8NameLen;

				///加入项
				iRet = PARAM_FormatMsg_ApendItem(PARAM_PHONEBOOK, u8ItemLen, pMsgBlock);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "UPDATE ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else if (pBook->u8SetType == ePHONEBOOK_APPEND)	///2,追加电话本
				{
			i = 0;
			while (i < u8ItemAmount) {
				///电话本的联系人CRC作为ID
				u8 u8NameLen = *(pMsgBlock + *(pMsgBlock + 1) + 2);	///号码长度
				u8 *pName = pMsgBlock + *(pMsgBlock + 1) + 2;
				u8ItemId = CalculateCRC8(pName, u8NameLen);
				u8ItemLen = *(pMsgBlock + 1) + 2 + u8NameLen;

				///加入项
				iRet = PARAM_FormatMsg_ApendItem(PARAM_PHONEBOOK, u8ItemLen, pMsgBlock);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Append ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else if (pBook->u8SetType == ePHONEBOOK_CHANGE)	///3,修改电话本(以联系人为索引)
				{
			i = 0;
			while (i < u8ItemAmount) {
				///电话本的联系人CRC作为ID
				u8 u8NameLen = *(pMsgBlock + *(pMsgBlock + 1) + 2);	///号码长度
				u8 *pName = pMsgBlock + *(pMsgBlock + 1) + 2;
				u8ItemId = CalculateCRC8(pName, u8NameLen);
				u8ItemLen = *(pMsgBlock + 1) + 2 + u8NameLen;

				///加入项
				iRet = PARAM_FormatMsg_ReplaceItem(PARAM_PHONEBOOK, u8ItemId, u8ItemLen, pMsgBlock);
				ERROR_HANDLE(iRet);

				TRACE_(QS_USER, NULL, "Replace ITEM[%d] info: id[%x], Len[%d], Msg[%s]", i, u8ItemId, u8ItemLen,
						(char*) pItemMsg);

				pMsgBlock += u8ItemLen;
				i++;
			}
		}
		else {
			///错误，回传普通错误应答消息
			Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, -1);
			return;
		}

		///写EEPROM索引头
		PARAM_SaveFMTParam(PARAM_PHONEBOOK);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);

		///通知UI，更新电话本，如需要
		QACTIVE_POST(AO_LCD, Q_NEW(QEvt, UI_REFRESH_PHONEBOOK_SIG), (void *)0);
	}
		break;

		//车辆控制
	case MSGID_S_VechicleCtrl:				//0x8500
	{
		TaskEvt *pe;
		u8 u8Ctrl;

		///0：车门解锁；1：车门加锁
		u8Ctrl = *pMsgBlock;

		if (u8Ctrl & 0x01) {
			BSP_Vechical_Lock();	///锁车
			QACTIVE_POST(AO_LCD, Q_NEW(QEvt, VECHICAL_CTRL_LOCK_SIG), (void *)0);
			TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_VechicleCtrl], ctrl[%s]", (u8) MSGID_S_VechicleCtrl,
					"LOCK");
		}
		else {
			BSP_Vechical_UnLock();	///解锁
			QACTIVE_POST(AO_LCD, Q_NEW(QEvt, VECHICAL_CTRL_UNLOCK_SIG), (void *)0);
			TRACE_(QS_USER, NULL, "@CMD id[ %x - MSGID_S_VechicleCtrl], ctrl[%s]", (u8) MSGID_S_VechicleCtrl,
					"UNLOCK");
		}

		///发送控制命令应答
		pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);
		pe->cmd = MSGID_T_VechicleCtrlAck;
		pe->resCmd = MSGID_S_VechicleCtrl;
		pe->sequence = GenerateCirculateSeqId();
		pe->resSeqence = pHead->u16MsgCircularId;

		QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
	}
		break;

		//车辆控制应答
	case MSGID_T_VechicleCtrlAck:			//0x0500
		break;

		//设置圆形区域
	case MSGID_S_SetRoundErea:			//0x8600
	{
		u8 u8Type = 0;
		u8 u8Amount = 0;
		u8 i;
		tItem_RoundArea *pRound;

		u8Type = *pMsgBlock++;
		u8Amount = *pMsgBlock++;

		if (u8Type == eAREA_UPDATE) {	///0,更新区域
			PARAM_Area_EraseAll(PARAM_AREA_ROUND); ///先清除原来区域信息

			i = 0;
			while (i < u8Amount) {
				pRound = (tItem_RoundArea *) pMsgBlock;

				///字段转换成小端
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
		if (u8Type == eAREA_APPEND) { 	///1,追加区域
			i = 0;
			while (i < u8Amount) {
				pRound = (tItem_RoundArea *) pMsgBlock;

				///字段转换成小端
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
		if (u8Type == eAREA_CHANGE) {	///2,修改区域
			i = 0;
			while (i < u8Amount) {
				pRound = (tItem_RoundArea *) pMsgBlock;

				///字段转换成小端
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

		///写EEPROM
//		PARAM_Area_Save(PARAM_AREA_ROUND);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//删除圆形区域
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

		///写EEPROM
//		PARAM_Area_Save(PARAM_AREA_ROUND);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//设置矩形区域
	case MSGID_S_SetRectangleErea:			//0x8602
	{
		u8 u8Type = 0;
		u8 u8Amount = 0;
		u8 i;
		tItem_RectangleArea *pRect;

		u8Type = *pMsgBlock++;
		u8Amount = *pMsgBlock++;

		if (u8Type == eAREA_UPDATE) ///0,更新区域
				{
			PARAM_Area_EraseAll(PARAM_AREA_RECTANGLE); ///先清除原来区域信息

			i = 0;
			while (i < u8Amount) {
				pRect = (tItem_RectangleArea *) pMsgBlock;

				///字段转换成小端
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
		if (u8Type == eAREA_APPEND) ///1,追加区域
				{
			i = 0;
			while (i < u8Amount) {
				pRect = (tItem_RectangleArea *) pMsgBlock;

				///字段转换成小端
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
		if (u8Type == eAREA_CHANGE) ///2,修改区域
				{
			i = 0;
			while (i < u8Amount) {
				pRect = (tItem_RectangleArea *) pMsgBlock;

				///字段转换成小端
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

		///写EEPROM
//		PARAM_Area_Save(PARAM_AREA_RECTANGLE);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//删除矩形区域
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

		///写EEPROM
//		PARAM_Area_Save(PARAM_AREA_RECTANGLE);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//设置多边形区域
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

		///顶点转成小端
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

		///写EEPROM
//		PARAM_Area_Save(PARAM_AREA_POLYGON);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//删除多边形区域
	case MSGID_S_DelPolygonErea:			//0x8605
	{
		u8 u8Amount = 0;
		u8 i;
		u32 u32Id;

		u8Amount = *pMsgBlock++;	///区域数

		i = 0;
		while (i < u8Amount) {
			u32Id = *((u32*) pMsgBlock);
			iRet = PARAM_Area_DeleteItem(PARAM_AREA_POLYGON, u32Id);
			ERROR_HANDLE(iRet);
			pMsgBlock += 4;
			i++;
		}

		///写EEPROM
//		PARAM_Area_Save(PARAM_AREA_POLYGON);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//设置路线
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

		///顶点转成小端
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

		///写EEPROM
//		PARAM_Area_Save(PARAM_AREA_ROUTE);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);
	}
		break;

		//删除路线
	case MSGID_S_DelPathline:			//0x8607
	{
		u8 u8Amount = 0;
		u8 i;
		u32 u32Id;

		u8Amount = *pMsgBlock++;	///区域数

		i = 0;
		while (i < u8Amount) {
			u32Id = *((u32*) pMsgBlock);
			iRet = PARAM_Area_DeleteItem(PARAM_AREA_ROUTE, u32Id);
			ERROR_HANDLE(iRet);
			pMsgBlock += 4;
			i++;
		}

		///写EEPROM
//		PARAM_Area_Save(PARAM_AREA_ROUTE);

		///回传普通成功应答消息
		Protocol_T_SendGeneralAck(pHead->u16MsgId, pHead->u16MsgCircularId, 0);

	}
		break;

		//行驶记录仪数据采集命令
	case MSGID_S_RecorderCapture:			//0x8700
	{
		RecorderStartEvt *pe;

		pe = Q_NEW(RecorderStartEvt, RECORDER_START_SIG);
		Q_ASSERT(pe);

		pe->u8Cmd = *pMsgBlock++;			///命令
		pe->u16CmdSeq = pHead->u16MsgCircularId; ///流水号
		memcpy_(pe->param, pMsgBlock, u16MsgBlockLen);	///参数

		QACTIVE_POST(AO_Recorder, (QEvt * )pe, (void *)0);
	}
		break;

		//行驶记录仪数据上传
	case MSGID_T_RecorderDataUpload:			//0x0700
		break;

		//行驶记录参数下传命令
	case MSGID_S_RecorderParamDownload:			//0x8701
	{
		///待完成。
	}
		break;

		///8.47 上报驾驶员身份信息请求, 0x8702
	case MSGID_S_RequestDriverInfo: {
		TaskEvt *pe;

		/**创建命令应答发送事件并发送*/
		pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
		Q_ASSERT(pe);
		pe->cmd = MSGID_T_DriverInfoRpt;
		pe->resCmd = MSGID_S_RequestDriverInfo;
		pe->sequence = GenerateCirculateSeqId();
		pe->resSeqence = pHead->u16MsgCircularId;

		QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
	}
		break;

		//电子运单上报
	case MSGID_T_EBillRpt:			//0x0701
		break;

		//驾驶员身份信息采集上报
	case MSGID_T_DriverInfoRpt:			//0x0702
		break;

#ifdef JTT808_Ver_2013
		//定位数据批量上传
	case MSGID_T_PositionInfoBatchRpt:			//0x0704
		break;

		//CAN总线数据上传
	case MSGID_T_CANDataUpload:			//0x0705
		break;

#endif
		//多媒体事件信息上传
	case MSGID_T_MediaEventInfoRpt:			//0x0800
		break;

		//多媒体数据上传
	case MSGID_T_MediaDataRpt:			//0x0801
		break;

		//多媒体数据上传应答
	case MSGID_S_MediaDataRptAck:			//0x8800
	{
		tMsg_SACK_Media_AckData *pData;
		MultimediaDataAckEvt *pe;

		pData = (tMsg_SACK_Media_AckData*) pMsgBlock;

		pMsgBlock += 3;

		pe = Q_NEW(MultimediaDataAckEvt, MULTIMEDIA_UPLOADPACK_ACK_SIG);
		Q_ASSERT(pe);

		pe->u32Id = pData->u32MediaDataId;			///命令
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

		///向那个状态机发？？？
		QACTIVE_POST(AO_Video, (QEvt* )pe, (void *)0);
	}
		break;

		//摄像头立即拍照命令
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
		//摄像头立即拍照命令应答
	case MSGID_S_TakePhotoAck:			//0x0805
		break;

#endif
		//存储多媒体数据检索命令
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

		//存储多媒体数据检索应答命令
	case MSGID_T_StoredMediaQueryAck:			//0x0802
		break;

		//存储多媒体数据上传命令
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

		//录音开始命令
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

		///	8.60 单条存储多媒体数据检索上传命令	消息ID：0x8805
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

		//数据下行透传命令
	case MSGID_S_TransparentTransfer:			//0x8900
	{
		TransparentUploadEvt *pe;

		pe = Q_NEW(TransparentUploadEvt, TRANSPARENT_UPLOAD_SIG);

		pe->u8Type = *pMsgBlock++;
		memcpy_(pe->data, pMsgBlock, u16MsgBlockLen - 1);

		QACTIVE_POST(AO_Audio, (QEvt* )pe, (void *)0);
	}
		break;

		//数据上行透传命令
	case MSGID_T_TransparentTransfer:			//0x0900
		break;

		//数据压缩上报
	case MSGID_T_DataCompressRpt:			//0x0901
		break;

		//平台RAS公钥
	case MSGID_S_PlatformRASKey:			//0x8A00
		break;

		//终端RAS公钥
	case MSGID_T_RASKey:			//0x0A00
		break;

	default:

		break;
	} ///switch
}

/**
 * 消息转义，编码
 *
 * 采用0x7e 表示，若校验码、消息头以及消息体中出现0x7e，则要进行转义处理，转义规则定义如下：
 * 0x7e <————> 0x7d 后紧跟一个0x02；
 * 0x7d <————> 0x7d 后紧跟一个0x01。
 * 转义处理过程如下：
 * 发送消息时：消息封装——>计算并填充校验码——>转义；
 * 接收消息时：转义还原——>验证校验码——>解析消息。
 * 示例：发送一包内容为0x30 0x7e 0x08 0x7d 0x55 的数据包，则经过封装如下：
 * 	0x7e 0x30 7d 0x02 0x08 0x7d	0x01 0x55 0x7e。
 *
 * @param pu8Data		[in]	待发送到数据首地址，
 * @param u16Length		[in]	待发送的数据长度
 * @param pu8DestData	[in]	转义数据首地址
 * @return	u16，转义数据长度
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
 * 接收到的消息转义
 *
 * @param pu8Data		[in]	接收到数据首地址，
 * @param u16Length		[in]	接收的数据长度
 * @param pu8DestData	[in]	转义完成存储的数据首地址
 * @return	u16，转义完成后数据长度
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
 * 计算异或校验和
 *
 * @param pData		[IN]	数据
 * @param u16Length	[IN]	数据长度
 * @return	u8,	校验码
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
 * 组织待发送数据包，在实际发送前调用
 *
 * @param pTaskEvt	 任务事件
 * @param pBuf		待发送缓区
 * @param bufSize	待发送缓区长度
 * @return	实际待发送字节数
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

	/**填充消息头字段*/
	pHead->u8Flag = MSG_FLAG_BYTE;	///标识位
	pHead->u16MsgId = pTaskEvt->cmd;	///消息ID
//	pHead->u16MsgProperty = 0; 	///消息属性，留待命令中填充
	memcpy_(pHead->u8BcdPhoneNum, ptParam_Runtime->aPhoneNum, 6);	///终端手机号
	pHead->u16MsgCircularId = pTaskEvt->sequence;	///消息流水号在事件创建时已生成

	pCursor = pBuf + sizeof(tMSGHEAD);
	pMsg = pCursor; ///保存消息体起始位置，方便计算消息长度

	TRACE_(QS_USER, NULL, "\n[SEND MSG]");

	switch (pTaskEvt->cmd) {
	//终端通用应答消息
	case MSGID_T_GeneralAck: 			///0x0001
	{
		tMsg_T_GeneralAck *pMsgBlock;

		/**消息体*/
		pMsgBlock = (tMsg_T_GeneralAck*) pCursor;
		pMsgBlock->u16CmdId = pTaskEvt->resCmd;	///对应的平台消息的ID
		pMsgBlock->u16ResponsePackId = pTaskEvt->resSeqence;	///对应的平台消息的流水号
		pMsgBlock->eResult = pTaskEvt->ret; ///0：成功/确认；1：失败；2：消息有误；3：不支持
		/**校验码*/
		pCursor += sizeof(tMsg_T_GeneralAck);

		TRACE_(QS_USER, NULL, "[T-SEND] type:GeneralAck, \n id:%x, \n u16CmdId=%x, \n u16ResponsePackId=%x, \n ret=%x",
		MSGID_T_GeneralAck, pMsgBlock->u16CmdId, pMsgBlock->u16ResponsePackId, pMsgBlock->eResult);
	}
		break;

//	//平台通用应答消息
//	case	MSGID_S_GeneralAck: 				//0x8001
//
//		break;

#ifdef JTT808_Ver_2013
		//补传分包请求
//	case MSGID_S_ReloadSubPack: 				//0x8003
//		break;
#endif

		//终端心跳
	case MSGID_T_Heatbeat: 				//0x0002
		/**终端心跳数据消息体为空。*/
		break;

		//终端注册
	case MSGID_T_Register: 				//0x0100
	{
		tMsg_T_Register *pRegInfo = (tMsg_T_Register*)pCursor;
		pRegInfo->u16ProviceId = ptParam_Device->u16ProviceId;		///省域ID
		pRegInfo->u16CountyId = ptParam_Device->u16CountyId = 0x0;		///市县域ID
		memcpy_(pRegInfo->au8FactoryId, ptParam_Device->au8FactoryId, 5);	///制造商ID
	#ifdef JTT808_Ver_2011
		memcpy_(pRegInfo->aType, ptParam_Device->aType, 8);	///终端型号，8个字节，此终端型号由制造商自行定义，位数不	足时，补空格。
		memcpy_(pRegInfo->aTId, ptParam_Device->aTId, 7);///终端ID,7 个字节，由大写字母和数字组成，此终端ID 由制造商自行定义，位数不足时，后补“0X00”。
		pRegInfo->u8PlateColor = ptParam_Device->u8PlateColor;///车牌颜色，按照JT/T415-2006 的5.4.12
	#endif
	#ifdef JTT808_Ver_2013
		memcpy_(pRegInfo->aType, ptParam_Device->aType, 20);	///终端型号20 个字节，此终端型号由制造商自行定义，位数不足时，后补“0X00”。
		memcpy_(pRegInfo->aTId, ptParam_Device->aTId, 7);		///终端ID
		pRegInfo->u8PlateColor = ptParam_Device->u8PlateColor;	///车牌颜色，按照JT/T415-2006 的5.4.12。未上牌时，取值为0。
	#endif///JTT808_Ver_2013

		memcpy_(pRegInfo->aPlate, ptParam_Device->aPlate, strlen_(ptParam_Device->aPlate)); /**V2011： 机动车号牌;
											 V2013：车牌车牌颜色为0 时，表示车辆VIN；否则，表示公安 交通管理部门颁发的机动车号牌。*/

		pCursor += sizeof(tMsg_T_Register) + strlen_((char*) ptParam_Device->aPlate);
	}
		break;

//	//终端注册应答
//	case	MSGID_S_RegisterAck: 				//0x8100
//
//		break;

		//终端注销
	case MSGID_T_Unregister: 				//0x0003
		/*终端注销消息体为空*/
		break;

		//终端鉴权
	case MSGID_T_Authentication: 				//0x0102
		/**终端重连后上报鉴权码*/
		strcpy_((char*) pCursor, (char*) ptParam_Runtime->sAuthorCode);
		pCursor += strlen_((char*) ptParam_Runtime->sAuthorCode);
		break;

		//设置终端参数
//	case	MSGID_S_SetParam: 				//0x8103
//		break;

		//查询终端参数
//	case	MSGID_S_QueryParam: 				//0x8104
//		break;

#ifdef JTT808_Ver_2013
		//查询指定终端参数
//	case	MSGID_T_QuerySpecifyParam: 			//0x8106
//		break;

#endif
		//查询终端参数应答
	case MSGID_T_QueryParamAck: 				//0x0104
	{
		u16 u16Len = 0;

		/**组织应答参数*/
		u16Len = Protocol_FormSysParams(pCursor, pTaskEvt->resSeqence);
		pCursor += u16Len;
	}
		break;

		//终端控制
//	case	MSGID_S_Control	: 				//0x8105
//		break;

#ifdef JTT808_Ver_2013
		//查询终端属性
//	case	MSGID_S_QueryProperty:			//0x8107
//	break;

		//查询终端属性应答
	case MSGID_T_QueryPropertyAck:			//0x0107
	{
		tMsg_TACK_QueryTerminalProperty *pProperty;

		pProperty = (tMsg_TACK_QueryTerminalProperty*)pCursor;

		pProperty->tType = ptParam_Device->tType;	///终端类型 WORD
		ENDIAN_U16(pProperty->tType);
		memcpy_(pProperty->factoryId, ptParam_Device->au8FactoryId, 5);	///制造商ID BYTE[5] 5 个字节，终端制造商编码。
		memcpy_(pProperty->class, ptParam_Device->aType, 20);	///终端型号 此终端型号由制造商自行定义，位数不足时，后补“0X00”。
		memcpy_(pProperty->tid, ptParam_Device->aTId, 7);	///终端ID 由大写字母和数字组成，此终端ID 由制造商自行定义，位数不足时，后补“0X00”。
		memcpy_(pProperty->ICCID, ptParam_Runtime->ICCID, 10);	///终端SIM 卡ICCID BCD[10] 终端SIM 卡ICCID 号
		pProperty->ver_hardware_len = strlen_((char*)ptParam_Device->ver_hardware);	///终端硬件版本号长度
		pCursor += 42;
		memcpy_(pCursor, ptParam_Device->ver_hardware, pProperty->ver_hardware_len);///终端硬件版本号 STRING
		pCursor += pProperty->ver_hardware_len;
		*pCursor++ = strlen_((char*)ptParam_Device->ver_software);	///终端软件版本号长度
		memcpy_(pCursor, ptParam_Device->ver_software, pProperty->ver_software_len);///终端软件版本号 STRING
		pCursor++;
		*pCursor++ = ptParam_Device->tGNSSModuleProperty; ///GNSS 模块属性 BYTE
		*pCursor = ptParam_Device->tCommModuleProperty; ///通信模块属性 BYTE

		pCursor += 42 + pProperty->ver_hardware_len + 1 + pProperty->ver_software_len + 2;
	}
		break;

		//下发终端升级包
//	case	MSGID_T_DispathUpgPack:			//0x8108
//	break;

		//终端升级结果通知
	case MSGID_T_UpgrageResult:			//0x0108
	{
		tMsg_TACK_UpgResult *pUpgRet = (tMsg_TACK_UpgResult *)pCursor;
		pUpgRet->upg_result = pTaskEvt->ret;	///升级结果
		pUpgRet->upg_type = pTaskEvt->U.arr[0];	///升级类型

		pCursor += sizeof(tMsg_TACK_UpgResult);
	}
		break;
#endif

		//位置信息汇报
	case MSGID_T_RptPositionInfo:			//0x0200
	{
		u16 u16Len = 0;

		u16Len = Protocol_GetPositionInfo(pCursor);	///取位置信息
		pCursor += u16Len;
	}
		break;

		//位置信息查询
//	case	MSGID_S_QueryPosition:			//0x8201
//		break;

		//位置信息查询应答
	case MSGID_T_QueryPositionAck:			//0x0201
	{
		tMsg_TACK_QueryPositionInfoAck *pAck;
		u16 u16Len = 0;

		pAck = (tMsg_TACK_QueryPositionInfoAck *)pCursor;

		pAck->u16SeqNum = pTaskEvt->resSeqence;
		ENDIAN_U16(pAck->u16SeqNum);

		u16Len = Protocol_GetPositionInfo((u8*)&pAck->tInfo);	///取位置信息

		pCursor += (u16Len + 1);
	}
		break;

		//临时位置跟踪控制
//	case	MSGID_S_Tracking:					//0x8202
//	break;

#ifdef JTT808_Ver_2013
		//人工确认报警消息
//	case	MSGID_S_ConfirmWarningInfo:		//0x8203
//	break;

#endif
		//文本信息下发
//	case	MSGID_S_DownloadTxtInfo:			//0x8300
//	break;

		//事件设置
//	case	MSGID_S_SetEvent:					//0x8301
//	break;

		//事件报告
	case MSGID_T_RptEvent:					//0x0301

		*pCursor++ = pTaskEvt->ret;	///事件ID复用了ret字段
		break;

		//提问下发
//	case	MSGID_S_AskQuestion:				//0x8302
//	break;

		//提问应答
	case MSGID_T_QuestionAck:				//0x0302
	{
		tMsg_TACK_AnswerQuestion *pAnswer;
		pAnswer = (tMsg_TACK_AnswerQuestion *)pCursor;

		pAnswer->u8AnswerId = pTaskEvt->ret;	///问题答案ID复用了ret字段
		pAnswer->u16SeqNum = pTaskEvt->resSeqence;
		ENDIAN_U16(pAnswer->u16SeqNum);

		pCursor += sizeof(tMsg_TACK_AnswerQuestion);
	}
		break;

		//信息点播菜单设置
//	case	MSGID_S_SetInfoOnDemandMenu:		//0x8303
//	break;

		//信息点播/取消
	case MSGID_T_OrderInfoCtrl:			//0x0303
	{
		tMsg_T_InfoOrderCtrl *pCtrl;
		pCtrl = (tMsg_T_InfoOrderCtrl *)pCursor;

		pCtrl->u8InfoOrderSetType = pTaskEvt->U.arr[0];
		pCtrl->u8OrderOrCancel = pTaskEvt->U.arr[1];

		pCursor += sizeof(tMsg_T_InfoOrderCtrl);
	}
		break;

		//信息服务
//	case	MSGID_S_InfoService:				//0x8304
//	break;

		//电话回拨
//	case	MSGID_S_Redial:					//0x8400
//	break;

		//设置电话本
//	case	MSGID_S_SetPhoneBook:				//0x8401
//	break;

		//车辆控制
//	case	MSGID_S_VechicleCtrl:				//0x8500
//	break;

		//车辆控制应答
	case MSGID_T_VechicleCtrlAck:			//0x0500
	{
		tMsg_TACK_VechicalCtrl *pAck;
		u16 u16Len = 0;

		pAck = (tMsg_TACK_VechicalCtrl *)pCursor;

		pAck->u16SeqNum = pTaskEvt->resSeqence;
		ENDIAN_U16(pAck->u16SeqNum);

		u16Len = Protocol_GetPositionInfo((u8*)&pAck->tBasicPos);	///取位置信息

		pCursor += (u16Len + 2);
	}
		break;

		//设置圆形区域
//	case	MSGID_S_SetRoundErea:			//0x8600
//	break;

		//删除圆形区域
//	case	MSGID_S_DelRoundErea:			//0x8601
//	break;

		//设置矩形区域
//	case MSGID_S_SetRectangleErea:			//0x8602
//
//		break;

		//删除矩形区域
//	case MSGID_S_DelRectangleErea:			//0x8603
//
//		break;

		//设置多边形区域
//	case	MSGID_S_SetPolygonErea:			//0x8604
//	break;

		//删除多边形区域
//	case	MSGID_S_DelPolygonErea:			//0x8605
//	break;

		//设置路线
//	case	MSGID_S_SetPathline:			//0x8606
//	break;

		//删除路线
//	case	MSGID_S_DelPathline:			//0x8607
//	break;

		//行驶记录仪数据采集命令
//	case	MSGID_S_RecorderCapture:			//0x8700
//	break;

		//行驶记录仪数据上传
	case MSGID_T_RecorderDataUpload:			//0x0700

		break;

		//行驶记录参数下传命令
//	case	MSGID_S_RecorderParamDownload:			//0x8701
//	break;

		//电子运单上报
	case MSGID_T_EBillRpt:			//0x0701
	{
		tETICKET *pTicket = (tETICKET *)pCursor;

		pTicket->u32ETicketLen = ptParam_Runtime->tEticket.u32ETicketLen;
		ENDIAN_U32(pTicket->u32ETicketLen);

		memcpy_(pTicket->aData, ptParam_Runtime->tEticket.aData, ptParam_Runtime->tEticket.u32ETicketLen);

		pCursor += (4 + ptParam_Runtime->tEticket.u32ETicketLen);
	}
		break;

		//驾驶员身份信息采集上报
	case MSGID_T_DriverInfoRpt:			//0x0702
	{
#ifdef JTT808_Ver_2011
	*pCursor++ = ptParam_Runtime.tDriverInfo.u8DriverNameLen;///驾驶员姓名长度
	memcpy_(pCursor, ptParam_Runtime.tDriverInfo.aDriverName,
			ptParam_Runtime.tDriverInfo.u8DriverNameLen);///驾驶员姓名，长度n
	pCursor += ptParam_Runtime.tDriverInfo.u8DriverNameLen;
	memcpy_(pCursor, ptParam_Runtime.tDriverInfo.aDrvIdCode, 20);///驾驶员身份证编码
	pCursor += 20;
	memcpy_(pCursor, ptParam_Runtime.tDriverInfo.aQulCode, 40);///从业资格证编码
	pCursor += 40;
	*pCursor++ = ptParam_Runtime.tDriverInfo.u8GovNameLength;///发证机构名称长度
	memcpy_(pCursor, ptParam_Runtime.tDriverInfo.aGovName,
			ptParam_Runtime.tDriverInfo.u8GovNameLength);///发证机构名称
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
	memcpy_(pCursor, ptParam_Runtime->tDriverInfo.aQulCode, 20);///从业资格证编码
	pCursor += 20;
	*pCursor++ = ptParam_Runtime->tDriverInfo.u8GovNameLength;///发证机构名称长度
	memcpy_(pCursor, ptParam_Runtime->tDriverInfo.aGovName,
			ptParam_Runtime->tDriverInfo.u8GovNameLength);///发证机构名称
	pCursor += ptParam_Runtime->tDriverInfo.u8GovNameLength;
	memcpy_(pCursor, ptParam_Runtime->tDriverInfo.LicieceValidation, 4);
	pCursor += 4;
#endif
	}
		break;

#ifdef JTT808_Ver_2013
		//定位数据批量上传
	case MSGID_T_PositionInfoBatchRpt:			//0x0704
	{
//		tMsg_T_PositionInfoBatchRpt *pMsg = (tMsg_T_PositionInfoBatchRpt *)pCursor;
	}
		break;

		//CAN总线数据上传
	case MSGID_T_CANDataUpload:			//0x0705

		break;

#endif
		//多媒体事件信息上传
	case MSGID_T_MediaEventInfoRpt:			//0x0800

		break;

		//多媒体数据上传
	case MSGID_T_MediaDataRpt:			//0x0801

		break;

		//多媒体数据上传应答
//	case	MSGID_S_MediaDataRptAck:			//0x8800
//	break;

		//摄像头立即拍照命令
//	case	MSGID_S_TakePhoto:			//0x8801
//	break;

#ifdef JTT808_Ver_2013
		//摄像头立即拍照命令应答
	case MSGID_S_TakePhotoAck:			//0x0805

		break;

#endif
		//存储多媒体数据检索命令
//	case	MSGID_S_StoredMediaQuery:			//0x8802
//	break;

		//存储多媒体数据检索应答命令
	case MSGID_T_StoredMediaQueryAck:			//0x0802

		break;

		//存储多媒体数据上传命令
//	case	MSGID_S_StoredMediaRpt:			//0x8803
//	break;

		//录音开始命令
//	case	MSGID_S_StartAudioRecord:			//0x8804
//	break;

		//数据下行透传命令
//	case	MSGID_S_TransparentTransfer:			//0x8900
//	break;

		//数据上行透传命令
	case MSGID_T_TransparentTransfer:			//0x0900

		break;

		//数据压缩上报
	case MSGID_T_DataCompressRpt:			//0x0901

		break;

		//平台RAS公钥
//	case	MSGID_S_PlatformRASKey:			//0x8A00
//	break;

		//终端RAS公钥
	case MSGID_T_RASKey:			//0x0A00

		break;

	default:

		break;
	}

	/**回填消息头属性：未作分包处理，到底是不是要分包，待考虑......*/
	pHead->tMsgProperty.msg_encrypt = 0;
	pHead->tMsgProperty.msg_subpack = 0;
	pHead->tMsgProperty.msg_len = (u16)(pCursor - pMsg) + 1;

	*pCursor = CalculateXorChecksum(pBuf, (u16)(pCursor - pBuf)); ///校验码
	pCursor++;  ///结束标志
	*pCursor = MSG_FLAG_BYTE;

	ENDIAN_MSGHEAD_LITTLE2BIG(pHead);	///消息头小端转换成大端

	/**最后是转义处理，但注意转义过程放在发送前转义，而不是在此处，否则的话需再增加一个临时缓区*/
	///EncodeData();
	return (pCursor - pBuf + 1); 	///返回实际帧长度
}

/**
 * 服务器通用应答消息处理
 *
 * @param pSAck		应答消息
 */
void Protocol_ServerGeneralAckProc(tMsg_S_GeneralAck *pSAck) {
//	SAckEvt *pe;
//	Q_ASSERT(pSAck);
//
//	/**申请服务器应答事件并发送*/
//	pe = Q_NEW(SAckEvt, GPRS_SACK_SIG);
//	Q_ASSERT(pe);
//	pe->u16RespMsgId = pSAck->u16RespMsgId;
//	pe->u16RespSeqId = pSAck->u16RespSeqId;
//	pe->ret = (u8) pSAck->eResult;
//
//	QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
}

/**
 * 终端发送通用命令应答消息
 *
 * @param u16ResMsgId		应答消息ID
 * @param u16ResSeqId		应答流水号
 * @param ret				命令处理结果
 */
void Protocol_T_SendGeneralAck(u16 u16ResMsgId, u16 u16ResSeqId, u8 ret) {
	TaskEvt *pe;

	/**创建命令应答发送事件并发送*/
	pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
	Q_ASSERT(pe);
	pe->cmd = MSGID_T_GeneralAck;	///0x0001
	pe->resCmd = u16ResMsgId;		///应答消息ID
	pe->sequence = GenerateCirculateSeqId();	///流水号
	pe->resSeqence = u16ResSeqId;	///应答消息ID
	pe->ret = ret;		///结果

	QACTIVE_POST(AO_Gprs, (QEvt* )pe, (void *)0);
}

/**
 * 循环产生消息流水号
 *
 * @return	流水号
 */
u16 GenerateCirculateSeqId() {
	static u16 seq = 0;
	return seq++;
}

/**
 * 消息内容校验
 *
 * 每条消息由标识位、消息头、消息体和校验码组成
 *
 * @param pMsg		[IN]	消息内容
 * @param u16Len	[IN]	消息长度
 * @return	TRUE=正确，FALSE=错误
 */
BOOL CheckFrame(u8 *pMsg, u16 u16Len) {
	if (*(pMsg + u16Len - 2) == CalculateXorChecksum(pMsg, u16Len - 2))
		return TRUE;

	return FALSE;
}

/**
 * 组织应答参数
 *
 * @param pCursor	缓区地址
 * @param seq		应答消息流水号
 * @return	参数长度
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

	/**	循环放入各个参数项*/
	u16Len += GetItem_AllSystemParam(pCursor);

	return u16Len;
}

/**
 * 映射参数ID->标志位，仅在“查询指定参数”命令用，将指定参数转换成标志数组，节省空间
 * 映射关系：                        7      6      5      4      3      2      1     0
 * 			 ———————————————————————————————————————————————————————
 *           |  7   |  6   |  5   |  4   |  3   |   2  |  1  |  0  |		pArr[0]
 * 			 ———————————————————————————————————————————————————————
 *           |  15  |  14  |  13  |  12  |  11  |  10  |  9  |  8  |		pArr[1]
 * 			 ———————————————————————————————————————————————————————
 *			 |   ...                                               |
 * 			 ————————————————————————————————————————————————————————
 *           |  79  |  78  |  77  |  76  |  75  |  74  |  73 |  72 |		pArr[9]
 * 			 ————————————————————————————————————————————————————————
 *
 * @param pArr	标志数组
 * @param len	标志数组长度
 * @param id	参数ID
 * @return	0=成功，-1=错误
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
 * 参数标记逆映射为参数ID
 *
 * @param pArr	标志数组
 * @param len	标志数组长度
 * @param id	参数ID
 * @return	0=成功，-1=错误
 */
u32 map_index_2_param_id(u8 row, u8 column) {
	return (param_id[(row << 3) + column]);
}

/**
 * 取下个字段，以‘;’分割
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
 * 取位置数据，包括基本信息和附加信息
 *
 * @param pBuf		内存缓区
 * @return		位置数据长度
 */
u16 Protocol_GetPositionInfo(u8 *pBuf) {
	u16 u16Len = 0;
	tMsg_T_PositionInfo *pInfo = NULL;

	pInfo = (tMsg_T_PositionInfo *)pBuf;

	memcpy_((u8*)&pInfo->tBaseInfo.tSysState, (u8*)&ptParam_Runtime->tSysState, sizeof(tSYSTEMSTATE));
	memcpy_((u8*)&pInfo->tBaseInfo.tWarningState, (u8*)&ptParam_Runtime->tWarningState, sizeof(tWARNINGSTATE));
	memcpy_((u8*)&pInfo->tBaseInfo.tGpsInfo, (u8*)&ptParam_Runtime->tGpsInfo, sizeof(tGPSINFO));

	///填充位置附加信息，未超速
	if(0 == ptParam_Runtime->bOverSpeed) {
		tMsg_Position_AppendInfo_NonOverSpeed * pApendInfo = &pInfo->tAppendInfo.tAppendInfo_NonOverSpeed;
		pApendInfo->ID_1 = 0x01;
		pApendInfo->ID_1_len = 4;
		pApendInfo->u32Kilemeters = ptParam_Runtime->u32Kilemeters;
		ENDIAN_U32(pApendInfo->u32Kilemeters);

		pApendInfo->ID_2 = 0x02;
		pApendInfo->ID_2_len = 2;
		pApendInfo->u16Oil = ptParam_Runtime->u16Oil; 	///0x02 2 油量，WORD，1/10L，对应车上油量表读数
		ENDIAN_U16(pApendInfo->u16Oil);

		pApendInfo->ID_3 = 0X03;
		pApendInfo->ID_3_len = 2;
		pApendInfo->u16Speed = ptParam_Runtime->u16Speed;	///0x03 2 行驶记录功能获取的速度，WORD，1/10km/h
		ENDIAN_U16(pApendInfo->u16Speed);

	#ifdef JTT808_Ver_2013
		pApendInfo->ID_4 = 0x04;
		pApendInfo->ID_4_len = 2;
		pApendInfo->u16WarningEvtId = ptParam_Runtime->u16WarningEvtId; ///0x04 2 需要人工确认报警事件的ID，WORD，从1 开始计数
		ENDIAN_U16(pApendInfo->u16WarningEvtId);
	#endif
		/**0x05-0x10 保留*/
		pApendInfo->ID_11 = 0x11;
		pApendInfo->ID_11_len = 1;
		pApendInfo->tNonOverspeedWarn.u8PosType = 0; ///1 或5 超速报警附加信息见表28

		pApendInfo->ID_12 = 0x12;
		pApendInfo->ID_12_len = sizeof(pApendInfo->tBarrierWarn);
		///6 进出区域/路线报警附加信息见表29
		pApendInfo->tBarrierWarn.u8PosType = ptParam_Runtime->tBarrierWarn.u8PosType;
		pApendInfo->tBarrierWarn.u8EnterOrLeave = ptParam_Runtime->tBarrierWarn.u8EnterOrLeave;
		pApendInfo->tBarrierWarn.u32AreaOrLineId = ptParam_Runtime->tBarrierWarn.u32AreaOrLineId;
		ENDIAN_U32(pApendInfo->tBarrierWarn.u32AreaOrLineId);

		///0x13 7 路段行驶时间不足/过长报警附加信息见表30
		pApendInfo->ID_13 = 0x13;
		pApendInfo->ID_13_len = sizeof(pApendInfo->tDrvTimeWarn);
		pApendInfo->tDrvTimeWarn.u8Result = ptParam_Runtime->tDrvTimeWarn.u8Result;
		pApendInfo->tDrvTimeWarn.u16DriveTime = ptParam_Runtime->tDrvTimeWarn.u16DriveTime;
		pApendInfo->tDrvTimeWarn.u32LineId = ptParam_Runtime->tDrvTimeWarn.u32LineId;
		ENDIAN_U16(pApendInfo->tDrvTimeWarn.u16DriveTime);
		ENDIAN_U32(pApendInfo->tDrvTimeWarn.u32LineId);

		/**0x14-0x24 保留*/
	#ifdef JTT808_Ver_2013
		pApendInfo->ID_25 = 0x25;
		pApendInfo->ID_25_len = 4;
		pApendInfo->u32ExtSignalState = ptParam_Runtime->u32ExtSignalState;	///0x25 4 扩展车辆信号状态位，定义见表31
		ENDIAN_U16(pApendInfo->u32ExtSignalState);

		pApendInfo->ID_2A = 0x2A;
		pApendInfo->ID_2A_len = 2;
		pApendInfo->u16IOState = ptParam_Runtime->u16IOState;	///0x2A 2 IO状态位，定义见表32
		ENDIAN_U16(pApendInfo->u16IOState);

		pApendInfo->ID_2B = 0x2B;
		pApendInfo->ID_2B_len = 4;
		pApendInfo->u32ADValue = ptParam_Runtime->u32ADValue;	///0x2B 4 模拟量，bit0-15，AD0；bit16-31，AD1。
		ENDIAN_U32(pApendInfo->u32ADValue);

		pApendInfo->ID_30 = 0x30;
		pApendInfo->ID_30_len = 1;
		pApendInfo->u8Csq = ptParam_Runtime->u8Csq;	///0x30 1 BYTE，无线通信网络信号强度

		pApendInfo->ID_31 = 0x31;
		pApendInfo->ID_31_len = 1;
		pApendInfo->u8GNSS_Satellites = ptParam_Runtime->u8GNSS_Satellites; ///0x31 1 BYTE，GNSS 定位卫星数

		pApendInfo->ID_E0;	///0xE0 后续信息长度 后续自定义信息长度
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
		pApendInfo->u16Oil = ptParam_Runtime->u16Oil; 	///0x02 2 油量，WORD，1/10L，对应车上油量表读数
		ENDIAN_U16(pApendInfo->u16Oil);

		pApendInfo->ID_3 = 0X03;
		pApendInfo->ID_3_len = 2;
		pApendInfo->u16Speed = ptParam_Runtime->u16Speed;	///0x03 2 行驶记录功能获取的速度，WORD，1/10km/h
		ENDIAN_U16(pApendInfo->u16Speed);

	#ifdef JTT808_Ver_2013
		pApendInfo->ID_4 = 0x04;
		pApendInfo->ID_4_len = 2;
		pApendInfo->u16WarningEvtId = ptParam_Runtime->u16WarningEvtId; ///0x04 2 需要人工确认报警事件的ID，WORD，从1 开始计数
		ENDIAN_U16(pApendInfo->u16WarningEvtId);
	#endif
		/**0x05-0x10 保留*/
		pApendInfo->ID_11 = 0x11;
		pApendInfo->ID_11_len = 1;
		///1 或5 超速报警附加信息见表28
		pApendInfo->tOverspeedWarn.u8PosType = ptParam_Runtime->tOverspeedWarn.u8PosType;
		pApendInfo->tOverspeedWarn.u32AreaOrLineId = ptParam_Runtime->tOverspeedWarn.u32AreaOrLineId;
		ENDIAN_U32(pApendInfo->tOverspeedWarn.u32AreaOrLineId);

		pApendInfo->ID_12 = 0x12;
		pApendInfo->ID_12_len = sizeof(pApendInfo->tBarrierWarn);
		///6 进出区域/路线报警附加信息见表29
		pApendInfo->tBarrierWarn.u8PosType = ptParam_Runtime->tBarrierWarn.u8PosType;
		pApendInfo->tBarrierWarn.u8EnterOrLeave = ptParam_Runtime->tBarrierWarn.u8EnterOrLeave;
		pApendInfo->tBarrierWarn.u32AreaOrLineId = ptParam_Runtime->tBarrierWarn.u32AreaOrLineId;
		ENDIAN_U32(pApendInfo->tBarrierWarn.u32AreaOrLineId);

		///0x13 7 路段行驶时间不足/过长报警附加信息见表30
		pApendInfo->ID_13 = 0x13;
		pApendInfo->ID_13_len = sizeof(pApendInfo->tDrvTimeWarn);
		pApendInfo->tDrvTimeWarn.u8Result = ptParam_Runtime->tDrvTimeWarn.u8Result;
		pApendInfo->tDrvTimeWarn.u16DriveTime = ptParam_Runtime->tDrvTimeWarn.u16DriveTime;
		pApendInfo->tDrvTimeWarn.u32LineId = ptParam_Runtime->tDrvTimeWarn.u32LineId;
		ENDIAN_U16(pApendInfo->tDrvTimeWarn.u16DriveTime);
		ENDIAN_U32(pApendInfo->tDrvTimeWarn.u32LineId);

		/**0x14-0x24 保留*/
	#ifdef JTT808_Ver_2013
		pApendInfo->ID_25 = 0x25;
		pApendInfo->ID_25_len = 4;
		pApendInfo->u32ExtSignalState = ptParam_Runtime->u32ExtSignalState;	///0x25 4 扩展车辆信号状态位，定义见表31
		ENDIAN_U32(pApendInfo->u32ExtSignalState);

		pApendInfo->ID_2A = 0x2A;
		pApendInfo->ID_2A_len = 2;
		pApendInfo->u16IOState = ptParam_Runtime->u16IOState;	///0x2A 2 IO状态位，定义见表32
		ENDIAN_U16(pApendInfo->u16IOState);

		pApendInfo->ID_2B = 0x2B;
		pApendInfo->ID_2B_len = 4;
		pApendInfo->u32ADValue = ptParam_Runtime->u32ADValue;	///0x2B 4 模拟量，bit0-15，AD0；bit16-31，AD1。
		ENDIAN_U32(pApendInfo->u32ADValue);

		pApendInfo->ID_30 = 0x30;
		pApendInfo->ID_30_len = 1;
		pApendInfo->u8Csq = ptParam_Runtime->u8Csq;	///0x30 1 BYTE，无线通信网络信号强度

		pApendInfo->ID_31 = 0x31;
		pApendInfo->ID_31_len = 1;
		pApendInfo->u8GNSS_Satellites = ptParam_Runtime->u8GNSS_Satellites; ///0x31 1 BYTE，GNSS 定位卫星数

		pApendInfo->ID_E0;	///0xE0 后续信息长度 后续自定义信息长度
		pApendInfo->ID_E0_len = 1;
		pApendInfo->u8ExtraInfoLen = 0;
	#endif

		u16Len += sizeof(tMsg_T_PositionInfo);
	}

	return u16Len;
}

/**
 * 服务器命令错误处理
 *
 */
void ERROR_Handle( iRet) {

}
