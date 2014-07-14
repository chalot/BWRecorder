#include <vdr.h>
#include <utility.h>
#include <stm32f2xx_conf.h>
#include <qactiveobjs.h>
#include <qp_port.h>
#include <comm.h>
#include <nandflash.h>

Q_DEFINE_THIS_MODULE("vdr.c")

/*日志，速度*/
//tSPEEDLOGHEAD tSpeedLogHead;

//串口收发缓区
#define RX_BUF_SIZE		1024
#define TX_BUF_SIZE		1056
static u8 buf_Rx[RX_BUF_SIZE];	//接收缓冲区
static u8 buf_Tx[TX_BUF_SIZE];	//发送缓冲区

static tCOMM Comm_Tx;  //发送缓区
static tCOMM Comm_Rx;  //发送缓区

///协议帧接收状态
enum {
	eCOMM_RECV_IDLE = 0,
	eCOMM_RECV_HEAD1,
	eCOMM_RECV_HEAD2,
	eCOMM_RECV_CMD,
	eCOMM_RECV_LEN1,
	eCOMM_RECV_LEN2,
	eCOMM_RECV_RESERVED,
	eCOMM_RECV_CRC,
	eCOMM_RECV_DATA,
};

//接收数据帧处理缓区
#define FRAME_MAX_LEN 	1024
static u8 aFrameBuf[FRAME_MAX_LEN];
static u8 eVdrCommRecvStatus = eCOMM_RECV_IDLE;	///串口帧接收状态

static tVDRPARAM_ST tVDRParam_St;	///记录仪全局参数

/*记录仪参数配置项*/
typedef struct {
	u32 u32Addr;	///FRAM中地址
	u8 *ptr;		///内存指针
	u16 u16Size;	///数据长度
} VDR_PARAM_ST;

/*记录仪各参数配置信息*/
static const VDR_PARAM_ST tParamConf[] = {
		{.u32Addr = FRAM_ADDR_VDR_PARAM_CARINFO,
				.ptr = (u8*)&tVDRParam_St.tCarInfo_St,
				.u16Size = sizeof(tCARINFO_ST)},
		{.u32Addr = FRAM_ADDR_VDR_PARAM_INSTALLTIME,
				.ptr = (u8*)&tVDRParam_St.tInstData_St,
				.u16Size = sizeof(tTIME_ST)},
		{.u32Addr = FRAM_ADDR_VDR_PARAM_STATECONFIG,
				.ptr = (u8*)&tVDRParam_St.tStateConf_St,
				.u16Size = sizeof(tSTATECONFIG_ST)},
		{.u32Addr = FRAM_ADDR_VDR_PARAM_RTC,
				.ptr = (u8*)&tVDRParam_St.tRTC_St,
				.u16Size = sizeof(tTIME_ST)},
		{.u32Addr = FRAM_ADDR_VDR_PARAM_PULSE,
				.ptr = (u8*)&tVDRParam_St.tPulse_St,
				.u16Size = sizeof(tPULSE_ST)},
		{.u32Addr = FRAM_ADDR_VDR_PARAM_INITDIST,
				.ptr = (u8*)&tVDRParam_St.tInitDist_St,
				.u16Size = sizeof(tINITDISTANCE_ST)},
		{.u32Addr = FRAM_ADDR_VDR_PARAM_LICENSE,
				.ptr = (u8*)&tVDRParam_St.tLicense_St,
				.u16Size = sizeof(tLICENSE_ST)},
		{.u32Addr = FRAM_ADDR_VDR_PARAM_ID,
				.ptr = (u8*)&tVDRParam_St.tID_St,
				.u16Size = sizeof(tUNIQID_ST)},
};


/**
 * 命令帧处理
 *
 * @param u16BeginPtr		帧数据在缓区中的起始位置
 * @param u16EndPtr			帧数据在缓区中的结尾位置
 */
