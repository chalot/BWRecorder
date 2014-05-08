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
 * 说明:
 * 1)大端格式
 * 2)尽量使得新旧国标兼容
 */

/**编译开关，控制新旧协议支持，两者同时只能打开一个*/
///#define JTT808_Ver_2011	///编译开关，打开使能旧国标协议
#define JTT808_Ver_2013	///新国标协议

#pragma pack(1)


/**************************** 协议命令字类型 *********************************/
#define	MSGID_T_GeneralAck				0x0001///终端通用应答消息
#define	MSGID_S_GeneralAck				0x8001///平台通用应答消息
#ifdef JTT808_Ver_2013
#define	MSGID_S_ReloadSubPack			0x8003///Ver2013,///补传分包请求
#endif///JTT808_Ver_2013
#define	MSGID_T_Heatbeat				0x0002///终端心跳
#define	MSGID_T_Register				0x0100///终端注册
#define	MSGID_S_RegisterAck				0x8100///终端注册应答
#define	MSGID_T_Unregister				0x0003///终端注销
#define	MSGID_T_Authentication			0x0102///终端鉴权
#define	MSGID_S_SetParam				0x8103///设置终端参数
#define	MSGID_S_QueryParam				0x8104///查询终端参数
#ifdef JTT808_Ver_2013
#define	MSGID_T_QuerySpecifyParam		0x8106///查询指定终端参数
#endif///JTT808_Ver_2013
#define	MSGID_T_QueryParamAck			0x0104///查询终端参数应答
#define	MSGID_S_Control					0x8105///终端控制
#ifdef JTT808_Ver_2013
#define	MSGID_S_QueryProperty			0x8107///查询终端属性
#define	MSGID_T_QueryPropertyAck		0x0107///查询终端属性应答
#define	MSGID_T_DispathUpgPack			0x8108///下发终端升级包
#define	MSGID_T_UpgrageResult			0x0108///终端升级结果通知
#endif///JTT808_Ver_2013
#define	MSGID_T_RptPositionInfo			0x0200///位置信息汇报
#define	MSGID_S_QueryPosition			0x8201///位置信息查询
#define	MSGID_T_QueryPositionAck		0x0201///位置信息查询应答
#define	MSGID_S_Tracking				0x8202///临时位置跟踪控制
#ifdef JTT808_Ver_2013
#define	MSGID_S_ConfirmWarningInfo		0x8203///人工确认报警消息
#endif///JTT808_Ver_2013
#define	MSGID_S_DownloadTxtInfo			0x8300///文本信息下发
#define	MSGID_S_SetEvent				0x8301///事件设置
#define	MSGID_T_RptEvent				0x0301///事件报告
#define	MSGID_S_AskQuestion				0x8302///提问下发
#define	MSGID_T_QuestionAck				0x0302///提问应答
#define	MSGID_S_SetInfoOnDemandMenu		0x8303///信息点播菜单设置
#define	MSGID_T_OrderInfoCtrl			0x0303///信息点播/取消
#define	MSGID_S_InfoService				0x8304///信息服务
#define	MSGID_S_Redial					0x8400///电话回拨
#define	MSGID_S_SetPhoneBook			0x8401///设置电话本
#define	MSGID_S_VechicleCtrl			0x8500///车辆控制
#define	MSGID_T_VechicleCtrlAck			0x0500///车辆控制应答
#define	MSGID_S_SetRoundErea			0x8600///设置圆形区域
#define	MSGID_S_DelRoundErea			0x8601///删除圆形区域
#define	MSGID_S_SetRectangleErea		0x8602///设置矩形区域
#define	MSGID_S_DelRectangleErea		0x8603///删除矩形区域
#define	MSGID_S_SetPolygonErea			0x8604///设置多边形区域
#define	MSGID_S_DelPolygonErea			0x8605///删除多边形区域
#define	MSGID_S_SetPathline				0x8606///设置路线
#define	MSGID_S_DelPathline				0x8607///删除路线
#define	MSGID_S_RecorderCapture			0x8700///行驶记录仪数据采集命令
#define	MSGID_T_RecorderDataUpload		0x0700///行驶记录仪数据上传
#define	MSGID_S_RecorderParamDownload	0x8701///行驶记录参数下传命令
#define	MSGID_T_EBillRpt				0x0701///电子运单上报
#define	MSGID_S_RequestDriverInfo		0x8702///上报驾驶人信息
#define	MSGID_T_DriverInfoRpt			0x0702///驾驶员身份信息采集上报
#ifdef JTT808_Ver_2013
#define	MSGID_T_PositionInfoBatchRpt	0x0704///定位数据批量上传
#define	MSGID_T_CANDataUpload			0x0705///CAN总线数据上传
#endif///JTT808_Ver_2013
#define	MSGID_T_MediaEventInfoRpt		0x0800///多媒体事件信息上传
#define	MSGID_T_MediaDataRpt			0x0801///多媒体数据上传
#define	MSGID_S_MediaDataRptAck			0x8800///多媒体数据上传应答
#define	MSGID_S_TakePhoto				0x8801///摄像头立即拍照命令
#ifdef JTT808_Ver_2013
#define	MSGID_S_TakePhotoAck			0x0805///摄像头立即拍照命令应答
#endif///JTT808_Ver_2013
#define	MSGID_S_StoredMediaQuery		0x8802///存储多媒体数据检索命令
#define	MSGID_T_StoredMediaQueryAck		0x0802///存储多媒体数据检索应答命令
#define	MSGID_S_StoredMediaRpt			0x8803///存储多媒体数据上传命令
#define	MSGID_S_StartAudioRecord		0x8804///录音开始命令
#define MSGID_S_SingleMediaRetrieve		0x8805///8.60 单条存储多媒体数据检索上传命令
#define	MSGID_S_TransparentTransfer		0x8900///数据下行透传命令
#define	MSGID_T_TransparentTransfer		0x0900///数据上行透传命令
#define	MSGID_T_DataCompressRpt			0x0901///数据压缩上报
#define	MSGID_S_PlatformRASKey			0x8A00///平台RAS公钥
#define	MSGID_T_RASKey					0x0A00///终端RAS公钥

///消息格式
///typedef struct
///{
///	u8	flag;
///
///	u8	flag;
///
///};

///消息标识位定义
#define		MSG_FLAG_BYTE		0x7E
#define		MSG_TRANS_BYTE		0x7D

#define ENCRYPT_FLAG	0	///加密标志，1=数据加密, 0=数据不加密


///组织消息属性格式
#define	FORM_MSG_PROPERTY(len, rsa, multipack) 	(((len) & 0x3FF) |		    \
												((rsa == 0) ? 0 : 0x0400) |	\
												((multipack == 0) ? 0 : 0x2000))
/**消息体属性*/
typedef struct
{
	u16 msg_len			:	10;		///消息体长度
	u16 msg_encrypt		:	3;		///消息体加密
	u16 msg_subpack		:	1;		///消息体分包
	u16 msg_reserved	:	2;

} MSG_PROPERTY;


/**取消息头分包标志*/
//#define GET_MULTIPACK_FLAG(prop)	(prop & 0x2000)

///消息头定义
typedef	struct
{
	u8	u8Flag;					///标识
	u16	u16MsgId;				///消息ID
//	u16	u16MsgProperty;			///消息属性
	MSG_PROPERTY tMsgProperty;	///消息属性
	u8 	u8BcdPhoneNum[6];		///终端手机号
	u16 u16MsgCircularId;		///消息流水号
}tMSGHEAD;

/////有分包消息头定义
//typedef		struct
//{
//	u16	u16MsgId;	///消息ID
//	u16	u16MsgProperty;		///消息属性
//	u8 	u8BcdTerminalPhoneNum[6];	///终端手机号
//	u16 u16MsgCircularId;	///消息流水号
//	u16 u16MsgTotalPackageNum;	///消息总包数
//	u16 u16MsgPackageSerial;	///消息包序号 从*1*开始，注意！！！
//}tMSGHEAD_PACKED;

//有分包时的附加项
typedef struct
{
	u16 u16MsgTotalPackageNum;	///消息总包数
	u16 u16MsgPackageSerial;	///消息包序号 从*1*开始，注意！！！
} tMSGHEAD_PACKED;

typedef		enum
{
	RET_SUCCESS	= 0,		///成功/确认
	RET_FAIL = 1,			///失败
	RET_ERROR = 2,			///消息有误
	RET_NOTSUPPORT = 3		///不支持
}eRESULT;

typedef		enum
{
	REG_SUCCESS			= 0,	///成功/确认
	REG_VECHICLEREGED	= 1,	///车辆已经注册
	REG_NOVEHICLE		= 2,	///数据库中无此车辆
	REG_TERMINALREGED	= 3,	///终端已经注册
	REG_NOTERMINAL		= 4,	///数据库中无此终端
} eREGRESULT;

/** 8.1终端通用应答, ID=0x0001 ***********************************************/
typedef	struct
{
	u16	u16ResponsePackId;	///对应的平台消息的流水号
	u16	u16CmdId;	///对应的平台消息的ID
	eRESULT	eResult;	///结果
} tMsg_T_GeneralAck;

/** 8.2 平台通用应答,ID=0x8001 ***********************************************/
typedef	struct
{
	u16	u16RespSeqId;	///对应的终端消息的流水号
	u16	u16RespMsgId;	///对应的终端消息的ID
	eRESULT	eResult;	///结果
} tMsg_S_GeneralAck;

/** 8.3 终端心跳,消息ID：0x0002 **********************************************/
/**终端心跳数据消息体为空。*/


#ifdef JTT808_Ver_2013
/** 8.4 补传分包请求,消息ID：0x8003。******************************************/
typedef struct
{
	u16 u16OriginalMsgSeq;	///原始消息流水号
	u8	u8ReloadPackCnt;	///重传包总数
	u8* pu8PackIds;			///重传包ID列表
} tMsg_S_ReloadSubPack;
#endif///JTT808_Ver_2013

#define PARAM_VIN_PLATE_MAXLEN		40	/**VIN码或车牌最大长度*/

/**终端基本信息*/
typedef struct
{
	u16	u16ProviceId;		///省域ID
	u16	u16CountyId;		///市县域ID
	u8	au8FactoryId[5];	///制造商ID
#ifdef JTT808_Ver_2011
	u8	aType[8];	///终端型号，8个字节，此终端型号由制造商自行定义，位数不	足时，补空格。
	u8	aTId[7];		///终端ID,7 个字节，由大写字母和数字组成，此终端ID 由制造商自行定义，位数不足时，后补“0X00”。
	u8	u8PlateColor;		///车牌颜色，按照JT/T415-2006 的5.4.12
#endif
#ifdef JTT808_Ver_2013
	u8	aType[20];///终端型号20 个字节，此终端型号由制造商自行定义，位数不足时，后补“0X00”。
	u8	aTId[7];		///终端ID
	u8	u8PlateColor;		///车牌颜色，按照JT/T415-2006 的5.4.12。未上牌时，取值为0。
#endif///JTT808_Ver_2013
} tTerminalInfo_Base;

/** 8.5 终端注册 消息ID：0x0100。**************************************************/
///终端注册消息结构体定义
typedef	struct
{
	tTerminalInfo_Base	tBase;
	u8	aPlate[0];		/**V2011： 机动车号牌;
						  V2013：车牌车牌颜色为0 时，表示车辆VIN；否则，表示公安
						  交通管理部门颁发的机动车号牌。*/
} tMsg_T_Register;


/** 8.6 终端注册应答消息结构体定义 ID=8100。************************************/
typedef	struct
{
	u16	u16SAckSeqNum;		///对应的平台消息的流水号
	eREGRESULT	eRet;	///结果
	u8	aAuthorCode[0];	///鉴权码
} tMsg_S_Ack_Register;

