#ifndef	_PARAMETER_H_
#define	_PARAMETER_H_

#include "stm32f2xx_conf.h"
#include "protocol.h"
#include "config.h"
#include <fram.h>


#pragma pack(1)


/**参数项ID定义*/
enum {
	ePARAM_ALIVE_INTERVAL = 0,	///			=	0x0001,		///终端心跳发送间隔，单位为s
	ePARAM_TCPACK_TIMEOUT,	///			=	0x0002,		///TCP应答超时，单位为s
	ePARAM_TCP_RESENDTIMES,	///			=	0x0003,		///TCP消息重传次数
	ePARAM_UDPACK_TIMEOUT,	///			=	0x0004,		///UDP应答超时，单位为s
	ePARAM_UDP_RESENDTIMES,	///			=	0x0005,		///UDP消息重传次数
	ePARAM_SMSACK_TIMEOUT,	///			=	0x0006,		///SMS应答超时，单位为s
	ePARAM_SMS_RESENDTIMES,	///			=	0x0007,		///SMS消息重传次数
	/**0X0008 ----0X000F 保留*/
	ePARAM_MAINSRV_APN = 7,	///				=	0x0010,		///主服务器APN，无线通信拨号访问点
	ePARAM_MAINSRV_USR,	///				=	0x0011,		///主服务器无线通信拨号用户名
	ePARAM_MAINSRV_PSWD,	///				=	0x0012,		///主服务器无线通信拨号密码
	ePARAM_MAINSRV_IP,	///				=	0x0013,		///主服务器地址，IP或域名
	ePARAM_VICESRV_APN,	///				=	0x0014,		///备份服务器APN，无线通信拨号访问点
	ePARAM_VICESRV_USR,	///				=	0x0015,		///备份服务器无线通信拨号用户名
	ePARAM_VICESRV_PSWD,	///				=	0x0016,		///备份服务器无线通信拨号密码
	ePARAM_VICESRV_IP,	///				=	0x0017,		///备份服务器地址，IP或域名
	ePARAM_TCP_PORT,	///				=	0x0018,		///TCP端口
	ePARAM_UDP_PORT,	///					=	0x0019,		///UDP端口
	/**0X001A ----0X001F 保留*/
#ifdef JTT808_Ver_2013	///新版增加协议部分
	ePARAM_ICVERIFY_MS_IPOrDNS = 17,	///		=	0x001A,		///道路运输证IC卡认证主服务器IP地址或域名
	ePARAM_ICVERIFY_TCPPORT,	///			=	0x001B,		///道路运输证IC卡认证主服务器TCP端口
	ePARAM_ICVERIFY_UDPPORT,	///			=	0x001C,		///道路运输证IC卡认证主服务器UDP端口
	ePARAM_ICVERIFY_BS_UDPPORT,	///		=	0x001D,		///道路运输证IC卡备份服务器IP地址或域名，端口同主服务器
#endif///JTT808_Ver_2013

	ePARAM_POSRPT_STRATEGE = 21,	///	终端位置汇报策略
				/**终端汇报方案，0:根据ACC状态，1:根据登录状态和ACC状态，先判断登录状态，
				 	 	 	 	 	 	 	 	 	 	 	 	若登录再根据ACC状态*/
	ePARAM_POSRPT_METHOD = 22,	///				=	0x0021,
	ePARAM_DRV_UNREG_RPTPERIOD = 23,	///		=	0x0022,		///驾驶员未登录汇报时间间隔，单位为秒
	/**0X0023 ----0X0026 保留*/
	ePARAM_RPTPERIOD_ONSLEEP,	///			=	0x0027,		///休眠时汇报时间间隔，单位为秒
	ePARAM_RPTPERIOD_URGENT,	///			=	0x0028,		///紧急报警时汇报时间间隔，单位为秒
	ePARAM_RPTPERIOD_DEFAULT,	///			=	0x0029,		///缺省时汇报时间间隔，单位为秒
	/**0X002A ----0X002B 保留*/
	ePARAM_RPTDIST_DEFAULT,	///			=	0x002C,		///缺省时距离汇报间隔，单位为米
	ePARAM_RPTDIST_DRV_UNREG,	///			=	0x002D,		///驾驶员未登录距离汇报间隔，单位为米
	ePARAM_RPTDIST_SLEEP,	///				=	0x002E,		///休眠时距离汇报间隔，单位为米
	ePARAM_RPTDIST_URGENT,	///			=	0x002F,		///紧急报警时距离汇报间隔，单位为米
	ePARAM_INFLECTION_POINT_ANGLE,	///	=	0x0030,		///拐点补传角度，单位为度(< 180)
	ePARAM_BARRIER_RADIUS,	///			=	0x0031,		///电子围栏半径(非法移位阈值)，单位为米
	/**0X0032----0X003F 保留*/
	ePARAM_MONITORCENTER_PHONENUM = 33,	///	=	0x0040,		///监控平台电话号码
	ePARAM_RESET_PHONENUM,	///			=	0x0041,		///复位电话号码，可采用此电话号码拨打终端电话让终端复位
				  ///回复出厂设置电话号码，可采用此电话号码拨打终端电话让终端恢复出厂设置
	ePARAM_RECOVERFACTORY_PHONENUM = 34,	///	=	0x0042,
	ePARAM_MONITORCENTER_SMS_NUM,	///		=	0x0043,		///监控平台SMS电话号码
	ePARAM_RECVTXTALARM_SMS_NUM,	///		=	0x0044,		///接收终端SMS文本报警电话号码
					///终端电话接听策略，0:自动接通，1:ACC ON时自动接通，OFF时手动接听
	ePARAM_ANSWERCALLING_STRATEGE,	///	=	0x0045,
			 ///终端每次通话时最长通话时间，单位为秒，0为不允许通话，0xffffffff为不限制
	ePARAM_TOUT_PERCALL,	///				=	0x0046,
					///终端每月最长通话时间，单位为秒，0为不允许通话，0xffffffff为不限制
	ePARAM_TOUT_PERMONTHCALL,	///			=	0x0047,
	ePARAM_MONITORLISTEN_PHONENUM,	///	=	0x0048,			///监听电话号码
	ePARAM_SUPERVISE_SMS_NUM = 41,	///			=	0x0049,			///监管平台特权短信号码
	/**0X004A----0X004F 保留*/

