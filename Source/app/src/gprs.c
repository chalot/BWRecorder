/**   
 * @Title: gprs.c
 * @Description: HUAWEI MG323通信模块
 * @author Zhengd.Gao zhengdgao@163.com
 * @date 2013-6-14 上午9:43:02
 * @version V1.0
 */

#include <stm32f2xx_conf.h>
#include "gprs.h"
#include "comm.h"
#include "bsp.h"
#include "parameter.h"
#include "utility.h"
#include "trace.h"
#include "protocol.h"
#include "qp_port.h"
#include "board.h"
#include <qevents.h>
#include <qactiveobjs.h>
#include <systimer.h>
#include <qevt.h>

Q_DEFINE_THIS_MODULE("gprs.c")

static volatile u8 s_csq = 0; //当前信号强度

#define		GPRS_MAIN_ID		0	//主连接号
#define		GPRS_VICE_ID		1	//辅助连接号
#define 	SOCKET_ID			0

//串口收发缓区
#define RX_BUF_SIZE		1024
#define TX_BUF_SIZE		1056
static u8 buf_Rx[RX_BUF_SIZE];	//接收缓冲区
static u8 buf_Tx[TX_BUF_SIZE];	//发送缓冲区

static tCOMM Comm_Tx;  //发送缓区
static tCOMM Comm_Rx;  //发送缓区

//接收数据帧处理缓区
#define BUFFER_LENGTH_GPRSFRAMERx 			1024
static u8 au8Buffer_GPRSFrameRx[BUFFER_LENGTH_GPRSFRAMERx];

#define CHAR_CR 	0x0D  //回车
#define CHAR_LF 	0x0A  //换行/*AT命令*/
const char* ATCMD[] = {
/*基本配置命令*/
"AT\r",							//链路同步
		"ATE0\r",						//取消回显

		/*识别类命令*/
		"ATI\r",							//产品标识信息查询
		"AT+CGMI\r",					//厂商信息查询
		"AT+CGMM\r",					//产品名称查询
		"AT+CGMR\r",					//请注意软件版本号，方便技术上的交流沟通 目前反映的为平台版本
		"AT+CGSN\r",					//ESN查询

		/*安全控制命令 */
		"AT+CPIN?\r",					//R-UIM 卡在位和 PIN1 码状态查询

		/*网络服务接口命令*/
		"AT+CSQ\r",						//检查当地的网络信号强度，31 最大，0 最小
		"AT+CREG?\r",					//查询网络注册

		/*数据业务类命令*/
		"AT+CGACT=1\r",					//设置PDP激活状态
		"AT+CGATT?\r",					//设置GPRS服务状态

		/*INTERNET服务接口命令*/
		"AT^SICS",						//设置Internet连接的Profile之连接类型
		"AT^SICS",						//设置Internet连接的Profile之APN
		"AT^SICS",						//设置Internet连接的Profile之PASSWD
		"AT^SICS",						//设置Internet连接的Profile之USER
		"AT^SISS",						//建立服务的ID
		"AT^SISS",						//建立internet服务的socket
		"AT^SISS",						//建立internet服务的IP地址
		"AT^SISO",						//打开连接
		"AT^SISC",						//关闭连接
		"AT^SISW",						//发送数据
		"AT^SISR",						//读取数据

		"AT^IOMODE=1,0\r",				 //设置IO模式

		/*短消息业务接口命令*/
		"AT+CNMI=1,1,0,1,0\r",//设置短信接收上报的机制，设置短消息通知方式为先将短消息存储在MT中，然后上报存储位置，不存储短消息状态报告，直接上报
		"AT+CPMS=\"SM\"\r",				//选择短信存储器
		"AT+CMGD=",	//删除短信		 格式为AT+CMGD=<index>,<deltag>,当deltag为3的时候，删除所有已读短信
		"AT+CMGR=",					//读取短信信息
		"AT+CMGL=\r",					//列出当前选择短信存储器全部短信的序号和状态
		"AT+CMGF=0\r",					//设置短信格式为 PDU //文本
		/*电话业务接口命令*/
		"AT+CLIP=1\r", "ATD",							//拨打电话
		"ATA",							//接通电话
		"AT+CHUP",						//挂断电话

		/*工作模式设置命令*/
		"AT+CFUN=1",						//设置工作模式，全速工作状态

		//查询系统时间
		"AT+CCLK=?",

};

/*AT应答格式*/
const char* ATACK[] = {
/*基本命令*/
"OK", "ATE0",

/*标识查询命令应答*/
"Huawei Technologies", "Manufacturer:", "Revision:", "ESN",

/*安全控制命令应答*/
"+CPIN: READY",

/*网络服务接口命令应答*/
"+CSQ: ", "+CGATT: ",

/*数据业务类命令应答*/
"+CREG: ",

		/*INTERNET服务接口命令应答*/
		"^SISW:",						//写入数据到缓冲区提示
		"^SISR:",						//从缓冲区读取数据提示

		/*短消息业务接口命令应答*/
		"+CPMS: ",				//设置短信存储位置
		"+CNMI:",				//新短信通知方式
		"+CMTI:",				//收到短信提示
		"+CMGL:",				//列举短信状态
		"+CMGR:",				//收到的短信信息
		"+CMGD:",				//剩余可删除短信

		/*电话业务相关应答*/
		"+CLIP:",				//来电提醒

		"+CCLK:",			//系统时间

		/*FinalResultCode*/
		"CONNECT", "+CRING:", "NO CARRIER", "ERROR", "NO DIALTONE", "BUSY",
		"NO ANSWER", "+CME ERROR:", "+CMS ERROR:", "COMMAND NOT SUPPORT",
		"TOO MANY PARAMETERS",

};

/*AT应答格式*/
const char* CMEErrCode[] = { "R-UIM not inserted",					//UIM卡不在位置
		"Connection already established",			//连接已建立
		"Destination address required",				//需要制定目标地址
		};

static const u8 aInitSteps[INIT_STEPS_AMOUNT] = {
/*模块初始化*/
_ATCMD_AT,			//AT
		_ATCMD_ATE0,
//	_ATCMD_SETMODE,
		_ATCMD_CPIN,		//R-UIM 卡在位和 PIN1 码状态查询
		_ATCMD_CSQ,			//检查当地的网络信号强度，31 最大，0 最小。
//	_ATCMD_IOMODE,

#if		_MSG_
		_ATCMD_CPMS,		//设置短信存储位置
		_ATCMD_CNMI,		//设置短信接收上报的机制，如果不需要短信功能可以不设置
		_ATCMD_CMGF,		//设置短信模式为PDU模式
#endif
//	_ATCMD_CGACT,		//设置PDP激活状态
		_ATCMD_CGATT,		//设置GPRS服务状态
		_ATCMD_PHONE_TIPS,    //设置来电提醒

//	_ATCMD_GETSYSTIME,	//查询系统时间

		};

static const u8 aConnectSteps[CONNECT_STEPS_AMOUNT] = { _ATCMD_CLOSE,	//关闭连接
		_ATCMD_SETCONNTYPE,		//设置连接类型
		_ATCMD_SETPWD,			//设置Password
		_ATCMD_SETUSR,			//设置User
		_ATCMD_SETAPN,			//设置APN
		_ATCMD_SETSOCKET,		//建立服务Socket
		_ATCMD_SETID,			//建立服务ID
		_ATCMD_SETADDR,			//建立服务地址
		_ATCMD_OPEN				//打开连接

		};

static u8 attach_state = 0;
static u8 register_state = 0;
static u16 queryBytes = 0;
static u8 net_State = 0;

static u16 s_u8PDUSMSRx_LenCount = 0;
static u8 au8Buffer_SMSRx[BUFFER_LENGTH_SMSRx];
static u8 u8SMGCount = 0;		//短信条数
static u8 u8MaxSMGCount = 0;	//短信最大条数
static u8 u8ReadIndex = 0;		//短信读序列
static u16 u16ReadDataTimeout = 0;	//发送数据读取命令超时
static u8 u8GPRS_7bitDecode_LeftCode = 0;
static u8 u8GPRS_7bitDecode_Sequence = 0;
static u8 u8PDUSMSRx_LenCount = 0;
static u8 fGPRS_7bitDecode_Skip = 0;

#if		_MSG_
#define		GPRS_SMG_CNT				30					//短信数组长度/*GPRS已收短信息处理状态*/
typedef enum {
	_eGPRS_SMG_IDLE = 0,					//空闲态
	_eGPRS_SMG_READ_REQ,				//已经发送短消息阅读请求
	_eGPRS_SMG_READ_OK,					//短消息已经阅读
	_eGPRS_SMG_READ_TIMEOUT,			//短消息阅读请求超时没有应答
	_eGPRS_SMG_DEL_REQ,					//已经发送短消息删除请求
	_eGPRS_SMG_DEL_OK,					//短消息已经删除
	_eGPRS_SMG_DEL_TIMEOUT,				//删除命令超时

	_eGPRS_SMG_INVALID = 0xFF
} eGPRSSMGSTATUS;

typedef struct {
	u8 u8Smg_Cnt;					//"ME"中保留的短信数目
	u8 u8CurSmg_Index;				//当前读取短信的序列号
	u8 u8ReadSmg_Rx;				//已收短息读指针
	u8 u8WriteSmg_Rx;			  	//已收短息写指针
	u8 au8MsgCnt[GPRS_SMG_CNT];	//已收短息序列数组
	u16 u16Smg_Timeout;				//短信处理间隔超时
	u16 u16CheckSmg_Timeout;		//查询"ME"中短信状态超时
	u8 u8SMsgLen;					//缓冲区中已收短息长度
	u8 u8PendingSmsg;				//是否应该提取缓冲区短信
	eGPRSSMGSTATUS eCdmaMsgStatus;		//GPRS已收短信息处理状态
} tGPRSSMGFSMType;

static tGPRSSMGFSMType tGPRSSMGFSM;

static u8 GPRS_GetPDUSMS_HexByte(u8* pu8GPRSMessage);
static u32 GPRS_SMS_7BitsDecode(u8 u8PDUByte, u8 u8Startup, u8 u8LeftBits);
static u8 PhoneNo_ConvertBaseToAscii(u8 *pcNo, _GPRS_SENDPDUSMSType *ptBaseNo,
		u8 nInternational);