void VDR_Comm_doProcMessage(u16 u16BeginPtr, u16 u16EndPtr) {
	u8* pFrame;
	u16 u16FrameLen;
	tFRAMEHEAD *ptHead = NULL;
	BOOL bResult = TRUE;
	u8 u8Cmd = 0;
	int ret;

	/*从接收缓区提取一帧，帧起始和结束位置已指定*/
	u16FrameLen = VDR_Comm_doRetrieveMessage(aFrameBuf, u16BeginPtr, u16EndPtr);
	if (0 == u16FrameLen)
		return;

	///帧校验
	bResult = VDR_Comm_CheckFrame(aFrameBuf, u16FrameLen);
	if (FALSE == bResult)
		return;

	ptHead = (tFRAMEHEAD*)aFrameBuf;

	//pFrame指向数据
	pFrame = aFrameBuf + sizeof(tFRAMEHEAD);

	switch (ptHead->u8Cmd) {
	/**
	 * 采集数据命令字定义
	 */
	case VDR_CMD_GET_VERSION:	///0x00	//采集记录仪版本
	case VDR_CMD_GET_DRIVERINFO:	///0x01	///驾驶人信息
	case VDR_CMD_GET_RTC:	///0x02	///时间
	case VDR_CMD_GET_DISTANCE:	///0x03	///里程
	case VDR_CMD_GET_PULSE:	///0x04	///脉冲系数
	case VDR_CMD_GET_CARINFO:	///0x05	///车辆信息
	case VDR_CMD_GET_STATECONFIG_INFO:	///0x06	///状态信号配置信息
	case VDR_CMD_GET_ID:	///0x07	///唯一性ID
	{
		VDRAckEvt *pe;
		pe = Q_NEW(VDRAckEvt, VDR_ACK_READY_SIG);
		pe->cmd = ptHead->u8Cmd;
		pe->ret = TRUE;
		QACTIVE_POST(AO_VDR, (QEvt* )pe, NULL);
	}
		break;
	case VDR_CMD_GET_SPEED:	///0x08	///行驶速度

		break;
	case VDR_CMD_GET_POSITION:	///0x09 	///位置

		break;
	case VDR_CMD_GET_ACCIDENT:	///0x10	///事故疑点

		break;
	case VDR_CMD_GET_OVERTIME_DRIVING:	///0x11	///超时驾驶

		break;
	case VDR_CMD_GET_DRIVER_LOG_INFO:	///0x12	///登入签出

		break;
	case VDR_CMD_GET_POWER:	///0x13	///供电

		break;
	case VDR_CMD_GET_PARAM_CHANGE:	///0x14	///参数修改

		break;
	case VDR_CMD_GET_SPEED_STATUS_LOG:	///0x15	///速度状态日志

		break;

		/**
		 * 设置参数类
		 */
	case VDR_CMD_SET_VECHICLE_INFO:	///0x82	///车辆信息
		ret = VDR_SaveParam(VDR_PARAM_CARINFO, pFrame, u16FrameLen);
		break;

	case VDR_CMD_SET_FIRST_INSTLL_DATA:	///0x83	///初次安装日期
		ret = VDR_SaveParam(VDR_PARAM_INSTALLTIME, pFrame, u16FrameLen);
		break;

	case VDR_CMD_SET_STATUS_CONFIG_INFO:	///0x84	///状态量配置信息
		ret = VDR_SaveParam(VDR_PARAM_STATECONFIG, pFrame, u16FrameLen);
		break;

	case VDR_CMD_SET_TIME:	///0xC2	///时间
		ret = VDR_SaveParam(VDR_PARAM_RTC, pFrame, u16FrameLen);
		break;

	case VDR_CMD_SET_PULSE:	///0xC3	///脉冲系数
		ret = VDR_SaveParam(VDR_PARAM_PULSE, pFrame, u16FrameLen);
		break;

	case VDR_CMD_SET_INITIALDIST:	///0xC4	///初始里程
		ret = VDR_SaveParam(VDR_PARAM_INITDIST, pFrame, u16FrameLen);
		break;

	default:
		break;
	}

	/*统一错误处理*/
	if(ret < 0)
		VDR_Err_Proc(ptHead->u8Cmd, ret);
}


/**
 * 构造应答帧
 *
 * @param pAckEvt		应答命令消息
 * @param pFrameBuf		应答帧缓区
 * @return	应答帧长度
 */
