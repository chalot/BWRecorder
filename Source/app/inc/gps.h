/**   
 * @Title: gps.h  
 * @Description: TODO(用一句话描述该文件做什么) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-7-29 上午10:33:01 
 * @version V1.0   
 */

#ifndef GPS_H_
#define GPS_H_

#include <stm32f2xx_conf.h>
#include "stm32_gpio.h"
#include "type.h"

/*=====     GPS信息定义    =====*/
#define		_GPSINFO_GPS_VALID					0x01		//GPS有效
#define		_GPSINFO_HEADING_VALID				0x02		//GPS信息航向有效
#define		_GPSINFO_SPEED_VALID				0x04		//GPS信息速度有效
#define		_GPSINFO_ALTITUDE_VALID				0x08		//GPS信息高度有效
#define		_GPSINFO_SATNUM_VALID				0x10		//GPS信息卫星数有效
#define		_GPSINFO_SATINFO_VALID				0x20		//GPS信息可见卫星有效
#define		_GPSINFO_FIRSTTIME					0x80		//GPS-首次


typedef struct {
	u16 u16GpsInfoValid;		//信息有效否
	u8 Latitude[4];			//纬度，单位=10^-3分
	u8 Longtitude[4];			//经度，单位=10^-3分
	u8 Altitude[2];			//海拔高度，单位=米
	u8 Speed[2];				//速度，单位=10米/时
	u8 Direction[2];				//航向，单位=分
	s32 s32Latitude;			//纬度，单位=10^-3分
	s32 s32Longitude;			//经度，单位=10^-3分
	s16 s16Altitude;			//海拔高度，单位=米
	u16 u16Speed;				//速度，单位=10米/时
	u16 u16Heading;				//航向，单位=分
	u8 u8ValidSatellites;		//正在使用卫星数
	u8 u8VisibleSatellites;	//可见卫星数
	u16 u16Year;				//当前取得的GPS信息-年
	u8 u8Month;				//当前取得的GPS信息-月
	u8 u8Day;					//当前取得的GPS信息-日
	u8 u8Hour;					//当前取得的GPS信息-时
	u8 u8Minute;				//当前取得的GPS信息-分
	u8 u8Second;				//当前取得的GPS信息-秒
	u8 bAntenaState;			//天线状态，1=拔出，0=正常
} _tGPSInfo;


/*********************** GPS模块电源控制 *****************************************************/
#define GPS_POWER_ON	GPIO_SetBits(GPIOC,GPIOC_GPS_CTRL_)  //模块开电
#define GPS_POWER_OFF	GPIO_ResetBits(GPIOC,GPIOC_GPS_CTRL_) //模块关电//网络指示灯打开/关闭
#define GPS_LED_OFF  	GPIO_SetBits(GPIOC, GPIOC_GPS_LED)
#define GPS_LED_ON  	GPIO_ResetBits(GPIOC, GPIOC_GPS_LED)

//LED指示灯闪烁
#define GPS_FLASH_LED  \
                        GPIO_WriteBit(GPIOC, GPIOC_GPS_LED,   \
                        (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOC, GPIOC_GPS_LED))))

//拉低GPS_M-RST管腿
#define GPS_RST_PULLDOWN  	GPIO_ResetBits(GPIOC, GPIOC_GPS_RST_)
#define GPS_RST_PULLUP  	GPIO_SetBits(GPIOC, GPIOC_GPS_RST_)

//GPS天线检测，高电平为拔出，低电平为插入，使用CAN 5V供电
#define GPS_GETANTENASTATE() 	GPIO_ReadInputDataBit(GPIOC,GPIOC_GPSANT_DET_)

#define ANTENA_OK			0  //天线状态：正常
#define ANTENA_ERR			1  //天线状态：故障/*********************** 函数声明 *****************************************************/

void GPS_Init(); //内存初始化
void GPS_SetAntenaState(u8); //设置天线状态
u8 GPS_GetAntenaState(); //读取天线状态
void GPS_ReportGpsInfo(); //上报GPS点位信息
_tGPSInfo* GPS_GetInfo(); //获取当前GPS状态
BOOL GPS_IsLocated();	//是否定位成功
void TRACE_GPSINFO(/*_tGPSInfo *pGpsInfo*/); //打印GPS信息
void GPS_DumpInfo();	//存储GPS数据
void GPS_Lowlevel_Init();

void ISR_UART_GPS(void); //串口中断入口

#endif /* GPS_H_ */