static u8 GPRS_StoreSMSContent(u8 u8SMS_DCS, u16 u16Code, s16 s16Index,
		s16 s16Bytes, s16 s16SequenceLongSMS);
static u8 SwapNibble(u8 u8Byte);
static u8 BCD2Decimal(u8 u8BCD);
static u8 GPRS_StoreSMSContent(u8 u8SMS_DCS, u16 u16Code, s16 s16Index,
		s16 s16Bytes, s16 s16SequenceLongSMS);

#endif

void GPRS_SetAttachState(u8 state) {
	attach_state = state;
}

u8 GPRS_GetAttachState() {
	return attach_state;
}

void GPRS_SetRegState(u8 state) {
	register_state = state;
}

u8 GPRS_GetRegState() {
	return register_state;
}

void GPRS_SetNetState(u8 state) {
	net_State = state;
}

u8 GPRS_GetNetState() {
	return net_State;
}

u8 GPRS_CheckCmd_IsAttaching(u8 step) {
	if (aInitSteps[step] == _ATCMD_CGATT)
		return 1;
	return 0;
}

//连接服务器类型
#define SERVER_TYPE	 SERVER_PRIMARY	// ptRuntimeParam->ServerType#define DIST(next, tail, size)  ((tail > next) ? (tail - next) : (size - next + tail))

//本地函数声明
static u8 GPRS_WriteData(u8* pu8Data, u16 u16Length);
static void UART_COMM_StartTransmit(void);
static _eATACK GPRS_GetRxFrameType(u8* pu8Buffer);
static u16 GPRS_TakeRxFrame(u8* pu8Buffer, u16 begin, u16 end);
static u16 GPRS_TakeRxSMG(u8* pu8Buffer);
//static void GPRS_TCPIPRecData_Process(u8 *pData, u16 len);
static void GPRS_ParseRxFrame(u8 *pBuffer, u16 len);

/**
 * @brief	内存初始化
 * @param	无
 * @return		无
 */
void GPRS_Init() {
	/*初始化收发缓区*/
	Comm_Init(&Comm_Tx, &buf_Tx[0], TX_BUF_SIZE);
	Comm_Init(&Comm_Rx, &buf_Rx[0], RX_BUF_SIZE);

	ZeroMem(&buf_Tx[0], TX_BUF_SIZE);
	ZeroMem(&buf_Rx[0], RX_BUF_SIZE);

	/*使能GPRS串口接收中断*/
	USART_Cmd(GSM_COM, ENABLE);
}

/**
 * @brief	串口IO重新初始化
 * @param	无
 * @return		无
 */void GPRS_UartReconfigure() {
#if 0
	GPIO_InitTypeDef GPIO_InitStructure;

	//管脚 = UART2-Tx，推挽复用
	GPIO_InitStructure.GPIO_Pin = GPIOA_CDMA_USART2_TX_;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//管脚 = UART2-Rx，输入悬浮
	GPIO_InitStructure.GPIO_Pin = GPIOA_CDMA_USART2_RX_;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_Cmd(GSM_COM, ENABLE);
#endif
}

/**
 * @brief	执行初始化序列
 * @param	initStep	[in]	AT序列
 * @return
 */
void GPRS_doInit(u8 initStep) {
	Q_ASSERT(initStep < INIT_STEPS_AMOUNT);
	if (initStep >= INIT_STEPS_AMOUNT)
		return;

	TRACE_(QS_USER, NULL, "[initialing], step = %d, cmd = %s", initStep,
			ATCMD[aInitSteps[initStep]]);

	//数据提交到发送串口
	GPRS_WriteData((u8*) ATCMD[aInitSteps[initStep]],
			strlen_(ATCMD[aInitSteps[initStep]]));
}

/**
 * @brief	串口发送数据
 * @param	pData		[in]	数据
 * 			dataLen		[in]	数据长度
 * @return
 */
u8 GPRS_WriteData(u8* pu8Data, u16 u16Length) {
	if ((pu8Data == NULL ) || (u16Length == 0)) {
		return 0;
	}

	//发送缓冲区不够大?
	if (u16Length > TX_BUF_SIZE) {
		return 0;
	}

	//复位读写指针
	Comm_Tx.read = 0;

	//拷贝待发数据到发送缓区
	memcpy_(Comm_Tx.pBuf, pu8Data, u16Length);

	//保存待发送数据长度
	Comm_Tx.dataSize = u16Length;

	//启动串口发送数据
	UART_COMM_StartTransmit();

	return 1;
}

/**
 * @brief	启动GPRS串口数据发送
 * @param	NULL
 * @return		NULL
 */
void UART_COMM_StartTransmit(void) {
	u8 u8SendData;

	//待发送数据填入串口TXBUFR
	u8SendData = *(Comm_Tx.pBuf + Comm_Tx.read++);
	USART_SendData(GSM_COM, u8SendData);

	//允许发送中断
	USART_ITConfig(GSM_COM, USART_IT_TXE, ENABLE);
}

//带循环的缓区比较
BOOL memcmp_wrap_(u8 *pBuf, u8 start, u8 end, u16 bufSize, char *pString) {
	u8 matchTimes = 0;
	u16 strlen = strlen_(pString);

	while ((start != end) && (matchTimes < strlen)) {
		if (*(pBuf + start) != *pString++) {
			break;
		}
		start++;
		WRAP_AROUND(start, bufSize);

		matchTimes++;
	}

	if (matchTimes == strlen)
		return TRUE;

	return FALSE;
}

/**
 * @brief	GPRS中断服务程序
 * @param	NULL
 * @return		NULL
 */
void ISR_GSM_COM(void) {
#define PREFIX_POS_UNKOWN	0xFFFF
	static u8 u8Data = 0;
	static u16 head = PREFIX_POS_UNKOWN;
	static u16 tail = 0;
	u16 u16RecvData;
#define STRING_MAX	7
	static char* ATRESP[] = { "OK", "ERROR", "^SISR", "^SISW", "+CMS ERROR",
			"+CME ERROR", "NO CARRIER", };

	//发送完成 并且发送中断使能
	if (USART_GetITStatus(GSM_COM, USART_IT_TXE)) {
		USART_ClearITPendingBit(GSM_COM, USART_IT_TXE);

		//数据发送完成了，关发送中断
		if (Comm_Tx.read == Comm_Tx.dataSize) {
			//关闭发送中断
			USART_ITConfig(GSM_COM, USART_IT_TXE, DISABLE);
		} else {
			//待发送数据填入串口TXBUFR
			u8Data = *(Comm_Tx.pBuf + Comm_Tx.read++);
			USART_SendData(GSM_COM, u8Data);
		}
	}

	//接收中断
	if (USART_GetITStatus(GSM_COM, USART_IT_RXNE)) {
		USART_ClearITPendingBit(GSM_COM, USART_IT_RXNE);

		//读取数据
		u16RecvData = USART_ReceiveData(GSM_COM);

		u8Data = (u8) u16RecvData;

		//存储数据
		*(Comm_Rx.pBuf + Comm_Rx.write) = u8Data;

#if 0
		/*
		 * 		在接收ISR里判断有效完整帧，采用事件通知方式，告诉应用层去提取帧
		 根据MG323AT手册，对于任何AT指令，正确的响应格式是<CR><LF>OK<CR><LF>，
		 错误的响应格式有如下几类：
		 <CR><LF>ERROR<CR><LF>
		 <CR><LF>+CME ERROR:<err><CR><LF>
		 <CR><LF>+CMS ERROR:<err><CR><LF>
		 因此，当接收到以上任何一帧时，表示当前接收到一个完整有效帧。

		 还有一种更简便的方法：<CR><LF>。。。<CR><LF>总是成对出现的，因此做个简单的
		 配对就可以了。不过要注意处理<CR><LF>可能出现在数据包中的情况。
		 本实现采用后一种方法。
		 */

		/*
		 * 匹配一个AT结果帧，方法：在接收队列中标记出前缀<CR><LF>和后缀<CR><LF>的位置，从
		 * 后缀出发逆向匹配中间字串，看是否是AT结果响应，如是，则说明匹配成功。否则，将
		 * 后缀作为前缀，待接收到新的后缀后重新逆向匹配。这样的做的原因是尽可能减少辅助内存
		 * 开销，同时保证匹配效率最高。
		 *
		 * */
#if 1
		//匹配第一个前缀，head_previous记录前缀<CR>位置
		if(head == PREFIX_POS_UNKOWN)
		{
			if(u8Data == CHAR_LF)
			{
				u8 head_previous;
				if(Comm_Rx.write == 0)
				head_previous = RX_BUF_SIZE - 1;
				else
				head_previous = Comm_Rx.write - 1;

				//匹配到前缀<CR><LF>
				//           ↑
				//			head
				if(*(Comm_Rx.pBuf + head_previous) == CHAR_CR)
				{
					head = head_previous;
				}
			}
		}
		//找到前缀，定位后缀
		else
		{
			if(u8Data == CHAR_LF)
			{
				u16 tail_previous;

				if(Comm_Rx.write == 0)
				tail_previous = RX_BUF_SIZE - 1;
				else
				tail_previous = Comm_Rx.write - 1;

				//找到后缀<CR><LF>
				//         ↑
				//	      tail
				if(*(Comm_Rx.pBuf + tail_previous) == CHAR_CR)
				{
					ATMsgEvt *pe;
					u16 next = 0;

					tail = tail_previous;				//Comm_Rx.write;

					//读指针指向前缀<LF>
					next = head + 1;
					WRAP_AROUND(next, RX_BUF_SIZE);

					//读指针指向前缀<CR><LF>下一个字节
					next ++;
					WRAP_AROUND(next, RX_BUF_SIZE);

					//前缀和后缀不是紧随<CR><LF><CR><LF>
					//							↑↑
					//						  next tail
					//说明中间有数据

					if((next != tail) && ((DIST(next, tail, RX_BUF_SIZE)) > 1))
					{
						u8 i;

						//串匹配
						for(i = 0; i < STRING_MAX; i++)
						{
							//带循环的缓区比较
							if(memcmp_wrap_(Comm_Rx.pBuf, next, tail, RX_BUF_SIZE, ATRESP[i]))
							break;
						}

						if(i < STRING_MAX) //匹配成功，说明提取到一个结果帧
						{
							//发送帧就绪消息
							pe = Q_NEW(ATMsgEvt, AT_DATAREADY_SIG);
							Q_ASSERT(pe);
							pe->begin = 0;//帧头数据位置
							pe->end = Comm_Rx.write + 1;//结束位置为最后一个有效字节的下一个字节
							WRAP_AROUND(pe->end, RX_BUF_SIZE);
							QACTIVE_POST(AO_Gprs, (QEvt*)pe, NULL);
						}
					}   //if(next != previous)

					//准备下一次匹配
					head = tail;

					//重新匹配帧头
//					head = PREFIX_POS_UNKOWN;
				}
			}
		}   //else
#endif
#endif

		//接收缓冲区写指针回头
		Comm_Rx.write++; //取第一个字符
		WRAP_AROUND(Comm_Rx.write, RX_BUF_SIZE);
	}

	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_ORE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_ORE);        //读SR

		//读取数据
		u16RecvData = USART_ReceiveData(GSM_COM);		  //读DR

	}
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_PE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_PE);        //读SR

		//读取数据
		u16RecvData = USART_ReceiveData(GSM_COM);		  //读DR

	}
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_FE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_FE);        //读SR

		//读取数据
		u16RecvData = USART_ReceiveData(GSM_COM);		  //读DR

	}
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_NE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_NE);        //读SR

		//读取数据
		u16RecvData = USART_ReceiveData(GSM_COM);		  //读DR

	}
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_IDLE) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_IDLE);        //读SR

		//读取数据
		u16RecvData = USART_ReceiveData(GSM_COM);		  //读DR

	}
	//LBD LIN断开检测标志
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_LBD) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_LBD);        //读SR
	}
	//传输完成检测标志
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_TC) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_TC);        //读SR
	}
	//CTS标志
	if (USART_GetFlagStatus(GSM_COM, USART_FLAG_CTS) != RESET) {
		USART_ClearFlag(GSM_COM, USART_FLAG_CTS);        //读SR
	}
}
#if 0
/*
 剔除   <CR><LF>.....<CR><LF><CR><LF>....<CR><LF>
 |    帧A             | 		帧B	           |
 间隔帧尾帧头匹配
 */
