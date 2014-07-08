#ifndef	_PARAMETER_H_
#define	_PARAMETER_H_

#include "stm32f2xx_conf.h"
#include "protocol.h"
#include "config.h"
#include <fram.h>

#pragma pack(1)


/**系统参数类*/
typedef enum {
	PARAM_FACTORY_SYSTEM 	= 0,	////出厂参数：系统参数
	PARAM_FACTORY_DEVICE,	////出厂参数：设备参数
	PARAM_DEVICE,			////设备信息
	PARAM_SYSTEM,			////系统信息
	PARAM_RUNTIME,			////运行信息
	PARAM_UPGRADE,			///升级信息
	PARAM_TYPEEND,
} eSYSPARAM;

///格式化参数类
typedef enum {
	PARAM_MSGODMENU = 0,		////点播菜单
	PARAM_MSGODINFO,		////点播消息
	PARAM_EVENT,			///事件
	PARAM_AREA_RECTANGLE,	///矩形
	PARAM_AREA_ROUND,		///圆形
	PARAM_AREA_POLYGON,		///多边形
	PARAM_AREA_ROUTE,		///路线
	PARAM_DRIVER,			///驾驶人信息
	PARAM_PHONEBOOK,		///电话本

	PARAM_FMTEND,
} eFMTMSG_PARAM;

///各类信息在EEPROM中的存储首地址
#define EEPROM_ADDR_FACTORY_SYSTEM		0x3333	///出厂参数-系统参数在EEPROM中首地址
#define EEPROM_ADDR_FACTORY_DEVICE		0x3333	///出厂参数-设备参数在EEPROM中首地址
#define EEPROM_ADDR_DEVICE		0x3333	///设备参数在EEPROM中首地址
#define EEPROM_ADDR_SYSTEM		0x3333	///系统参数在EEPROM中首地址
#define EEPROM_ADDR_RUNTIME		0x3333	///运行参数在EEPROM中首地址
#define EEPROM_ADDR_DRIVER		0x3333	///驾驶人信息在EEPROM中首地址
#define EEPROM_ADDR_UPGRADE		0x3333	///升级参数在EEPROM中首地址

#define EEPROM_ADDR_EVENT		0x3333	///事件信息在EEPROM中首地址
#define EEPROM_ADDR_MSGOD_MENU	0x3333	///点播菜单信息在EEPROM中首地址
#define EEPROM_ADDR_MSGOD_MSG	0x3333	///点播信息在EEPROM中首地址
#define EEPROM_ADDR_PHONEBOOK	0x3333	///电话本在EEPROM中首地址

#define EEPROM_ADDR_ROUND		0x3333	///圆形区域在EEPROM中首地址
#define EEPROM_ADDR_RECTANGLE	0x3333	///矩形区域在EEPROM中首地址
#define EEPROM_ADDR_POLYGON		0x3333	///多边形区域在EEPROM中首地址
#define EEPROM_ADDR_ROUTE		0x3333	///路线在EEPROM中首地址

#define EVT_MSG_SIZE			50 	///事件内容最大长度
#define EVT_ITEM_SIZE			50	///事件最大个数
#define MSGOD_MENU_MSG_SIZE		20 	///点播菜单内容长度
#define MSGOD_MENU_ITEM_SIZE	50 	///点播菜单条数
#define MSGOD_MSG_MSG_SIZE		50	///点播消息内容长度
#define MSGOD_MSG_ITEM_SIZE		50	///点播消息条数
#define PHONEBOOK_MSG_SIZE		20	///电话本单条记录长度
#define PHONEBOOK_ITEM_SIZE		20	///电话本记录总条数

