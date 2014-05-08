/*
 * tts.h
 *
 *  Created on: 2014年4月25日
 *      Author: ThinkPad User
 */

#ifndef TTS_H_
#define TTS_H_

#include <type.h>

#pragma	pack(1)

//短信编码方式
#define CODE_GB2312								0x00				//GB2312#define CODE_UNICODE							0x03				//Unicode
#define	TTS_BUFFER_SIZE							500					//TTS缓区最大长度#define	TTS_FRAME_SIZE							204					//一帧最大长度
#define	TIMEOUT_TTS_RESET						1					//芯片复位低电平持续时间，10ms#define	TIMEOUT_TTS_SLEEP						30//120					//芯片进入休眠持续时间，2分钟
#define	TTS_AUDIO_ON							1					//打开语音输出#define	TTS_AUDIO_OFF							0					//关闭语音输出
//模块电源状态
typedef enum {
	_ePOWER_ON = 0,													//正常上电
	_ePOWER_SLEEP,													//休眠
	_ePOWER_OFF,													//断电
	_ePOWER_RESET,													//复位

} _ePOWER_STATE;

typedef enum {
	T1_CMD_IDLE = 0, T1_CMD_VOICE
} T1_CMD_TYPE;

//TTS帧格式
typedef struct {
	u8 u8Head;												//帧头：0xFE
	u8 u8Cmd;												//命令字
	u8 u8Reserved;											//长度高字节
	u8 u8Len;												//长度低字节
	u8 u8Code;												//文本编码类型
	u8 *pData;												//待合成文本

} _tTTSFRAMETYPE;

#define		MSG_LEN_MAX			256				//短信最大长度
//TTS有限状态机
typedef struct {
	u8 ePowerState;										//电源状态
	u8 u8ResetReq :1;										//复位请求
	u8 u8ResetOk :1;										//复位成功
	u8 fReserved :6;
	u8 u8ResetTimeout;										//复位超时时间
	u16 u16TimeElapsed_Reset;								//复位低电平维持计时器
	u16 u16TimeElapsed_Sleep;								//进入休眠计时器
	u8 bAudioState;										//语音输出状态，0=关闭，1=开启

	u8 eCMD_Type;
	u8 u8SendStatus;										//当前发送状态
	u8 u8SendBitsCount;									//发送位计数
	u8 u8SendingData;										//当前发送的字节

	u8 bCommingMsg;										//下发短信标志
	u8 u8Msg[MSG_LEN_MAX];									//下发短信内容
	u8 u8MsgLen;											//下发短信长度
	u8 u8ReadTimes;										//朗读次数
	u8 u8RepeatInterval;									//朗读时间间隔
	u8 u8TimeElapsed_RepeatInterval;						//朗读超时计数
	u8 u8CurSendByte;										//已发送字节数
	u8 u8TimeElapsed_TTSAudioOff;							//延时关闭音源

	u16 u16Read;											//读位置
	u16 u16Write;											//写位置
	u16 u16AvailableBytes;									//剩余字节数
	u8 au8Buffer[TTS_BUFFER_SIZE];							//待发送帧缓区

} tTTSFSMType;

/**外部接口 **************************************************************************************/
extern void TTS_Init(void);
extern void TTS_Speak(u8 u8CodeType, u8 *pTxt, u8 u8TxtLen, u8 u8RepeatInterval,
		u8 u8ReadTimes);
extern void TTS_EnableSound(void);							//允许语音输出
extern void TTS_DisableSound(void);							//禁止语音输出

/**内部接口 **************************************************************************************/
void TTS_Process(void);
void TTS_ReadMsgProcess(void);
void TTS_SendByte(u8 u8Data);
void TTS_Stop(void);
void TTS_Sleep(void);
void TTS_Wakeup(void);		//唤醒
BOOL TTS_IsChipIdle(void);	//语音播报状态
void TTS_SendFrame(u8 *pFrame, u8 u8FrameLen);
void TIM_Tts_Configuration();

/**ISR *******************************************************************************************/
extern void Isr_TTS_Timeout10ms(void);
extern void Isr_TTS_Timeout1s(void);	//延时关闭TTS音源
extern void T1_Voice_Interrupt(void);

void TTS_Lowlevel_Init();
#pragma	pack()

#endif /* TTS_H_ */