	///报警屏蔽字，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警被屏蔽
	ePARAM_ALARM_MASK = 42,	///				=	0x0050,
	///报警发送文本SMS开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时发送文本SMS
	ePARAM_SMSCTRL_ONALARM,	///			=	0x0051,
	///报警拍摄开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时摄像头拍摄
	ePARAM_PICCTRL_ONALARM,	///			=	0x0052,
	///报警拍摄存储标志，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时拍的照片进行存储，否则实时上传
	ePARAM_PICCTRL_STORE_ONALARM,	///		=	0x0053,
	///关键标志，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警为关键报警
	ePARAM_KEYALARM_FLAG,	///	 			=	0x0054,
	ePARAM_MAXSPEED,	///					=	0x0055,	   	///最高速度，单位为公里每小时(km/h)
	ePARAM_OVERSPEED_LASTING_TIMEOUT,	///	=	0x0056,		///超速持续时间，单位为秒
	ePARAM_CONTINUOUS_DRV_TIMEOUT,	///	=	0x0057,		///连续驾驶时间门限，单位为秒
	ePARAM_DRV_TOTAL_EACHDAY_TIMEOUT,	///	=	0x0058,		///当天累计驾驶时间，单位为秒
	ePARAM_LEAST_REST_TIMEOUT,	///		=	0x0059,		///最小休息时间，单位为秒
	ePARAM_MAX_STOPPING_TIMEOUT = 52,	///		=	0x005A,		///最长停车时间，单位为秒
	/**0X005B----0X006F */
#ifdef JTT808_Ver_2013
	ePARAM_OVERSPEED_PREALARM_DIF,	///	=	0x005B,		///超速报警预警差值，单位1/10Km/h
	ePARAM_FATIGE_PREALARM_DIF,	///		=	0x005C,		///疲劳驾驶预警差值，单位为秒，>0
		  ///b7-b0:碰撞时间，单位4ms；b15-b8:碰撞加速度，单位0.1g，设置范围0~79，默认10
	ePARAM_COLLISION_SETTING,	///			=	0x005D,
									   ///侧翻报警参数设置，侧翻角度，单位1度，默认30度
	ePARAM_SIDECOLLAPSE_SETTING,	///		=	0x005E,
	/**0X005F----0X0063 */
	ePARAM_PERIODIC_PHOTO_CTRL,	///		=	0x0064,		///定时拍照控制
	ePARAM_PERDIST_PHOTO_CTRL = 58,	///		=	0x0065,		///定距拍照控制
	/**0X0066----0X006F */
#endif///JTT808_Ver_2013