next = head + 1;
WRAP_AROUND(next, RX_BUF_SIZE);

if(next != previous)
{
//						//同时找到前缀和后缀，匹配AT指令,从前缀的紧随下一个字节开始匹配
//						//发送帧就绪消息
//						pe = Q_NEW(ATMsgEvt, AT_DATAREADY_SIG);
//						Q_ASSERT(pe);
//						pe->begin = next; //帧头数据位置
//						pe->end = previous;   //帧尾数据位置
//						QACTIVE_POST(AO_Gprs, (QEvt*)pe, NULL);
}
#endif

#if 0
head ++; //取第一个字符
WRAP_AROUND(head, RX_BUF_SIZE);

//根据第一个字符定位匹配类型
for(i = 0; i < 6; i++)
{
	if(*(Comm_Rx.pBuf + head) == ATRESP[i][0])
	break;
}

//找到匹配的第一个字符所在串
if(i < 6)
{
	//进行全字符匹配
	u8 j = 0;
	for(j = 1; j < strlen_(ATRESP[i]); j++)
	{
		head ++; //取第一个字符
		WRAP_AROUND(head, RX_BUF_SIZE);

		if(*(Comm_Rx.pBuf + head) != ATRESP[i][j])
		break;
	}

	if(j == strlen_(ATRESP[i])) //匹配成功，说明提取到一个结果帧
	{
		//发送帧就绪消息
		QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_DATAREADY_SIG), NULL);
	}
}
#endif

#if 0
//根据第一个字符定位匹配类型
for(i = 0; i < 6; i++)
{
	if(*(Comm_Rx.pBuf + next) == ATRESP[i][0])
	break;
}

//找到匹配的第一个字符所在串
if(i < 6)
{
	//进行全字符匹配
	u8 j = 0;
	for(j = 1; j < strlen_(ATRESP[i]); j++)
	{
		next ++; //取第一个字符
		WRAP_AROUND(next, RX_BUF_SIZE);

		if(*(Comm_Rx.pBuf + next) != ATRESP[i][j])
		break;
	}

	if(j == strlen_(ATRESP[i])) //匹配成功，说明提取到一个结果帧
	{
//								static const QEvt dataReadyEvt = {AT_DATAREADY_SIG, 0};

		/*
		 注意，消息传递开销较大，因此优化措施：
		 1、使用静态事件，免于内存分配；
		 2、最大限度减少信号发送次数，取决于匹配策略。本实现中，只对结果帧进行匹配，
		 如果匹配成功才发送信号。因为AT响应一般前面是数据，后面加成功/失败状态，因此
		 匹配AT结果不会对之前的接收过程产生任何干扰，稳妥可靠。
		 */
		//发送帧就绪消息
//								QACTIVE_POST(AO_Gprs, &dataReadyEvt, NULL);
		pe = Q_NEW(ATMsgEvt, AT_DATAREADY_SIG);
		Q_ASSERT(pe);
		pe->begin = 0;//head;//head_previous; //帧头数据位置
		pe->end = Comm_Rx.write;//帧尾数据位置
		QACTIVE_POST(AO_Gprs, (QEvt*)pe, NULL);
	}
}   //if(i < 6)
#endif

/**
 * @brief	执行初始化序列
 * @param	initStep	[in]	AT序列
 * @return
 */
void GPRS_doConnect(u8 connectStep) {
	u8 au8Cmd[50];
	_eATCMD eCmd;
	u8 channelId = 0;

	Q_ASSERT(connectStep < CONNECT_STEPS_AMOUNT);
	if (connectStep >= CONNECT_STEPS_AMOUNT)
		return;

	//每次都要先清空命令缓区
	ZeroMem(au8Cmd, 50);

	//根据连接服务器类型用不同的通道号
	channelId = GPRS_MAIN_ID;

	//取指令
	eCmd = (_eATCMD) aConnectSteps[connectStep];

	//以下命令运行时构造AT命令字符串
	if (eCmd == _ATCMD_CLOSE) {
		sprintf_((u8*) &au8Cmd[0], "AT^SISC=%d\r", channelId);
	}
	//以下命令运行时构造AT命令字符串
	else if (eCmd == _ATCMD_SETCONNTYPE) {
		sprintf_((u8*) &au8Cmd[0], "AT^SICS=%d,conType,GPRS0\r", channelId);
	}
	//设置Password
	else if (eCmd == _ATCMD_SETPWD) {
		sprintf_((u8*) &au8Cmd[0], "AT^SICS=%d,passwd,gprs\r", channelId);
	}
	//设置User
	else if (eCmd == _ATCMD_SETUSR) {
		sprintf_((u8*) &au8Cmd[0], "AT^SICS=%d,user,cm\r", channelId);
	} else if (eCmd == _ATCMD_SETAPN) {
		sprintf_((u8*) &au8Cmd[0], "AT^SICS=%d,apn,UNINET\r", channelId);
	} else if (eCmd == _ATCMD_SETID) {
		sprintf_((u8*) &au8Cmd[0], "AT^SISS=%d,conId,0\r", channelId);
	} else if (eCmd == _ATCMD_SETSOCKET) {
		sprintf_((u8*) &au8Cmd[0], "AT^SISS=%d,srvType,socket\r", channelId);
	}
#if	CONNMODE  == _UDP_ //UDP连接	else if(eCmd == _ATCMD_SETADDR)	{
		if(SERVER_TYPE == SERVER_PRIMARY)
		{
			sprintf_((u8*)&au8Cmd[0],"at^siss=%d,address,\"sockudp://%d.%d.%d.%d:%d\"\r",
					GPRS_MAIN_ID,
					ptWorkParam->ip[0],
					ptWorkParam->ip[1],
					ptWorkParam->ip[2],
					ptWorkParam->ip[3],
					ptWorkParam->port
			);
		}
		else if(SERVER_TYPE == SERVER_VICE)
		{
			sprintf_((u8*)&au8Cmd[0],"at^siss=%d,address,\"sockudp://%d.%d.%d.%d:%d\"\r",
					GPRS_VICE_ID,
					ptWorkParam->ip[0],
					ptWorkParam->ip[1],
					ptWorkParam->ip[2],
					ptWorkParam->ip[3],
					ptWorkParam->port
			);
		}
	}
#elif CONNMODE  == _TCP_
	else if (eCmd == _ATCMD_SETADDR) {
		if (SERVER_TYPE == SERVER_PRIMARY) {
//			sprintf_((u8*)&au8Cmd[0],"at^siss=%d,address,\"socktcp://%d.%d.%d.%d:%d\"\r",
//						GPRS_MAIN_ID,
//						ptWorkParam->ip[0],
//						ptWorkParam->ip[1],
//						ptWorkParam->ip[2],
//						ptWorkParam->ip[3],
//						ptWorkParam->port
//					);
		}
//		else if(SERVER_TYPE  == SERVER_VICE)
//		{
//			sprintf_((u8*)&au8Cmd[0],"at^siss=%d,address,\"socktcp://%d.%d.%d.%d:%d\"\r",
//						GPRS_VICE_ID,
//						ptCommParam->aViceServer_IP[0],
//						ptCommParam->aViceServer_IP[1],
//						ptCommParam->aViceServer_IP[2],
//						ptCommParam->aViceServer_IP[3],
//						ptCommParam->u16ViceServer_Port
//					);
//		}
	}

#endif
	else if (eCmd == _ATCMD_OPEN) {
		sprintf_((u8*) &au8Cmd[0], "AT^SISO=%d\r", channelId);
	} else {
		//未知命令，出错了
		return;
	}

	TRACE_(QS_USER, NULL, "[connecting], step = %d, cmd = %s ", connectStep,
			au8Cmd);

	//AT命令写入串口
	GPRS_WriteData(au8Cmd, strlen_(au8Cmd));
}

//执行网络注册序列
void GPRS_doRegister() {
	char strCmd[30] = { 0 };

	TRACE_(QS_USER, NULL, "[registering], cmd = AT+CREG? ");

	sprintf_((u8*) &strCmd, "AT+CREG?\r");
	GPRS_WriteData(strCmd, strlen_(strCmd));
}

