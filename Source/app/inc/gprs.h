/**   
* @Title: gprs.h  
* @Description: TODO(用一句话描述该文件做什么) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-14 上午9:42:51 
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
//检测UIM卡
#define		_UIM_DETECT			0

//#define 	BUFFER_LENGTH_GPRSFRAMERx					(1200)		//接收数据帧处理缓冲区大小
#define		MAX_DATA_LEN								1500
#define		BUFFER_LENGTH_SMSRx							160			//短信接收缓冲区

//网络连接状态
#define 	NET_STATE_UNKNOWN							0	 		//未知
#define		NET_STATE_LINKED							1			//网络已连接

//#define		GPRS_SMG_CNT_		10


//#define	LINK_MAINTAIN_CYCLE_SHORT					10				//等待服务器登录反馈短周期
//#define	TIMEOUT_SERVER_DISCONNECT					120				//链路没有连接超时
//#define	TIMEOUT_EM310_READY							3				//EM310允许发送新数据包
//
//#define	SIGNAL_DENSITY_CHECK_TIMEOUT				40				//查询信号强度
//
//#define	SOCKET_ID										0			//socket通道号
//#define	CDMA_IPSENDEX_MODE								1			//数据转换发送方式
//#define	CDMA_LOACL_PORT									8855		//本地端口号
//#define	CDMA_DELALL_READ								3			//删除短信标识，3表示删除所有短信


//网络状态
#define NET_STATE_REGISTED            0X01 //已注册
#define NET_STATE_CONNECTING          0X02 //连接服务器
#define NET_STATE_CONNECTED           0X03 //通信已建立
#define NET_STATE_SERVERCONNECTED     0X03 //通信已建立
#define NET_STATE_UNKOWN              0XFF //未知


//电话状态
typedef enum
{

	ePHONE_STATE_IDLE = 0,						//空闲
	ePHONE_STATE_DIAL,							//已拨号
	ePHONE_STATE_CALLING,						//对方来电
	ePHONE_STATE_TALKING,						//通话中
	ePHONE_STATE_BUSY,							//电话忙
	ePHONE_STATE_HANGING,						//挂机中
	ePHONE_STATE_BREAK							//挂断
}ePHONESTATE;

//电话控制状态
typedef enum
{
	ePHONE_CTRL_IDLE = 0,						//空闲
	ePHONE_CTRL_DIAL,						//请求拨号
	ePHONE_CTRL_ANSWER,						//请求接听
	ePHONE_CTRL_HANG						//请求挂断
}ePHONECTRL;

//电话响铃次数
#define	GPRS_PHONE_ANSWERCLIP_COUNTS   2	//来电响铃n次后接听

//语音监听状态
typedef enum
{
	ePHONE_MONITOR_IDLE = 0,						//空闲
	ePHONE_MONITOR_REQ,							//请求监听
	ePHONE_MONITOR_MONITORING				//正在监听
}ePHONEMONITOR;


//服务器连接状态定义
typedef enum
{
	eSERVER_CONN_IDLE = 0,						//无效
	eSERVER_CONN_UNCONND,						//未连接

	eSERVER_CONN_SERV_CONNED,					//已经连接
	eSERVER_CONN_SERVER_APP_CONNED                 //数据链路已经建立
}eSERVERCONNSTATE;


//服务器连接过程
typedef enum
{

	_eCONN_IDLE = 0,					//空闲
	_eBEGIN_CONN_PRMSRV,			//开始连接主服务器
	_eCONN_PRMSRV_FINISH,			//连接主服务器过程完成
	_eBEGIN_CONN_VICESRV,			//开始连接辅助服务器
	_eCONN_VICESRV_FINISH			//连接辅助服务器过程完成

}eCONNPROCEDURE;

//电源状态
typedef enum
{
	_eGPRS_POWER_IDLE 			= 0,						 //电源空
	_eGPRS_POWER_ON,									 //电源开
	_eGPRS_POWER_BEFOE_ONING,								 //电源开过程中
	_eGPRS_POWER_ONING,								 //电源开过程中
	_eGPRS_POWER_OFF,									 //电源关
	_eGPRS_POWER_OFFING,								 //电源关过程中
	_eGPRS_POWER_RESETING								 //电源复位过程中
} _eGPRSPOWERSTATE;

//电源请求状态
typedef enum
{
	_eGPRS_POWER_REQ_IDLE = 0,								  //无请求
	_eGPRS_POWER_REQ_OFF, 									 //请求关闭电源
	_eGPRS_POWER_REQ_ON,									 //请求打开电源
    _eGPRS_POWER_REQ_RESET						 			 //请求复位电源
} _eGPRSPOWERREQ;

//GSM休眠控制
#define	GPRS_SLEEP_IDLE									0			//空闲
#define	GPRS_SLEEP_PROCESS								1			//休眠处理中
#define	GPRS_SLEEP_OK									2			//休眠

//电话响铃次数
#define	GPRS_PHONE_ANSWERCLIP_COUNTS				2			//来电响铃n次后接听


//连接的服务器类型
#define	SERVER_PRIMARY									1  			//主服务器并保存
#define	SERVER_VICE										2			//备用服务器并保存
#define	SERVER_SPECIFIED_PRIM							3			//指定主服务器

#define	GPRS_DATA_CR_									0x0D
#define	GPRS_DATA_LF_									0x0A




//AT命令类型
typedef	enum
{
/*基本配置命令 2*/
	_ATCMD_AT = 0,			//AT			   0
	_ATCMD_ATE0,			//取消回显	 	   1