	ePARAM_VIDEO_QUALITY = 59,	///		=	0x0070,		///图片/视频质量，1-10,1最好
	ePARAM_BRIGHTNESS,	///				=	0x0071,		///亮度，0-255
	ePARAM_CONTRASTNESS,	///				=	0x0072,		///对比度，0-127
	ePARAM_SATURATION,	///				=	0x0073,		///饱和度，0-127
	ePARAM_CHROMA,	///					=	0x0074,		///色度，0-255
	/**0X0075----0X007F*/
	ePARAM_VEHICLE_KILOMETRES,	///		=	0x0080,		///车辆里程表读数，1/10km
	ePARAM_VEHICLE_PROVINCE_ID,	///		=	0x0081,		///车辆所在地省域ID
	ePARAM_VEHICLE_CITY_ID,	///			=	0x0082,		///车辆所在地市域ID
	ePARAM_VEHICLE_ID,	/// 				=	0x0083,		///车牌号码
	ePARAM_VEHICLE_ID_COLOR,	///			=	0x0084,		///车牌颜色

#ifdef JTT808_Ver_2013
/**以下为新协议增加的参数项*/
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
 * 协议规定的需保存参数项：
 *
1、对于终端发送的关键报警消息，若达到重传次数后仍未收到应答，则应对其进行保存。以后在发送其它消
息前要先发送保存的关键报警消息。
2、平台通过发送事件设置消息，将事件列表发到终端存储，驾驶员在遇到相应事件后可进
入事件列表界面进行选择，选择后终端向平台发出事件报告消息。
3、平台通过发送信息点播菜单设置消息，将信息点播项列表发到终端存储，驾驶员可以通
过菜单选择点播/取消相应的信息服务，选择后终端向平台发出信息点播/取消消息。
4、平台通过发送设置电话本消息，对终端设置电话本，该消息需要终端回复终端通用应答
消息。
5、平台通过发送设置圆型区域、设置矩形区域、设置多边形区域、设置路线等消息，对终
端进行区域和线路设置
6、行驶记录参数
7、多媒体数据
8、摄像头图片/视频
9、音频数据
*/

/**参数类型*/
typedef enum
{
///	PARAM_FACTORY_DEFAULT 	= 0,	////出厂参数
	PARAM_TERMINALINFO 		= 1,	////设备信息
	PARAM_SYSTEM			= 2,	////系统信息
	PARAM_RUNTIME			= 3,	////运行信息
	PARAM_RECTANGLEAREA		= 4,	////矩形区域
	PARAM_ROUNDAREA			= 5,	////圆形区域
	PARAM_POLYGONAREA		= 6,	////多边形区域
	PARAM_ROUTE				= 7,	////路线
	PARAM_ONDEMANDMSGINFO	= 8,	////点播消息
	PARAM_EVENT,					///事件

	PARAM_TYPEEND,

} ePARAMTYPE;

/**参数内容校验前缀码，加上它是为了避免从外部存储器读参数发生错误时，内容和校验码
 * 全为0，则仍能通过校验，当做有效参数的情况*/
#define PARAM_PREFIX  0x12345678


/*

///系统存储的电话簿
#define		PHONE_NUM_MAX_LENGTH	16	///电话号码长度
#define		PHONE_NAME_MAX_LENGTH	16	///姓名长度
#define		PHONE_NUM_MAX_CNT		50	///电话号码数目

///电话号码项
typedef      struct
{
	u8		u8CodeLength;									///电话号码长度
	u8		au8CodeNum[PHONE_NUM_MAX_LENGTH];				///电话号码内容
	u8		u8NameLength;									///姓名长度
	u8		au8Name[PHONE_NAME_MAX_LENGTH];					///姓名内容
}tPHONENUMITEM;


///通话记录簿
typedef 	struct
{
	u8				u8OutDialingCnt;								///已拨号码数目
	tPHONENUMITEM	tOutDialPhoneNumItem[PHONE_NUM_MAX_CNT];		///已拨电话号码数组
	u8				u8UnListenedCnt;								///未接来电数目
	tPHONENUMITEM	tUnListenedPhoneNumItem[PHONE_NUM_MAX_CNT];		///未接电话号码数组
	u8				u8ListenedCnt;									///已接来电数目
	tPHONENUMITEM	tListenedPhoneNumItem[PHONE_NUM_MAX_CNT];		///已接电话号码数组
	u8				u8CRC;											///校验和
}tPHONELOGINFOType;

typedef	struct
{
	u8 	u8PhoneTag;					///电话标志1:呼入；2:呼出；3:呼入/呼出
	tPHONENUMITEM tPhoneNumItem;
}tPHONEBOOKITEM;
///电话号码簿
typedef 	struct
{
	u8				u8Amount;										///电话号码数目
	tPHONEBOOKITEM	tPhoneNumItem[PHONE_NUM_MAX_CNT];				///电话号码
	u8				u8CRC;											///校验和
}tPHONEBOOKINFOType;


typedef	struct	
{
	u8 		u8PhoneNumLength;				///电话号码长度
	u8		au8PhoneNum[20];				///电话号码
}tPHONENUMITEM;
*/


#define PLATE_SIZE	30	///牌照数组长度


#define PARAM_FIX_ITEM_CNT	10	///定长系统参数个数
#define PARAM_STR_ITEM_CNT	10	///变长系统参数个数，字符串



/////参数项头部
//typedef 	struct
//{
//	u32		u32Id;		///参数ID
//	u8		u8Len;		///参数长度
//	u8		aValue[0];	///参数内容
//} tPARAMITEMHEAD;

/**定长系统参数项消息体，u8/u16/u32*/
typedef struct
{
	u32		u32Id;		///参数ID
	u8		u8Len;		///参数长度
	union{
		u8	u8Val;
		u16	u16Val;
		u32	u32Val;
	} val;

} tPARAMITEM_FIXED;

/**变长参数项消息体，字符串*/
//typedef struct
//{
//	u32		u32Id;		///参数ID
//	u8		u8Len;		///参数长度
//	u8		str[STR_SIZE];	///字符串
//
//} tPARAMITEM_STR;

#if 1
/**系统参数 *******************************************************************/
typedef	struct
{
/**通信参数*/
	u32 u32LinkAliveTimeOut;	///终端心跳包发送间隔，单位为秒(s)
	u32 u32TCPAckTimeout;		///TCP应答超时时间，单位为秒(s)
	u32 u32TCPReSendTimes;		///TCP消息重传次数
	u32 u32UDPAckTimeout;		///UDP应答超时时间，单位为秒(s)
	u32 u32UDPReSendTimes;		///UDP消息重传次数
	u32 u32SMSAckTimeout;		///SMS应答超时时间，单位为秒(s)
	u32 u32SMSReSendTimes;		///SMS消息重传次数

///主服务器参数
	u8 au8MainServerApn[STR_SIZE];	///主服务器APN
	u8 au8MainServerUsr[STR_SIZE];	///主服务器无线通信用户名
	u8 au8MainServerPswd[STR_SIZE];	///主服务器无线通信密码
	u8 au8MainServerIP[STR_SIZE];		///主服务器IP地址,IP或域名
///备份服务器参数
	u8 au8ViceServerApn[STR_SIZE];	///备份服务器APN
	u8 au8ViceServerUsr[STR_SIZE];	///备份服务器无线通信用户名
	u8 au8ViceServerPswd[STR_SIZE];	///备份服务器无线通信密码
	u8 au8ViceServerIP[STR_SIZE];		///备份服务器IP地址
	u32 u32TCPPort;	///服务器TCP端口
	u32 u32UDPPort;	///服务器UDP端口
#ifdef JTT808_Ver_2013
	u8	IC_Verify_MS_IPOrDNS[STR_SIZE];	///道路运输证IC卡认证主服务器IP地址或域名
	u32 IC_Verify_TCP_Port;		///道路运输证IC卡认证主服务器TCP端口
	u32 IC_Verify_UDP_Port;		///道路运输证IC卡认证主服务器UDP端口
	u8	IC_Verify_BS_IPOrDNS[STR_SIZE];	///道路运输证IC卡备份服务器IP地址或域名，端口同主服务器
#endif

/**位置汇报参数*/
	u32	u32PosRptStrategy;	///位置汇报策略
	u32	u32PosRptMethod;	///位置汇报方案
	u32	u32RptIntervalDrvUnReg;	///驾驶员未登录汇报时间间隔，单位 为秒(s)
	u32	u32RptIntervalSleep;	///休眠汇报时间间隔，单位 为秒(s)
	u32	u32RptIntervalEmergency;	///紧急汇报时间间隔，单位 为秒(s)
	u32	u32RptIntervalDefault;		///缺省汇报时间间隔，单位 为秒(s)
	u32	u32RptDistanceDefault;	///缺省汇报距离间隔，单位 为米(m)
	u32	u32RptDistanceDrvUnReg;		///驾驶员未登录汇报距离间隔，单位 为米(m)
	u32	u32RptDistanceSleep;	///休眠汇报距离间隔，单位 为米(m)
	u32	u32RptDistanceEmergency;	///紧急汇报距离间隔，单位 为米(m)
	u32	u32InflectionSuppleAngle;	///拐点补传角度，单位为度, < 180
	u16 u16BarrierRadius;		///电子围栏半径(非法移位阈值)，单位为米

/**电话参数*/
	u8 aMonitorPlatformPhone[STR_SIZE];	///监控平台号码内容
	u8 aResetPhone[STR_SIZE];		///复位电话号码内容
	u8 aRecoverFactorySettingPhone[STR_SIZE];		///恢复出厂设置电话号码内容
	u8 aCenterSMSPhone[STR_SIZE];		///监控平台号码内容
	u8 aRcvTerminalSMSAlarmPhone[STR_SIZE];	///接收终端SMS报警号码内容

	u32 u32PhoneListenStratagy;		///终端电话接听策略
	u32 u32PhoneConnTimeoutEachTime;	///每次通话最长时间，单位为秒(s)
	u32 u32PhoneConnTimeoutCurMonth;	///当月通话最长时间，单位为秒(s)

	u8 aVoiceMonitorPhone[STR_SIZE];		///监听电话号码内容
	u8 aMonitorPlatformPriviligPhone[STR_SIZE];	///监管平台特权电话号码内容

/**报警参数*/
	u32 u32AlarmMask;	///报警屏蔽字，与位置信息汇报消息中的报警标志相对应，相应位为1则相应报警被屏蔽
	u32 u32SendSMSOnAlarmCtrl;	///报警发送文本SMS开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时发送文本SMS
	u32 u32CaptureOnAlarmCtrl;	///报警拍摄开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时摄像头拍摄
	u32 u32AlarmPictureStorageCtrl;		///报警拍摄存储标志，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时拍的照片进行存储，否则实时上传
	u32 u32KeyAlarmMask;		///关键标志，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警为关键报警

/**超速监测参数*/
	u32 u32SpeedMax;		///最高速度，单位为km/h
	u32 u32OverSpeedContinousTime;		///超速持续时间，单位为秒(s)
	u32 u32ContinousDriveTimeThreshold;		///连续驾驶时间门限，单位为秒(s)
	u32 u32DriveTimesTodayThreshold;		///当天累计驾驶时间门限，单位为秒(s)
	u32 u32MinRestTime;			///最小休息时间，单位为秒(s)
	u32 u32MaxStopTime;			///最长停车时间，单位为秒(s)
#ifdef JTT808_Ver_2013
	u16 u16OverSpeed_PreAlarm_Dif;	///超速报警预警差值，单位1/10Km/h
	u16 u16Fatige_PreAlarm_Dif;		///疲劳驾驶预警差值，单位为秒，>0
	u16 u16CollisionSetting;	/**碰撞报警参数设置，b7-b0： 碰撞时间，单位4ms；
				b15-b8：碰撞加速度，单位0.1g，设置范围在：0-79 之间，默认为10*/
	u16 u16SideCollapseSetting;	///侧翻报警参数设置：侧翻角度，单位1 度，默认为30 度。
/**0X005F----0X0063 */
	tParam_PeriodicTimeVideoCapture tPhotoCtrl_Periodic;	///定时拍照控制
	tParam_PeriodicDistVideoCapture tPhotoCtrl_Dist;	///定距拍照控制
#endif

///摄像头相关参数设置
	u32 u32PictureQuality;		///图片/视频质量，1-10,1最好
	u32 u32PictureBrightness;	///亮度，0 -255
	u32 u32PictureContrastness;		///对比度，0 -255
	u32 u32PictureSaturation;	///饱和度，0 -255
	u32 u32PictureChroma;	///色度，0 -255

///车辆属性相关参数设置
	u32	u32Kilometers;		///车辆行驶里程
	u16	u16ProviceId;		///省域ID
	u16	u16CountyId;		///市县域ID
	u8	au8Plate[PLATE_SIZE];		///车牌
	u8	u8Color;		///车牌颜色

#ifdef JTT808_Ver_2013
/**以下为新协议增加的参数项*/
/**
GNSS 定位模式，定义如下：
bit0，0：禁用GPS 定位， 1：启用GPS 定位；
bit1，0：禁用北斗定位， 1：启用北斗定位；
bit2，0：禁用GLONASS 定位， 1：启用GLONASS 定位；
bit3，0：禁用Galileo 定位， 1：启用Galileo 定位。
*/
	u8 u8GNSS_Mode;	///	0x0090
/**
GNSS 波特率，定义如下：
0x00：4800；0x01：9600；
0x02：19200；0x03：38400；
0x04：57600；0x05：115200。
*/
	u8 u8GNSS_Baund;	///PARAM_GNSS_BOUNDRATE	0x0091
/**
GNSS 模块详细定位数据输出频率，定义如下：
0x00：500ms；0x01：1000ms（默认值）；
0x02：2000ms；0x03：3000ms；
0x04：4000ms。
*/
	u8 u8GNSS_DetailMsg_OutFreq;	///0x0092
/**0x0093 DWORD GNSS 模块详细定位数据采集频率，单位为秒，默认为1。*/
	u32 u32GNSS_DetailMsg_RetrieveFreq;	///0x0093
/**
GNSS 模块详细定位数据上传方式：
0x00，本地存储，不上传（默认值）；
0x01，按时间间隔上传；
0x02，按距离间隔上传；
0x0B，按累计时间上传，达到传输时间后自动停止上传；
0x0C，按累计距离上传，达到距离后自动停止上传；
0x0D，按累计条数上传，达到上传条数后自动停止上传。
*/
	u8 u8GNSS_DetailMsg_RptType;	///0x0094
/**
GNSS 模块详细定位数据上传设置：
上传方式为0x01 时，单位为秒；
上传方式为0x02 时，单位为米；
上传方式为0x0B 时，单位为秒；
上传方式为0x0C 时，单位为米；
上传方式为0x0D 时，单位为条。
*/
	u32 u32GNSS_DetailMsg_RptSetting;	///0x0095

/**0x0100 DWORD CAN 总线通道1 采集时间间隔(ms)，0 表示不采集*/
	u32 u32CAN_Ch1_RetrieveInterval;	///0x0100
/**0x0101 WORD CAN 总线通道1 上传时间间隔(s)，0 表示不上传*/
	u16 u16CAN_Ch1_UploadInterval;	///0x0101

/**0x0102 DWORD CAN 总线通道2 采集时间间隔(ms)，0 表示不采集*/
	u32 u32CAN_Ch2_RetrieveInterval;	///0x0102
/**0x0103 WORD CAN 总线通道2 上传时间间隔(s)，0 表示不上传*/
	u16 u16CAN_Ch2_UploadInterval;		///0x0103

/**
		0x0110 BYTE[8]
CAN 总线ID 单独采集设置：
bit63-bit32 表示此ID 采集时间间隔(ms)，0 表示不采集；
bit31 表示CAN 通道号，0：CAN1，1：CAN2；
bit30 表示帧类型，0：标准帧，1：扩展帧；
bit29 表示数据采集方式，0：原始数据，1：采集区间的计算值；
bit28-bit0 表示CAN 总线ID。
*/
	u8 u8CAN_SingleRetrieveSetting[8];	///0x0110

/**0x0111-0x01FF BYTE[8] 用于其他CAN 总线ID 单独采集设置*/
	u8 u8CAN_SingleRetrieveSetting_Others[8]; ///0x0111-0x01FF

/**0xF000-0xFFFF 用户自定义*/

#endif

} tPARAM_SYSTEM;
#endif

/** 系统参数存储结构体 */
typedef struct
{
	u32	prefix;	///前缀
	tPARAM_SYSTEM tSystemParam;	/**系统参数*/
	u8 u8CRC;
} tPARAM_SYSTEM_STORAGE;

#define PARAM_AUTHORCODE_LEN	100  	/**终端鉴权码长度*/


/** 设备参数 *****************************************************************/
typedef	struct
{
	tTerminalInfo_Base	tBase; /**基本信息*/
	u8	aPlate[PARAM_VIN_PLATE_MAXLEN];		/**V2011： 机动车号牌;
						  V2013：车牌车牌颜色为0 时，表示车辆VIN；否则，表示公安
						  交通管理部门颁发的机动车号牌。*/

#ifdef JTT808_Ver_2013
	tTerminalType tType;	/**终端类型*/
#endif
	tGNSSModule_Property	tGNSSModuleProperty; 	/**GNSS模块属性*/
	tCommModule_Property	tCommModuleProperty;	/**通信模块属性*/

	u8	ver_hardware[50];	///终端硬件版本号 STRING
	u8	ver_software[50];	///终端软件版本号 STRING

} tPARAM_DEVICE;

/**设备参数存储结构体*/
typedef	struct
{
	u32	prefix;	///前缀
	tPARAM_DEVICE	tDeviceParam; ///设备参数
	u8 u8CRC; ///校验码
} tPARAM_DEVICE_STORAGE;

/** 运行参数 *****************************************************************/
typedef	struct
{
	u8	u8CommMode;		///通讯模式，缺省为TCP
	u8	aServerIP[4];	///服务器IP
	u16	u16Server_Port;		///服务器端口号

	u8   u8ConnectionCtrl;	///连接控制
	char sURL[100];	///URL地址
	char sPPP_name[10];///拨号点名称;
	char sPPP_user[10];///拨号用户名;
	char sPPP_password[10]; ///拨号密码;
	char sIP[30];///地址;IP或DNS
	u16  u16TCP_port;///TCP 端口;
	u16  u16UDP_port;///UDP 端口;
	char sManufacture_ID[20]; ///制造商ID;
	char sHardware_Version[30];///硬件版本;
	char sFirmware_Version[30];///固件版本;
	u16  u16TOUT_ServerConnect; ///连接到指定服务器时限,单位分

	u8	sAuthorCode[PARAM_AUTHORCODE_LEN];	/**终端鉴权码*/
	u8	ICCID[10];		///终端SIM 卡ICCID BCD[10]
	u8 	u8BcdPhoneNum[6];	///终端手机号,BCD码

	tWARNINGSTATE 	tWarningState;	/**报警标志位*/
	tSYSTEMSTATE 	tSysState;	/**状态位*/

	u16	u16UpReportInterval;	///呼叫上报-上报周期，单位秒
	u16	u16KeepOnTime;		///呼叫上报-持续时长，单位秒

	u8 u8VechicalCtrlState;	///车辆控制状态，0=解锁，1=锁车

} tPARAM_RUNTIME;

/**运行参数存储结构体*/
typedef	struct
{
	u32	prefix;	///前缀
	tPARAM_RUNTIME	tRuntimeParam; ///运行参数
	u8 u8CRC; ///校验码
} tPARAM_RUNTIME_STORAGE;


#ifdef JTT808_Ver_2013
/**升级类型*/
typedef enum
{
	UPG_TERMINAL = 0, ///终端
	UPG_IC_Reader = 12, ///道路运输证IC 卡读卡器
	UPG_GNSS = 52, ///北斗卫星定位模块
} eUPG_TYPE;

/** 升级信息 *****************************************************************/
typedef struct
{
	u8 upg_type;	///升级类型 0：终端，12：道路运输证IC 卡读卡器，52：北斗卫星定位模块
	u8 factoryId[5];	///制造商ID BYTE[5] 制造商编号
	u8 version_len;		///版本号长度 BYTE n
	u8 version[50];		///版本号 STRING
	u32 u32UpgPack_len;	///7+n 升级数据包长度 DWORD 单位为BYTE
	u8 result;     ///升级结果  0：成功，1：失败，2：取消
} tUPGRADEINFO;

/**升级信息存储结构体*/
typedef	struct
{
	u32	prefix;	///前缀
	tUPGRADEINFO	tUpgradeInfo; ///运行参数
	u8 u8CRC; ///校验码
} tUPGRADEINFO_STORAGE;
#endif

/** 事件/点播菜单/点播消息/电话本参数  ***********************************************************/

enum {
	PARAM_FORMATMSG_EVT = 0,
	PARAM_FORMATMSG_MSGOD_MENU,
	PARAM_FORMATMSG_MSGOD_MSG,
	PARAM_FORMATMSG_PHONEBOOK,