//网络数据请求
BOOL GPRS_doQueryData(void) {
	u8 au8Cmd[20] = { 0 };
	BOOL bret;

	//构造命令字符串
	sprintf_(au8Cmd, "AT^SISR=%d,%d\r", SOCKET_ID, MAX_DATA_LEN);
	//发送命令
	bret = GPRS_WriteData(au8Cmd, strlen_(au8Cmd));

	TRACE_(QS_USER, NULL, "[TCP connected]:Query data, cmd = %s ", au8Cmd);

	return bret;
}

/* 发送TCPIP前缀 */
void GPRS_SendTCPIPPrefix(u16 dataLen) {
	u8 au8Cmd[50] = { 0 };

	sprintf_(au8Cmd, "AT^SISW=%d,%d\r\n", SOCKET_ID, dataLen);

	TRACE_(GPRS_STAT, NULL, "[UART]send : %s", au8Cmd);

	//成功写入数据到发送缓冲区
	GPRS_WriteData(au8Cmd, strlen_(au8Cmd));
}

/* 填充待发送数据内容 */
void GPRS_FillinTCPIPData(u8 *pDataBuf, u16 dataLen) {
	if ((pDataBuf == NULL ) || (dataLen == 0)) {
		return;
	}

	*(pDataBuf + dataLen) = '\r';
	*(pDataBuf + dataLen + 1) = '\n';

	TRACE_(QS_USER, NULL, "[UART] frame sendout. waiting ret...... ");
	GPRS_WriteData(pDataBuf, dataLen + 2);
}

/*帧提取*/
void GPRS_doRetrieveFrame(u16 begin, u16 end) {
	u16 u16FrameSize = 0;

	//循环提取完整帧，因为可能同时存在数据帧和结果帧
	for (;;) {
		//提取接收帧
		u16FrameSize = GPRS_TakeRxFrame(au8Buffer_GPRSFrameRx, begin, end);

		//帧长度有效
		if (u16FrameSize == 0)
			break;

		if (u16FrameSize > 0) {
			//字符串结束符
			*(au8Buffer_GPRSFrameRx + u16FrameSize) = '\0';
			TRACE_(SYSTEM_STAT, NULL, "[UART] RX: %s", au8Buffer_GPRSFrameRx);

			//解析并处理帧
			GPRS_ParseRxFrame(au8Buffer_GPRSFrameRx, u16FrameSize);
		}
	}
}

//逆向提取TCP包
void GPRS_doTakeTCPPacketData() {
	u16 _len;
	u16 _read;

	_read = Comm_Rx.read;

	WRAP_REVERSE(_read, RX_BUF_SIZE);
	Q_ASSERT(*(Comm_Rx.pBuf + _read) == CHAR_LF);

	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<LF>
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<CR>
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //K
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //O
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<LF>
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<CR>

	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<LF>
	WRAP_REVERSE(_read, RX_BUF_SIZE);   //<CR>

	ZeroMem(au8Buffer_GPRSFrameRx, BUFFER_LENGTH_GPRSFRAMERx);
	_len = queryBytes;
	while (queryBytes > 0) {
		au8Buffer_GPRSFrameRx[queryBytes - 1] = *(Comm_Rx.pBuf + _read);
		WRAP_REVERSE(_read, RX_BUF_SIZE);

		queryBytes--;
	}

	au8Buffer_GPRSFrameRx[_len] = '\0';
	TRACE_(QS_USER, NULL, "[TCP] Get Msg : %s", au8Buffer_GPRSFrameRx);

	//此后是有效数据内容，处理网络协议数据
	Protocol_TCPIPData_Process(au8Buffer_GPRSFrameRx, _len);
}

//帧解析处理
void GPRS_ParseRxFrame(u8 *pBuffer, u16 len) {
	u8 *pData = NULL;
	_eATACK eACK;

	//获取帧类型
	eACK = GPRS_GetRxFrameType(pBuffer);

	//指向参数内容
	pData = pBuffer;

	//跳过AT命令头
	if (_ATACK_UNDEFINE != eACK) {
		pData += strlen_(ATACK[(u8) eACK]);
	}

	switch (eACK) {
	case _ATACK_OK:

		QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_OK_SIG), NULL);
		break;
	case _ATACK_CPIN:

//				QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_OK_SIG), NULL);
		break;

	case _ATACK_CGATT: //GPRS服务状态，0：分离状态，1：附着状态

		if (*pData == '1') {
//					QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_OK_SIG), NULL);
			GPRS_SetAttachState(1);
		} else {
			GPRS_SetAttachState(0);
		}
		break;

	case _ATACK_CREG:  //网络注册状态

		pData += 2;

		if ((*pData == '1') || (*pData == '5')) {
			GPRS_SetRegState(1);
//					QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_NETREGGED_SIG), NULL);
		} else {
			GPRS_SetRegState(0);
		}
		break;

		//"+CSQ:<SQM>,<99>",SQM:信号强度，0~31
	case _ATACK_CSQ: {
		u8 csq = 0;
		CSQEvt *pe = NULL;

		//更新信号质量
		while (*pData != ',') {
			if (*pData != 0x20) {
				csq = csq * 10 + (*pData - 0x30);
			}
			pData++;
		}

		GPRS_SetCSQ(csq);

//				//广播CSQ值
//				pe = Q_NEW(CSQEvt, AT_CSQ_SIG);
//				Q_ASSERT(pe);
//				pe->csq = csq;
//				QACTIVE_POST(AO_Gprs, pe, NULL);
	}
		break;

		/*
		 * URC   <CR><LF>^SISW：<srvProfileId>, <urcCauseId>[,<unackData>]<CR><LF>
		 功能      数据的主动上报命令^SISR 通知上层服务已经建立可以写入数据。
		 <srvProfileId>：数值型参数，Internet 服务profile 标识，取值范围为0~9
		 <reqWriteLength>：整数型参数，指定^SISW 要写入的数据字节长度，取值范围为0~1500
		 */
	case _ATACK_SISW: //准备发送数据提示
	{
//				u8		u8Index = 0;
//				u8		u8Break = 0;

		//提示网络可以发送的字节数
		if (memcmp_(pBuffer, (u8*) "^SISW: 0,1,", strlen_("^SISW: 0,1,"))) {
			//报告已接受到等待提示符
//					QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_SENDOK_SIG), NULL);
		}
		//提示发送结果
		else if (memcmp_(pBuffer, (u8*) "^SISW: 0,1", strlen_("^SISW: 0,1"))
				&& (len == strlen_("^SISW: 0,1"))) {
			//报告数据发送成功
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_SENDOK_SIG), NULL);
		} else {
			//报告已接受到数据输入提示符
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_FILLDATATIP_SIG), NULL);
		}
	}
		break;

		/*
		 * 分两种情况：
		 * 一、缓区空，数据到达时主动上报
		 URC <CR><LF>^SISR: <srvProfileId >,<urcCauseId><CR><LF>
		 功能 数据的主动上报命令^SISR 通知上层有可以读取的数据。
		 二、对于主动下发查询数据命令AT+SISR的响应
		 <CR><LF>^SISR:<srvProfileId>,<reqReadLength>[,<remainUdpPacketLength>]<CR><LF>
		 ……(返回读取到的数据)<CR><LF>
		 <CR><LF>OK<CR><LF>
		 */
	case _ATACK_SISR: {
		u16 _len = 0;
		_len = strlen_("^SISR: 0, 1");

		//情况一：接收到数据主动上报
		if (memcmp_(pBuffer, "^SISR: 0, 1", _len) && (len == _len)) {
			QACTIVE_POST(AO_Gprs, Q_NEW(QEvent, AT_DATAREADY_SIG), (void*) 0);
		}
		//情况二，暂不提取数据，因为此时数据可能尚未接收完全；稳妥的作法是此处仅保存数据长度，
		//等待<CR><LF>OK<CR><LF>到达再提取，那是数据必已接收完全
		else {
			_len = 0;

			//查询的响应
			while (*pData != ',') {
				pData++;
			}

			pData++; //跳过','

			//计算数据长度<reqReadLength>
			while ((*pData >= '0') && (*pData <= '9')) {
				_len = _len * 10 + (*pData - 0x30);
				pData++;
			}

			//记录待接收数据长度
			queryBytes = _len;
		}
	}

		break;

		//系统时间,格式为“yy/MM/dd,hh:mm:ss”
	case _ATACK_SYSTIME: {
		SYSTIMER timer;

		while (!((*pBuffer >= '0') && (*pBuffer <= '0'))) {
			pBuffer++;
		}

		while (*pBuffer != '/') {
			timer.year = 10 * timer.year + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while (*pBuffer != '/') {
			timer.month = 10 * timer.month + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while (*pBuffer != ',') {
			timer.day = 10 * timer.day + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while (*pBuffer != ':') {
			timer.hour = 10 * timer.hour + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while (*pBuffer != ':') {
			timer.minute = 10 * timer.minute + (*pBuffer - 0x30);
			pBuffer++;
		}

		pBuffer++;
		while ((*pBuffer >= '0') && (*pBuffer <= '0')) {
			timer.second = 10 * timer.second + (*pBuffer - 0x30);
			pBuffer++;
		}

		SysTimer_Set(&timer);
	}
		break;

		//"CONNECT"
	case _ATACK_CONNECT:

		break;
#if	_PHONE_
		/*电话处理*/
		//"RING",响铃
		case _ATACK_RING:

		//累计响铃次数
		tGPRSFSM.u8PhoneRingCount++;

		break;

		//"+CLIP:"，来电
		case _ATACK_CLIP:

		//跳过<+CLIP: >和<,>,指向数据长度首地址
		pData += 2;

		GPRS_ClipProcess(pData);

		break;

		//"NO CARRIER",
		case _ATACK_NO_CARRIER:
		//当前正在响铃，或正在接听，或正在拨号，或正在呼出，说明已经挂断
		if ( (tGPRSFSM.ePhoneState == ePHONE_STATE_DIAL) || (tGPRSFSM.ePhoneState == ePHONE_STATE_CALLING)
				|| (tGPRSFSM.ePhoneState == ePHONE_STATE_TALKING) || (tGPRSFSM.ePhoneState == ePHONE_STATE_BUSY)
				|| (tGPRSFSM.ePhoneState == ePHONE_STATE_HANGING) )
		{
			tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;

			//语音选择为空
			PERI_AudioSel(_AUDIO_NONE_);

		}

		//有语音监听请求，或正在进行语音监听
		if ( tGPRSFSM.eVoiceMonitorState == ePHONE_MONITOR_REQ)
		{
			//语音监听，中心拒接后续处理
			GPRS_MonitorRefusedPostProcess();
		}
		else if (tGPRSFSM.eVoiceMonitorState == ePHONE_MONITOR_MONITORING)
		{
			//监听结束
			tGPRSFSM.eVoiceMonitorState = ePHONE_MONITOR_IDLE;

			tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;
		}

		//如果当前拨打电话
		if (tGPRSFSM.u8ATCmd == _ATCMD_PHONE_DIAL)
		{
			tGPRSFSM.ePhoneState = ePHONE_STATE_DIAL;
			tGPRSFSM.u8ATCmd = _ATCMD_IDLE_;
		}
		break;

		//"NO DIALTONE",
		case _ATACK_NO_DIALTONE:

		break;

		//"BUSY",
		case _ATACK_BUSY:
		//当前正在响铃，或正在接听，或正在拨号，或正在呼出，说明已经挂断
		if (tGPRSFSM.ePhoneState == ePHONE_STATE_DIAL)
		{
			tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;

			//语音选择为空
			PERI_AudioSel(_AUDIO_NONE_);
		}
		//有语音监听请求，或正在进行语音监听
		if ( tGPRSFSM.eVoiceMonitorState == ePHONE_MONITOR_REQ)
		{
			//语音监听，中心拒接后续处理
			GPRS_MonitorRefusedPostProcess();
		}

		//如果当前拨打电话
		if (tGPRSFSM.u8ATCmd == _ATCMD_PHONE_DIAL)
		{
			tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;

			tGPRSFSM.u8ATCmd = _ATCMD_IDLE_;
		}
		break;

		//"NO ANSWER",
		case _ATACK_NO_ANSWER:
		tGPRSFSM.ePhoneState = ePHONE_STATE_IDLE;
		break;
#endif

#if	_MSG_
		/*短信*/
		//短信存储器设置状态
	case _ATACK_CPMS: {
		u8 u8Num = 0, u8Index = 1;

		ZeroMem((u8*) &tGPRSSMGFSM, sizeof(tGPRSSMGFSMType));

		while (*pData != ',') {
			u8Num = 10 * u8Num + (*pData - 0x30);
			pData++;
		}
		pData++;
		u8SMGCount = u8Num;

		u8Num = 0;
		while (*pData != ',') {
			u8Num = 10 * u8Num + (*pData - 0x30);
			pData++;
		}

		u8MaxSMGCount = u8Num;	//短信最大条数
		u8ReadIndex = u8SMGCount;	//短信读序列
	}
		break;
		//列举短信状态
	case _ATACK_CMGL: {
		u16 u16Length;

		while (*pData != ',' || *(pData + 1) != ',') {
			pData++;
		}
		pData += 2;
		/*

		 //获取PDU消息长度和内容
		 u16Length = GPRS_GetPduCode(au8Buffer_GPRSFrameRx);

		 //解析PDU短信
		 GPRS_PDUSMS_Decode(au8Buffer_GPRSFrameRx,u16Length);
		 */

	}
		break;
		//删除短信
	case _ATACK_CMGD:

		/*
		 //删除短信操作状态切换
		 if(tGPRSSMGFSM.eCdmaMsgStatus ==  _eGPRS_SMG_DEL_REQ)
		 {
		 tGPRSSMGFSM.eCdmaMsgStatus = _eGPRS_SMG_DEL_OK;
		 }
		 */
		break;
		//新短信提示
	case _ATACK_CMTI: {
		u8 u8Index = 0;
		while (*pData != ',') {
			pData++;
		}
		//跳过','
		pData++;

		while (*pData >= 0x30 && *pData <= 0x39) {
			u8Index = u8Index * 10 + *pData - 0x30;
			pData++;
		}
		if (u8Index > 0) {
			//立即查询所有短信
			tGPRSSMGFSM.u16CheckSmg_Timeout = 1;

			u8ReadIndex = u8Index;

			++u8SMGCount;
		}

	}
		break;
		//读取短信
	case _ATACK_CMGR: {
		/*
		 while(*pData != ',' || *(pData + 1)!= ',')
		 {
		 pData++;
		 }
		 pData += 2;
		 //空的短消息，说明此序列位置没有存储短信，则直接操作下个位置
		 if(*pData == 0x30)
		 {
		 --u8ReadIndex;

		 if(u8ReadIndex == 0 && !tGPRSFSM.fReadMsgFromHigh)
		 {
		 u8ReadIndex = u8MaxSMGCount;

		 tGPRSFSM.fReadMsgFromHigh = 1;
		 }
		 else
		 {
		 //如果已经从最后短信缓冲区位置读取，那么已经读取完成了	u8ReadIndex == 0
		 if(tGPRSFSM.fReadMsgFromHigh == 1 && u8ReadIndex == 0)
		 {
		 tGPRSFSM.fReadMsgFromHigh = 0;

		 u8SMGCount = 0;

		 if(tGPRSFSM.u8PendingReconnectServer)
		 {
		 tGPRSFSM.u8PendingReconnectServer = 0;

		 //重连服务器
		 GPRS_ReconnectServer();
		 }
		 //系统软复位标志置位
		 if(tGPRSFSM.u8PendingResetProgram)
		 {
		 tGPRSFSM.u8PendingResetProgram = 0;

		 //保存系统CAN参数
		 Save_CanMiles();

		 //系统软复位，重新引导系统
		 SystemReset();
		 }
		 if(tGPRSFSM.u8PendingResetgprs)
		 {
		 tGPRSFSM.u8PendingResetgprs = 0;

		 GPRS_ResetPower();
		 }
		 }
		 }

		 tGPRSSMGFSM.eCdmaMsgStatus = _eGPRS_SMG_IDLE;


		 }
		 //阅读短信操作状态切换
		 else
		 {
		 u16	u16Length;
		 if(tGPRSSMGFSM.eCdmaMsgStatus ==  _eGPRS_SMG_READ_REQ)
		 {
		 tGPRSSMGFSM.eCdmaMsgStatus = _eGPRS_SMG_READ_OK;
		 }
		 //获取PDU消息长度和内容
		 u16Length = GPRS_GetPduCode(au8Buffer_GPRSFrameRx);

		 //解析PDU短信
		 GPRS_PDUSMS_Decode(au8Buffer_GPRSFrameRx,u16Length);

		 }

		 }

		 //串口解锁
		 UART_UNLOCK();

		 tGPRSFSM.u8ATCmd = _ATCMD_IDLE_;
		 */
	}
		break;
#endif
		/*错误处理*/
		//"ERROR",
	case _ATACK_ERROR:

		//"+CMS ERROR:",
	case _ATACK_CMS_ERROR:

		//"COMMAND NOT SUPPORT",
	case _ATACK_COMMAND_NOT_SUPPORT:

		//"TOO MANY PARAMETERS",
	case _ATACK_TOO_MANY_PARAMETERS:

		//"+CME ERROR:"
	case _ATACK_CME_ERROR:

		//所有错误类型统一处理
		QACTIVE_POST(AO_Gprs, Q_NEW(QEvt, AT_ERROR_SIG), NULL);
		break;
		//未知错误
	case _ATACK_UNDEFINE:
	default:
		break;

	}
}

u16 GPRS_TakeRxSMG(u8* pu8Buffer) {
#if 0
	u16 u16AvailableBytes = 0;

	if(pu8Buffer == NULL)
	{
		return 0;
	}

	while(*(tGPRSFSM.pu8Buffer_Rx + tGPRSFSM.u16Read_Rx) != GPRS_DATA_CR_)
	{
		*pu8Buffer++ = *(tGPRSFSM.pu8Buffer_Rx + tGPRSFSM.u16Read_Rx);

		++tGPRSFSM.u16Read_Rx;
		if ( tGPRSFSM.u16Read_Rx >= tGPRSFSM.u16BufferSize_Rx)
		{
			tGPRSFSM.u16Read_Rx = 0;
		}

		u16AvailableBytes++;
	}
	//去除数据尾部的0x0D 0x0A
	tGPRSFSM.u16Read_Rx += 2;
	if ( tGPRSFSM.u16Read_Rx >= tGPRSFSM.u16BufferSize_Rx)
	{
		tGPRSFSM.u16Read_Rx -= tGPRSFSM.u16BufferSize_Rx;
	}
	return u16AvailableBytes;
#endif
	return 0;
}

/**
 * @brief	从串口接收缓冲区中提取数据帧<CR><LF>[data]<CR><LF><CR><LF>[RESULT]<CR><LF>
 * @param	pu8Buffer		[in]	接收帧缓区
 * 			begin			[in]	起始提取位置，无效，实际为Comm_Rx.read
 * 			end				[in]	截止提取位置，仅此参数有效，最后一个可提取字符位置
 * @return		实际数据长度
 */
u16 GPRS_TakeRxFrame(u8* pu8Buffer, u16 begin, u16 end) {
#define PREFIX_POS_UNKOWN	0xFFFF
	static u16 _head = PREFIX_POS_UNKOWN;
	static u8 u8Data = 0;
	static u16 _tail = 0;
	static u16 _write = 0;
	static u16 _read = 0;
	u8 *pBuf = pu8Buffer;
	u16 len = 0;

	/*暂存缓区读、写位置*/
	_write = Comm_Rx.write;
	_read = Comm_Rx.read;

//	if(_read >= _write)
//		return 0;

	while (_read != _write) {
		u8Data = *(Comm_Rx.pBuf + _read);

		//匹配第一个前缀，head_previous记录前缀<CR>位置
		if (_head == PREFIX_POS_UNKOWN) {
			if (u8Data == CHAR_LF) {
				u8 head_previous;
				if (_read == 0)
					head_previous = RX_BUF_SIZE - 1;
				else
					head_previous = _read - 1;

				//匹配到前缀<CR><LF>
				//           ↑
				//			_head
				if (*(Comm_Rx.pBuf + head_previous) == CHAR_CR) {
					_head = head_previous;
				}
			}
		}
		//找到前缀，定位后缀
		else {
			if (u8Data == CHAR_LF) {
				u16 tail_previous;

				if (_read == 0)
					tail_previous = RX_BUF_SIZE - 1;
				else
					tail_previous = _read - 1;

				//找到后缀<CR><LF>
				//         ↑
				//	      _tail
				if (*(Comm_Rx.pBuf + tail_previous) == CHAR_CR) {
					//				ATMsgEvt *pe;
					u16 next = 0;

					_tail = tail_previous;				//Comm_Rx.write;

					//读指针指向前缀<LF>
					next = _head + 1;
					WRAP_AROUND(next, RX_BUF_SIZE);

					//读指针指向前缀<CR><LF>下一个字节
					next++;
					WRAP_AROUND(next, RX_BUF_SIZE);

					//前缀和后缀不是紧随<CR><LF><CR><LF>
					//							↑↑
					//						  next _tail
					//说明中间有数据
					while (next != _tail) {
						*pBuf++ = *(Comm_Rx.pBuf + next);
						next++;
						WRAP_AROUND(next, RX_BUF_SIZE);
						len++;
					}					//if(next != previous)

					//读指针指向下一个待读位置
					_read++;
					WRAP_AROUND(_read, RX_BUF_SIZE);

					//找到一个完整帧，读指针指向下一帧首字节，否则不更新读指针位置
					Comm_Rx.read = _read;

					//准备下一次匹配
					_head = _tail;
					return len;
				}
			}
		}					//else

		_read++;
		WRAP_AROUND(_read, RX_BUF_SIZE);
	}					//end while

	return 0;
}

#if 0

while(start_cursor != end)
{
	//跳过非前缀字符
	if((*(Comm_Rx.pBuf + start_cursor) != CHAR_CR) && (*(Comm_Rx.pBuf + start_cursor) != CHAR_LF))
	{
		start_cursor++;
	}
	else if()

	//跳过<CR><LF>前缀字符
	if(memcmp_wrap_(Comm_Rx.pBuf, start_cursor, end, RX_BUF_SIZE, "\r\n"))
	{
		start_cursor += 2;
	}
	else
	{
		break;
	}

	/*
	 //跳过<CR><LF>前缀字符
	 if((*(Comm_Rx.pBuf + start_cursor) != CHAR_CR) && (*(Comm_Rx.pBuf + start_cursor) != CHAR_LF))
	 {
	 start_cursor++;
	 }
	 */
}

//错误
if(start_cursor == end)
{
	Comm_Rx.read = start_cursor;
	return 0;
}

//开始接收数据内容
start_cursor++;
WRAP_AROUND(start_cursor, RX_BUF_SIZE);

//转储数据内容，直至匹配到后缀停止
while(start_cursor != end)
{
	flag_cur = *(Comm_Rx.pBuf + start_cursor);

	//后缀出现，退出拷贝
	if((flag_former == CHAR_CR) && (flag_cur == CHAR_LF))
	break;

	*pu8Buffer++ = *(Comm_Rx.pBuf + start_cursor++);
	WRAP_AROUND(start_cursor, RX_BUF_SIZE);
	dataLen++;

	flag_former = flag_cur;
}

//读位置最后跳过帧后缀<LF>
start_cursor++;
WRAP_AROUND(start_cursor, RX_BUF_SIZE);

//更新读位置，指向尾缀下一字符
Comm_Rx.read = start_cursor;

//返回实际数据长度，去掉一个后缀<CR>
return dataLen - 1;

#endif

/************************************************************************************************************
 *功能描述：		得到GPRS接收数据帧类型
 *入	   口：		pu8Buffer，		[IN]数据帧缓冲区
 *出    口：		数据帧类型
 ************************************************************************************************************/
_eATACK GPRS_GetRxFrameType(u8* pu8Buffer) {
	u8 i = 0;

	//指针为空，返回未知类型
	if (pu8Buffer == NULL)
		return _ATACK_UNDEFINE;

	//判断接收的数据帧类型
	for (i = 0; i < (u8) _ATACK_END; i++) {
		if (memcmp_(pu8Buffer, (u8*) ATACK[i], strlen_(ATACK[i]))) {
			break;
		}
	}

	if (i < (u8) _ATACK_END) {
		return (_eATACK) i;
	}

	return _ATACK_UNDEFINE;
}

/**
 * @brief	进入休眠模式
 * @param	无
 * @return		无
 */
void GPRS_RequestSleep() {
	static const char cmd[] = "AT+CFUN=0\r";

	TRACE_(QS_USER, NULL, "[UART]Tx: %s", cmd);

	//成功写入数据到发送缓冲区
	GPRS_WriteData(cmd, strlen_(cmd));
}

/**
 * @brief	唤醒模式
 * @param	无
 * @return		无
 */
void GPRS_RequestWakeup() {
	static const char cmd[] = "AT+CFUN=1\r";

	TRACE_(QS_USER, NULL, "%s", cmd);

	//成功写入数据到发送缓冲区
	GPRS_WriteData(cmd, strlen_(cmd));
}

/**
 * @brief	设置CSQ
 * @param	csq		[IN]	当前信号强度
 * @return		无
 */
void GPRS_SetCSQ(u8 csq) {
	s_csq = csq;
}

u8 GPRS_GetCSQ() {
	return s_csq;
}

/**
 * @brief	PDU短消息解码
 * @param	pu8GPRSMessage	[IN]	PDU串指针
 * 			u16GPRSMsgLen	[IN]	PDU串长度
 * @return		1=成功，0=失败
 */
u8 GPRS_PDUSMS_Decode(u8* pu8GPRSMessage, u16 u16GPRSMsgLen) {
	u8 i;
	u8 u8SMS_LongMsg;					//长消息标志
	u8 u8SMS_Type;						//短消息类型
	_GPRS_SENDPDUSMSType tSMS_OA;	//源号码
	u8 u8SMS_DCS;						//数据编码方式
	u8 u8SMS_UDL;						//用户数据长度
	u8 u8SMS_UDHI_Len;					//用户数据头信息长度
	u8 u8SMS_UDHI_ElementA;			//用户数据头信息元素A
	u8 u8SMS_UDHI_ElementA_Len;		//用户数据头信息元素A组长度
	u8 u8SMS_LongMsg_Ref;				//超长短消息参考
	u8 u8SMS_LongMsg_Total;			//超长短消息总链接数
	u8 u8SMS_LongMsg_Sequence;			//超长短消息当前序号
	u16 u16SMS_Year;					//短消息提交-年
	u8 u8SMS_Month;					//短消息提交-月
	u8 u8SMS_Day;						//短消息提交-日
	u8 u8SMS_Hour;						//短消息提交-时
	u8 u8SMS_Minute;					//短消息提交-分
	u8 u8SMS_Second;					//短消息提交-秒
	s32 s32SMS_Store_Index;				//短消息当前存储指针
	u8 u8SMS_LeftBits = 0;				//短消息7bit编码时，在有信息头的情况下多余码
	u8 u8SMSCLen;						//短信服务中心号码长度
	u8 u8Byte;							//解出码长度(0/1/2字节)
	u32 u32Coded;
	u8 u8SMS_OA_Type;
	u8* pu8GPRSMsgTail = NULL;
	u8 cPhoneNum[20] = { 0 };
	u8 u8PhoneLen = 0;

	if (pu8GPRSMessage == NULL || u16GPRSMsgLen == 0) {
		return 0;
	}

	pu8GPRSMsgTail = pu8GPRSMessage + u16GPRSMsgLen;

	//是一条普通消息
	u8SMS_LongMsg = 0;

	//第1段>:	短信服务中心号码(SMSC)，1~12个字节
	//SMSC格式:	字节数，号码类型，服务中心号码
	//取短信服务中心号码-字节数
	u8SMSCLen = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);					//"08"
	pu8GPRSMessage += 2;

	//读指针跳过短消息中心号码
	pu8GPRSMessage += (u8SMSCLen << 1);						//"91683108501505F0"

	//第2段>	PDU协议数据类型(Type)
	//Bit7:		应答路径(RP)。“0”表示未设置路径，“1”表示设置了应答路径。一般为“0”。
	//Bit6:		用户数据头标签(UDHI)。“0”表示用户数据(UD)部分不包含头信息，“1”表示用
	//			户数据部分的开始包含头信息。对于超长(>140字符)短消息、语音消息、图片、
	//			铃声等，其数据长度均超过140字节，此时该位置为“1”。
	//Bit5:		状态报告指示(SRI)。“0”表示状态报告不包含在短信文本中，“1”表示状态
	//			报告含在短信文本中。
	//Bi4-3:	0
	//Bit2:		待续(MMS)。此值被服务中心设置，“0”表示服务中心还存在更多的未被送达
	//			的短信，“1”表示接收到的是最后一条短信。接收端原则上可以不处理该位。
	//Bit1-0:	消息类型(MTI)。“00”表示短信接收，“01”表示发送短信，“10”表示命令，
	//			“11”为保留。
	u8SMS_Type = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);				//"04"
	pu8GPRSMessage += 2;
	if ((u8SMS_Type & 0x03) != 0) {
		//不是一条接收短消息
		return 0;
	}

	//第3段:	发送者号码(caOA)，2~12字节
	//发送者号码格式:	字节数，号码类型，服务中心号码
	//第3.1段> 源号码长度(数字个数)
	tSMS_OA.u8DigitalLength = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);	//"0B"
	pu8GPRSMessage += 2;
	if (tSMS_OA.u8DigitalLength > 20) {
		//号码错误，太长
		return 0;
	}

	//号码有效
	tSMS_OA.cValidTag = 0x55;

	//第3.2段> 源号码类型，91H=国际号码，A1H=国内号码
	u8SMS_OA_Type = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);
	pu8GPRSMessage += 2;
	if (u8SMS_OA_Type == 0xA1) {
		//国内
		tSMS_OA.fNumberType = 0;
	} else if (u8SMS_OA_Type == 0x91) {
		//国际
		tSMS_OA.fNumberType = 1;
	} else {
		//号码错误
		return FALSE;
	}

	//第3.3段> 源号码
	for (i = 0; i < (tSMS_OA.u8DigitalLength + 1) / 2; i++) {
		//取号码,BCD类型
		tSMS_OA.au8Number[i] = SwapNibble(
				GPRS_GetPDUSMS_HexByte(pu8GPRSMessage));
		pu8GPRSMessage += 2;
	}

	//源号码合法性检查
	u8PhoneLen = PhoneNo_ConvertBaseToAscii(cPhoneNum, &tSMS_OA, 0);