u16 VDR_Comm_FormAckFrame(VDRAckEvt *pAckEvt, u8 *pFrameBuf) {
	u16 u16FrameLen = 0;
	tFRAMEHEAD *pHead = NULL;
	u16 u16FrameLen = 0;
	u8 *pData = NULL;
	u16 u16DataLen = 0;

	pHead = (tFRAMEHEAD *)pFrameBuf;
	pHead->u8Head1 = _VDR2PC_COMM_FRAME_HEAD1;
	pHead->u8Head2 = _VDR2PC_COMM_FRAME_HEAD2;
	pHead->u8Cmd = pAckEvt->cmd;
	pHead->u8Reserved = 0;

	pData = pFrameBuf + sizeof(tFRAMEHEAD);
	u16FrameLen = sizeof(tFRAMEHEAD);

	switch (pAckEvt->cmd) {
	case VDR_CMD_GET_ERR_ACK: 	///0xAA,出错命令字，自定义
	{
		tACK_ERRFRAME *pFrame = (tACK_ERRFRAME *) pFrameBuf;
		pFrame->u8Head1 = _VDR2PC_COMM_FRAME_HEAD1;
		pFrame->u8Head2 = _VDR2PC_COMM_FRAME_HEAD2;
		pFrame->u8ErrTag = _VDR2PC_COMM_GET_ERR_TAG;
		pFrame->u8Reserved = 0;
		pFrame->u8Checksum = CalculateXOR((u8*) pFrameBuf, sizeof(tACK_ERRFRAME) - 1);

		u16FrameLen = sizeof(tACK_ERRFRAME);
		return u16FrameLen;
	}
		break;
	case VDR_CMD_SET_ERR_ACK: 	///0xBB,出错命令字，自定义
	{
		tACK_ERRFRAME *pFrame = (tACK_ERRFRAME *) pFrameBuf;
		pFrame->u8Head1 = _VDR2PC_COMM_FRAME_HEAD1;
		pFrame->u8Head2 = _VDR2PC_COMM_FRAME_HEAD2;
		pFrame->u8ErrTag = _PC2VDR_COMM_SET_ERR_TAG;
		pFrame->u8Reserved = 0;
		pFrame->u8Checksum = CalculateXOR((u8*) pFrameBuf, sizeof(tACK_ERRFRAME) - 1);

		u16FrameLen = sizeof(tACK_ERRFRAME);
		return u16FrameLen;
	}
		break;

		/**
		 * 采集数据命令字定义
		 */
	case VDR_CMD_GET_VERSION:	///0x00	//采集记录仪版本
	{
		tACK_VERSION *pVersion = (tACK_VERSION *)pData;
		pVersion->u8Version = 0;
		pVersion->u8FixID = 0;

		u16DataLen = sizeof(tACK_VERSION);
	}
		break;
	case VDR_CMD_GET_DRIVERINFO:	///0x01	///驾驶人信息
	{
		tACK_DRIVERLICENSEINFO *pDriverInfo = (tACK_DRIVERLICENSEINFO *)pData;
		memcpy_(pDriverInfo->aDriverLicense, tVDRParam_St.tLicense_St.aLicense, 18);
		u16DataLen = sizeof(tACK_DRIVERLICENSEINFO);
	}
		break;
	case VDR_CMD_GET_RTC:	///0x02	///时间
	{
		tACK_RTC *pRTC = (tACK_RTC *)pData;
		pRTC->time.u8Year = tVDRParam_St.tRTC_St.time.u8Year;
		pRTC->time.u8Month = tVDRParam_St.tRTC_St.time.u8Month;
		pRTC->time.u8Day = tVDRParam_St.tRTC_St.time.u8Day;
		pRTC->time.u8Hour = tVDRParam_St.tRTC_St.time.u8Hour;
		pRTC->time.u8Minute = tVDRParam_St.tRTC_St.time.u8Minute;
		pRTC->time.u8Second = tVDRParam_St.tRTC_St.time.u8Second;

		u16DataLen = sizeof(tACK_RTC);
	}
		break;
	case VDR_CMD_GET_DISTANCE:	///0x03	///里程
	{
		memcpy(pData, (u8*)&tVDRParam_St.tInitDist_St.tInitDist, sizeof(tINITDISTANCE));
		u16DataLen = sizeof(tACK_INITDISTANCE);
	}
		break;
	case VDR_CMD_GET_PULSE:	///0x04	///脉冲系数
	{
		memcpy_(pData, (u8*)&tVDRParam_St.tPulse_St.tPulse, sizeof(tACK_PULSE));
		u16DataLen = sizeof(tACK_PULSE);
	}
		break;
	case VDR_CMD_GET_CARINFO:	///0x05	///车辆信息
	{
		memcpy_(pData, (u8*)&tVDRParam_St.tCarInfo_St.tCarInfo, sizeof(tACK_CARINFO));
		u16DataLen = sizeof(tACK_CARINFO);
	}
		break;
	case VDR_CMD_GET_STATECONFIG_INFO:	///0x06	///状态信号配置信息
	{
		memcpy_(pData, (u8*)&tVDRParam_St.tStateConf_St.tStateConf, sizeof(tACK_STATECONFIG));
		u16DataLen = sizeof(tACK_STATECONFIG);
	}
		break;
	case VDR_CMD_GET_ID:	///0x07	///唯一性ID
	{
		memcpy_(pData, (u8*)&tVDRParam_St.tID_St.tID, sizeof(tACK_UNIQID));
		u16DataLen = sizeof(tACK_UNIQID);
	}
		break;
	case VDR_CMD_GET_SPEED:	///0x08	///行驶速度
	{

	}

		break;
	case VDR_CMD_GET_POSITION:	///0x09 	///位置
	{

	}

		break;
	case VDR_CMD_GET_ACCIDENT:	///0x10	///事故疑点
	{

	}

		break;
	case VDR_CMD_GET_OVERTIME_DRIVING:	///0x11	///超时驾驶
	{

	}

		break;
	case VDR_CMD_GET_DRIVER_LOG_INFO:	///0x12	///登入签出
	{

	}

		break;
	case VDR_CMD_GET_POWER:	///0x13	///供电
	{

	}

		break;
	case VDR_CMD_GET_PARAM_CHANGE:	///0x14	///参数修改
	{

	}

		break;
	case VDR_CMD_GET_SPEED_STATUS_LOG:	///0x15	///速度状态日志
	{

	}
		break;

	/**
	 * 设置参数类，无应答
	 */
	case VDR_CMD_SET_VECHICLE_INFO:	///0x82	///车辆信息
	case VDR_CMD_SET_FIRST_INSTLL_DATA:	///0x83	///初次安装日期
	case VDR_CMD_SET_STATUS_CONFIG_INFO:	///0x84	///状态量配置信息
	case VDR_CMD_SET_TIME:	///0xC2	///时间
	case VDR_CMD_SET_PULSE:	///0xC3	///脉冲系数
	case VDR_CMD_SET_INITIALDIST:	///0xC4	///初始里程
		break;
	default:
		break;
	}

	/*回填头长度和校验和*/
	pHead->u16DataLength = u16DataLen;
	ENDIAN_U16(pHead->u16DataLength);

	u16FrameLen += u16DataLen + 1;
	*(pData + u16FrameLen - 1) = CalculateXOR((u8*) pFrameBuf, u16FrameLen - 1);

	return u16FrameLen;
}