#define ROUND_AREA_ITEM_SIZE	50	///圆形区域项总数
#define RECT_AREA_ITEM_SIZE		24	///矩形区域项总数
#define POLYGON_AREA_ITEM_SIZE	50	///多边形区域项总数


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
	ePARAM_ICVERIFY_MS_IPOrDNS = 17,///		=	0x001A,		///道路运输证IC卡认证主服务器IP地址或域名
	ePARAM_ICVERIFY_TCPPORT,///			=	0x001B,		///道路运输证IC卡认证主服务器TCP端口
	ePARAM_ICVERIFY_UDPPORT,///			=	0x001C,		///道路运输证IC卡认证主服务器UDP端口
	ePARAM_ICVERIFY_BS_UDPPORT,	///		=	0x001D,		///道路运输证IC卡备份服务器IP地址或域名，端口同主服务器
#endif///JTT808_Ver_2013

	ePARAM_POSRPT_STRATEGE = 21,	///	终端位置汇报策略
	/**终端汇报方案，0:根据ACC状态，1:根据登录状态和ACC状态，先判断登录状态，
	 若登录再根据ACC状态*/
	ePARAM_POSRPT_METHOD = 22,	///				=	0x0021,
	ePARAM_DRV_UNREG_RPTPERIOD = 23,///		=	0x0022,		///驾驶员未登录汇报时间间隔，单位为秒
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
	ePARAM_RESET_PHONENUM,///			=	0x0041,		///复位电话号码，可采用此电话号码拨打终端电话让终端复位
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
	ePARAM_SUPERVISE_SMS_NUM = 41,///			=	0x0049,			///监管平台特权短信号码
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
	ePARAM_MAXSPEED,///					=	0x0055,	   	///最高速度，单位为公里每小时(km/h)
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

/**参数内容校验前缀码，加上它是为了避免从外部存储器读参数发生错误时，内容和校验码
 * 全为0，则仍能通过校验，当做有效参数的情况*/
#define PARAM_PREFIX  	0x12345678
#define PREFIX_SIZE		(sizeof(u32))

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
typedef struct {
	u32 u32Id;		///参数ID
	u8 u8Len;		///参数长度
	union {
		u8 u8Val;
		u16 u16Val;
		u32 u32Val;
	} val;

} tPARAMITEM_FIXED;

#if 1
/**系统参数 *******************************************************************/
typedef struct {
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
	u8 IC_Verify_MS_IPOrDNS[STR_SIZE];	///道路运输证IC卡认证主服务器IP地址或域名
	u32 IC_Verify_TCP_Port;		///道路运输证IC卡认证主服务器TCP端口
	u32 IC_Verify_UDP_Port;		///道路运输证IC卡认证主服务器UDP端口
	u8 IC_Verify_BS_IPOrDNS[STR_SIZE];	///道路运输证IC卡备份服务器IP地址或域名，端口同主服务器
#endif

	/**位置汇报参数*/
	u32 u32PosRptStrategy;	///位置汇报策略
	u32 u32PosRptMethod;	///位置汇报方案
	u32 u32RptIntervalDrvUnReg;	///驾驶员未登录汇报时间间隔，单位 为秒(s)
	u32 u32RptIntervalSleep;	///休眠汇报时间间隔，单位 为秒(s)
	u32 u32RptIntervalEmergency;	///紧急汇报时间间隔，单位 为秒(s)
	u32 u32RptIntervalDefault;		///缺省汇报时间间隔，单位 为秒(s)
	u32 u32RptDistanceDefault;	///缺省汇报距离间隔，单位 为米(m)
	u32 u32RptDistanceDrvUnReg;		///驾驶员未登录汇报距离间隔，单位 为米(m)
	u32 u32RptDistanceSleep;	///休眠汇报距离间隔，单位 为米(m)
	u32 u32RptDistanceEmergency;	///紧急汇报距离间隔，单位 为米(m)
	u32 u32InflectionSuppleAngle;	///拐点补传角度，单位为度, < 180
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
	u32 u32SendSMSOnAlarmCtrl;///报警发送文本SMS开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时发送文本SMS
	u32 u32CaptureOnAlarmCtrl;	///报警拍摄开关，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时摄像头拍摄
	u32 u32AlarmPictureStorageCtrl;	///报警拍摄存储标志，与位置信息汇报消息中的报警标志位相对应，相应位为1则相应报警时拍的照片进行存储，否则实时上传
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
	u16 u16CollisionSetting; /**碰撞报警参数设置，b7-b0： 碰撞时间，单位4ms；
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
	u32 u32Kilometers;		///车辆行驶里程
	u16 u16ProviceId;		///省域ID
	u16 u16CountyId;		///市县域ID
	u8 au8Plate[PLATE_SIZE];		///车牌
	u8 u8Color;		///车牌颜色

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
//	u8 u8CAN_SingleRetrieveSetting[8];	///0x0110
	tCONF_CAN	tCanConf;	///0x0110

	/**0x0111-0x01FF BYTE[8] 用于其他CAN 总线ID 单独采集设置*/
	u8 u8CAN_SingleRetrieveSetting_Others[8]; ///0x0111-0x01FF

/**0xF000-0xFFFF 用户自定义*/

#endif

} tPARAM_SYSTEM;
#endif

