/**   
 * @Title: exception.h  
 * @Description: TODO(系统故障定义)
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-8-13 下午3:11:42 
 * @version V1.0   
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include "type.h"

/*错误类型*/
typedef enum {
	CAN_CLASS = 0, SYS_CLASS, NET_CLASS, GPS_CLASS, SD_CLASS,

	MAX_CLASS,

} eFAULT_CLASS;

#define FAULT_NONE					0x00	//无故障/*CAN故障*///#define CAN_FAULT_NONE				0x01	//无故障#define CAN_FAULT_TX				0x01	//CAN发送故障#define CAN_FAULT_RX				0x02	//CAN接收故障#define CAN_FAULT_OTHER				0x04	//其他故障/*系统软件故障*///#define SYS_FAULT_NONE				0x01	//无故障#define SYS_FAULT_PWR				0x01	//电源故障#define SYS_FAULT_EXCEPTION			0x02	//异常陷入#define SYS_FAULT_OTHER				0x04	//其他故障/*网络通信故障*///#define NET_FAULT_NONE				0x01	//无故障#define NET_FAULT_OUTOFSERVICE		0x01	//欠费#define NET_FAULT_SIMCARD			0x02	//SIM卡故障#define NET_FAULT_UNREACHABLE		0x04	//网络不可达/*GPS定位故障*///#define GPS_FAULT_NONE				0x01	//无故障#define GPS_FAULT_ANTENA			0x01	//GPS天线故障#define GPS_FAULT_INVALID			0x02	//定位无效#define GPS_FAULT_OTHER				0x04	//保留/*SD卡故障*///#define SD_FAULT_NONE				0x01	//无故障#define SD_FAULT_NOTDETECTED		0x01	//SD卡检测失败#define SD_FAULT_READ				0x02	//SD卡读故障#define SD_FAULT_WRITE				0x04	//SD卡写故障/*** @Func 设置错误类型* @Param err [IN] 错误代码* @Ret 无*/voidSetErrType(eFAULT_CLASS class, u8 err);
/**
 * @Func	读取错误类型
 * @Param	无
 * @Ret		u32 	[OUT]	系统错误码
 */
u8
GetErrType(eFAULT_CLASS class);

/**
 * @Func	清除某个错误类型
 * @Param	err			[IN]	错误代码
 * @Ret		无
 */
void
ClearErrType(eFAULT_CLASS class, u8 err);

/**
 * @Func	重置错误类型
 * @Param	无
 * @Ret		无
 */
void
ResetErrType(eFAULT_CLASS class);

/**
 * @Func	重置错误类型
 * @Param	无
 * @Ret		无
 */
BOOL
CheckSpecifiedErrTypeExisted(eFAULT_CLASS class, u8 err);

/**
 * @Func	检查系统是否有故障出现
 * @Param	无
 * @Ret		TRUE=有故障，FALSE=无故障
 */
BOOL
CheckIfNoErrTypeOccured();

#endif /* EXCEPTION_H_ */
