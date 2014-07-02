/**   
* @Title: sdcard.c  
* @Description: 鐎规矮绠烠AN閸樺棗褰堕弫鐗堝祦閹镐椒绠欓崠鏍у煂SD閸楋紕娈戠�妯哄絿閹垮秳缍旈惃鍕瑐鐏炲倸绨查悽銊﹀复閸欙絻锟�
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-5-27 娑撳﹤宕�1:28:52
* @version V1.0   
*/ 

#include "sdcard_interface.h"
#include "ff.h"
#include "error.h"
#include "diskio.h"


/**
 * @Func	SD卡初始化
 * @Param	无
 * @Ret		0=成功，否则为错误代码(RC)
 */
int SDCard_Init()
{
	if(!SD_Detect())
	{
		return -ERR_SDCARD_NOTEXISTED;
	}

	//初始化成功
	if(disk_initialize(0) != RES_OK)
	{
		return -ERR_SDCARD_INIT;
	}

	return 0;
}

/**
 * @Func	存储原始帧
 * @Param	pFrame			[in]	帧数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
int SD_DumpFrame(CANRAWFRAME* pFrame)
{
	return 0;
}

/**
 * @Func 	在SD卡中根据起始时间和接收时间查找历史记录，精确到秒
 *			提交一次查询请求对应创建一个新的查询索引号，根据此索引号区别不同的查询请求，和定位数据段；
 *			一般同一时间只能存在一个有效查询索引
 * @Param	tick_Begin		[in]	起始时间
 * 			tick_End		[in]	结束时间
 * 			rId				[in]	有效查询索引号
 * @Ret		0=成功；否则失败，返回错误代码（RC）
  */
int SD_StartNewQuery(eSTORAGECLASS class, TIME *tick_Begin, TIME *tick_End)
{
	/*直接调用创建查询会话过程*/
	return SD_NewQuerySession(/*class, */tick_Begin, tick_End);
}

/**
 * @Func 	读取帧块数据，根据查询索引ID来唯一确定查询请求，可能对同一索引ID执行多次查询请求，这发生
 * 			在数据量大需要分块读取的情况下。
 * @Param	rId				[in]	查询索引号
 * 			pMsgBuf			[in]	接收缓区
 * 			u16MsgBufSize	[in]	接收缓区长度
 * @Ret		u16，实际读取字节数，=0说明读取结束
 */
u16 SD_RetrieveFrameBlock(eSTORAGECLASS class, u8 *pMsgBuf, u16 u16MsgBufSize)
{


	return 0;
}

/**
 * @Func	通知车辆启动或开始充电状态，依据协议要求，视为开始一个新的驾驶循环或充电循环，
 * 			需创建一个新的存储文件保存CAN帧数据
 * @Param	time			[in]	当前时刻，可以作为文件名命名依据
 * @Ret		无
 */
void SD_NotifyDriveOrChargeSessionStart(TIME *time)
{


}

/**
 * @Func	通知车辆停车或结束充电状态，依据协议要求，视为一个驾驶循环或充电循环结束，
 * 			终止文件存储
 * @Param	time			[in]	当前时刻
 * @Ret		无
 */
void SD_NotifyDriveOrChargeSessionTerminal(TIME *time)
{


}

/**
 * @Func	存储GPS数据
 * @Param	pGpsInfo			[in]	定位数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
//int SD_DumpGpsInfo(_tGPSSaveInfo* pGpsInfo)
//{
//
//	return 0;
//}


BOOL SDCard_Clear()
{

	return TRUE;
}

//SD卡拔插状态
u8 SDCard_GetExistedState()
{

	return 0;
}

//SD卡存储状态
u8 SDCard_GetStorageState()
{
	FATFS *fs;
	DWORD fre_clust, fre_sect, tot_sect;

	return 0;

}

//SD卡读写状态
u8 SDCard_GetRWState()
{
	return 0;

}
//
////SD卡容量
//u8 SDCard_GetVulm()
//{
//
//	return 4;
//
//}


/*------------------------- 閻劋绶�-----------------------------------*/
#if 0
/**
 * CAN娣団剝浼匰D閸椔ゎ嚢閸欙拷
 */
void read()
{
#define BUF_SIZE	1000
	u8 buf[BUF_SIZE];
	TIME	start;
	TIME	end;
	int ret = 0;
	u8 rId = 0;
	u16 len = 0;

	/*鐠佸墽鐤嗙挧宄邦瀶閵嗕胶绮ㄩ弶鐔告闂傚鍋�/
	start.year = 2013;
	start.month = 5;
	start.day = 12;
	start.hour = 14;
	start.minute = 35;
	start.second = 1;

	end.year = 2013;
	end.month = 5;
	end.day = 12;
	end.hour = 14;
	end.minute = 36;
	end.second = 13;

	/*閸掓繂顬婇崠鏈閸楋拷/
	ret = SD_Init();
	if(ret < 0)
	{
		/*閸掓繂顬婇崠鏍с亼鐠愶拷/
		TRACE(GetErrorDescription(-ret));
		return;
	}

	/*鐏忔繆鐦铏圭彌閺屻儴顕楃拠閿嬬湴*/
	ret = SD_RetrieveFrameSegment(&start, &end, &rId);
	if(ret < 0)	/*婢惰精瑙�/
	{
		/*err occurred,handle error...*/
		/*doHandleErr();*/
		return;
	}

	/*閼惧嘲绶遍弻銉嚄缁便垹绱㊣D閿涘本宓佸銈呮儕閻滎垵骞忛崣鏍ㄥ閺堝鏆熼幑顔兼健*/
	do
	{
		/*妞ゅ搫绨拠璇插絿鐢勬殶閹诡喖娼�/
		len = SD_RetrieveFrameBlock(rId, buf, BUF_SIZE);

		/*闂�灝瀹虫稉锟介弮鎯邦嚛閺勫氦顕伴崣鏍х暚濮ｆ洩绱濋柅锟藉毉*/
		if(len == 0)
			break;

		/*閹恒儲鏁归崚棰佺娑擃亝鏆熼幑顔兼健閿涘苯顦甸悶锟�.*/

	}while(1);
}

/**
 * CAN鐢潛D閸椻�鐡ㄩ崒锟� */
void Save()
{
	CANRAWFRAME frame;



	SD_DumpFrame(&frame);
}
#endif