/** 系统参数存储结构体 */
typedef struct {
	u32 prefix;	///前缀
	tPARAM_SYSTEM tSystemParam; /**系统参数*/
	u8 u8CRC;
} tPARAM_SYSTEM_STORAGE;

#define PARAM_AUTHORCODE_LEN	100  	/**终端鉴权码长度*/

/** 设备属性参数 *****************************************************************/
typedef struct {
	u16 u16ProviceId;		///省域ID
	u16 u16CountyId;		///市县域ID
	u8 au8FactoryId[5];	///制造商ID
#ifdef JTT808_Ver_2011
	u8 aType[8];	///终端型号，8个字节，此终端型号由制造商自行定义，位数不	足时，补空格。
	u8 aTId[7];///终端ID,7 个字节，由大写字母和数字组成，此终端ID 由制造商自行定义，位数不足时，后补“0X00”。
	u8 u8PlateColor;///车牌颜色，按照JT/T415-2006 的5.4.12
#endif
#ifdef JTT808_Ver_2013
	u8 aType[20];		///终端型号20 个字节，此终端型号由制造商自行定义，位数不足时，后补“0X00”。
	u8 aTId[7];		///终端ID
	u8 u8PlateColor;		///车牌颜色，按照JT/T415-2006 的5.4.12。未上牌时，取值为0。
#endif///JTT808_Ver_2013

	u8 aPlate[PARAM_VIN_PLATE_MAXLEN]; /**V2011： 机动车号牌;
										 V2013：车牌车牌颜色为0 时，表示车辆VIN；否则，表示公安 交通管理部门颁发的机动车号牌。*/
#ifdef JTT808_Ver_2013
//	bit0，0：不适用客运车辆，1：适用客运车辆；
//	bit1，0：不适用危险品车辆，1：适用危险品车辆；
//	bit2，0：不适用普通货运车辆，1：适用普通货运车辆；
//	bit3，0：不适用出租车辆，1：适用出租车辆；
//	bit6，0：不支持硬盘录像，1：支持硬盘录像；
//	bit7，0：一体机，1：分体机。
	u16 tType; /**终端类型*/ ///tTerminalType
#endif

	u8 ver_hardware[50];	///终端硬件版本号 STRING
	u8 ver_software[50];	///终端软件版本号 STRING

	u8 tGNSSModuleProperty; /**GNSS模块属性*/
	u8 tCommModuleProperty; /**通信模块属性*/

} tPARAM_DEVICE;

/**设备参数存储结构体*/
typedef struct {
	u32 prefix;	///前缀
	tPARAM_DEVICE tDeviceParam; ///设备参数
	u8 u8CRC; ///校验码
} tPARAM_DEVICE_STORAGE;

/** 运行参数 *****************************************************************/
#define ETICKET_MAXLEN	50	///电子运单最大长度