/** 8.7 终端注销 ID=0003。*****************************************************/
/**终端注销消息体为空。*/

/** 8.8 终端鉴权  ID=0102。****************************************************/
/**终端重连后上报鉴权码*/
typedef	struct
{
	u8	aAuthorCode[0];	///鉴权码
} tMsg_T_RptAuthorCode;

/**日期格式*/
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
///参数项ID定义
#define	PARAM_ALIVE_INTERVAL			0x0001		///终端心跳发送间隔，单位为s
#define	PARAM_TCPACK_TIMEOUT			0x0002		///TCP应答超时，单位为s
#define	PARAM_TCP_RESENDTIMES			0x0003		///TCP消息重传次数
#define	PARAM_UDPACK_TIMEOUT			0x0004		///UDP应答超时，单位为s
#define	PARAM_UDP_RESENDTIMES			0x0005		///UDP消息重传次数
#define	PARAM_SMSACK_TIMEOUT			0x0006		///SMS应答超时，单位为s
#define	PARAM_SMS_RESENDTIMES			0x0007		///SMS消息重传次数
/**0X0008 ----0X000F 保留*/
#define	PARAM_MAINSRV_APN				0x0010		///主服务器APN，无线通信拨号访问点
#define	PARAM_MAINSRV_USR				0x0011		///主服务器无线通信拨号用户名
#define	PARAM_MAINSRV_PSWD				0x0012		///主服务器无线通信拨号密码
#define	PARAM_MAINSRV_IP				0x0013		///主服务器地址，IP或域名
#define	PARAM_VICESRV_APN				0x0014		///备份服务器APN，无线通信拨号访问点
#define	PARAM_VICESRV_USR				0x0015		///备份服务器无线通信拨号用户名
#define	PARAM_VICESRV_PSWD				0x0016		///备份服务器无线通信拨号密码
#define	PARAM_VICESRV_IP				0x0017		///备份服务器地址，IP或域名
#define	PARAM_TCP_PORT					0x0018		///TCP端口
#define	PARAM_UDP_PORT					0x0019		///UDP端口
/**0X001A ----0X001F 保留*/
#ifdef JTT808_Ver_2013	///新版增加协议部分
#define	PARAM_ICVERIFY_MS_IPOrDNS		0x001A		///道路运输证IC卡认证主服务器IP地址或域名
#define	PARAM_ICVERIFY_TCPPORT			0x001B		///道路运输证IC卡认证主服务器TCP端口
#define	PARAM_ICVERIFY_UDPPORT			0x001C		///道路运输证IC卡认证主服务器UDP端口
#define	PARAM_ICVERIFY_BS_UDPPORT		0x001D		///道路运输证IC卡备份服务器IP地址或域名，端口同主服务器
#endif///JTT808_Ver_2013

					 /**终端位置汇报策略，0:定时汇报，1:定距汇报，2:定时和定距汇报*/
#define	PARAM_POSRPT_STRATEGE		0x0020
			/**终端汇报方案，0:根据ACC状态，1:根据登录状态和ACC状态，先判断登录状态，
			 	 	 	 	 	 	 	 	 	 	 	 	若登录再根据ACC状态*/
#define	PARAM_POSRPT_METHOD				0x0021
#define	PARAM_DRV_UNREG_RPTPERIOD		0x0022		///驾驶员未登录汇报时间间隔，单位为秒
/**0X0023 ----0X0026 保留*/
#define	PARAM_RPTPERIOD_ONSLEEP			0x0027		///休眠时汇报时间间隔，单位为秒
#define	PARAM_RPTPERIOD_URGENT			0x0028		///紧急报警时汇报时间间隔，单位为秒
#define	PARAM_RPTPERIOD_DEFAULT			0x0029		///缺省时汇报时间间隔，单位为秒
/**0X002A ----0X002B 保留*/
#define	PARAM_RPTDIST_DEFAULT			0x002C		///缺省时距离汇报间隔，单位为米
#define	PARAM_RPTDIST_DRV_UNREG			0x002D		///驾驶员未登录距离汇报间隔，单位为米
#define	PARAM_RPTDIST_SLEEP				0x002E		///休眠时距离汇报间隔，单位为米
#define	PARAM_RPTDIST_URGENT			0x002F		///紧急报警时距离汇报间隔，单位为米
#define	PARAM_INFLECTION_POINT_ANGLE	0x0030		///拐点补传角度，单位为度(< 180)
#define	PARAM_BARRIER_RADIUS			0x0031		///电子围栏半径(非法移位阈值)，单位为米
/**0X0032----0X003F 保留*/
#define	PARAM_MONITORCENTER_PHONENUM	0x0040		///监控平台电话号码
#define	PARAM_RESET_PHONENUM	0x0041		///复位电话号码，可采用此电话号码拨打终端电话让终端复位
			  ///回复出厂设置电话号码，可采用此电话号码拨打终端电话让终端恢复出厂设置
#define	PARAM_RECOVERFACTORY_PHONENUM	0x0042
#define	PARAM_MONITORCENTER_SMS_NUM		0x0043		///监控平台SMS电话号码
#define	PARAM_RECVTXTALARM_SMS_NUM		0x0044		///接收终端SMS文本报警电话号码
				///终端电话接听策略，0:自动接通，1:ACC ON时自动接通，OFF时手动接听
#define	PARAM_ANSWERCALLING_STRATEGE	0x0045
		 ///终端每次通话时最长通话时间，单位为秒，0为不允许通话，0xffffffff为不限制
#define	PARAM_TOUT_PERCALL				0x0046
				///终端每月最长通话时间，单位为秒，0为不允许通话，0xffffffff为不限制
#define	PARAM_TOUT_PERMONTHCALL			0x0047
#define	PARAM_MONITORLISTEN_PHONENUM	0x0048			///监听电话号码
#define	PARAM_SUPERVISE_SMS_NUM			0x0049			///监管平台特权短信号码
/**0X004A----0X004F 保留*/

///报警屏蔽字，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警被屏蔽
#define	PARAM_ALARM_MASK				0x0050
///报警发送文本SMS开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时发送文本SMS
#define	PARAM_SMSCTRL_ONALARM			0x0051
///报警拍摄开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时摄像头拍摄
#define	PARAM_PICCTRL_ONALARM			0x0052
///报警拍摄存储标志，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时拍的照片进行存储，否则实时上传
#define	PARAM_PICCTRL_STORE_ONALARM		0x0053
///关键标志，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警为关键报警
#define	PARAM_KEYALARM_FLAG	 			0x0054
#define	PARAM_MAXSPEED					0x0055	   ///最高速度，单位为公里每小时(km/h)
#define	PARAM_OVERSPEED_LASTING_TIMEOUT		0x0056	///超速持续时间，单位为秒
#define	PARAM_CONTINUOUS_DRV_TIMEOUT		0x0057	///连续驾驶时间门限，单位为秒
#define	PARAM_DRV_TOTAL_EACHDAY_TIMEOUT		0x0058	///当天累计驾驶时间，单位为秒
#define	PARAM_LEAST_REST_TIMEOUT			0x0059	///最小休息时间，单位为秒
#define	PARAM_MAX_STOPPING_TIMEOUT			0x005A	///最长停车时间，单位为秒
/**0X005B----0X006F */
#ifdef JTT808_Ver_2013
#define	PARAM_OVERSPEED_PREALARM_DIF	0x005B	///超速报警预警差值，单位1/10Km/h
#define	PARAM_FATIGE_PREALARM_DIF		0x005C	///疲劳驾驶预警差值，单位为秒，>0
	  ///b7-b0:碰撞时间，单位4ms；b15-b8:碰撞加速度，单位0.1g，设置范围0~79，默认10
#define	PARAM_COLLISION_SETTING			0x005D
								   ///侧翻报警参数设置，侧翻角度，单位1度，默认30度
#define	PARAM_SIDECOLLAPSE_SETTING		0x005E
/**0X005F----0X0063 */
#define	PARAM_PERIODIC_PHOTO_CTRL		0x0064	///定时拍照控制
#define	PARAM_PERDIST_PHOTO_CTRL		0x0065	///定距拍照控制
/**0X0066----0X006F */
#endif///JTT808_Ver_2013

#define	PARAM_VIDEO_QUALITY				0x0070		///图片/视频质量，1-10,1最好
#define	PARAM_BRIGHTNESS				0x0071		///亮度，0-255
#define	PARAM_CONTRASTNESS				0x0072		///对比度，0-127
#define	PARAM_SATURATION				0x0073		///饱和度，0-127
#define	PARAM_CHROMA					0x0074		///色度，0-255
/**0X0075----0X007F*/
#define	PARAM_VEHICLE_KILOMETRES		0x0080		///车辆里程表读数，1/10km
#define	PARAM_VEHICLE_PROVINCE_ID		0x0081		///车辆所在地省域ID
#define	PARAM_VEHICLE_CITY_ID			0x0082		///车辆所在地市域ID
#define	PARAM_VEHICLE_ID 				0x0083		///车牌号码
#define	PARAM_VEHICLE_ID_COLOR			0x0084		///车牌颜色
#endif

#ifdef JTT808_Ver_2013
/**以下为新协议增加的参数项*/
/**
GNSS 定位模式，定义如下：
bit0，0：禁用GPS 定位， 1：启用GPS 定位；
bit1，0：禁用北斗定位， 1：启用北斗定位；
bit2，0：禁用GLONASS 定位， 1：启用GLONASS 定位；
bit3，0：禁用Galileo 定位， 1：启用Galileo 定位。
*/
#define PARAM_GNSS_MODE		0x0090
/**
GNSS 波特率，定义如下：
0x00：4800；0x01：9600；
0x02：19200；0x03：38400；
0x04：57600；0x05：115200。
*/
#define PARAM_GNSS_BOUNDRATE	0x0091
/**
GNSS 模块详细定位数据输出频率，定义如下：
0x00：500ms；0x01：1000ms（默认值）；
0x02：2000ms；0x03：3000ms；
0x04：4000ms。
*/
#define PARAM_GNSS_DMSG_OUTFREQ	0x0092
/**0x0093 DWORD GNSS 模块详细定位数据采集频率，单位为秒，默认为1。*/
#define PARAM_GNSS_DMSG_RETRIEVEFREQ	0x0093
/**
GNSS 模块详细定位数据上传方式：
0x00，本地存储，不上传（默认值）；
0x01，按时间间隔上传；
0x02，按距离间隔上传；
0x0B，按累计时间上传，达到传输时间后自动停止上传；
0x0C，按累计距离上传，达到距离后自动停止上传；
0x0D，按累计条数上传，达到上传条数后自动停止上传。
*/
#define PARAM_GNSS_DMSG_RPTTYPE	0x0094
/**
GNSS 模块详细定位数据上传设置：
上传方式为0x01 时，单位为秒；
上传方式为0x02 时，单位为米；
上传方式为0x0B 时，单位为秒；
上传方式为0x0C 时，单位为米；
上传方式为0x0D 时，单位为条。
*/
#define PARAM_GNSS_DMSG_RPTSETTING	0x0095

/**0x0100 DWORD CAN 总线通道1 采集时间间隔(ms)，0 表示不采集*/
#define PARAM_CAN_CH1_RETRIEVEINTERVAL	0x0100
/**0x0101 WORD CAN 总线通道1 上传时间间隔(s)，0 表示不上传*/
#define PARAM_CAN_CH1_UPLOADINTERVAL	0x0101