/**
 * 帧校验
 *
 * @param aFrameBuf
 * @param u16FrameLen
 * @return
 */
BOOL VDR_Comm_CheckFrame( aFrameBuf, u16FrameLen) {
	u8 u8Checksum;

	if (0 == u16FrameLen)
		return FALSE;

	if (aFrameBuf[0] != _PC2VDR_COMM_FRAME_HEAD1)
		return FALSE;

	if (aFrameBuf[1] != _PC2VDR_COMM_FRAME_HEAD2)
		return FALSE;

	u8Checksum = CalculateXOR(aFrameBuf, u16FrameLen - 1);
	if (u8Checksum != aFrameBuf[u16FrameLen - 1])
		return FALSE;

	return TRUE;
}

/**
 * VDR数据初始化
 */
extern void VDR_Comm_MemInit(void) {
	/*初始化收发缓区*/
	Comm_Init(&Comm_Tx, &buf_Tx[0], TX_BUF_SIZE);
	Comm_Init(&Comm_Rx, &buf_Rx[0], RX_BUF_SIZE);

	ZeroMem(&buf_Tx[0], TX_BUF_SIZE);
	ZeroMem(&buf_Rx[0], RX_BUF_SIZE);

	/*使能串口接收中断*/
	USART_Cmd(UART_VDR, ENABLE);
}