/**电子运单信息*/
typedef struct {
	u32 u32ETicketLen;	///电子运单长度
	u8 aData[ETICKET_MAXLEN];	///电子运单数据包
} tETICKET;

/**驾驶员信息*/
typedef struct {

#ifdef JTT808_Ver_2011
	u8 u8DriverNameLen;	///驾驶员姓名长度
	u8 aDriverName[20];///驾驶员姓名，长度n
	u8 aDrvIdCode[20];///驾驶员身份证编码
	u8 aQulCode[40];///从业资格证编码
	u8 u8GovNameLength;///发证机构名称长度
	u8 aGovName[40];///发证机构名称
#endif

#ifdef JTT808_Ver_2013
	u8 state; /** 状态 BYTE 0x01：从业资格证IC 卡插入（驾驶员上班）；
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
	u8 u8DriverNameLen;	///驾驶员姓名长度
	u8 aDriverName[20];		///驾驶员姓名，长度n
	u8 aQulCode[20];		///从业资格证编码,不足补0
	u8 u8GovNameLength;	///发证机构名称长度
	u8 aGovName[40];		///发证机构名称
	u8 LicieceValidation[4];	///证件有效期 BCD[4] YYYYMMDD
#endif
} tDRIVERINFO;

/**运行时参数*/
typedef struct {
	u8 u8CommMode;		///通讯模式，缺省为TCP
	u8 aServerIP[4];	///服务器IP
	u16 u16Server_Port;		///服务器端口号

	u8 u8ConnectionCtrl;	///连接控制
	char sURL[100];	///URL地址
	char sPPP_name[10];	///拨号点名称;
	char sPPP_user[10];	///拨号用户名;
	char sPPP_password[10]; ///拨号密码;
	char sIP[30]; ///地址;IP或DNS
	u16 u16TCP_port; ///TCP 端口;
	u16 u16UDP_port; ///UDP 端口;
	char sManufacture_ID[20]; ///制造商ID;
	char sHardware_Version[30]; ///硬件版本;
	char sFirmware_Version[30]; ///固件版本;
	u16 u16TOUT_ServerConnect; ///连接到指定服务器时限,单位分

	u8 sAuthorCode[PARAM_AUTHORCODE_LEN]; /**终端鉴权码*/
	u8 ICCID[10];		///终端SIM 卡ICCID BCD[10]
	u8 aPhoneNum[6];	///终端手机号,BCD码

	u16 u16UpReportInterval;	///呼叫上报-上报周期，单位秒
	u16 u16KeepOnTime;		///呼叫上报-持续时长，单位秒

	tWARNINGSTATE tWarningState; /**报警标志位*/
	tSYSTEMSTATE tSysState; /**状态位*/
	tGPSINFO tGpsInfo; ///GPS位置信息
	tETICKET tEticket;	///电子运单
	tDRIVERINFO tDriverInfo;	///驾驶人信息

	u8 bOverSpeed;	///超速判断
	u32 u32Kilemeters;	///0x01 4 里程，DWORD，1/10km，对应车上里程表读数
	u16 u16Oil; 	///0x02 2 油量，WORD，1/10L，对应车上油量表读数
	u16 u16Speed;	///0x03 2 行驶记录功能获取的速度，WORD，1/10km/h
	u16 u16WarningEvtId; 	///0x04 2 需要人工确认报警事件的ID，WORD，从1 开始计数
	tAppendInfo_OverspeedWarning tOverspeedWarn; ///1 或5 超速报警附加信息见表28
	tAppendInfo_BarrierAndPathWarning tBarrierWarn; ///6 进出区域/路线报警附加信息见表29
	tAppendInfo_DrivingTimeInadequetWarning tDrvTimeWarn; ///0x13 7 路段行驶时间不足/过长报警附加信息见表30
	u32 u32ExtSignalState;	///0x25 4 扩展车辆信号状态位，定义见表31
	u16 u16IOState;	///0x2A 2 IO状态位，定义见表32
	u32 u32ADValue;	///0x2B 4 模拟量，bit0-15，AD0；bit16-31，AD1。
	u8 u8Csq;	///0x30 1 BYTE，无线通信网络信号强度
	u8 u8GNSS_Satellites; ///0x31 1 BYTE，GNSS 定位卫星数

	/**定位数据批量存储头指针*/




} tPARAM_RUNTIME;

/**运行参数存储结构体*/
typedef struct {
	u32 prefix;	///前缀
	tPARAM_RUNTIME tRuntimeParam; ///运行参数
	u8 u8CRC; ///校验码
} tPARAM_RUNTIME_STORAGE;


/** 事件/点播菜单/点播消息/电话本参数  ***********************************************************/

/** 事件 ****************************************************************/
/*存储容量：53 * 50 + 1 = 26501=26K*/
typedef struct {
	u8 u8EventId;		///事件ID,若终端已有同ID的事件，则被覆盖
	u8 u8EventLen;		///事件长度
	u8 aEventMsg[EVT_MSG_SIZE];		///事件内容
} tEVENTITEM;

typedef struct {
	u8 u8Amount;	///事件总数
	tEVENTITEM index[EVT_ITEM_SIZE];	///事件索引数组
} tEVT_INFO;

///事件信息存储格式
typedef struct {
	u32 prefix;	///前缀
	tEVT_INFO tEvt;
	u8 u8CRC; ///校验码
} tPARAM_EVT_STORAGE;

/** 点播菜单 ****************************************************************/
/*存储容量：23 * 50 + 1 = 1151=2K*/
///信息点播信息项定义
typedef struct {
	u8 u8InfoType;			///信息类型
	u16 u16InfoNameLen;		///信息名称长度
	u8 aInfoName[20];		///信息名称
} tMSGODMENUITEM;

///点播菜单存储头
typedef struct {
	u8 u8Amount;	///事件总数
	tMSGODMENUITEM index[MSGOD_MENU_ITEM_SIZE];	///事件索引数组
} tMSGODMENU_INFO;

///点播菜单存储格式
typedef struct {
	u32 prefix;	///前缀
	tMSGODMENU_INFO tMsgODMenu;
	u8 u8CRC; ///校验码
} tPARAM_MSGOD_MENU_STORAGE;

/** 点播信息 ****************************************************************/
/*存储容量：53 * 50 + 1 = 26501=26K*/
///点播信息项定义
typedef struct {
	u8 u8InfoType;			///信息类型
	u16 u16InfoNameLen;		///信息名称长度
	u8 aInfoName[MSGOD_MSG_MSG_SIZE];		///信息名称
} tMSGODITEM;

///点播信息存储头
typedef struct {
	u8 u8Amount;	///事件总数
	tMSGODITEM index[MSGOD_MSG_ITEM_SIZE];	///事件索引数组
} tMSGODMSG_INFO;

///点播信息存储格式
typedef struct {
	u32 prefix;	///前缀
	tMSGODMSG_INFO tMsgODMsg;
	u8 u8CRC; ///校验码
} tPARAM_MSGOD_MSG_STORAGE;


/** 电话本 ****************************************************************/
/*存储容量：40*50=2K*/
#define		PHONE_NUM_SIZE		16	///电话号码长度
#define		PHONE_NAME_SIZE		20///50	///姓名长度
#define		PHONE_ITEM_SIZE		50	///电话号码数目

///电话号码项
typedef struct {
	u8 u8Flag;		///1：呼入；2：呼出；3：呼入/呼出
	u8 u8PhoneLen;		///电话号码长度
	u8 aPhoneNum[PHONE_NUM_SIZE];	///电话号码内容
	u8 u8NameLen;		///姓名长度
	u8 aName[PHONE_NAME_SIZE];		///姓名内容
} tPHONEBOOK_ITEM;

///电话本存储头
typedef struct {
	u8 u8Amount;		///联系人总数
	tPHONEBOOK_ITEM tPhoneBook[PHONE_ITEM_SIZE];	///联系人项
} tPHONEBOOK_INFO;

///电话本存储格式
typedef struct {
	u32 prefix;	///前缀
	tPHONEBOOK_INFO tPhoneBook;
	u8 u8CRC; ///校验码
} tPARAM_PHONEBOOK_STORAGE;

/** 圆形区域 ****************************************************************/
/*存储容量： 24 * 37 < 960B = 1K*/
#define PARAM_ROUNDAREA_MAXITEMS	24 ////圆形区域最大项数
/**圆形区域项扩展结构体，存储格式，增加项有效标志*/
typedef struct {
	u8 u8IsValid;	///有效标志，0：无效，1：有效，默认有效；增加此字段是区别该区域是否被删除
	tItem_RoundArea tItem;
} tItem_RoundArea_Ext;

typedef struct {
//	u8	u8AreaProperty;		///圆形区域属性,eAREATYPE
	u8 u8Amount;	///区域总数
	tItem_RoundArea_Ext atItems[PARAM_ROUNDAREA_MAXITEMS];	///区域项
} tPARAM_ROUNDAREA;

/**圆形区域存储格式*/
typedef struct {
	u32 prefix;	///前缀
	tPARAM_ROUNDAREA tRoundArea;
	u8 u8CRC; ///校验码
} tPARAM_ROUNDAREA_STORAGE;

/** 矩形区域 ****************************************************************/
/*存储容量：37*24 = 960B=1K*/
#define PARAM_RECTAREA_MAXITEMS	24 ///最大项目条数

/**区域项扩展结构体，存储格式，增加项有效标志*/
typedef struct {
	u8 u8IsValid;	///有效标志，0：无效，1：有效，默认有效；增加此字段是区别该区域是否被删除
	tItem_RectangleArea tItem;
} tItem_RectArea_Ext;

typedef struct {
//	u8	u8AreaProperty;		///设置矩形区域属性
	u8 u8AreaTotalCount;	///区域总数
	tItem_RectArea_Ext atItems[PARAM_RECTAREA_MAXITEMS];
} tPARAM_RECTAREA;

/***矩形区域存储格式*/
typedef struct {
	u32 prefix;	///前缀
	tPARAM_RECTAREA tRectArea;
	u8 u8CRC; ///校验码
} tPARAM_RECTAREA_STORAGE;

/** 多边形区域 **************************************************************/
/*存储容量：344*24= 1376 + 6880 = 7216 = 7K*/
#define PARAM_POLYGONAREA_ITEMS_MAX	24 ///最大项目条数
#define PARAM_POLYGONAREA_POINT_MAX	40 ///多边形定点数

/**项扩展结构体，存储格式，增加项有效标志*/
typedef struct { ///24+ 320 = 344B
	u8 u8IsValid;	///有效标志，0：无效，1：有效，默认有效；增加此字段是区别该区域是否被删除
	tItem_PolygonArea tHead;
	tPOINT points[PARAM_POLYGONAREA_POINT_MAX];	///顶点项
} tItem_Polygon_Ex;

typedef struct {
	u8 u8Amount;
	tItem_Polygon_Ex atItems[PARAM_POLYGONAREA_ITEMS_MAX];
} tPARAM_POLYGONAREA;

/**多边形区域存储格式*/
typedef struct {
	u32 prefix;	///前缀
	tPARAM_POLYGONAREA tPolygonArea;
	u8 u8CRC; ///校验码
} tPARAM_POLYGONAREA_STORAGE;

/** 路线 **************************************************************/
/*存储容量：1020*24=24K*/
#define PARAM_ROUTE_POINT_MAX	40
#define PARAM_ROUTE_ITEM_MAX	24

/**角点结构体*/
typedef struct { ///25*40+20=1020B
	u32 u32LineId;		///路线ID
	u16 u16LineProperty;	///路线属性
	tBCDTime tBcdStartTime;	///起始时间，若区域属性0位为0则没有该字段
	tBCDTime tBcdEndTime;	///结束时间，若区域属性0位为0则没有该字段
	u16 u16InflectionAmount;	///路线总拐点数
	tCORNERPOINT ptItem[PARAM_ROUTE_POINT_MAX]; 	///拐点项
} tITEM_CORNERPOINT;

/**项扩展结构体，存储格式，增加项有效标志*/
typedef struct {
	u8 u8IsValid;	///有效标志，0：无效，1：有效，默认有效；增加此字段是区别该区域是否被删除
	tITEM_CORNERPOINT point;	///顶点项
} tITEM_CORNERPOINT_Ex;

typedef struct {
	u8 u8Amount;
	tITEM_CORNERPOINT_Ex atItems[PARAM_ROUTE_ITEM_MAX];
} tPARAM_ROUTE;

/**多边形区域存储格式*/
typedef struct {
	u32 prefix;	///前缀
	tPARAM_ROUTE tRoute;
	u8 u8CRC; ///校验码
} tPARAM_ROUTE_STORAGE;

/** 驾驶员信息 **********************************************************************/
///车辆对应的驾驶员信息
typedef struct {
	u8 au8DriverId[8];			///驾驶员代码，从IC卡中读取，或者从平台下发命令
	u8 u8DriverNameLength;		///驾驶员姓名长度
	u8 au8DriveName[20];		///驾驶员姓名
	u8 au8DriverIdCode[20];		///驾驶员身份证编码
	u8 au8QulifiedCode[40];		///驾驶员从业资格证编码
	u8 u8GovNameLength;			///发证机构名称长度
	u8 au8GovName[20];			///发证机构名称
} tITEM_DRIVER;

///一辆车对应的最多驾驶员数目
#define	DRIVERS_PERCAR_MAXNUM		6
typedef struct {
	u8 u8Amount;
	u8 u8CurDriver;		///当前驾驶员 0xFF表示无效
//	u8 u8DriverCnt;		///有效驾驶员个数
	tITEM_DRIVER tDriverInfo[DRIVERS_PERCAR_MAXNUM]; 	///驾驶员列表
} tPARAM_DRIVER;

/**驾驶人信息存储格式*/
typedef struct {
	u32 prefix;	///前缀
	tPARAM_DRIVER tDrivers;
	u8 u8CRC; ///校验码
} tPARAM_DRIVER_STORAGE;

/** 升级 *****************************************************************/
#ifdef JTT808_Ver_2013
/**升级类型*/
typedef enum {
	UPG_TERMINAL = 0, ///终端
	UPG_IC_Reader = 12, ///道路运输证IC 卡读卡器
	UPG_GNSS = 52, ///北斗卫星定位模块
} eUPG_TYPE;

/** 升级信息 */
typedef struct {
	u8 upg_type;	///升级类型 0：终端，12：道路运输证IC 卡读卡器，52：北斗卫星定位模块
	u8 factoryId[5];	///制造商ID BYTE[5] 制造商编号
	u8 version_len;		///版本号长度 BYTE n
	u8 version[50];		///版本号 STRING
	u32 u32UpgPack_len;	///7+n 升级数据包长度 DWORD 单位为BYTE
	u8 result;     ///升级结果  0：成功，1：失败，2：取消
} tUPGRADEINFO;

/**升级信息存储结构体*/
typedef struct {
	u32 prefix;	///前缀
	tUPGRADEINFO tUpgradeInfo; ///运行参数
	u8 u8CRC; ///校验码
} tUPGRADEINFO_STORAGE;
#endif

/** 出厂参数  *****************************************************************/
typedef struct {

	tPARAM_SYSTEM tSystemParam;
	tPARAM_DEVICE tDeviceParam;
	tPARAM_RUNTIME tRuntimeParam;
} tPARAM_FACTORY;

/**出厂参数存储结构体*/
typedef struct {
	u32 prefix;	///前缀
	tPARAM_FACTORY tFactoryParam; ///出厂参数
	u8 u8CRC; ///校验码
} tPARAM_FACTORY_STORAGE;


/** 盲区补传定位数据历史记录索引  ************************************************************/
#define EEPROM_ADDR_POSITION_RECORDER		0x333	///定位数据索引在FRAM中的存储位置
/**盲区补传定位数据历史记录存储头*/
typedef struct {
	u16 u16StartBlockId;  ///起始块号
	u8	u8BlockNums;	///总块数
	u16 u16WriteBlockId;///当前写块号
	u8  u8WritePageId;	///写页号
	u8	u8ReadPageId;	///读页号
	u16 u16WCursor;		///写指针
} tBLANDPOSINFO;

/**出厂参数存储结构体*/
typedef struct {
	u32 prefix;	///前缀
	tBLANDPOSINFO tBlandPosInfo; ///出厂参数
	u8 u8CRC; ///校验码
} tBLANDPOSINFO_STORAGE;

int PARAM_LoadPositionRecorderIndex(); ///读取索引头
int PARAM_SavePostionRecorderIndex();	///保存索引头


/** 外部全局访问参数对象  *****************************************************************/
extern tPARAM_FACTORY *ptParam_Factory;
extern tPARAM_SYSTEM *ptParam_System;
extern tPARAM_DEVICE *ptParam_Device;
extern tPARAM_RUNTIME *ptParam_Runtime;
extern tPARAM_DRIVER *ptParam_Driver;
extern tUPGRADEINFO *ptUpgInfo;
extern tPARAM_RECTAREA *ptParam_Rect;
extern tPARAM_ROUNDAREA *ptParam_RoundArea;
extern tPARAM_POLYGONAREA *ptParam_Polygon;
extern tPARAM_ROUTE *ptParam_Route;
extern tEVT_INFO *ptParam_Evt;
extern tMSGODMENU_INFO *ptParam_MsgODMenu;
extern tMSGODMSG_INFO *ptParam_MsgODMsg;
extern tPHONEBOOK_INFO *ptParam_PhoneBook;
extern tBLANDPOSINFO *ptBlandPosInfo;	///盲区补传定位数据索引头

/** 外部访问接口  *****************************************************************/
int RecoverSystemDefaultParam(void);
int Load_CommonParams(void);
int Save_CommonParams(void);
int Get_SystemParam(u32 id, void* val);
int Set_SystemParam(u32 id, u8 len, u8 *pVal);
u16 GetItem_AllSystemParam(u8 *pData);
int PARAM_LoadSysParam(u8 type);
int PARAM_SaveSysParam(u8 type);
int PARAM_EraseSysParam(u8 type);
int PARAM_LoadFMTParam(u8 type);
int PARAM_SaveFMTParam(u8 type);
int PARAM_EraseFMTParam(eFMTMSG_PARAM type);
u8 PARAM_GetAmount(eFMTMSG_PARAM type);


/** 事件/点播菜单/点播消息/电话本外部接口 */
void PARAM_FormatMsg_ApendItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_ReplaceItem(u8 type, u8 u8ItemId, u8 u8ItemLen,	u8 *pItemMsg);

/** 圆形/矩形/多边形/外部接口 */
int PARAM_Area_AppendItem(u8 type, u32 u32Id, u8 *pItem, u16 u16ItemLen);
int PARAM_Area_ReplaceItem(u8 type, u32 u32Id, u8 *pItem, u16 u16ItemLen);
int PARAM_Area_DeleteItem(u8 type, u32 u32Id);

void PARAM_SetFactoryParams();

#pragma pack()

#endif   ///_GPS_PARAMETER_H_