/**0x0102 DWORD CAN 总线通道2 采集时间间隔(ms)，0 表示不采集*/
#define PARAM_CAN_CH2_RETRIEVEINTERVAL	0x0102
/**0x0103 WORD CAN 总线通道2 上传时间间隔(s)，0 表示不上传*/
#define PARAM_CAN_CH2_UPLOADINTERVAL	0x0103


/**
		0x0110 BYTE[8]
CAN 总线ID 单独采集设置：
bit63-bit32 表示此ID 采集时间间隔(ms)，0 表示不采集；
bit31 表示CAN 通道号，0：CAN1，1：CAN2；
bit30 表示帧类型，0：标准帧，1：扩展帧；
bit29 表示数据采集方式，0：原始数据，1：采集区间的计算值；
bit28-bit0 表示CAN 总线ID。
*/
#define PARAM_CAN_SOLORETRIEVE	0x0110

/**0x0111-0x01FF BYTE[8] 用于其他CAN 总线ID 单独采集设置*/
#define PARAM_CAN_SOLORETRIEVE_OTHER	0x0111

/**0xF000-0xFFFF 用户自定义*/

/**

表 13 定时拍照控制位定义
位 定义 描述及要求
0 摄像通道1 定时拍照开关标志 0：不允许； 1：允许
1 摄像通道2 定时拍照开关标志 0：不允许； 1：允许
2 摄像通道3 定时拍照开关标志 0：不允许； 1：允许
3 摄像通道4 定时拍照开关标志 0：不允许； 1：允许
4 摄像通道5 定时拍照开关标志 0：不允许； 1：允许
5-7 保留
8 摄像通道1 定时拍照存储标志 0：存储； 1：上传
9 摄像通道2 定时拍照存储标志 0：存储； 1：上传
10 摄像通道3 定时拍照存储标志 0：存储； 1：上传
11 摄像通道4 定时拍照存储标志 0：存储； 1：上传
12 摄像通道5 定时拍照存储标志 0：存储； 1：上传
13-15 保留
16 定时时间单位
0：秒，当数值小于5 秒时，终端按5 秒处理；
1：分。
17-31 定时时间间隔 收到参数设置或重新启动后执行
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

表14 定距拍照控制位定义
位 定义 描述及要求
0 摄像通道1 定距拍照开关标志 0：不允许； 1：允许
1 摄像通道2 定距拍照开关标志 0：不允许； 1：允许
2 摄像通道3 定距拍照开关标志 0：不允许； 1：允许
3 摄像通道4 定距拍照开关标志 0：不允许； 1：允许
4 摄像通道5 定距拍照开关标志 0：不允许； 1：允许
5-7 保留
8 摄像通道1 定距拍照存储标志 0：存储； 1：上传
9 摄像通道2 定距拍照存储标志 0：存储； 1：上传
10 摄像通道3 定距拍照存储标志 0：存储； 1：上传
11 摄像通道4 定距拍照存储标志 0：存储； 1：上传
12 摄像通道5 定距拍照存储标志 0：存储； 1：上传
13-15 保留
16 定距距离单位
0：米，当数值小于100 米时，终端按100 米处理；
1：公里。
17-31 定距距离间隔 收到参数设置或重新启动后执行
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

///参数项头部
typedef 	struct
{
	u32		u32Id;		///参数ID
	u8		u8Len;		///参数长度
	u8		aValue[0];	///参数内容
} tPARAMITEMHEAD;

///参数项头部
typedef 	struct
{
	u32		u32Id;		///参数ID
	u8		u8Len;		///参数长度
	u32		u32Value;	///DWORD
} tPARAMITEM_U32;

///参数项头部
typedef 	struct
{
	u32		u32Id;		///参数ID
	u8		u8Len;		///参数长度
	u16		u16Value;	///WORD
} tPARAMITEM_U16;

///参数项头部
typedef 	struct
{
	u32		u32Id;		///参数ID
	u8		u8Len;		///参数长度
	u8		u8Value;	///BYTE
} tPARAMITEM_U8;

///参数项头部
typedef 	struct
{
	u32		u32Id;		///参数ID
	u8		u8Len;		///参数长度
	u8		str[0];		///BYTE
} tPARAMITEM_STR;



/** 8.9 设置终端参数,消息ID：0x8103。*****************************************/
typedef	struct
{
	u8		u8Amount;			///参数总数
///	u8		u8PackageParamNum;	///包参数总数
	tPARAMITEMHEAD	atItems[0];		///参数项
} tMsg_SCMD_SetParam;


/** 8.10 查询终端参数 消息ID：0x8104。*****************************************/
/**查询终端参数消息体为空。*/

/** 8.11 查询指定终端参数 消息ID：0x8106。**************************************/
/**终端采用0x0104指令应答。*/
typedef struct
{
	u8	u8Amount;	///参数总数,参数总数为n
	u32	aIds[0];    ///参数ID 列表 BYTE[4*n],参数顺序排列，如"参数ID1 参数ID2......参数IDn"
} tMSG_SCMD_QuerySpecicialParam;

/** 8.12 查询终端参数应答 消息ID：0x0104。**************************************/
typedef	struct
{
	u16	u16SequenceNumber;	///对应的平台消息的流水号
	u8 u8ParamAmount;		///参数总数
	tPARAMITEMHEAD atItems[0];	///参数项
} tMSG_TACK_QueryParams;

///终端控制命令字
typedef enum
{
	CMD_WIRELESS_UPG			= 1,	///无线升级
	CMD_CONNECT_ALLOCATE_SERVER = 2,	///控制终端连接指定服务器
	CMD_POWER_OFF 				= 3,	///控制终端关机
	CMD_RESET 					= 4,	///控制终端复位
	CMD_RESUME_FACTORY_SET 		= 5,	///终端恢复出厂设置
	CMD_CLOSE_DATA_LINK 		= 6,	///关闭数据通信
	CMD_CLOSE_ALL_LINK 			= 7,	///关闭所有无线通信
}eTERMINALCTRL;

/** 8.13 终端控制,消息ID：0x8105。********************************************/
/**
0 命令字 	BYTE 	终端控制命令字说明见表18
1 命令参数 STRING 	命令参数格式具体见后面描述，每个字段之间采用半角”;”分隔，每个
					STRING 字段先按GBK 编码处理后再组成消息

命令字                    命令参数                         描述及要求
1 命令参数格式见表19      无线升级。参数之间采用半角分号分隔。指令如下：“URL 地址;拨号点
                                                                   名称;拨号用户名;拨号密码;地址;TCP 端口;UDP 端口;制造商ID; 硬件
                                                                  版本;固件版本; 连接到指定服务器时限”，若某个参数无值，则放空
2 命令参数格式见表19 		控制终端连接指定服务器。参数之间采用半角分号分隔。控制指令如下：
						“连接控制;监管平台鉴权码;拨号点名称;拨号用户名;拨号密码;地
						址;TCP 端口;UDP 端口;连接到指定服务器时限”，若某个参数无值，则
						放空，若连接控制值为1，则无后继参数
3 无 终端关机
4 无 终端复位
5 无 终端恢复出厂设置
6 无 关闭数据通信
7 无 关闭所有无线通信

表 19 命令参数格式
字段          		 数据类型 	描述及要求
连接控制   	BYTE    	0：切换到指定监管平台服务器，连接到该服务器后即进入应急状态，此状态下仅有下发控制指
						令的监管平台可发送包括短信在内的控制指令；
						1：切换回原缺省监控平台服务器，并恢复正常状态。
						拨号点名称 STRING 一般为服务器APN，无线通信拨号访问点，若网络制式为CDMA，则该值
						为PPP 连接拨号号码
拨号用户名 	STRING 		服务器无线通信拨号用户名
拨号密码 		STRING 		服务器无线通信拨号密码
地址 		STRING 		服务器地址,IP 或域名
TCP端口 		WORD 		服务器TCP 端口
UDP端口 		WORD 		服务器UDP 端口
制造商ID 	BYTE[5] 	终端制造商编码
监管平台鉴权码 STRING 	监管平台下发的鉴权码，仅用于终端连接到监管平台之后的鉴权，终端
						连接回原监控平台还用原鉴权码
硬件版本 		STRING 		终端的硬件版本号，由制造商自定
固件版本 		STRING 		终端的固件版本号，由制造商自定
URL地址 		STRING 		完整URL 地址
连接到指定服务器时限
			WORD 		单位：分（min），值非0 表示在终端接收到升级或连接指定服务器指令
						后的有效期截止前，终端应连回原地址。若值为0，则表示一直连接指
						定服务器
*/

#ifdef JTT808_Ver_2013
/** 8.14 查询终端属性,消息ID：0x8107。*****************************************/
/**查询终端属性消息体为空。*/
#endif ///JTT808_Ver_2013

/** 8.15 查询终端属性应答,消息ID：0x0107。*************************************/
#ifdef JTT808_Ver_2013
/**终端类型*/
typedef struct
{
	u16 bit_PassengerTransFittable	:	1;	///bit0，0：不适用客运车辆，1：适用客运车辆；
	u16 bit_DangerousFittable		:	1;	///bit1，0：不适用危险品车辆，1：适用危险品车辆；
	u16 bit_FreightTransFittable	:	1;	///bit2，0：不适用普通货运车辆，1：适用普通货运车辆；
	u16 bit_TaxiFittable			:	1;	///bit3，0：不适用出租车辆，1：适用出租车辆；
	u16 bit_reserved_4_5	:	2;
	u16 bit_SupportVideoRecord		:	1;	///bit6，0：不支持硬盘录像，1：支持硬盘录像；
	u16 bit_Integration				:	1;		///bit7，0：一体机，1：分体机。
	u16 bit_reserved_8_15	:	8;
} tTerminalType;
#endif///JTT808_Ver_2013

/**GNSS 模块属性*/
typedef struct
{
	u16 bit_Support_GPS		:	1;	///bit0，0：不支持GPS 定位， 1：支持GPS 定位；
	u16 bit_Support_Beidou	:	1;	///bit1，0：不支持北斗定位， 1：支持北斗定位；
	u16 bit_Support_GLONASS	:	1;	///bit2，0：不支持GLONASS 定位， 1：支持GLONASS 定位；
	u16 bit_Support_GALILEO	:	1;	///bit3，0：不支持Galileo 定位， 1：支持Galileo 定位。
	u16 bit_Property_reserved_4_15	: 	12;
} tGNSSModule_Property;

/**通信模块属性 BYTE*/
typedef struct
{
	u8 bit_SupportGPRS		:	1;	///bit0，0：不支持GPRS通信， 1：支持GPRS通信；
	u8 bit_SupportCDMA		:	1;	///bit1，0：不支持CDMA通信， 1：支持CDMA通信；
	u8 bit_SupportTDCDMA	:	1;	///bit2，0：不支持TD-SCDMA通信， 1：支持TD-SCDMA通信；
	u8 bit_SupportWCDMA		:	1;	///bit3，0：不支持WCDMA通信， 1：支持WCDMA通信；
	u8 bit_SupportCDMA2000	:	1;	///bit4，0：不支持CDMA2000通信， 1：支持CDMA2000通信。
	u8 bit_SupportTDLTE		:	1;	///bit5，0：不支持TD-LTE通信， 1：支持TD-LTE通信；
	u8 bit_CommProperty_reserved_6	:	1;
	u8 bit_SupportOther		:	1;	///bit7，0：不支持其他通信方式， 1：支持其他通信方式。
} tCommModule_Property;
//#endif