/**
 * VDR串口中断服务程序
 *
 */
void ISR_UART_VDR(void) {
	static u8 u8Data = 0;
	static u16 u16RecvData;
	static u16 u16FrameLength = 0;
	static u16 u16CurSavedBytes = 0;
	static u16 u16Frame_StartPtr = 0;

	//发送完成 并且发送中断使能
	if (USART_GetITStatus(UART_VDR, USART_IT_TXE)) {
		USART_ClearITPendingBit(UART_VDR, USART_IT_TXE);

		//数据发送完成了，关发送中断
		if (Comm_Tx.read == Comm_Tx.dataSize) {
			//关闭发送中断
			USART_ITConfig(UART_VDR, USART_IT_TXE, DISABLE);
		}
		else {
			//待发送数据填入串口TXBUFR
			u8Data = *(Comm_Tx.pBuf + Comm_Tx.read++);
			USART_SendData(UART_VDR, u8Data);
		}
	}

	//接收中断
	if (USART_GetITStatus(UART_VDR, USART_IT_RXNE)) {
		USART_ClearITPendingBit(UART_VDR, USART_IT_RXNE);

		//读取数据
		u16RecvData = USART_ReceiveData(UART_VDR);

		u8Data = (u8) u16RecvData;

		//存储数据
		*(Comm_Rx.pBuf + Comm_Rx.write) = u8Data;

		//头字低字节是否收到?
		if (eVdrCommRecvStatus == eCOMM_RECV_IDLE) {
			//数据字节是头字的低字节0xAA
			if (u8Data == _PC2VDR_COMM_FRAME_HEAD1) {
				eVdrCommRecvStatus = eCOMM_RECV_HEAD1;
				u16Frame_StartPtr = Comm_Rx.write;
			}
		}
		else if (eVdrCommRecvStatus == eCOMM_RECV_HEAD1) {
			if (u8Data == _PC2VDR_COMM_FRAME_HEAD2) {
				eVdrCommRecvStatus = eCOMM_RECV_HEAD2;
			}
			else
				eVdrCommRecvStatus = eCOMM_RECV_IDLE;
		}
		else if (eVdrCommRecvStatus == eCOMM_RECV_HEAD2) {
			eVdrCommRecvStatus = eCOMM_RECV_CMD;
		}
		else if (eVdrCommRecvStatus == eCOMM_RECV_CMD) {
			eVdrCommRecvStatus = eCOMM_RECV_LEN1;
		}
		else if (eVdrCommRecvStatus == eCOMM_RECV_LEN1) {
			eVdrCommRecvStatus = eCOMM_RECV_LEN2;
			u16FrameLength = u8Data;
		}
		else if (eVdrCommRecvStatus == eCOMM_RECV_LEN2) {
			eVdrCommRecvStatus = eCOMM_RECV_RESERVED;
			u16FrameLength |= (u16) u8Data << 8;
		}
		else if (eVdrCommRecvStatus == eCOMM_RECV_RESERVED) {
			u16CurSavedBytes = 0;
			if (0 == u16FrameLength) {
				eVdrCommRecvStatus = eCOMM_RECV_CRC;
			}
			else {
				eVdrCommRecvStatus = eCOMM_RECV_DATA;
			}
		}
		else if (eVdrCommRecvStatus == eCOMM_RECV_DATA) {
			u16CurSavedBytes++;
			if (u16CurSavedBytes >= u16FrameLength) {
				eVdrCommRecvStatus = eCOMM_RECV_CRC;
			}
		}
		else if (eVdrCommRecvStatus == eCOMM_RECV_CRC) {
			///一帧提取完成，发送帧到达消息，包括起始位置和结束位置，以方便提取
			VDRRetrieveEvt *pe;
			pe = Q_NEW(VDRRetrieveEvt, VDR_FRAME_READY_SIG);
			Q_ASSERT(pe);
			pe->u16BeginPtr = u16Frame_StartPtr;	//帧头数据位置
			pe->u16EndPtr = Comm_Rx.write;	//帧尾数据位置
			QACTIVE_POST(AO_VDR, (QEvt* ) pe, NULL);

			eVdrCommRecvStatus = eCOMM_RECV_IDLE;
		}
		else {	///异常情况，忽略
			eVdrCommRecvStatus = eCOMM_RECV_IDLE;
		}

		Comm_Rx.write++; 	//接收缓冲区写指针回头
		WRAP_AROUND(Comm_Rx.write, RX_BUF_SIZE);
	}
}