#if 0
	//号码长度错误
	if(0 == u8PhoneLen || u8PhoneLen != ptCommParam->u8CenterMonitorPhoneLen)
	{
		return 0;
	}

	//号码不一致，不执行任何命令
	if(!memcmp(cPhoneNum, ptCommParam->aCenterMonitorPhone, ptCommParam->u8CenterMonitorPhoneLen))
	{
		return 0;
	}
#endif

	//第4段>	协议标签(PID)，1字节
	//Bit7-6:	作用。“00”表示指定的“类型”有效，“01”表示GSM3.4协议标识，“10”为保留，
	//			“11”表示特别专用于服务中心。
	//Bit5:		协议。“0”表示点对点短消息协议，不使用远程网络，“1”表示使用远程网络。
	//Bit4-0:	类型。“00000”表示隐含，“00001”表示电传，“00010”表示Group3传真，
	//			“00100”表示语音，“00101”表示欧洲无线信息系统(ERMES)，“00110”表示国内系统，
	//			“10001”表示任何基于X.400的公用信息处理系统，“10010”表示Email。
	GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);								//跳过"00"
	pu8GPRSMessage += 2;

	//第5段>	数据编码方式(DCS)
	//Bit7-6:	0
	//Bit5:		压缩。“0”表示短消息文本不压缩，“1”文本用GSM标准算法压缩。
	//Bit4:		指定。“0”表示“类型”定义无作用，“1”表示“类型”定义有效，接收端要按照指定
	//			的“类型”进行消息的处理。
	//Bit3-2:	编码。“00”表示文本在8位数据模式下采用7位编码，字符采用标准的GSM字符集，
	//			这种编码方式下可实际传输160个7位字符(140 8/7=160)。“01”表示8位编码方式，
	//			可实际传输140字节数据，主要用于数据传输。“10”表示UCS2编码，为Unicode的
	//			双字节字符集，汉字短消息采用这种编码。“11”为保留。
	//Bit1-0:	类型。“00”为Class 0，表示接收端应直接将短消息显示在屏幕上(称为闪信)，不存储，
	//			也不需经过任何操作进行显示。“01”为Class 1，该类短消息由GSM模块控制，而且应该
	//			存储在SIM卡或模块存储区中。“10”为Class 2，主要用于SIM卡的特定信息，“11”为
	//			Class 3，常规短消息，由AT+CNMI命令控制。特别地，当“指定”为0时，“类型”作用
	//			无效，短消息将存储在SIM卡或模块的存储区中。
	u8SMS_DCS = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);				//编码方式DCS
	pu8GPRSMessage += 2;

	//第6段> 短信服务中心时间戳(SCTS)
	u16SMS_Year = (u16) BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage))) + 2000;	//年(BCD)-"50"
	pu8GPRSMessage += 2;
	u8SMS_Month = BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//月(BCD)-"60"
	pu8GPRSMessage += 2;
	u8SMS_Day = BCD2Decimal(SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//日(BCD)-"01"
	pu8GPRSMessage += 2;
	u8SMS_Hour = BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//时(BCD)-"90"
	pu8GPRSMessage += 2;
	u8SMS_Minute = BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//分(BCD)-"33"
	pu8GPRSMessage += 2;
	u8SMS_Second = BCD2Decimal(
			SwapNibble(GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)));//秒(BCD)-"72"
	pu8GPRSMessage += 2;
	GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);							//跳过:时区-"20"
	pu8GPRSMessage += 2;

	//第7段> 用户数据长度(UDL)
	u8SMS_UDL = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);					//"1C"
	pu8GPRSMessage += 2;

	//短消息包含用户数据头信息(UDHI)?
	if (u8SMS_Type & 0x40) {
		//用户数据头长度
		u8SMS_UDHI_Len = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);
		pu8GPRSMessage += 2;

		//7位编码方式?
		if ((u8SMS_DCS & 0x0C) == 0x00) {

			//实际用户数据多余位,在开始解码前要忽略的位(属于信息头的按照7位方式的压缩位)
			u8SMS_LeftBits = ((u8SMS_UDHI_Len + 1) * 8) % 7;

			//用户数据长度-UDHI长度
			u8SMS_UDL -= ((u8SMS_UDHI_Len + 1) * 8 / 7);
			if (u8SMS_LeftBits) {
				//如果有多余位，则再减1字节
				u8SMS_UDL--;
			}
		}
		//8位或16位Unicode编码
		else {
			//用户数据长度-UDHI长度
			u8SMS_UDL -= (u8SMS_UDHI_Len + 1);
		}

		//处理信息头
		while (u8SMS_UDHI_Len) {
			//读元素标签
			u8SMS_UDHI_ElementA = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);
			pu8GPRSMessage += 2;

			//用户数据头长度-1
			u8SMS_UDHI_Len--;

			switch (u8SMS_UDHI_ElementA) {
			//元素类型00H：链接，8位
			case 0x00:
				//元素A长度
				u8SMS_UDHI_ElementA_Len = GPRS_GetPDUSMS_HexByte(
						pu8GPRSMessage);
				pu8GPRSMessage += 2;

				//用户数据头长度-1
				u8SMS_UDHI_Len--;

				//链接短消息元素A长度必须=3
				if (u8SMS_UDHI_ElementA_Len == 3) {
					//链接短消息参考
					u8SMS_LongMsg_Ref = GPRS_GetPDUSMS_HexByte(pu8GPRSMessage);
					pu8GPRSMessage += 2;
					//链接短消息总数
					u8SMS_LongMsg_Total = GPRS_GetPDUSMS_HexByte(
							pu8GPRSMessage);
					pu8GPRSMessage += 2;
					//链接短消息当前顺序号
					u8SMS_LongMsg_Sequence = GPRS_GetPDUSMS_HexByte(
							pu8GPRSMessage);
					pu8GPRSMessage += 2;

					//是一条纯粹的超长短消息
					u8SMS_LongMsg = 1;

				}
				//错误的头
				else {
					//跳过该信息头
					pu8GPRSMessage += (u8SMS_UDHI_ElementA_Len << 1);
				}

				//用户数据头长度-元素长度
				u8SMS_UDHI_Len -= u8SMS_UDHI_ElementA_Len;

				break;
			}
		}

	}								//end of 短消息包含用户数据头信息(UDHI)?

	//本短消息是超长短消息吗?
	if (u8SMS_LongMsg) {
	} else {
		//普通短消息
//		m_cGPRS_SMS_LongMsg_Total = 0;
//		m_cGPRS_SMS_LongMsg_Count = 0;

		s_u8PDUSMSRx_LenCount = 0;

		//从头开始存储
		s32SMS_Store_Index = 0;

		//存储到普通短消息存储区中
		u8SMS_LongMsg_Sequence = 0;
	}

	//8位模式的7位编码?
	if ((u8SMS_DCS & 0x0C) == 0x00) {
		//启动7位模式解码
		GPRS_SMS_7BitsDecode(0, 1, u8SMS_LeftBits);
	}

	//第8段> 用户数据(UD)
	while (u8SMS_UDL) {
		//8位模式的7位编码?
		if ((u8SMS_DCS & 0x0C) == 0x00) {
			//7位模式解码
			u32Coded = GPRS_SMS_7BitsDecode(
					GPRS_GetPDUSMS_HexByte(pu8GPRSMessage), 0, 0);
			pu8GPRSMessage += 2;

			//判断解出码长
			if ((u32Coded & 0xFFFF0000) == 0xFFFF0000) {
				//如果剩余字符数为1，则只取解出码低字节
				if (u8SMS_UDL < 2) {
					//单解出码，实际上是最后一个字符，高字节为填充符(一般为0)
					u8Byte = 1;
				} else {
					//双解出码
					u8Byte = 2;
				}
			} else if ((u32Coded & 0xFFFF0000) == 0x00FF0000) {
				//第1个单解出码
				u8Byte = 1;
			} else if ((u32Coded & 0xFFFF0000) == 0xFF000000) {
				//第2个单解出码,移到低位
				u32Coded >>= 8;

				u8Byte = 1;
			} else {
				//无解出码
				u8Byte = 0;

				//当指针超出PDU串尾
				if (pu8GPRSMessage > pu8GPRSMsgTail) {
					return 0;
				}
			}

			if (u8Byte) {
				//存储短消息内容
				if (!GPRS_StoreSMSContent(u8SMS_DCS, (u16) (u32Coded & 0xFFFF),
						s32SMS_Store_Index, u8Byte, u8SMS_LongMsg_Sequence)) {
					//出错处理
					//		GPRS_SMS_Clear();
					s_u8PDUSMSRx_LenCount = 0;

					//满/出错
					return 0;
				}

				//存储索引位置+字节数
				s32SMS_Store_Index += u8Byte;

				//用户数据长度-字节数
				u8SMS_UDL -= u8Byte;
			}
		}

		//8位编码?
		else if ((u8SMS_DCS & 0x0C) == 0x04) {
			//存储短消息内容
			if (!GPRS_StoreSMSContent(u8SMS_DCS,
					(u16) GPRS_GetPDUSMS_HexByte(pu8GPRSMessage),
					s32SMS_Store_Index, 1, u8SMS_LongMsg_Sequence)) {
				//出错处理
				//		GPRS_SMS_Clear();

				s_u8PDUSMSRx_LenCount = 0;

				//满/出错
				return 0;
			}

			pu8GPRSMessage += 2;

			//存储索引位置+1
			s32SMS_Store_Index++;

			//用户数据长度-1
			u8SMS_UDL--;
		}

		//16位Unicode编码?
		else if ((u8SMS_DCS & 0x0C) == 0x08) {
			u32Coded = ((((u16) GPRS_GetPDUSMS_HexByte(pu8GPRSMessage)) << 8)
					& 0xFF00);
			pu8GPRSMessage += 2;
			u32Coded |= ((u16) GPRS_GetPDUSMS_HexByte(pu8GPRSMessage) & 0x00FF);
			pu8GPRSMessage += 2;

			//存储短消息内容
			if (!GPRS_StoreSMSContent(u8SMS_DCS, u32Coded, s32SMS_Store_Index,
					2, u8SMS_LongMsg_Sequence)) {
				//出错处理
				//		GPRS_SMS_Clear();

				s_u8PDUSMSRx_LenCount = 0;

				//满/出错
				return 0;
			}
			//存储索引位置+2
			s32SMS_Store_Index += 2;

			//用户数据长度-2
			u8SMS_UDL -= 2;
		}

		//未知编码
		else {
			//清除记录
			//		GPRS_SMS_Clear();

			s_u8PDUSMSRx_LenCount = 0;

			return 0;
		}
	}
	if (u8SMS_LongMsg) {
		//短消息接收超时时间=10分钟
//		m_dwGPRS_SMS_Timeout = 600000;
	}