#ifdef JTT808_Ver_2013
typedef struct
{
	tTerminalType tType;	///终端类型 WORD
	u8	factoryId[5];	///制造商ID BYTE[5] 5 个字节，终端制造商编码。
	u8  class[20];		///终端型号 此终端型号由制造商自行定义，位数不足时，后补“0X00”。
	u8	tid[7];			///终端ID 由大写字母和数字组成，此终端ID 由制造商自行定义，位数不足时，后补“0X00”。
	u8	ICCID[10];		///终端SIM 卡ICCID BCD[10] 终端SIM 卡ICCID 号
	u8	ver_hardware_len;	///终端硬件版本号长度
	u8*	ver_hardware;	///终端硬件版本号 STRING
	u8	ver_software_len;	///终端硬软件版本号长度
	u8*	ver_software;	///终端软件版本号 STRING
	tGNSSModule_Property tGNSSProperty; ///GNSS 模块属性 BYTE
	tCommModule_Property tCommProperty; ///通信模块属性 BYTE

} tMsg_TACK_QueryTerminalProperty;

#endif	///JTT808_Ver_2013

#ifdef JTT808_Ver_2013
/** 8.16 下发终端升级包,消息ID：0x8108。***************************************/
/**下发终端升级包消息体数据格式见表21。对该命令终端使用通用应答确认是否正确收
到升级包数据。
*/
typedef struct
{
	u8 upg_type;	///升级类型 0：终端，12：道路运输证IC 卡读卡器，52：北斗卫星定位模块
	u8 factoryId[5];	///制造商ID BYTE[5] 制造商编号
	u8 version_len;		///版本号长度 BYTE n
	u8 version;		///版本号 STRING
	u32 upgPack_len;	///7+n 升级数据包长度 DWORD 单位为BYTE
	u8 *pUpgPack;		///11+n 升级数据包
} tMsg_SCMD_dispatchUpgPackage;

#endif///JTT808_Ver_2013

#ifdef JTT808_Ver_2013
/** 8.17 终端升级结果通知,消息ID：0x0108。*************************************/
/**终端在升级完成并重新连接后使用该命令通知监控中心。终端升级结果通知消息体数
据格式见表22。
*/
typedef struct
{
	u8 upg_type;	///升级类型  0：终端，12：道路运输证IC 卡读卡器，52：北斗卫星定位模块
	u8 upg_result;	/// 升级结果  0：成功，1：失败，2：取消
} tMsg_TACK_UpgResult;

#endif///JTT808_Ver_2013



/**报警标志位定义*/
#define WARNING_Emergency		((u32)0x00000001)	///1：紧急报警，触动报警开关后触发 收到应答后清零
#define WARNING_Overspeed		(((u32)0x00000001) << 1)	///1：超速报警 标志维持至报警条件解除
#define WARNING_Fatigure		(((u32)0x00000001) << 2)	///2 1：疲劳驾驶 标志维持至报警条件解除
#define WARNING_PreDangerous	(((u32)0x00000001) << 3)	///3 1：危险预警 收到应答后清零
#define WARNING_GNSS_Module_Fault		(((u32)0x00000001) << 4)	///4 1：GNSS 模块发生故障 标志维持至报警条件解除
#define WARNING_GNSS_Antena_Breakdown	(((u32)0x00000001) << 5)	///5 1：GNSS 天线未接或被剪断 标志维持至报警条件解除
#define WARNING_GNSS_Antena_ShortCircuit	(((u32)0x00000001) << 6)	///6 1：GNSS 天线短路 标志维持至报警条件解除
#define WARNING_MPwr_LowVoltage		(((u32)0x00000001) << 7)	///7 1：终端主电源欠压 标志维持至报警条件解除
#define WARNING_MPwr_poweroff		(((u32)0x00000001) << 8)	///8 1：终端主电源掉电 标志维持至报警条件解除
#define WARNING_LCD_Fault		(((u32)0x00000001) << 9)	///9 1：终端LCD 或显示器故障 标志维持至报警条件解除
#define WARNING_TTS_Fault		(((u32)0x00000001) << 10)	///10 1：TTS 模块故障 标志维持至报警条件解除
#define WARNING_Video_Fault		(((u32)0x00000001) << 11)	///11 1：摄像头故障 标志维持至报警条件解除
#ifdef JTT808_Ver_2013
#define WARNING_IC_Fault		(((u32)0x00000001) << 12)	///12 1：道路运输证IC 卡模块故障 标志维持至报警条件解除
#define WARNING_PreOverspeed	(((u32)0x00000001) << 13)	///13 1：超速预警 标志维持至报警条件解除
#define WARNING_PreFatigure		(((u32)0x00000001) << 14)	///14 1：疲劳驾驶预警 标志维持至报警条件解除
#endif///JTT808_Ver_2013
#define WARNING_TOUT_Driving_day	(((u32)0x00000001) << 18)	///18 1：当天累计驾驶超时 标志维持至报警条件解除
#define WARNING_TOUT_Stopping		(((u32)0x00000001) << 19)	///19 1：超时停车 标志维持至报警条件解除
#define WARNING_CrossBarrier		(((u32)0x00000001) << 20)	///20 1：进出区域 收到应答后清零
#define WARNING_CrossPath		(((u32)0x00000001) << 21)	///21 1：进出路线 收到应答后清零
#define WARNING_DriveTime_NotEnough	(((u32)0x00000001) << 22)	///22 1：路段行驶时间不足/过长 收到应答后清零
#define WARNING_Path_Deviate		(((u32)0x00000001) << 23)	///23 1：路线偏离报警 标志维持至报警条件解除
#define WARNING_Vechicle_VSS_Fault		(((u32)0x00000001) << 24)	///24 1：车辆VSS 故障 标志维持至报警条件解除
#define WARNING_Vechicle_Oil_Fault		(((u32)0x00000001) << 25)	///25 1：车辆油量异常 标志维持至报警条件解除
#define WARNING_Vechicle_Stolen		(((u32)0x00000001) << 26)	///26 1：车辆被盗(通过车辆防盗器) 标志维持至报警条件解除
#define WARNING_Vechicle_Illegical_Launch	(((u32)0x00000001) << 27)	///27 1：车辆非法点火 收到应答后清零
#define WARNING_Vechicle_Illegical_Moving	(((u32)0x00000001) << 28)	///28 1：车辆非法位移 收到应答后清零
#define WARNING_Vechicle_PreCollision	(((u32)0x00000001) << 29)	///29 1：碰撞预警 标志维持至报警条件解除
///#define warning_reserved_30_31	: 	2;	///30-31保留
#ifdef JTT808_Ver_2013
#define WARNING_Vechicle_PreSidestroke		(((u32)0x00000001) << 30)	///30 1：侧翻预警 标志维持至报警条件解除
#define WARNING_Vechicle_Illegical_DoorOpen		(((u32)0x00000001) << 31)	///31 1：非法开门报警（终端未设置区域时，不判断非法开门）收到应答后清零
#endif ///JTT808_Ver_2013

/**报警标志位定义*/
typedef struct
{
	u32 warning_Emergency		:	1;	///1：紧急报警，触动报警开关后触发 收到应答后清零
	u32 warning_Overspeed		:	1;	///1：超速报警, 标志维持至报警条件解除
	u32 warning_Fatigure		:	1;	///2 1：疲劳驾驶, 标志维持至报警条件解除
	u32 warning_PreDangerous	:	1;	///3 1：危险预警, 收到应答后清零
	u32 warning_GNSS_Module_Fault		:	1;	///4 1：GNSS 模块发生故障标志,维持至报警条件解除
	u32 warning_GNSS_Antena_Breakdown	:	1;	///5 1：GNSS 天线未接或被剪断标志, 维持至报警条件解除
	u32 warning_GNSS_Antena_ShortCircuit	:	1;	///6 1：GNSS 天线短路, 标志维持至报警条件解除
	u32 warning_MPwr_LowVoltage		:	1;	///7 1：终端主电源欠压, 标志维持至报警条件解除
	u32 warning_MPwr_poweroff		:	1;	///8 1：终端主电源掉电, 标志维持至报警条件解除
	u32 warning_LCD_Fault		:	1;	///9 1：终端LCD 或显示器故障, 标志维持至报警条件解除
	u32 warning_TTS_Fault		:	1;	///10 1：TTS 模块故障, 标志维持至报警条件解除
	u32 warning_Video_Fault		:	1;	///11 1：摄像头故障, 标志维持至报警条件解除
#ifdef JTT808_Ver_2011
	u32 warning_reserved_12_17	: 	6;	///12-17保留
#endif
#ifdef JTT808_Ver_2013
	u32 warning_IC_Fault		:	1;	///12 1：道路运输证IC 卡模块故障 标志维持至报警条件解除
	u32 warning_PreOverspeed	:	1;	///13 1：超速预警 标志维持至报警条件解除
	u32 warning_PreFatigure		:	1;	///14 1：疲劳驾驶预警 标志维持至报警条件解除
	u32 warning_reserved_15_17	:	3;	///15-17 保留
#endif///JTT808_Ver_2013
	u32 warning_TOUT_Driving_day	:	1;	///18 1：当天累计驾驶超时 标志维持至报警条件解除
	u32 warning_TOUT_Stopping		:	1;	///19 1：超时停车 标志维持至报警条件解除
	u32 warning_CrossBarrier		:	1;	///20 1：进出区域 收到应答后清零
	u32 warning_CrossPath		:	1;	///21 1：进出路线 收到应答后清零
	u32 warning_DriveTime_NotEnough	:	1;	///22 1：路段行驶时间不足/过长 收到应答后清零
	u32 warning_Path_Deviate		:	1;	///23 1：路线偏离报警 标志维持至报警条件解除
	u32 warning_Vechicle_VSS_Fault		:	1;	///24 1：车辆VSS 故障 标志维持至报警条件解除
	u32 warning_Vechicle_Oil_Fault		:	1;	///25 1：车辆油量异常 标志维持至报警条件解除
	u32 warning_Vechicle_Stolen		:	1;	///26 1：车辆被盗(通过车辆防盗器) 标志维持至报警条件解除
	u32 warning_Vechicle_Illegical_Launch	:	1;	///27 1：车辆非法点火 收到应答后清零
	u32 warning_Vechicle_Illegical_Moving	:	1;	///28 1：车辆非法位移 收到应答后清零
	u32 warning_Vechicle_PreCollision	:	1;	///29 1：碰撞预警 标志维持至报警条件解除
#ifdef JTT808_Ver_2011
	u32 warning_reserved_30_31	: 	2;	///30-31保留
#endif
#ifdef JTT808_Ver_2013
	u32 warning_Vechicle_PreSidestroke		:	1;	///30 1：侧翻预警 标志维持至报警条件解除
	u32 warning_Vechicle_Illegical_DoorOpen		:	1;	///31 1：非法开门报警（终端未设置区域时，不判断非法开门）收到应答后清零
#endif///JTT808_Ver_2013
} tWARNINGSTATE;