/*标识信息查询命令 5*/
	_ATCMD_I,				//产品标识信息查询	 2
	_ATCMD_CGMI,			//厂商信息查询		 3
	_ATCMD_CGMM,			//产品名称查询		 4
	_ATCMD_CGMR,			//查询软件版本号	 5
	_ATCMD_CGSN,			//ESN查询			 6

/*安全控制命令 1*/
	_ATCMD_CPIN,			//R-UIM 卡在位和 PIN1 码状态查询	7


/*网络服务接口命令*/
	_ATCMD_CSQ,				//检查当地的网络信号强度，31 最大，0 最小。	 8
	_ATCMD_CREG,			//网络注册命令								 9

/*数据业务类命令*/
	_ATCMD_CGACT,			//设置PDP激活状态							10
	_ATCMD_CGATT,			//设置GPRS服务状态							11

/*INTERNET服务接口命令 5*/
	_ATCMD_SETCONNTYPE,		//设置连接类型								12
	_ATCMD_SETAPN,			//设置APN									13
	_ATCMD_SETPWD,			//设置Password								14
	_ATCMD_SETUSR,			//设置User									15
	_ATCMD_SETID,			//建立服务ID								 16
	_ATCMD_SETSOCKET,		//建立服务Socket							 17
	_ATCMD_SETADDR,			//建立服务地址								 18
	_ATCMD_OPEN,			//打开连接									 19
	_ATCMD_CLOSE,			//关闭连接									 20
	_ATCMD_SEND,			//发送数据									 21
	_ATCMD_READ,			//读取数据									 22

	_ATCMD_IOMODE,			//设置IO模式								 23


/*短消息业务接口命令 5*/
 	_ATCMD_CNMI,			//设置短信接收上报的机制，如果不需要短信功能可以不设置		24
	_ATCMD_CPMS,			//设置短信存储位置										   25
 	_ATCMD_CMGD,			//删除短信												   26
	_ATCMD_CMGR,			//读取短信信息											   27
	_ATCMD_CMGL,			//列出当前选择短信存储器全部短信的序号和状态			   28
	_ATCMD_CMGF,			//设置短信格式											   29
/*电话业务接口命令*/
	_ATCMD_PHONE_TIPS,					//来电提醒
	_ATCMD_PHONE_DIAL,					//拨打电话
	_ATCMD_PHONE_ANSWERCALL,			//接通电话
	_ATCMD_PHONE_HANGCALL,			//挂断电话

/*工作模式设置命令*/
	_ATCMD_SETMODE,						//设置工作模式

	_ATCMD_GETSYSTIME,		//查询系统时间

	_ATCMD_NUM_
}_eATCMD;