	PARAM_FORMATMSG_END,
};

///各类信息在EEPROM中的存储首地址
#define EEPROM_ADDR_EVENT		0x3333	///事件信息在EEPROM中首地址
#define EEPROM_ADDR_MSGOD_MENU	0x3333	///点播菜单信息在EEPROM中首地址
#define EEPROM_ADDR_MSGOD_MSG	0x3333	///点播信息在EEPROM中首地址
#define EEPROM_ADDR_PHONEBOOK	0x3333	///电话本在EEPROM中首地址

#define EEPROM_ADDR_ROUND		0x3333	///圆形区域在EEPROM中首地址
#define EEPROM_ADDR_RECTANGLE	0x3333	///矩形区域在EEPROM中首地址
#define EEPROM_ADDR_POLYGON		0x3333	///多边形区域在EEPROM中首地址

#define EVT_MSG_SIZE			50 	///事件内容最大长度
#define EVT_ITEM_SIZE			50	///事件最大个数
#define MSGOD_MENU_MSG_SIZE		50 	///点播菜单内容长度
#define MSGOD_MENU_ITEM_SIZE	50 	///点播菜单条数
#define MSGOD_MSG_MSG_SIZE		50	///点播消息内容长数
#define MSGOD_MSG_ITEM_SIZE		50	///点播消息条数
#define PHONEBOOK_MSG_SIZE		50	///电话本单条记录长数
#define PHONEBOOK_ITEM_SIZE		50	///电话本记录总条数

#define ROUND_AREA_ITEM_SIZE	50	///圆形区域项总数
#define RECT_AREA_ITEM_SIZE		50	///矩形区域项总数
#define POLYGON_AREA_ITEM_SIZE	50	///多边形区域项总数

///事件项索引
typedef struct
{
	u8	u8Id;	///事件ID,若终端已有同ID的事件，则被覆盖
	u8	u8Len;	///事件信息实际长度
	u16 u16Offset;	///事件信息偏移地址，相对于索引头
	u8  u8CRC;		///事件内容CRC
} tITEMINDEX;

///事件信息存储头
typedef struct
{
	u8 u8Amount;	///事件总数
	tITEMINDEX index[EVT_ITEM_SIZE];	///事件索引数组
} tEVT_HEADINFO;

///事件信息存储格式
typedef struct
{
	u32	prefix;	///前缀
	tEVT_HEADINFO	headInfo;
	u8 u8CRC; ///校验码
} tPARAM_EVT_HEADINFO_STORAGE;

///点播菜单存储头
typedef struct
{
	u8 u8Amount;	///事件总数
	tITEMINDEX index[MSGOD_MENU_ITEM_SIZE];	///事件索引数组
} tMSGODMENU_HEADINFO;

///点播菜单存储格式
typedef struct
{
	u32	prefix;	///前缀
	tMSGODMENU_HEADINFO	headInfo;
	u8 u8CRC; ///校验码
} tPARAM_MSGOD_MENU_HEADINFO_STORAGE;

///点播信息存储头
typedef struct
{
	u8 u8Amount;	///事件总数
	tITEMINDEX index[MSGOD_MSG_ITEM_SIZE];	///事件索引数组
} tMSGODMSG_HEADINFO;

///点播信息存储格式
typedef struct
{
	u32	prefix;	///前缀
	tMSGODMSG_HEADINFO	headInfo;
	u8 u8CRC; ///校验码
} tPARAM_MSGOD_MSG_HEADINFO_STORAGE;

///电话本存储头
typedef struct
{
	u8 u8Amount;	///电话本总数
	tITEMINDEX index[MSGOD_MSG_ITEM_SIZE];	///索引数组
} tPHONEBOOK_HEADINFO;

///电话本存储格式
typedef struct
{
	u32	prefix;	///前缀
	tMSGODMSG_HEADINFO	headInfo;
	u8 u8CRC; ///校验码
} tPARAM_PHONEBOOK_HEADINFO_STORAGE;

/** 电话本 **/
#define		PHONE_NUM_SIZE		16		///电话号码长度
#define		PHONE_NAME_SIZE		0xFF	///姓名长度
#define		PHONE_ITEM_SIZE		50		///电话号码数目

///电话号码项
typedef struct
{
	u8	u8Flag;		///1：呼入；2：呼出；3：呼入/呼出
	u8	u8PhoneLen;		///电话号码长度
	u8	aPhoneNum[PHONE_NUM_SIZE];	///电话号码内容
	u8	u8NameLen;		///姓名长度
	u8	aName[PHONE_NAME_SIZE];		///姓名内容
}tPHONEBOOK_ITEM;


/** 圆形区域 ****************************************************************/
#define PARAM_ROUNDAREA_MAXITEMS	24 ////圆形区域最大项数
/**圆形区域项扩展结构体，存储格式，增加项有效标志*/
typedef	struct
{
	u8  u8IsValid;	///有效标志，0：无效，1：有效，默认有效；增加此字段是区别该区域是否被删除
	tItem_RoundArea tItem;
} tItem_RoundArea_Ext;

typedef	struct
{
//	u8	u8AreaProperty;		///圆形区域属性,eAREATYPE
	u8	u8Amount;	///区域总数
	tItem_RoundArea_Ext atItems[PARAM_ROUNDAREA_MAXITEMS];	///区域项
} tPARAM_ROUNDAREA;

/**圆形区域存储格式*/
typedef struct
{
	u32	prefix;	///前缀
	tPARAM_ROUNDAREA	tRoundArea;
	u8 u8CRC; ///校验码
} tPARAM_ROUNDAREA_STORAGE;

/** 矩形区域 ****************************************************************/
#define PARAM_RECTAREA_MAXITEMS	24 ///最大项目条数

/**区域项扩展结构体，存储格式，增加项有效标志*/
typedef	struct
{
	u8  u8IsValid;	///有效标志，0：无效，1：有效，默认有效；增加此字段是区别该区域是否被删除
	tItem_RectangleArea tItem;
} tItem_RectArea_Ext;

typedef	struct
{
//	u8	u8AreaProperty;		///设置矩形区域属性
	u8	u8AreaTotalCount;	///区域总数
	tItem_RectArea_Ext	atItems[PARAM_RECTAREA_MAXITEMS];
} tPARAM_RECTAREA;

/***矩形区域存储格式*/
typedef struct
{
	u32	prefix;	///前缀
	tPARAM_RECTAREA	tRectArea;
	u8 u8CRC; ///校验码
} tPARAM_RECTAREA_STORAGE;

/** 多边形区域 **************************************************************/
#define PARAM_POLYGONAREA_ITEMS_MAX	24 ///最大项目条数
#define PARAM_POLYGONAREA_POINT_MAX	40 ///多边形定点数

/**项扩展结构体，存储格式，增加项有效标志*/
typedef	struct
{
	u8  u8IsValid;	///有效标志，0：无效，1：有效，默认有效；增加此字段是区别该区域是否被删除
	tItem_PolygonArea tHead;
	tPOINT	points[PARAM_POLYGONAREA_POINT_MAX];	///顶点项
} tItem_Polygon_Ex;

typedef struct
{
	u8	u8Amount;
	tItem_Polygon_Ex	atItems[PARAM_POLYGONAREA_ITEMS_MAX];
} tPARAM_POLYGONAREA;

/**多边形区域存储格式*/
typedef struct
{
	u32	prefix;	///前缀
	tPARAM_POLYGONAREA	tPolygonArea;
	u8 u8CRC; ///校验码
} tPARAM_POLYGONAREA_STORAGE;

/** 路线 **************************************************************/
#define PARAM_ROUTE_POINT_MAX	40
#define PARAM_ROUTE_ITEM_MAX	24

typedef	struct
{
	u32	u32LineId;		///路线ID
	u16	u16LineProperty;	///路线属性
	tBCDTime tBcdStartTime;	///起始时间，若区域属性0位为0则没有该字段
	tBCDTime tBcdEndTime;	///结束时间，若区域属性0位为0则没有该字段
	u16	u16InflectionAmount;	///路线总拐点数
	tCORNERPOINT ptItem[PARAM_ROUTE_POINT_MAX]; 	///拐点项
}tITEM_CORNERPOINT;

/**项扩展结构体，存储格式，增加项有效标志*/
typedef	struct
{
	u8  u8IsValid;	///有效标志，0：无效，1：有效，默认有效；增加此字段是区别该区域是否被删除
	tITEM_CORNERPOINT	point;	///顶点项
} tITEM_CORNERPOINT_Ex;

typedef struct
{
	u8	u8Amount;
	tITEM_CORNERPOINT_Ex	atItems[PARAM_ROUTE_ITEM_MAX];
} tPARAM_ROUTE;

/**多边形区域存储格式*/
typedef struct
{
	u32	prefix;	///前缀
	tPARAM_ROUTE	tRoute;
	u8 u8CRC; ///校验码
} tPARAM_ROUTE_STORAGE;


/** 驾驶员信息 **********************************************************************/
///车辆对应的驾驶员信息
typedef	struct
{
	u8 au8DriverId[8];			///驾驶员代码，从IC卡中读取，或者从平下发命令
	u8 u8DriverNameLength;		///驾驶员姓名长度
	u8 au8DriveName[20];		///驾驶员姓名
	u8 au8DriverIdCode[20];		///驾驶员身份证编码
	u8 au8QulifiedCode[40];		///驾驶员从业资格证编码
	u8 u8GovNameLength;			///发证机构名称长度
	u8 au8GovName[20];			///发证机构名称
}tDRIVERINFO;


///一辆车对应的最多驾驶员数目
#define	ONE_VEHICLE_DRIVER_NUNBER_MAX		6
typedef	struct
{
	u8 	u8CurDriver;		///当前驾驶员 0xFF表示无效
	u8 	u8DriverCnt;		///有效驾驶员个数
	tDRIVERINFO	 tDriverInfo[ONE_VEHICLE_DRIVER_NUNBER_MAX]; 	///驾驶员列表
	u8 	u8CRC;
}tVEHICLEDRIVERType;

/**
 * 恢复出厂参数，recovery system default setting from EEPROM
 *
 * @return 	int，0=成功，-1=失败
 */
int RecoverSystemDefaultParam(void);

/**
 * @brief	载入所有系统参数
 * @return	int，0=成功，-1=失败
 */
int Load_CommonParams(void);

/**
 * @brief	保存所有系统参数
 * @return	int，0=成功，-1=失败
 */
int Save_CommonParams(void);

/**
 * @brief	读取指定类型参数
 * @param 	eType	参数类型
 * @return	int，0=成功，-1=失败
 */
int Load_Params(ePARAMTYPE eType);

/**
 * @brief	保存指定类型参数
 * @param 	eType 参数类型
 * @return	int，0=成功，-1=失败
 */
int Save_Params(ePARAMTYPE eType);

/**
 * 设置系统参数
 * @param id	参数类型
 * @param len	参数长度
 * @param pVal	参数值
 * @return	0=成功，-1=失败
 */
int Set_SystemParam(u32 id, u8 len, u8 *pVal);

/**
 * 读取点播信息，从EEPROM载入
 *
 * @return
 */
int PARAM_LoadMsgOD();

/**
 * 保存点播消息，写入EEPROM
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


/** 事件/点播菜单/点播消息/电话本外部接口 *******************************************/
int PARAM_FormatMsg_GetHead(u8 type);
int PARAM_FormatMsg_SaveHead(u8 type);
int PARAM_FormatMsg_GetAmount(u8 type);
int PARAM_FormatMsg_GetItem(u8 type, u8 u8ItemIndex, u8* pu8ItemId, u8 *pu8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_ApendItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_ReplaceItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_EraseAll(u8 type);

/** 圆形/矩形/多边形/外部接口 ******************************************************/
int PARAM_Area_Load(u8 type);
int PARAM_Area_EraseAll(u8 type);
int PARAM_Area_Save(u8 type);
int PARAM_Area_AppendItem(u8 type, u32 u32Id, u8 *pItem, u16 u16ItemLen);
int PARAM_Area_ReplaceItem(u8 type, u32 u32Id, u8 *pItem, u16 u16ItemLen);
int PARAM_Area_DeleteItem(u8 type, u32 u32Id);

u16 GetItem_AllSystemParam(u8 *pData);



#pragma pack()
#endif   ///_GPS_PARAMETER_H_