/**状态位定义*/
typedef struct
{
	/**状态位*/
	u32	state_ACC			:	1;	///0:ACC关，1:ACC开
	u32	state_Location		:	1;	///0:未定位，1:定位
	u32	state_Lat			:	1;	///0:北纬，1:南纬
	u32	state_Long			:	1;	///0:东经，1:西经
	u32	state_OnService		:	1;	///0:运营状态，1:停运状态
	u32	state_Encrypt		:	1;	///0:经纬度未经保密插件加密，1:经纬度已经保密插件加密
#ifdef JTT808_Ver_2011
	u32 state_reserved_6_9	:	4; 	///保留
#endif
#ifdef JTT808_Ver_2013
	u32 state_reserved_6_7	:	2;	///保留
	u32 state_Loading		:	2;	///00：空车；01：半载；10：保留；11：满载
		///	（可用于客车的空、重车及货车的空载、满载状态表示，人工输入或传感器获取）
#endif///JTT808_Ver_2013

	u32 state_Oil			:	1;	///10 0：车辆油路正常；1：车辆油路断开
	u32 state_ElectricCircuit	:	1;	///11 0：车辆电路正常；1：车辆电路断开
	u32 state_DoorLock			:	1;	///12 0：车门解锁；1：车门加锁
#ifdef JTT808_Ver_2011
	u32 state_Reserved_13_31	:	19;	/// 保留
#endif

#ifdef JTT808_Ver_2013
	u32 state_Door_1			:	1;	///13 0：门1 关；1：门1 开（前门）
	u32 state_Door_2			:	1;	///14 0：门2 关；1：门2 开（中门）
	u32 state_Door_3			:	1;	///15 0：门3 关；1：门3 开（后门）
	u32 state_Door_4			:	1;	///16 0：门4 关；1：门4 开（驾驶席门）
	u32 state_Door_5			:	1;	///17 0：门5 关；1：门5 开（自定义）
	u32 state_GPS_Used			:	1;	///18 0：未使用GPS 卫星进行定位；1：使用GPS 卫星进行定位
	u32 state_Beidou_Used		:	1;	///19 0：未使用北斗卫星进行定位；1：使用北斗卫星进行定位
	u32 state_GLONASS_Used		:	1;	///20 0：未使用GLONASS 卫星进行定位；1：使用GLONASS 卫星进行定位
	u32 state_Galileo_Used		:	1;	///21 0：未使用Galileo 卫星进行定位；1：使用Galileo 卫星进行定位
	u32 state_Reserved_22_31	:	10;	/// 保留
#endif///JTT808_Ver_2013

} tSYSTEMSTATE;

/**位置基本信息数据格式*/
typedef struct
{
	tWARNINGSTATE 	tWarningState;	/**u32 报警标志位定义*/
	tSYSTEMSTATE 	tSysState;	/**状态位*/

	u32 u32Lat;		///8 纬度 DWORD 以度为单位的纬度值乘以10 的6 次方，精确到百万分之一度
	u32 u32Long;	///12 经度 DWORD 以度为单位的经度值乘以10 的6 次方，精确到百万分之一度
	u16 u16Height;	///16 高程 WORD 海拔高度，单位为米（m）
	u16 u16Speed;	///18 速度 WORD 1/10km/h
	u16 u16Direct;	///20 方向 WORD 0-359，正北为0，顺时针
	u8	time[6];	///21 时间 BCD[6] YY-MM-DD-hh-mm-ss（GMT+8 时间，本标准中之后涉及的时间均采用此时区）
} tMsg_Position_BaseInfo;

///超速或区域/路线报警时的类型定义
typedef	enum
{
	eNORMAL	 		 	= 0,	///无特定位置
	eROUND_AREA 		= 1,	///圆形区域
	eRECT_AREA			= 2,	///矩形区域
	eMULTIMEDIA_AREA 	= 3,	///多边形区域
	eLINE 				= 4		///路段
}ePOSType;

///无超速报警附加信息消息体数据格式
typedef 	struct
{
	ePOSType   	ePosType;				///位置类型
}tAppendInfo_NonOverspeedWarning;

///超速报警附加信息消息体数据格式
typedef 	struct
{
	ePOSType   	ePosType;				///位置类型
	u32			u32AreaOrLineId;		///区域或者路段ID
}tAppendInfo_OverspeedWarning;

///进出区域方向定义
#define		DIR_ENTER_AREA				0
#define		DIR_LEAVE_AREA				1

///进出区域/路线报警附加信息消息体数据格式
typedef		struct
{
	ePOSType   	ePosType;				///位置类型
	u32			u32AreaOrLineId;		///区域或路线ID
	u8			u8EnterOrLeave;			///方向
}tAppendInfo_BarrierAndPathWarning;

///路段行驶时间不足/过长报警附加信息消息体数据格式
typedef		struct
{
	u32			u32LineId;				///路线ID
	u16			u16DriveTime;			///行驶时间，单位为秒
	u8			u8Result;				///结果，0:不足，1:过长
}tAppendInfo_DrivingTimeInadequetWarning;

/**扩展车辆信号状态位*/
#define VECHICLE_EXTSIG_CloseLight	((u32)0x00000001)	///0 1：近光灯信号
#define VECHICLE_EXTSIG_FarLight	(((u32)0x00000001) << 1)	///1 1：远光灯信号
#define VECHICLE_EXTSIG_RightLight	(((u32)0x00000001) << 2)	///2 1：右转向灯信号
#define VECHICLE_EXTSIG_LeftLight	(((u32)0x00000001) << 3)	///3 1：左转向灯信号
#define VECHICLE_EXTSIG_StopLight	(((u32)0x00000001) << 4)	///4 1：制动信号
#define VECHICLE_EXTSIG_ReverseGearLight	(((u32)0x00000001) << 5)	///5 1：倒档信号
#define VECHICLE_EXTSIG_FogLight	(((u32)0x00000001) << 6)	///6 1：雾灯信号
#define VECHICLE_EXTSIG_WideLight	(((u32)0x00000001) << 7)	///7 1：示廓灯
#define VECHICLE_EXTSIG_SpeakerLight	(((u32)0x00000001) << 8)	///8 1：喇叭信号
#define VECHICLE_EXTSIG_AirConditionLight	(((u32)0x00000001) << 9)	///9 1：空调状态
#define VECHICLE_EXTSIG_NullGearLight	(((u32)0x00000001) << 10)	///10 1：空挡信号
#define VECHICLE_EXTSIG_RetardWorking	(((u32)0x00000001) << 11)	///11 1：缓速器工作
#define VECHICLE_EXTSIG_ABSWorking	(((u32)0x00000001) << 12)	///12 1：ABS 工作
#define VECHICLE_EXTSIG_HeatWorking	(((u32)0x00000001) << 13)	///13 1：加热器工作
#define VECHICLE_EXTSIG_ClutchState	(((u32)0x00000001) << 14)	///14 1：离合器状态
/**15-31 保留*/

/**IO 状态位*/
#define VECHICLE_IOSTATE_DeepSleep	((u16)0x00000001)	///0 1：深度休眠状态
#define VECHICLE_IOSTATE_Sleep		(((u16)0x00000001) << 1)	///1 1：休眠状态
/**2-15 保留*/

/**位置附加信息项-未超速*/
typedef struct
{
	u8	msgId;	///附加信息ID BYTE 1-255
	u8	msgLen;	///附加信息长度 BYTE
	u8  tag_1;
	u32	kilemeters;	///0x01 4 里程，DWORD，1/10km，对应车上里程表读数
	u8  tag_2;
	u16 oil; 	///0x02 2 油量，WORD，1/10L，对应车上油量表读数
	u8  tag_3;
	u16 speed;	///0x03 2 行驶记录功能获取的速度，WORD，1/10km/h
	u8  tag_4;
	u16 warningEvtId; 	///0x04 2 需要人工确认报警事件的ID，WORD，从1 开始计数
	/**0x05-0x10 保留*/
	u8  tag_11;
	tAppendInfo_NonOverspeedWarning tNonOverspeedWarn; ///1 或5 超速报警附加信息见表28
	u8  tag_12;
	tAppendInfo_BarrierAndPathWarning tBarrierWarn;///6 进出区域/路线报警附加信息见表29
	u8  tag_13;
	tAppendInfo_DrivingTimeInadequetWarning tDrvTimeWarn; ///0x13 7 路段行驶时间不足/过长报警附加信息见表30
	/**0x14-0x24 保留*/
	u8  tag_25;
	u32 vechicalExtraSignal;	///0x25 4 扩展车辆信号状态位，定义见表31
	u8  tag_2A;
	u16 IOState;	///0x2A 2 IO状态位，定义见表32
	u8  tag_2B;
	u32 ADValue;	///0x2B 4 模拟量，bit0-15，AD0；bit16-31，AD1。
	u8  tag_30;
	u8 	csq;	///0x30 1 BYTE，无线通信网络信号强度
	u8  tag_31;
	u8	GNSS_Satellites; ///0x31 1 BYTE，GNSS 定位卫星数
	u8  tag_E0;	///0xE0 后续信息长度 后续自定义信息长度
	u8	extraInfoLen;
	/**0xE1-0xFF 自定义区域*/
} tMsg_Position_AppendInfo_NonOverSpeed;

/**位置附加信息项-超速*/
typedef struct
{
	u8	msgId;	///附加信息ID BYTE 1-255
	u8	msgLen;	///附加信息长度 BYTE
	u8  tag_1;
	u32	kilemeters;	///0x01 4 里程，DWORD，1/10km，对应车上里程表读数
	u8  tag_2;
	u16 oil; 	///0x02 2 油量，WORD，1/10L，对应车上油量表读数
	u8  tag_3;
	u16 speed;	///0x03 2 行驶记录功能获取的速度，WORD，1/10km/h
	u8  tag_4;
	u16 warningEvtId; 	///0x04 2 需要人工确认报警事件的ID，WORD，从1 开始计数
	/**0x05-0x10 保留*/
	u8  tag_11;
	tAppendInfo_NonOverspeedWarning tNonOverspeedWarn; ///1 或5 超速报警附加信息见表28
	u8  tag_12;
	tAppendInfo_BarrierAndPathWarning tBarrierWarn;///6 进出区域/路线报警附加信息见表29
	u8  tag_13;
	tAppendInfo_DrivingTimeInadequetWarning tDrvTimeWarn; ///0x13 7 路段行驶时间不足/过长报警附加信息见表30
	/**0x14-0x24 保留*/
	u8  tag_25;
	u32 vechicalExtraSignal;	///0x25 4 扩展车辆信号状态位，定义见表31
	u8  tag_2A;
	u16 IOState;	///0x2A 2 IO状态位，定义见表32
	u8  tag_2B;
	u32 ADValue;	///0x2B 4 模拟量，bit0-15，AD0；bit16-31，AD1。
	u8  tag_30;
	u8 	csq;	///0x30 1 BYTE，无线通信网络信号强度
	u8  tag_31;
	u8	GNSS_Satellites; ///0x31 1 BYTE，GNSS 定位卫星数
	u8  tag_E0;	///0xE0 后续信息长度 后续自定义信息长度
	u8	extraInfoLen;
	/**0xE1-0xFF 自定义区域*/
} tMsg_Position_AppendInfo_OverSpeed;

typedef union
{
	tMsg_Position_AppendInfo_NonOverSpeed	tAppendInfo_NonOverSpeed;
	tMsg_Position_AppendInfo_OverSpeed 	tAppendInfo_OverSpeed;
} U_AppendInfo;

/** 8.18 位置信息汇报,消息ID：0x0200。****************************************/
typedef struct
{
	tMsg_Position_BaseInfo 	tBaseInfo;		///基本信息
	U_AppendInfo			tAppendInfo;	///附加信息
} tMsg_T_PositionInfo;


/**8.19 位置信息查询 消息ID：0x8201。******************************************/
/**位置信息查询消息体为空。*/

/**8.20 位置信息查询应答 消息ID：0x0201。**************************************/
/**位置信息查询应答消息体数据格式见表33。
表33 位置信息查询应答消息体数据格式
起始字节 字段 数据类型 描述及要求
0 应答流水号 WORD 对应的位置信息查询消息的流水号
2 位置信息汇报 位置信息汇报见8.12
！！！！！ 注：新协议中也是【8.12】，检查旧协议，8.12是位置汇报，新协议中应该是笔误！！！！
*/
typedef struct
{
	u16	u16SeqNum;	///应答流水号
	tMsg_T_PositionInfo tInfo;	///位置信息
} tMsg_TACK_QueryPositionInfoAck;

