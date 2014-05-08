#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f2xx_conf.h"

///#define ADC3_DR_ADDRESS    ((u32)0x4001224C)

//定义按键操作模式，抬起操作或者是按下就操作
//当前定义的是按下操作
//按键按下操作
#define	_KEY_DOWN_OPERATE_		0
//按键抬起操作
#define	_KEY_UP_OPERATE_		1
//按键按下操作
#define	_KEY_OPERATE_MODE		_KEY_UP_OPERATE_ ///_KEY_DOWN_OPERATE_ ///按键抬起时响应，按下时伴随BEEP音
#define	_KEY_BUF_LEN_			5		//按键缓冲区大小定义#define	_KEY_SAMPLE_TIMES_		8		//按键采样次数定义
//按键的采样AD值
#define	_KEY_AD_OK_			0xA90		//OK键参考值#define	_KEY_AD_UP_			0x98E		//UP键参考值#define	_KEY_AD_DOWN_		0x7FA		//DOWN键参考值#define	_KEY_AD_MENU_		0x555		//CANCEL键参考值
#define	_KEY_AD_OFFSET_		0x00B0		//参考值左右偏差
///不再区分长、短按键
//#define	_KEY_ACTION_SHORT_DOWN_		0x01	//按键短按
//#define	_KEY_ACTION_LONG_DOWN_		0x02	//按键长按

//按键消息定义
typedef enum {
	KEY_OK = 0,		//确定键
	KEY_UP,			//向上键
	KEY_DOWN,		//向下键
	KEY_MENU,		//菜单键
	KEY_UNKNOWN,
} eKEY;

/**
 //定义需要处理按键的操作
 typedef enum
 {
 _eKEYOK_SHORT_ = 1,				//确定键短按
 _eKEYSELUP_SHORT_,				//选择向上键短按	
 _eKEYSELDOWN_SHORT_,			//选择向下键短按
 _eKEYCANCEL_SHORT_,				//取消短按键
 
 _eKEYOK_LONG_= 0x81,			//确定键长按			
 _eKEYSELUP_LONG_,				//选择向上键长按
 _eKEYSELDOWN_LONG_,				//选择向下键长按
 _eKEYCANCEL_LONG_,				//取消长按键	
 
 _eKEY_NULL = 0xFF				//空按键
 }_eKEY_OPERATOR_;
 */

#define	_KEY_CODE_OFFSET  	0x80	//按键编码短按和长按的偏移值是0x80	
//消抖时间定义
#define	KEY_DETECT_JITTER_TIME_			5		//消抖时间20ms
//按键长按下时间定义2s
#define	KEY_DETECT_LONGDOWN_TIME_       100		//长按时间2s
//按键按下时LED点亮时间定义5s
#define		KEY_LED_TIME_       500		//按键按下LED点亮时间5s//按键检测步骤
typedef enum {
	_eKEY_DETECTIONSTATUS_IDEL_ = 0,		//检测空闲
	_eKEY_DETECTIONSTATUS_JITTERY_,					//检测消抖
	_eKEY_DETECTIONSTATUS_KEYDOWN_DET_,				//检测按键按下
	_eKEY_DETECTIONSTATUS_KEYDOWN_,					//检测到按键按下
	_eKEY_DETECTIONSTATUS_KEYLONGDOWN_DET_,			//检测按键长按
	_eKEY_DETECTIONSTATUS_KEYLONGDOWN_,				//检测到按键长按
	_eKEY_DETECTIONSTATUS_KEYUP_DET_,				//检测按键抬起
	_eKEY_DETECTIONSTATUS_KEYUP_					//检测到按键抬起
} eKEYDETECTIONSTATUSType;

//按键状态机
typedef struct {
	eKEYDETECTIONSTATUSType eDetect_Status;		//检测状态
	u8 u8KeyAction;		//按键动作

	u16 u16Key_SmpRslt[_KEY_SAMPLE_TIMES_];		//多次采样，取平均值
	u8 u8SampleTimes;			//采样次数
	u8 u8JitterTimeout;
	u8 u8LedlightTimeout;		//按键按下时LED点亮时间超时值
	u8 u8KeyBuffer[_KEY_BUF_LEN_];		//按键缓冲区
	u8 u8Read;		//读指针
	u8 u8Write;	//写指针

} tKEYFSMType;

#define Key_GetADCValue()	ADC_GetConversionValue(ADC3)	///取按键检测值
void Key_Put2Buffer(u8 u8KeyCode);
BOOL Key_ADInrange(u16 u16Sample);

eKEY Key_GetCode(u16 u16SmpValue);
void ISR_ADC_Key(void);

extern void Key_Process(void);

void Key_LowLevel_Init();

#endif