typedef	enum
{
/*基本命令*/
	_ATACK_OK = 0,
	_ATACK_ATE0,

/*标识查询命令应答*/
	_ATACK_CGMI,
	_ATACK_CGMM,
	_ATACK_CGMR,
	_ATACK_CGSN,

/*安全控制命令应答*/
	_ATACK_CPIN,

/*网络服务接口命令应答*/
	_ATACK_CSQ,
	_ATACK_CGATT,

/*数据业务类命令应答*/
	_ATACK_CREG,

/*INTERNET服务接口命令应答*/
	_ATACK_SISW,
	_ATACK_SISR,


/*短消息业务接口命令应答*/
	_ATACK_CPMS,				//设置短信存储位置
	_ATACK_CNMI,				//设置新短信通知方式
	_ATACK_CMTI,				//收到短信提示
	_ATACK_CMGL,				//列举所有短信状态
	_ATACK_CMGR,				//阅读短信
	_ATACK_CMGD,				//删除短信

/*电话业务相关应答*/
	_ATACK_CLIP,				//来电提醒

	_ATACK_SYSTIME,				//系统时间

 /*其他应答*/
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

//AT错误定义
typedef enum
{
	_ATERR_UIM_NOT_INSERT_,						//UIM卡未插入
	_ATERR_CNN_ALREADY_ESTABLISHED_,			//连接已建立
	_ATERR_DEST_ADDR_REQ_,						//未指定目标地址
	_ATERR_END_,
	_ATERR_UNDEFIN_ = 0xFF
}_eATERR;


//AT命令执行结果
typedef	enum
{
	_ATRET_OK = 0,				//成功
	_ATRET_ERROR,				//失败
	_ATRET_TIMEOUT,			//超时
	_ATRET_PENDING,			//等待结果

}	_eATRET;

/*初始化命令序列*/
#if	_MSG_
#define	ATCMD_INIT_STEPS	    10		//初始化步骤
#else
#define	ATCMD_INIT_STEPS		7		//初始化步骤
#endif

#define	ATCMD_INIT_STEP1_		4		//初始化第一步步骤，当检测到UIM卡插入后，才开始进行后续的初始化过程
										//如果UIM卡检测不在位后，则后续的初始化过程就没有必要进行下去

#if	(_MSG_ == 0)
#define INIT_STEPS_AMOUNT	   6
#else
#define INIT_STEPS_AMOUNT	   9
#endif
#define CONNECT_STEPS_AMOUNT   9

/*网络连接命令序列*/

#define	ATCMD_CONNECT_STEPS		9//8		//初始化步骤




//电话号码类结构定义
typedef	struct
{
	u8	au8Number[10];			//电话号码,BCD码。当号码数字个数为奇数时，以“F”结束。“A”=“*”,“B”=“#”，“E”=“+”
	u8	u8DigitalLength;		//电话号码数字个数， 3 <= 个数 <= 20, 个数为“0”时表示无效
	u8	au8Data[140];			//短信数据内容
	u8	u8Data_Len;				//短信数据内容长度
	u8	cValidTag;				//电话号码有效标志，“55H”表示有效，其它为无效
	u8	fNumberType	:	1;		//类型，0=国内，1=国际
	u8	fUnused		:	7;

} _GPRS_SENDPDUSMSType;



void GPRS_Init();
void GPRS_UartReconfigure();
void GPRS_SendTCPIPPrefix(u16 dataLen);/* 发送TCPIP前缀 */
void GPRS_FillinTCPIPData(u8 *pDataBuf, u16 dataLen);/* 填充待发送数据内容 */

void GPRS_doInit(u8 initStep); //执行初始化序列
void GPRS_doConnect(u8 connectStep);//执行连接序列
void GPRS_doRegister();//执行网络注册序列
BOOL GPRS_doQueryData(void);//网络数据请求


/* 组织待发送数据内容 */
u16 GPRS_FormRawFrame( u8 *pSendBuf,u16 cmd,u16 sequence,
                                void* address, u16 dataSize, u16 offset);

/*帧提取*/
void GPRS_doRetrieveFrame(u16 begin, u16 end);

/*使能模块休眠*/
void GPRS_doEnterIntoLowerPowerState();

/*进入休眠模式*/
void GPRS_RequestSleep();

/*唤醒模式*/
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

/*********************** GPRS模块电源控制 *****************************************************/
#define GPRS_POWER_ON	GPIO_SetBits(GPIOA,GPIOA_CDMA_CTRL_)  //模块开电
#define GPRS_POWER_OFF	GPIO_ResetBits(GPIOA,GPIOA_CDMA_CTRL_) //模块关电


//网络指示灯打开/关闭
#define GPRS_LED_OFF()  GPIO_ResetBits(GPIOC, GPIOC_CDMA_LED)
#define GPRS_LED_ON()  GPIO_SetBits(GPIOC, GPIOC_CDMA_LED)

//LED指示灯闪烁
#define GPRS_FLASH_LED()  	\
						do {	\
                            GPIO_WriteBit(GPIOC, GPIOC_CDMA_LED,   \
                            (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOC, GPIOC_CDMA_LED))));	\
						}while(0)

void GPRS_Lowlevel_Init();

#endif /* GPRS_H_ */