/** 8.21 临时位置跟踪控制,消息ID：0x8202。*************************************/
typedef struct
{
	u16	u16TimeInterval;///时间间隔,单位为秒（s），0 则停止跟踪。停止跟踪无需带后继字段
	/**终端在接收到位置跟踪控制消息后，在有效 期截止时间之前，依据消息中的时间间隔发送位置汇报*/
	u32 u32TrackingTimeout;	///位置跟踪有效期 	单位为秒（s）
} tMsg_SCMD_TemproryTrack;

#ifdef JTT808_Ver_2013
/**8.22 人工确认报警消息 消息ID：0x8203****************************************/

typedef struct
{
	u32	bit_Warning_ConfirmEmergency			:	1;	///0 1：确认紧急报警；
	u32	bit_Warning_Reserved_1_2				:	2;	///1-2 保留
	u32	bit_Warning_ConfirmDanger				:	1;	///3 1：确认危险预警；
	u32	bit_Warning_Reserved_4_19				:	15;	///4-19 保留
	u32	bit_Warning_Confirm_CrossArea			:	1;	///20 1：确认进出区域报警；
	u32	bit_Warning_Confirm_CrossPath			:	1;	///21 1：确认进出路线报警；
	u32	bit_Warning_ConfirmDrvTimeInadequet		:	1;	///22 1：确认路段行驶时间不足/过长报警；
	u32	bit_Warning_Reserved_23_26				:	4;	///23-26 保留
	u32	bit_Warning_ConfirmIllegicalLaunch		:	1;	///27 1：确认车辆非法点火报警；
	u32	bit_Warning_ConfirmIllegicalMoving		:	1;	///28 1：确认车辆非法位移报警；
	u32	bit_Warning_Reserved_29_31				:	3;	///29-31 保留
} tManualVerifyWarnType;

/**人工确认报警消息体数据格式见表35。*/
typedef struct
{
	u16	u16SeqNum;	///报警消息流水号
	u32 u32WarnType;///报警类型
} tMsg_SCMD_MannalVerifyWarning;

#endif///JTT808_Ver_2013

/**8.23 文本信息下发 消息ID：0x8300。******************************************/
typedef struct
{
	u8	flag_Emergency		:	1;	/// 0 1：紧急
	u8  flag_Reserved_1		:	1;	///1 保留
	u8  flag_Display		:	1;	///2 1：终端显示器显示
	u8  flag_TTSRead		:	1;	///3 1：终端TTS 播读
	u8  flag_Advertise		:	1;	///4 1：广告屏显示
#ifdef JTT808_Ver_2013
	u8  flag_MsgInfo		:	1;	///5 0：中心导航信息，1：CAN 故障码信息
	u8  flag_Reserved_6_7	:	2;	///6-7 保留
#endif///JTT808_Ver_2013
#ifdef JTT808_Ver_2011
	u8  flag_Reserved_5_7	:	3;	///5-7 保留
#endif
	u8	msg[0];	///文本信息 STRING 最长为1024 字节，经GBK 编码
} tMsg_SCMD_DispatchTxtInfo;

///终端事件设置类型
typedef	enum
{
	eDEL_ALL_EVENT 		= 0,	///删除终端现有所有事件，该命令不带有后续字节
	eUPDATE_EVENT 		= 1,	///更新事件
	eAPPEND_EVENT 		= 2,	///追加事件
	eCHANGE_EVENT 		= 3,	///修改事件
	eDEL_SPECIFIC_EVENT = 4,	///删除特定几项事件，之后事件项中无需带实践内容
}eEVENTSETType;

///事件项定义
typedef	struct
{
	u8	u8EventId;			///事件ID,若终端已有同ID的事件，则被覆盖
	u8	u8EventLength;		///事件长度
	u8 *sEventMsg;			///事件内容

}tEventItem;

/** 8.24 事件设置,消息ID：0x8301。********************************************/
typedef	struct
{
	eEVENTSETType	eEventSetType;	///设置事件类型
	u8		u8SetCount;				///设置总数
	tEventItem*	ptEventItem;		///事件项内容指针
} tMsg_SCMD_SetEvent;

/**8.25 事件报告 消息ID：0x0301。**********************************************/
typedef struct
{
	u8	u8EventId;	///事件ID
} tMsg_T_EventRptInfo;

/**8.26 提问下发 *************************************************************/
///文本信息含义定义
#define	QUESTION_TYPE_ERGENT			0X01	///紧急
#define	QUESTION_TYPE_TTS_READ			0X08	///终端TTS播读
#define	QUESTION_TYPE_DISPLAY			0X10	///广告屏显示

///提问下发候选答案消息体数据格式
typedef	struct
{
	u8	u8AnswerId;			///答案ID
	u16	u16AnswerLength;	///答案长度
	u8*	pu8AnswerContent;	///答案内容
}tQUESTIONANSWERITEM;

/** 8.27 提问应答,消息ID：0x0302。********************************************/
typedef	struct
{
	u8	u8QuestionTag;		///提问下发标志位定义
	u8	u8QuestionContentLength;	///提问字段字节长度
	u8*	pu8QuestionContent;			///问题文本，经GBK编码，长度为N
	tQUESTIONANSWERITEM	*ptQuestionAnswer;///提问候选答案指针
} tMsg_SCMD_AskQuestion;

///提问应答结构体定义
typedef	struct
{
	u16	u16SeqNum;	///应答的流水号，对应的提问下发消息的流水号
	u8	u8AnswerId;		///答案ID,提问下发中附带的答案ID
} tMsg_TACK_AnswerQuestion;

/** 8.28 信息点播菜单设置  ID：0x8303 *****************************************/
///信息点播菜单设置
///信息点播菜单设置类型
#define	MSGOD_MENU_DELETE	0	///删除终端现有信息项
#define	MSGOD_MENU_UPDATE	1	///更新菜单
#define	MSGOD_MENU_APPEND	2	///追加菜单
#define	MSGOD_MENU_CHANGE	3	///修改菜单

///信息点播信息项定义
typedef	struct
{
	u8	u8InfoType;				///信息类型
	u16	u16InfoNameLength;		///信息名称长度
	u8	aInfoName[0];			///信息名称
} tMSGODMENUITEM;

typedef	struct
{
	u8	u8InfoOrderSetType;		///信息点播菜单设置类型
	u8	u8SetCount;				///设置总数
	tMSGODMENUITEM	tInfoOrderItem[0];	///信息点播菜单
} T_SCMD_InfoOrder;

/** 8.29 信息点播/取消 ID：0x0303 *********************************************/
typedef	struct
{
	u8	u8InfoOrderSetType;		///信息类型
	u8	u8OrderOrCancel;		///点播/取消标志，0:取消；1:点播
}T_SCMD_InfoOrderCtrl;

/** 8.30 信息服务 消息ID：0x8304 **********************************************/
typedef	struct
{
	u8	u8InfoType;				///信息类型
	u16	u16InfoTotalLength;		///信息总长度
	u8	aInfo[0];			///信息内容，经GBK编码
} T_SCMD_InfoService;

/** 8.31 电话回拨 消息ID：0x8400 **********************************************/
typedef	struct
{
	u8	u8Flag;			///标志，0:普通通话;1:监听
	u8	aPhoneNum[0];		///电话号码。最长20字节
} T_SCMD_ReverseDial;

/** 8.32 设置电话本 消息ID：0x8401 ********************************************/
///电话本设置类型
typedef	enum
{
	ePHONEBOOK_DELETE	= 0,	///删除终端上所有存储的联系人
	ePHONEBOOK_UPDATE 	= 1,	///更新电话本(删除终端中已有的全部联系人并追加消息中的联系人)
	ePHONEBOOK_APPEND	= 2,	///追加电话本
	ePHONEBOOK_CHANGE	= 3		///修改电话本(以联系人为索引)
}ePHONESETTYPE;

///电话本联系人项定义
typedef	struct
{
	u8	u8PhoneNumTag;		///标志，1:呼入,2:呼出,3:呼入/呼出
	u8	u8PhoneNumLength;	///号码长度
	u8	aPhoneNum[0];		///电话号码
	u8	u8NameLength;		///联系人长度
	u8	aName[0];			///联系人
}tPHONEBOOKITEM;

///设置电话本
typedef	struct
{
	u8	u8SetType;						///设置类型
	u8	u8ConnPersonCount;				///联系人总数
	tPHONEBOOKITEM	tPhoneBookItem[0];	///联系人项
} T_SCMD_SetPhoneBook;

/*************8.33 车辆控制 消息ID：0x8500*************/
#define	VEHICLE_CTRL_UNLOCK		0	///车门解锁
#define	VEHICLE_CTRL_LOCK		1	///车门加锁
///控制命令消息体
typedef struct
{
	u8	bit_DoorLock			:	1;	/// 0：车门解锁；1：车门加锁
	u8	bit_vechical_reserved	:	7;
} tMsg_SCMD_VechicalCtrl;

/*************8.34 车辆控制应答 消息ID：0x0500。*************/
///车辆控制应答结构体定义
typedef	struct
{
	u16	u16SeqNum;		///应答的流水号，对应的提问下发消息的流水号
	tMsg_T_PositionInfo	tBasicPos;		///位置信息汇报
} tMsg_TACK_VechicalCtrl;

/*************8.35 设置圆形区域 消息ID：0x8600。*************/
///设置区域属性
typedef	enum
{
	eAREA_UPDATE	= 0,	///更新区域
	eAREA_APPEND 	= 1,	///追加区域
	eAREA_CHANGE	= 2		///修改区域
}eAREATYPE;

/**区域的区域属性定义*/
typedef struct
{
	u16	Property_BaseOnTime					:	1;	///0 1：根据时间
	u16	Property_LimitSpeed					:	1;	///1 1：限速
	u16	Property_Alarm2DriverOnEnterErea	:	1;	///2 1：进区域报警给驾驶员
	u16	Property_Alarm2ServerOnEnterErea	:	1;	///3 1：进区域报警给平台
	u16	Property_Alarm2DriverOnLeftErea		:	1;	///4 1：出区域报警给驾驶员
	u16	Property_Alarm2ServerOnLeftErea		:	1;	///5 1：出区域报警给平台
	u16	Property_LatFlag					:	1;	///6 0：北纬；1：南纬
	u16	Property_LongFlag					:	1;	///7 0：东经；1：西经
#ifdef JTT808_Ver_2011
	u16	Property_rerserved_8_15				:	8;	///9-13 保留
#endif
#ifdef JTT808_Ver_2013
	u16	Property_DoorOpenEnable				:	1;	///8 0：允许开门；1：禁止开门
	u16	Property_Rerserved_9_13				:	5;	///9-13 保留
	u16	Property_CommModuleEnableOnEnterArea	:	1;	///14 0：进区域开启通信模块；1：进区域关闭通信模块
	u16	Property_GNSSDetailMsgEnableOnEnterArea	:	1;	///15 0：进区域不采集GNSS 详细定位数据；1：进区域采集GNSS 详细定位数据
#endif///JTT808_Ver_2013
} tAreaProperty;