//	GPRS_NewSMS_Process(au8Buffer_SMSRx,s_u8PDUSMSRx_LenCount);

	s_u8PDUSMSRx_LenCount = 0;

	return 1;
}

/***********************************************************************************************************
 * 功能描述：	获取PDU格式短消息一字节内容
 * 入    口：	pu8GPRSMessage,	[IN]GPRS响应字符串指针引用
 * 出    口：	字节内容
 * 说	明：
 ***********************************************************************************************************/
u8 GPRS_GetPDUSMS_HexByte(u8* pu8GPRSMessage) {
	u8 cResult, c;

	//第1字节
	c = *pu8GPRSMessage++;

	//0-9
	if (c <= 0x39) {
		c -= 0x30;
	}
	//A-F
	else {
		c -= 0x37;
	}

	cResult = c << 4;

	//第2字节
	c = *pu8GPRSMessage++;

	//0-9
	if (c <= 0x39) {
		c -= 0x30;
	}
	//A-F
	else {
		c -= 0x37;
	}

	//组合
	cResult |= c;

	return cResult;
}

/***********************************************************************************************************
 * 功能描述：	7位PDU短消息解码
 * 入    口：	u8PDUu8，	[IN] 当前字节
 bStartup，	[IN] 启动标志,真=启动
 cLeftBits，	[IN] 前向余码位
 * 出    口：	u16，bit0-7为第1个解出码内容
 bit8-15为第2个解出码内容
 bit16-23为FF，则第1个解出码有效
 bit24-31为FF则第2个解出码有效
 * 说	明：
 ***********************************************************************************************************/