/**
 * 串口发送数据
 *
 * @param pu8Data			待发送数据
 * @param u16Length			待发送数据长度
 * @return
 */
u8 VDR_UART_WriteData(u8* pu8Data, u16 u16Length) {
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
 * 启动串口数据发送,发送数据应已填入发送缓区
 */
void UART_COMM_StartTransmit(void) {
	u8 u8SendData;

	//取第一个待发送数据
	u8SendData = *(Comm_Tx.pBuf + Comm_Tx.read++);

	//开始以中断方式发送
	USART_SendData(UART_VDR, u8SendData);
	USART_ITConfig(UART_VDR, USART_IT_TXE, ENABLE);
}

/**
 * 提取VDR数据帧
 *
 * @param pMsg				帧缓区
 * @param u16BeginPtr		帧在接收缓区的起始位置
 * @param u16EndPtr			帧在接收缓区的结束位置
 * @return	帧长度，0：提取失败，否则帧长度
 */
u16 VDR_Comm_doRetrieveMessage(u8 *pMsg, u16 u16BeginPtr, u16 u16EndPtr) {
	u16 u16FrameLen = 0;
	u16 ptr = u16BeginPtr;

	while (ptr != u16EndPtr) {
		aFrameBuf[u16FrameLen++] = Comm_Rx.pBuf + ptr;
		ptr++; 	//接收缓冲区写指针回头
		WRAP_AROUND(ptr, RX_BUF_SIZE);
	}

	return u16FrameLen;
}

/**
 * 字节转换为BCD码
 *
 * @param u8Byte
 * @return
 */
u8 BYTE2BCD(u8 u8Byte) {
	if (u8Byte >= 100) {
		return 0;

	}
	return ((u8Byte / 10) << 4) | (u8Byte % 10);
}

/**
 * BCD码转换为字节
 *
 * @param u8Bcd
 * @return
 */
u8 BCD2BYTE(u8 u8Bcd) {
	return (u8Bcd >> 4) * 10 + (u8Bcd & 0x0F);
}

#if 0
/**
 * 每秒钟保存一次速度记录
 *
 * @param pTime			当前时刻
 * @param pSpeedLog		速度信息
 */
void VDR_Log_SaveSpeedInfo(tTIME *pTime, tSPEEDLOGPERSEC *pSpeedLog) {
	u16 u16ItemOffset = 0;
	_eERRType err;
	tSPEEDLOGITEM *pSpeedItem;

	pSpeedItem = (tSPEEDLOGITEM *)tSpeedLogHead.aPageBuf;

	/*根据日期定位存储位置*/
	/*页缓区为空，开始新的存储周期*/
	if(tSpeedLogHead.bPageEmpty) {
		ZeroMem(tSpeedLogHead.aPageBuf, 2048);
		memcpy_(&pSpeedItem->time_Start, pTime, sizeof(tTIME));
		pSpeedItem->time_Start.u8Second = 0;
		memcpy_(pSpeedItem->tSpeed[u16ItemOffset], pSpeedLog, sizeof(tSPEEDLOGPERSEC));

		tSpeedLogHead.bPageEmpty = 0;
	}
	else {
		u16 u16TimeElapse = TIME_ELLAPSED_INSEC(pTime, pSpeedItem->time_Start);
		if(u16TimeElapse <= 8*60) { ///页未满，时间在8分钟内，页内保存，不写FLASH
			u8 u8ItemNum = u16TimeElapse / 60;
			pSpeedItem += u8ItemNum;
			memcpy_(pSpeedItem->tSpeed[pTime->u8Second], pSpeedLog, sizeof(tSPEEDLOGPERSEC));
		}
		/*页满，写FLASH，清空页缓区，并初始化第一个存储项*/
		if(u16TimeElapse >= 8*60) {
			err = NF_PageWrite(tSpeedLogHead.u16BlockId_Head, tSpeedLogHead.u8PageId_Head,
					NF_PAGE_SIZE, tSpeedLogHead.aPageBuf);
			if (_NO_ERR_ == err) {
				/*在页空闲区保存第一个记录的时间*/
				NF_WriteSpareArea(tSpeedLogHead.u16BlockId_Head, tSpeedLogHead.u8PageId_Head,
						sizeof(tTIME), (u8*)pTime);

				/*更新下一写位置*/
				tSpeedLogHead.u8PageId_Head++;
				if (tSpeedLogHead.u8PageId_Head == NF_PAGE_AMOUNT) {
					tSpeedLogHead.u16BlockId_Head = GetNextValidBlock(tSpeedLogHead.u16BlockId_Start,
							tSpeedLogHead.u16BlockId_Last, tSpeedLogHead.u16BlockId_Head);
					tSpeedLogHead.u8PageId_Head = 0;
					err = NF_EraseBlock(tSpeedLogHead.u16BlockId_Head);
					if (_NO_ERR_ != err) {
						NF_MarkErrBlock(tSpeedLogHead.u16BlockId_Head);
						return;
					}
					/*以下处理索引头和读头也被写回头擦除的情况*/
					if (tSpeedLogHead.u16BlockId_Tail == tSpeedLogHead.u16BlockId_Head) {
						tSpeedLogHead.u16BlockId_Tail = GetNextValidBlock(tSpeedLogHead.u16BlockId_Start,
								tSpeedLogHead.u16BlockId_Last, tSpeedLogHead.u16BlockId_Tail);
					}

					if(tSpeedLogHead.u16BlockId_UplCursor == tSpeedLogHead.u16BlockId_Head) {
						tSpeedLogHead.u16BlockId_Tail = GetNextValidBlock(tSpeedLogHead.u16BlockId_Start,
								tSpeedLogHead.u16BlockId_Last, tSpeedLogHead.u16BlockId_UplCursor);
					}
				}
			}
			else {
				NF_MarkErrBlock(tSpeedLogHead.u16BlockId_Head);
			}

			tSpeedLogHead.bPageEmpty = 1;
		}
	}
}

/**
 * 指定开始时间和结束时间，得到数据索引位置
 * 查找方法：逆序先从最后一块的最后一页读取空闲区时间标志，块内二分法查找到页
 * 问题：数据可能在页面缓区中，页未满，尚未存入NANDFLASH
 *
 * @param pTime_Begin
 * @param pTime_End
 * @return 起始数据块页号及页内偏移，结束数据块页号及页内偏移
 */
int VDR_Log_LocateSpeedInfo(tTIME *pTime_Begin, tTIME *pTime_End) {
	/*先查找内存数据*/
	if(-1 == TIME_COMPARE(pTime_Begin, &tSpeedLogHead.)) {





	}




	/*查找最后一条记录位置*/






	/*查找第一条记录位置*/




}

/**
 * 提取一组速度记录数据，待上传。
 *
 * @param pBuf
 * @return
 */
int VDR_Log_ReadSpeedInfo(u8 *pBuf) {
	u16 u16DataLen = 0;


	return u16DataLen;

}

/**
 * 计算下一个有效块，跳过坏块
 *
 * @param u16BlockId_Start
 * @param u16BlockId_Last
 * @param u16BlockId_Cur
 * @return
 */
u16 GetNextValidBlock(u16 u16BlockId_Start, u16 u16BlockId_Last, u16 u16BlockId_Cur) {
	u16 u16BlockId = u16BlockId_Cur;

	u16BlockId++;

	if (u16BlockId > u16BlockId_Last)
		u16BlockId = u16BlockId_Start;

	while (u16BlockId != u16BlockId_Last) {
		if (!NF_IsErrBlock(u16BlockId))
			break;
		u16BlockId++;
	}

	if (u16BlockId == u16BlockId_Last)
		return NF_BLOCK_INVALID;

	return u16BlockId;
}
#endif

/**
 * 时间比较
 * @param pTime1
 * @param pTime2
 * @return	1: t1 > t2; 0:t1=t2; -1:t1<t2
 */
int TIME_COMPARE(tTIME *pTime1, tTIME *pTime2) {
	if(pTime1->u8Year > pTime2->u8Year)
		return 1;
	else if(pTime1->u8Year < pTime2->u8Year)
		return -1;
	else if(pTime1->u8Month > pTime2->u8Month)
		return 1;
	else if(pTime1->u8Month < pTime2->u8Month)
		return -1;
	else if(pTime1->u8Day > pTime2->u8Day)
		return 1;
	else if(pTime1->u8Day < pTime2->u8Day)
		return -1;
	else if(pTime1->u8Hour > pTime2->u8Hour)
		return 1;
	else if(pTime1->u8Hour < pTime2->u8Hour)
		return -1;
	else if(pTime1->u8Minute > pTime2->u8Minute)
		return 1;
	else if(pTime1->u8Minute < pTime2->u8Minute)
		return -1;
	else if(pTime1->u8Second > pTime2->u8Second)
		return 1;
	else if(pTime1->u8Second < pTime2->u8Second)
		return -1;
	else
		return 0;
}

/**
 * 计算两个时刻相差秒数
 *
 * @param t1
 * @param t2
 * @return
 */
u16 TIME_ELLAPSED_INSEC(tTIME *t1, tTIME *t2) {
	u16 u16TimeElapse = 0;

	return 0;
}

/**
 * 保存记录仪参数
 *
 * @param eParam	参数类型
 * @param pBuf		参数缓区
 * @param u16Len	参数长度
 * @return 0=成功，否则错误代码
 */
int VDR_SaveParam(eVDRPARAM eParam, u8 *pBuf, u16 u16Len) {
	int ret;
	u8 u8CRC = 0;

	if(pBuf == NULL)
		return -ERR_PARAM_INVALID;

	if(eParam >= VDR_PARAM_END)
		return -ERR_PARAM_INVALID;

	if(0 == u16Len)
		return 0;

	if(u16Len > tParamConf[eParam].u16Size - 1)
		return ERR_PARAM_INVALID;

	memcpy_(tParamConf[eParam].ptr, pBuf, u16Len);

	/*填充CRC码*/
	u8CRC = CalculateCRC8(pBuf, u16Len);
	*(tParamConf[eParam].ptr + tParamConf[eParam].u16Size - 1) = u8CRC;

	/*保存*/
	ret = FRAM_WriteBuffer(tParamConf[eParam].ptr, tParamConf[eParam].u16Size);
	if(ret < 0)
		return ret;

	/*校验*/
	if(u8CRC != CalculateCRC8(tParamConf[eParam].ptr, tParamConf[eParam].u16Size - 1)) {
		return -ERR_CHECKSUM;
	}

	return 0;
}

/**
 * 一次性读取所有记录仪参数
 *
 * @return	0=成功，否则错误代码
 */
int VDR_LoadParam(void) {
	int ret;
	u8 i;
	u8 u8CRC = 0;

	/*读出参数*/
	ret = FRAM_ReadBuffer(FRAM_ADDR_VDR_PARAM_START, &tVDRParam_St, sizeof(tVDRPARAM_ST));
	if(ret < 0)
		return ret;

	/*各项参数校验*/
	i = 0;
	while(i < (u8)VDR_PARAM_END) {
		u8CRC = *(tParamConf[i].ptr + tParamConf[i].u16Size - 1);
		if(u8CRC != CalculateCRC8(tParamConf[i].ptr, tParamConf[i].u16Size - 1)) {
			break;
		}
		i ++;
	}
	if(i >= (u8)VDR_PARAM_END)
		return -ERR_CHECKSUM;

	return 0;
}

/**
 * 取参数项
 *
 * @param eParam
 * @return
 */
u8* VDR_GetParam(eVDRPARAM eParam) {
	return tParamConf[eParam].ptr;
}

/**
 * 统一错误处理
 *
 * @param u8Cmd
 * @param ret
 */
void VDR_Err_Proc(u8 u8Cmd, int ret) {

}