/**圆形区域项结构体定义*/
typedef	struct
{
	u32	u32AreaId;		///区域ID
	tAreaProperty	tAreaProperty;	///区域属性
	u32	u32CenterPtLatitude;	///中心点纬度
	u32	u32CenterPtLongitude;	///中心点经度
	u32	u32Radius;		///半径，单位为m;路段为该拐点到下一拐点
	tBCDTime tBcdStartTime;	///起始时间，若区域属性0位为0则没有该字段
	tBCDTime tBcdEndTime;	///结束时间，若区域属性0位为0则没有该字段
	u16	u16SpeedMax;	///最高速度，km/h，若区域属性1位为0则没有该字段
	u8	u8OverSpeedContinuousTime;	///超速持续时间，单位为秒，若区域属性1位为0则没有该字段
} tItem_RoundArea;

///设置圆形区域消息体数据格式
typedef	struct
{
	u8	u8AreaProperty;		///圆形区域属性,eAREATYPE
	u8	u8AreaAmount;	///区域总数
///	u8	u8PackageAreaCount;		///包区域个数
	tItem_RoundArea	atItems[0];	///区域项
} tMsg_SCMD_SetRoundArea;


/** 8.36 删除圆形区域 消息ID：0x8601。*****************************************/
typedef	struct
{
	/**区域个数，本条消息中包含的区域个数，不超过125个，多于125个建议用多条消息，0为删除所有区域*/
	u8	u8AreaCount;	/**待删除区域总数*/
	u32 au32AreaIdList[0];	///区域ID1,...
} tMsg_SCMD_DeleteRoundArea;

/** 8.37 设置矩形区域 消息ID：0x8602。*****************************************/
/**矩形区域项结构体定义*/
typedef	struct
{
	u32	u32AreaId;			///区域ID
	u16	u16AreaProperty;	///区域属性

	u32	u32LT_Lat;	///左上点纬度 	以度为单位的纬度值乘以10 的6 次方，精确到百万分之一度
	u32	u32LT_Long;	/// 左上点经度 DWORD	以度为单位的纬度值乘以10 的6 次方，精确到百万分之一度
	u32	u32RB_Lat;	/// 14 右下点纬度 DWORD以度为单位的纬度值乘以10 的6 次方，精确到百万分之一度
	u32	u32RB_Long;	/// 18 右下点经度 DWORD以度为单位的纬度值乘以10 的6 次方，精确到百万分之一度

	tBCDTime tBcdStartTime;	///起始时间，若区域属性0位为0则没有该字段
	tBCDTime tBcdEndTime;	///结束时间，若区域属性0位为0则没有该字段
	u16	u16SpeedMax;		///最高速度，km/h，若区域属性1位为0则没有该字段
	u8	u8KeepingOverSpeedTimeout;///超速持续时间，单位为秒，若区域属性1位为0则没有该字段
} tItem_RectangleArea;

///设置矩形区域消息体数据格式
typedef	struct
{
	u8	u8AreaProperty;			///设置矩形区域属性
	u8	u8AreaTotalCount;		///区域总数
	u8	u8PackageAreaCount;		///包区域个数
	tItem_RectangleArea	atItems[0];	///区域项
} tMsg_SCMD_SetRectangleArea;

/** 8.38 删除矩形区域 消息ID：0x8603。*****************************************/
///删除矩形区域消息体数据格式
typedef	struct
{
	u8	u8Amount;	///区域个数，本条消息中包含的区域个数，不超过125个，
	u32	au32Ids[0];		///区域ID1,...,区域IDn
} tMsg_SCMD_DeleteRectangleArea;

/** 8.39 设置多边形区域 消息ID：0x8604。***************************************/
///顶点
typedef	struct
{
	u32	u32Lat;	///纬度
	u32	u32Long;///经度
} tPOINT;

///#define	MAX_REGION_VERTEX_NUMBER	30

///服务器下发的设置多边形区域消息体
typedef struct
{
	u32	u32AreaId;	///区域ID
	tAreaProperty tProperty;	///区域属性
	tBCDTime tBcdStartTime;		///起始时间，若区域属性0位为0则没有该字段
	tBCDTime tBcdEndTime;	///结束时间，若区域属性0位为0则没有该字段
	u16	u16SpeedMax;	///最高速度，km/h，若区域属性1位为0则没有该字段
	u8	u8OverSpeedContinuousTime;///超速持续时间，单位为秒，若区域属性1位为0则没有该字段
	u16	u16AreaTotalVertexCount;	///区域总顶点数
} tItem_PolygonArea;

typedef struct
{
	tItem_PolygonArea tHead;
	tPOINT	atPoints[0];	///顶点项
} tMsg_SCMD_SetPolygonArea;

/** 8.40 删除多边形区域 消息ID：0x8604。***************************************/
///删除多边形区域消息体数据格式
typedef	struct
{
	u8	u8Amount;  /**区域个数，本条消息中包含的区域个数，不超过125个，多于125个
												建议用多条消息，0为删除所有区域*/
	u32 au32Ids[0];	///区域ID1
} tMsg_SCMD_DeletePolygonArea;

/** 8.41 设置路线 消息ID：0x8606。*********************************************/
///路线属性定义
#define	ALARM_TIME					0x0001	///根据时间
#define	ALARM_2DRIVER_IN_LINE		0x0004	///进路线报警给驾驶员
#define	ALARM_2PLATFORM_IN_LINE		0x0008	///进路线报警给平台
#define	ALARM_2DRIVER_OUT_LINE		0x0010	///出路线报警给驾驶员
#define	ALARM_2PLATFORM_OUT_LINE	0x0020	///出路线报警给平台

///路段属性定义
#define	ROADSECTION_DIRVE_TIME			0x01	///行驶时间
#define	ROADSECTION_RESTRICT_SPEED		0x02	///限速
#define	ROADSECTION_NORTH_LATITUDE		0x04	///0:北纬,1:南纬
#define	ROADSECTION_EAST_LONGITUDE		0x08	///0:东经,1:西经

///#define	MAX_INFLECTION_PT_NUMBER	20

///拐点项定义
typedef	struct
{
	u32 u32CornerId;	///拐点ID
	u32	u32RoadSectionId;	///路段ID
	u32	u32Latitude;		///拐点纬度
	u32	u32Longitude;		///拐点经度
    u8	u8RoadSectionWidth;		///路段宽度，单位为米，路段为该拐点到下一拐点
	u8	u8RoadSectionProperty;		///路段属性
	u16	u16TimeOverThreshold;	///路段行驶过长阈值，单位为秒，若路段属性0位为0则没有该字段
	u16	u16TimeNotEnoughThreshold;	///路段行驶不足阈值，单位为秒，若路段属性0位为0则没有该字段
	u16	u16SpeedMax;		///路段最高速度，km/h，若路段属性1位为0则没有该字段
	u8	u8OverSpeedKeptTime;	///路段超速持续时间，单位为秒，若路段属性1位为0则没有该字段
}tCORNERPOINT;

/** 8.42 删除路线 消息ID：0x8607。*********************************************/
///删除路线消息体数据格式
typedef	struct
{
	u8	u8LineCount;	    /**路线个数，本条消息中包含的区域个数，不超过125个，多于
											125个建议用多条消息，0为删除所有区域*/
	u32	u32LineId[0];	///路线ID1
} tMsg_SCMD_DeleteLine;

/** 8.43 行驶记录数据采集命令 消息ID：0x8700。**********************************/
typedef	struct
{
	u8	u8Cmd;	///命令字
	u8*	pu8Data;	/**数据块 数据块内容格式见GB/T 19056 中相关内容，包含
										GB/T 19056 要求的完整数据包，可为空。*/
} tMsg_SCMD_Recorder_CaptureData;


/** 8.44 行驶记录数据上传 消息ID：0x0700。*************************************/
typedef	struct
{
	u16	u16SeqNum;	///应答流水号 WORD 对应的行驶记录数据采集命令消息的流水号
	u8	u8Cmd;	///命令字
	u8*	pu8Data;	/**数据块 数据块内容格式见GB/T 19056 中相关内容，包含
										GB/T 19056 要求的完整数据包*/
} tMsg_TACK_Recorder_CaptureData;

/** 8.45 行驶记录参数下传命令 消息ID：0x8701。**********************************/
typedef	struct
{
	u8	u8Cmd;	///命令字
	u8*	pu8Data;	/**数据块 数据块内容格式见GB/T 19056 中相关内容，包含
										GB/T 19056 要求的完整数据包，可为空。*/
} tMsg_SCMD_Recorder_DispatchParams;

/** 8.46 电子运单上报 消息ID：0x0701。*****************************************/
typedef	struct
{
	u32	u32ETicketLen;	///电子运单长度
	u8*	pu8Data;	///电子运单数据包
}tMsg_T_ETicketRpt;

/** 8.47 上报驾驶员身份信息请求 消息ID：0x8702。********************************/  ///----------????????
///上报驾驶员身份信息请求消息体为空。


/** 8.48 驾驶员身份信息采集上报 消息ID：0x0702。********************************/
#ifdef JTT808_Ver_2011
///驾驶员身份信息采集上报
typedef	struct
{
	u8	u8DriverNameLen;	///驾驶员姓名长度
	u8*	pau8DriverName;		///驾驶员姓名，长度n
	u8	au8DrvIdCode[20];	///驾驶员身份证编码
	u8	au8QulCode[40];		///从业资格证编码
	u8	u8GovNameLength;	///发证机构名称长度
	u8*	pau8GovName;		///发证机构名称
} tMsg_T_DriverInfoRpt;
#endif

#ifdef JTT808_Ver_2013
typedef	struct
{
	u8 state;	/** 状态 BYTE 0x01：从业资格证IC 卡插入（驾驶员上班）；
							0x02：从业资格证IC 卡拔出（驾驶员下班）。*/
	u8 time_BCD[6];	///1 时间 BCD[6] 插卡/拔卡时间，YY-MM-DD-hh-mm-ss；
/**以下字段在状态为0x01 时才有效并做填充。*/
	u8 IC_read_ret;	///IC 卡读取结果 BYTE
///						0x00：IC 卡读卡成功；
///						0x01：读卡失败，原因为卡片密钥认证未通过；
///						0x02：读卡失败，原因为卡片已被锁定；
///						0x03：读卡失败，原因为卡片被拔出；
///						0x04：读卡失败，原因为数据校验错误。

/**以下字段在IC 卡读取结果等于0x00 时才有效。*/
	u8	u8DriverNameLen;	///驾驶员姓名长度
	u8*	pau8DriverName;		///驾驶员姓名，长度n
	u8	au8QulCode[20];		///从业资格证编码,不足补0
	u8	u8GovNameLength;	///发证机构名称长度
	u8*	pau8GovName;		///发证机构名称
	u8	LicieceValidation[4];	///证件有效期 BCD[4] YYYYMMDD
} tMsg_T_DriverInfoRpt;
#endif///JTT808_Ver_2013

/** 8.49 定位数据批量上传 消息ID：0x0704。*************************************/
typedef struct
{
	u16	u16ItemAmount;	///0 数据项个数 WORD 包含的位置汇报数据项个数，>0
	u8	u8Type;		///1 位置数据类型 BYTE 0：正常位置批量汇报，1：盲区补报
	u16 u16ItemBlockLen;	/// 位置汇报数据体长度 WORD 位置汇报数据体长度，n
	tMsg_T_PositionInfo tPositionBlock;	///t2 位置汇报数据体 BYTE[n] 定义见8.12 位置信息汇报
} tMsg_T_PositionInfoBatch;

#ifdef JTT808_Ver_2013
/** 8.50 CAN 总线数据上传 消息ID：0x0705。*************************************/
///CAN 总线数据项数据格式
typedef struct
{
	/**bit31 表示CAN 通道号，0：CAN1，1：CAN2；
	bit30 表示帧类型，0：标准帧，1：扩展帧；
	bit29 表示数据采集方式，0：原始数据，1：采
	集区间的平均值；
	bit28-bit0 表示CAN 总线ID。
	*/
	u8 CAN_ID[4];	///CAN ID
	u8 CAN_DATA[8];	///CAN 数据
} tCANItem;