u32 GPRS_SMS_7BitsDecode(u8 u8PDUByte, u8 u8Startup, u8 u8LeftBits) {
	u8 u8DecodedByte;
	u32 u32FinalDecoded;

	//重新开始解码?
	if (u8Startup) {
		//重新开始
		u8GPRS_7bitDecode_LeftCode = 0;
		u8GPRS_7bitDecode_Sequence = u8LeftBits;

		//前有余码，则首个字符不解,如果余码为6位，则下个字符正好作为高8位解出
		if (u8LeftBits) {
			fGPRS_7bitDecode_Skip = 1;
		}

		return 0;
	}

	//无解出码
	u32FinalDecoded = 0;

	u8DecodedByte = (u8PDUByte << u8GPRS_7bitDecode_Sequence)
			| u8GPRS_7bitDecode_LeftCode;
	u8GPRS_7bitDecode_LeftCode = u8PDUByte >> (7 - u8GPRS_7bitDecode_Sequence);

	//解码移位计数+1
	u8GPRS_7bitDecode_Sequence++;

	//第1个解出码
	u32FinalDecoded |= ((u8DecodedByte & 0x7F) | 0x00FF0000);

	//跳过首字符?
	if (fGPRS_7bitDecode_Skip) {
		fGPRS_7bitDecode_Skip = 0;

		u32FinalDecoded &= 0xFF00FFFF;
	}

	//循环，每解7个字节，会多出1字节
	if (u8GPRS_7bitDecode_Sequence == 7) {
		//解码移位计数=0
		u8GPRS_7bitDecode_Sequence = 0;

		//第2个解出码
		u32FinalDecoded |= ((((u16) u8GPRS_7bitDecode_LeftCode) << 8) & 0x7F00);

		//双解出码
		u32FinalDecoded |= 0xFF000000;

		//无剩余码
		u8GPRS_7bitDecode_LeftCode = 0;
	}

	return u32FinalDecoded;
}

/*******************************************************************************************
 功能描述：	将基本结构号码转换成ASCII串
 入口：	pcNo,			[OUT] ASCII号码指针
 ptBaseNo,		[IN]  基本号码指针
 nInternational,	[IN]  0=国内，只拷贝国内号码部分，1=国际，拷贝全部号码
 返回：	u8,			号码长度(字节数)
 设计：	廖颖
 *******************************************************************************************/
u8 PhoneNo_ConvertBaseToAscii(u8 *pcNo, _GPRS_SENDPDUSMSType *ptBaseNo,
		u8 nInternational) {
	u8 nLen = 0;
	u8 nSkipDigits = 0;
	u8 i = 0;

	//号码无效?
	if ((ptBaseNo->cValidTag != 0x55) || (ptBaseNo->u8DigitalLength == 0)) {
		return 0;
	}

	//如果只要求国内号码，而基本号码是国际号码，则去掉国际区号
	if ((nInternational == 0) && (ptBaseNo->fNumberType == 1)) {
		if (ptBaseNo->au8Number[0] == 0x86) {
			//跳过国际区号
			nSkipDigits = 2;
		}
	}

	//拷贝号码
	for (i = 0; i < (ptBaseNo->u8DigitalLength + 1) / 2; i++) {

		u8 cNo = ptBaseNo->au8Number[i];
		u8 c;

		//号码结束?
		if ((cNo & 0xF0) == 0xF0) {
			//结束
			break;
		}

		if (nSkipDigits == 0) {
			//高位转换成ASCII

			//是“*”?
			if ((cNo & 0xF0) == 0xA0) {
				c = '*';
			}
			//是“#”?
			else if ((cNo & 0xF0) == 0xB0) {
				c = '#';
			} else {
				c = ((cNo >> 4) & 0x0F) + 0x30;
			}

			//存储
			*(pcNo + nLen++) = c;
		} else {
			//跳过1位
			nSkipDigits--;
		}

		//号码结束?
		if ((cNo & 0x0F) == 0x0F) {
			//结束
			break;
		}

		if (nSkipDigits == 0) {
			//低位转换成ASCII

			//是“*”?
			if ((cNo & 0x0F) == 0x0A) {
				c = '*';
			}
			//是“#”?
			else if ((cNo & 0x0F) == 0x0B) {
				c = '#';
			} else {
				c = (cNo & 0x0F) + 0x30;
			}

			//存储
			*(pcNo + nLen++) = c;
		} else {
			//跳过1位
			nSkipDigits--;
		}
	}

	//返回号码长度
	return nLen;
}

/***********************************************************************************************************
 * 功能描述：	交换半位元组
 * 入    口：	u8Byte，	[IN]输入字节
 *				0xMN -> 0xNM
 * 出    口：	结果字节
 ***********************************************************************************************************/
u8 SwapNibble(u8 u8Byte) {
	return ((u8Byte << 4) & 0xF0) | ((u8Byte >> 4) & 0x0F);
}

/***********************************************************************************************************
 * 功能描述：	BCD转换为十进制
 * 入    口：	u8BCD，	[IN]输入字节
 * 出    口：	结果字节
 ***********************************************************************************************************/
u8 BCD2Decimal(u8 u8BCD) {
	return ((u8BCD >> 4) * 10) + (u8BCD & 0x0F);
}

/*******************************************************************************************
 *功能描述：	短消息码元存储
 *入	   口：	u8SMS_DCS				[IN] 短信编码方式
 *			u16Code,				[IN] 需要存储的码元
 *			s16Index,				[IN] 存储位置索引
 *			s16Bytes,				[IN] 字节数,1/2
 *			s16SequenceLongSMS,		[IN] 不为0时，超长短消息序号；=0时表示普通短消息
 *出    口：	1=存储成功，=0存储失败
 *******************************************************************************************/
u8 GPRS_StoreSMSContent(u8 u8SMS_DCS, u16 u16Code, s16 s16Index, s16 s16Bytes,
		s16 s16SequenceLongSMS) {
	//是超长短消息?
	if (s16SequenceLongSMS) {
		return 0;
	}

	//普通短消息
	//Unicode?
	if ((u8SMS_DCS & 0x0C) == 8) {
		//Unicode必须是2字节
		if (s16Bytes != 2) {
			return 0;
		}

		//存储区满?
		if (s_u8PDUSMSRx_LenCount < 139) {
			//存储高字节
			au8Buffer_SMSRx[s16Index] = (u16Code >> 8) & 0xFF;

			//存储低字节
			au8Buffer_SMSRx[s16Index + 1] = u16Code & 0xFF;

			//本组长度+2
			s_u8PDUSMSRx_LenCount += 2;
		} else {
			//存储区满
			return 0;
		}
	}

	else {
		//存储区满?
		if (s_u8PDUSMSRx_LenCount < 160) {
			//存储一个字节
			au8Buffer_SMSRx[s16Index] = u16Code & 0xFF;
			//本组长度+1
			s_u8PDUSMSRx_LenCount++;
		}

		else {
			//存储区满
			return 0;
		}

		//双字节?
		if (s16Bytes == 2) {
			//存储区满?
			if (s_u8PDUSMSRx_LenCount < 160) {
				//存储一个字节
				au8Buffer_SMSRx[s16Index + 1] = (u16Code >> 8) & 0xFF;
				//本组长度+1
				s_u8PDUSMSRx_LenCount++;
			}

			else {
				//存储区满
				return 0;
			}
		}
	}

	return 1;
}

/**
 * GPRS硬件初始化
 *
 */
void GPRS_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = GSM_PWR_PIN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GSM_DTR_PIN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GSM_PWKEY_PIN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	///串口初始化
	USART_Configuration(COM_GPRS, 115200);
}