///CAN 总线数据上传
typedef struct
{
	u16 u16ItemAmount;	///0 数据项个数 WORD 包含的CAN 总线数据项个数，>0
	u8	u8RevTime[5];	///2 CAN 总线数据接收时间 BCD[5] 第1 条CAN 总线数据的接收时间，hh-mm-ss-msms
	tCANItem tItem[0];	///CAN数据项
} tMsg_T_CANInfoUpload;
#endif///JTT808_Ver_2013


/** 8.51 多媒体事件信息上传 消息ID：0x0800。***********************************/
///多媒体类型
#define	MEDIA_TYPE_PHOTO		0	///图像
#define	MEDIA_TYPE_AUDIO		1	///音频
#define	MEDIA_TYPE_VIDEO		2	///视频

///多媒体格式编码
#define	MEDIA_STRUCT_JPEG		0	///JPEG
#define	MEDIA_STRUCT_TIF		1	///TIF
#define	MEDIA_STRUCT_MP3		2	///MP3
#define	MEDIA_STRUCT_WAV		3	///WAV
#define	MEDIA_STRUCT_WMV		4	///WMV

///事件项编码
#define	MEDIA_EVT_PLATFORM			0	///平台下发指令
#define	MEDIA_EVT_TIME				1	///定时动作
#define	MEDIA_EVT_ROBBER			2	///抢劫报警出发
#define	MEDIA_EVT_CRUSH				3	///碰撞侧翻报警触发
#ifdef JTT808_Ver_2013
#define	MEDIA_EVT_DOOROPEN_PIC		4	///门开拍照；
#define	MEDIA_EVT_DOOCLOSE_PIC		5	///门关拍照；
#define	MEDIA_EVT_DOOROPEN2CLOSE 	6	///车门由开变关，时速从＜20 公里到超过20 公里；
#define	MEDIA_EVT_DIST_PIC			7	///定距拍照；其他保留
#endif///JTT808_Ver_2013

typedef	struct
{
	u32	u32MediaDataId;		///多媒体数据ID
	u8	u8MediaType;			///多媒体类型
	u8	u8MediaFormatCode;		///多媒体格式编码
	u8	u8EventCode;		///事件项编码
	u8	u8ChannelId;		///通道ID
} tMsg_T_Media_EvtUpload;

/** 8.52 多媒体数据上传 消息ID：0x0801。***************************************/
typedef	struct
{
	u32	u32MediaDataId;	///多媒体数据ID
	u8	u8MediaType;		///多媒体类型
	u8	u8MediaFormatCode;		///多媒体格式编码
	u8	u8EventCode;	///事件项编码
	u8	u8ChannelId;	///通道ID
#ifdef JTT808_Ver_2013
	tMsg_Position_BaseInfo tPositionBaseInfo;	///基本位置信息
#endif///JTT808_Ver_2013
	u8	Data[0];	///多媒体数据包
} tMsg_T_Media_DataUpload;

/** 8.53 多媒体数据上传应答 消息ID：0x8800。***********************************/
#ifdef JTT808_Ver_2011
///多媒体数据上传应答
typedef	struct
{
///	u16	u16ResponsePackId;	///对应的平台消息的流水号
	u32	u32MediaDataId;	///多媒体数据ID
	u8	u8ReSendPackAmount;	///重传包总数
	u8	IDList[0];		///重传包ID列表，不超过125项，无改字段则表明已收到全部数据包
} tMsg_SACK_Media_AckData;
#endif

#ifdef JTT808_Ver_2013
/**
多媒体数据上传应答
注：对此消息的应答应采用0x0801 消息将重传包ID 列表中的分包重发一次，与原始分包
消息完全一致。
*/
typedef	struct
{
	u32	u32MediaDataId;	///多媒体ID DWORD >0，如收到全部数据包则没有后续字段
	u8	u8ReSendPackAmount;	///重传包总数 BYTE n
	u8	IDList[0];	///重传包ID 列表 BYTE[2*n]	重传包序号顺序排列，如“包ID1 包ID2......包IDn”
} tMsg_SACK_Media_AckData;
#endif///JTT808_Ver_2013

/** 8.54 摄像头立即拍摄命令 消息ID：0x8801。***********************************/
///分辨率定义
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
	u8	u8ChannelId;	///通道ID
	u16	u16Cmd;		///拍照命令，0:表示停止拍摄;0xfffff表示录像；其他表示拍照张数
	u16	u16PicIntervalOrRecordTime;		///拍照间隔或者录像时间
	u8	u8SaveTag;		///保存标志，1:保存，0:实时上传
	u8	u8Resolve;		///分辨率
	u8	u8AVQuality;	///图像/视频质量，1代表质量损失最小，10表示压缩比最大
	u8	u8Brightness;	///亮度
	u8	u8Contrastness;		///对比度
	u8	u8Saturation;	///饱和度
	u8	u8Chrome;	///色度
} tMsg_SCMD_InstantPhoto;
/**终端若不支持系统要求的分辨率，则取最接近的分辨率拍摄并上传*/

#ifdef JTT808_Ver_2013
/** 8.55 摄像头立即拍摄命令应答 消息ID：0x0805。********************************/
///该命令用于应答监控中心下发的摄像头立即拍摄命令0x8801。
typedef struct
{

	u16 u16SeqNum;	///0 应答流水号 WORD 对应平台摄像头立即拍摄命令的消息流水号
	u8	u8Result;	///2 结果 BYTE 0：成功；1：失败；2：通道不支持。
/**以下字段在结果=0 时才有效。*/
	u16 u16MediaIdAmount;	///3 多媒体ID 个数 WORD n，拍摄成功的多媒体个数
	u32 u32MediaId[0];	///4 多媒体ID 列表 BYTE[4*n]
} tMsg_TACK_InstantlyTakePhone;
#endif///JTT808_Ver_2013

/** 8.56 存储多媒体数据检索 消息ID：0x8802。***********************************/
///注：不按时间范围则将起始时间/结束时间都设为00-00-00-00-00-00。
typedef	struct
{
	u8 u8MediaType;	///多媒体类型0：图像；1：音频；2：视频
	u8 u8ChannelId;	///通道ID表示检索该媒体类型的所有通道
	u8 u8EventCode;	/**事件项编码0：平台下发指令；1：定时动作；2：抢劫报警触
											发；3：碰撞侧翻报警触发；其他保留*/
	tBCDTime tBcdStartTime;	///起始时间,yy-mm-dd-hh-mm-ss
	tBCDTime tBcdEndTime;	///结束时间,yy-mm-dd-hh-mm-ss
} tMsg_SCMD_MediaDataRetrieve;

/** 8.57 存储多媒体数据检索应答 消息ID：0x0802。********************************/
///多媒体检索项数据格式
typedef		struct
{
	u8 u8MediaType;		///多媒体类型
	u8 u8ChannelId;		///通道ID
	u8 u8EventCode;		///事件项编码
	tMsg_T_PositionInfo	tPosInfo;	///位置信息汇报，表示拍摄或录制的起始时刻的汇报消息
} tMediaIndexItem;

typedef	struct
{
	u16	u16SeqNum;		///对应的平台消息的流水号
	u16	u16MediaItemAmount;		///多媒体数据包总项数
	tMediaIndexItem	IndexItem;	///检索项
} tMsg_TACK_StoredMediaDataRetrieve;

/** 8.58 存储多媒体数据上传命令 消息ID：0x8803 *********************************/
///存储多媒体数据上传命令
typedef	struct
{
	u8 u8MediaType;		///多媒体类型
	u8 u8ChannelId;		///通道ID
	u8 u8EventCode;		///事件项编码
	tBCDTime tBcdStartTime;		///起始时间,yy-mm-dd-hh-mm-ss
	tBCDTime tBcdEndTime;	///结束时间,yy-mm-dd-hh-mm-ss
	u8 u8DeleteTag;		///删除标志，0:保留，1:删除
} tMsg_SCMD_MediaDataUpload;

/** 8.59 录音开始命令 消息ID：0x8804*******************************************/
typedef	struct
{
	u8 u8RecordCmd;		///0:停止录音；1:开始录音
	u16	u16RecordTime;	///录音时间，单位为秒，0表示一直录音
	u8 u8SaveTag;	///保存标志，0:实时上传，1:保存
	u8 u8SampleRate;	///音频采样率，0:8K; 1: 11K;2: 23K; 3:32K;其他保留
} tMsg_SCMD_RecordAudio;


#ifdef JTT808_Ver_2013
/** 8.60 单条存储多媒体数据检索上传命令 消息ID：0x8805**************************/
typedef struct
{
	u32 u32MediaId;		///多媒体ID DWORD >0
	u8	u8DeleteFlag;	///删除标志 BYTE 0：保留；1：删除；
} tMsg_SCMD_SingleStoredMediaUpload;
#endif///JTT808_Ver_2013


/** 8.61 数据下行透传 消息ID： 0x8900。*****************************************/		/////////////////////////????????????未定义透传数据格式
#ifdef JTT808_Ver_2013
/**数据上行透传消息体数据格式*/
#define TRANSPARENTMSGTYPE_GNSS		0x00	///GNSS 模块详细定位数据 0x00 GNSS 模块详细定位数据
#define TRANSPARENTMSGTYPE_IC		0x0B	/**道路运输证IC 卡信息 0x0B 道路运输证IC 卡信息上传消息为64Byte，
												下传消息为24Byte。道路运输证IC卡认证透传超时时间为30s。超时后，不重发。*/
#define TRANSPARENTMSG_UART1		0x41	///0x41 串口1 透传消息
#define TRANSPARENTMSG_UART2		0x42	///0x42 串口2 透传消息
///用户自定义透传 0xF0-0xFF 用户自定义透传消息
#endif///JTT808_Ver_2013

typedef	struct
{
	u8	u8MsgType;	///透传消息类型
	u8	Data[0];	///透传消息内容
} tMsg_SCMD_DownloadTransparentData;


/** 8.62 数据上行透传 消息ID： 0x0900。*****************************************/	/////////////////////////????????????未定义透传数据格式
typedef	struct
{
	u8	u8MsgType;	///透传消息类型
	u8	Data[0];	///透传消息内容
} tMsg_T_UploadTransparentData;

/** 8.63 数据压缩上报 消息ID：0x0901。*****************************************/
///压缩消息体为需要压缩的消息经过GZIP 压缩算法后的消息
typedef	struct
{
	u32	u32CompressMsgLength;	///压缩消息长度
	u8	Data[0];	///压缩消息体
} tMsg_T_UploadCompressedMsg;

/** 8.64 平台RSA 公钥 消息ID：0x8A00。*****************************************/
///平台RAS公钥
typedef	struct
{
	u32	u32RAS_e;	///平台RAS公钥e,n中的e
	u8	RAS_n[128];	///平台RAS公钥e,n中的n
} tMsg_SCMD_RAS_PlatformPublicKey;

/** 8.65 终端RSA 公钥 消息ID：0x0A00。*****************************************/
///终端RAS公钥
typedef	struct
{
	u32	u32E;	///终端RAS公钥e,n中的e
	u8	au8Data[128];	///终端RAS公钥e,n中的n
} tMsg_T_RAS_TerminalPublicKey;

/** 函数声明 ******************************************************************/
void MSGID_TCPIPData_Process(u8 *pMsg, u16 msgLen);
void MSGID_ServerAckProc(tMsg_S_GeneralAck *pSAck);


#pragma pack()

#endif /** PROTOCOL_H_ */
