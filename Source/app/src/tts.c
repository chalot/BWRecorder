/*
 * tts.c
 *
 *  Created on: 2014年4月25日
 *      Author: ThinkPad User
 */

#include "stm32f2xx_conf.h"
#include "tts.h"
#include "utility.h"
#include <board.h>

static volatile tTTSFSMType tTTSFSM;

/**
 * TTS硬件初始化
 *
 */
void TTS_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(
	RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);

	////语音模块IO口定义
	// Configure GPIOE_TTS_RST & TTS_RX as output push up
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = TTS_RST_PIN;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TTS_TX_PIN;
	GPIO_Init(TTS_TX_PORT, &GPIO_InitStructure);

	// Configure TTS SHUNT as output  function
	GPIO_InitStructure.GPIO_Pin = TTS_SHUNT_PIN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TTS_READY_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(TTS_TX_PORT, &GPIO_InitStructure);

//	TIM_Tts_Configuration();
}

/**
 * 104uS定时器设置，主要是产生TTS语音播放的位时间间隔，波特率9600，每个数据位为104us。
 *
 */
void TIM_Tts_Configuration() {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	//Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 156;
	TIM_TimeBaseStructure.TIM_Prescaler = 79;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIMTts, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIMTts, TIM_FLAG_Update);
	TIM_Cmd(TIMTts, ENABLE);
}

/**
 * 语音模块初始化
 *
 */
void TTS_Init(void) {
	ZeroMem((u8*) &tTTSFSM, sizeof(tTTSFSMType));

	tTTSFSM.u8ResetReq = 1;										//复位请求
	tTTSFSM.u8ResetOk = 0;										//复位成功

	//TTS模拟串口初始电平置高
	GPIO_SetBits(TTS_TX_PORT, TTS_TX_PIN);

}

/**
 * 语音发送处理
 *
 */
void TTS_Process(void) {
#if 0
	if(tTTSFSM.u8ResetOk == 0)
	{
		if(tTTSFSM.u8ResetReq == 1)
		{
			tTTSFSM.u8ResetReq = 0;

			GPIO_ResetBits(GPIOF, TTS_RST_PIN);

			tTTSFSM.u16TimeElapsed_Reset = 5;
		}
		else if(tTTSFSM.u16TimeElapsed_Reset == 0)
		{
			GPIO_SetBits(GPIOF, TTS_RST_PIN);

			tTTSFSM.u8ResetOk = 1;
		}
	}
	if(tTTSFSM.u8ResetOk != 1)
	{
		return;
	}
	//若需要重复朗读，则到达间隔时间后，重复朗读
	if(tTTSFSM.u8ReadTimes > 0)
	{
		if(tTTSFSM.u8TimeElapsed_TTSAudioOff == 0)
		{
			//等待音源通道空闲时才可以播放
			if(tSYSFSM.u16VoiceSel == _AUDIO_PHONE_)
			{
				return;
			}

			//复位朗读字节数指针
			tTTSFSM.u8CurSendByte = 0;

			//准备发送第一个字节
			tTTSFSM.u8SendingData = tTTSFSM.u8Msg[tTTSFSM.u8CurSendByte++];

			//已发送位数
			tTTSFSM.u8SendBitsCount = 0;

			//设置位发送状态
			tTTSFSM.eCMD_Type = T1_CMD_VOICE;

			//打开TTS音源
			PERI_AudioSel(_AUDIO_TTS_);

			//打开定时器开始发送
			TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);

			//启动关闭音源计时
			tTTSFSM.u8TimeElapsed_TTSAudioOff = tTTSFSM.u8MsgLen >> 1;
		}
	}
	//朗读完毕，关闭TTS音源
	else
	{
		if(tTTSFSM.u8TimeElapsed_TTSAudioOff == 0)
		{
			if(tSYSFSM.u16VoiceSel == _AUDIO_TTS_)
			{
				//关闭TTS音源
				PERI_AudioSel(_AUDIO_NONE_);
			}
		}
	}

	/*
	 //短信播报处理
	 //	TTS_ReadMsgProcess();

	 //有待朗读短信，朗读两次
	 if(tTTSFSM.bCommingMsg)
	 {
	 //朗读两次完毕，清除朗读标志
	 if(tTTSFSM.u8ReadTimes >= 2)
	 {
	 tTTSFSM.bCommingMsg = 0;
	 tTTSFSM.u8ReadTimes = 0;
	 return;
	 }

	 //等待当前播报空闲，
	 if(	(tTTSFSM.eCMD_Type == T1_CMD_IDLE) 	&&
	 (tTTSFSM.u16AvailableBytes == 0) 	&&
	 (tTTSFSM.u8RepeatInterval == 0)
	 )
	 {
	 //播报短信内容
	 TTS_Speak(CODE_GB2312, tTTSFSM.u8CommingMsg, tTTSFSM.u8CommingMsgLen);

	 //累计朗读次数
	 tTTSFSM.u8ReadTimes++;
	 }
	 }

	 //有未发完的位，等待发送完毕
	 if(tTTSFSM.eCMD_Type != T1_CMD_IDLE)
	 {
	 return;
	 }

	 //无待发送字节，返回
	 if(tTTSFSM.u16AvailableBytes == 0)
	 {
	 return;
	 }

	 //开始发送下一字节
	 u8SendByte = *(tTTSFSM.au8Buffer + tTTSFSM.u16Read++);

	 //读指针回头
	 if(tTTSFSM.u16Read == TTS_BUFFER_SIZE)
	 {
	 tTTSFSM.u16Read = 0;
	 }

	 //更新剩余字节数
	 if(tTTSFSM.u16AvailableBytes > 0)
	 {
	 tTTSFSM.u16AvailableBytes --;
	 }

	 //启动模拟串口，发送一个字节
	 TTS_SendByte(u8SendByte);
	 */
#endif
}

/**
 * 播报短信，循环播报两次
 *
 */
void TTS_ReadMsgProcess(void) {
	/*	//有待朗读短信，朗读两次
	 if(tTTSFSM.bCommingMsg)
	 {
	 //朗读两次完毕，清除朗读标志
	 if(tTTSFSM.u8ReadTimes >= 2)
	 {
	 tTTSFSM.bCommingMsg = 0;
	 tTTSFSM.u8ReadTimes = 0;
	 return;
	 }

	 //等待当前播报空闲，
	 if(	(tTTSFSM.eCMD_Type == T1_CMD_IDLE) 	&&
	 (tTTSFSM.u16AvailableBytes == 0) 	&&
	 (tTTSFSM.u8RepeatInterval == 0)
	 )
	 {
	 //打开TTS音源
	 PERI_AudioSel(_AUDIO_TTS_);

	 //播报短信内容
	 TTS_Speak(CODE_GB2312, tTTSFSM.u8CommingMsg, tTTSFSM.u8CommingMsgLen);

	 //设定朗读时间间隔，根据短信字数近似
	 tTTSFSM.u8RepeatInterval = tTTSFSM.u8CommingMsgLen >> 1;

	 //累计朗读次数
	 tTTSFSM.u8ReadTimes++;
	 }
	 }
	 */
}

/**
 * 向语音芯片发送一字节数据
 *
 * @param u8Data		字节数据
 */
void TTS_SendByte(u8 u8Data) {
	//保存待发送字节
	tTTSFSM.u8SendingData = u8Data;

	//设置位发送状态
	tTTSFSM.eCMD_Type = T1_CMD_VOICE;

	//已发送位数
	tTTSFSM.u8SendBitsCount = 0;

	//打开定时器开始发送
	TIM_ITConfig(TIMTts, TIM_IT_CC1, ENABLE);
}

/**
 * 朗读字符串
 *
 * @param u8CodeType		编码类型
 * @param pTxt				文本
 * @param u8TxtLen			文本长度
 * @param u8RepeatInterval	重复间隔
 * @param u8ReadTimes		重复朗读次数
 */
void TTS_Speak(u8 u8CodeType, u8 *pTxt, u8 u8TxtLen, u8 u8RepeatInterval,
		u8 u8ReadTimes) {
	_tTTSFRAMETYPE tFrame;

	//无发送内容，返回
	if (pTxt == NULL || u8TxtLen == 0) {
		return;
	}
//
//	//语音通道被占用，不朗读
//	if(_AUDIO_PHONE_ == tSYSFSM.u16VoiceSel)
//	{
//		return;
//	}

	//打开TTS音源
//	PERI_AudioSel(_AUDIO_TTS_);

	/*
	 //判断当前TTS状态，是否需要唤醒
	 if(tTTSFSM.ePowerState == _ePOWER_SLEEP)
	 {
	 //唤醒
	 TTS_Wakeup();
	 }

	 //打开语音输出
	 TTS_DisableSound();
	 */

	//填充内容
	tFrame.u8Head = 0xFE;
	tFrame.u8Cmd = 0x05;
	tFrame.u8Code = u8CodeType;
	tFrame.u8Reserved = 0;
	tFrame.u8Len = u8TxtLen + 1;
	tFrame.pData = pTxt;

	//拷贝帧头
	memcpy_(tTTSFSM.u8Msg, (u8*) &tFrame, 5);

	//拷贝内容
	memcpy_(tTTSFSM.u8Msg + 5, pTxt, u8TxtLen);

	//保存长度
	tTTSFSM.u8MsgLen = u8TxtLen + 5;

	//设定朗读时间间隔，根据短信字数近似
	tTTSFSM.u8TimeElapsed_TTSAudioOff = (u8) (u8TxtLen * 0.7);

	//保持朗读间隔和朗读次数
	tTTSFSM.u8ReadTimes = u8ReadTimes;

	//复位朗读字节数指针
	tTTSFSM.u8CurSendByte = 0;

	//准备发送第一个字节
	tTTSFSM.u8SendingData = tTTSFSM.u8Msg[tTTSFSM.u8CurSendByte++];

	//已发送位数
	tTTSFSM.u8SendBitsCount = 0;

	//设置位发送状态
	tTTSFSM.eCMD_Type = T1_CMD_VOICE;

	//打开定时器开始发送
	TIM_ITConfig(TIMTts, TIM_IT_CC1, ENABLE);

}

/**
 * 停止合成
 *
 */
void TTS_Stop(void) {
	u8 aFrame[] = { 0xFE, 0x02 };

	//帧写入到发送缓区
	TTS_SendFrame(aFrame, 2);
}

/**
 * 语音芯片休眠
 *
 */
void TTS_Sleep(void) {
	u8 aFrame[] = { 0xFE, 0x88 };

	if (tTTSFSM.ePowerState == _ePOWER_SLEEP) {
		return;
	}

	//帧写入到发送缓区
	TTS_SendFrame(aFrame, 2);
}

/**
 * 允许语音输出
 *
 */
void TTS_EnableSound(void) {
	GPIO_SetBits(GPIOE, TTS_SHUNT_PIN);

}

/**
 * 禁止语音输出
 *
 */
void TTS_DisableSound(void) {
	if (tTTSFSM.bAudioState == TTS_AUDIO_ON) {
		GPIO_ResetBits(GPIOE, TTS_SHUNT_PIN);

		tTTSFSM.bAudioState = TTS_AUDIO_OFF;
	}
}

/**
 * 唤醒
 *
 */
void TTS_Wakeup(void) {
	//复位信号，低电平=10ms，拉高后持续50ms
	GPIO_ResetBits(GPIOF, TTS_RST_PIN);

	//电源状态 = 复位
	tTTSFSM.ePowerState = _ePOWER_RESET;

	//开始复位计时
	tTTSFSM.u16TimeElapsed_Reset = 0;
}

/**
 * 查询当前语音播报状态
 *
 * @return	TRUE=空闲，FALSE=忙
 */
BOOL TTS_IsChipIdle(void) {
	return TRUE;
}

/**
 * 发送帧
 *
 * @param pFrame		帧数据
 * @param u8FrameLen	帧长度
 */
void TTS_SendFrame(u8 *pFrame, u8 u8FrameLen) {
	u8 i = 0;

	//参数校验错误，返回
	if (pFrame == NULL || u8FrameLen == 0 || TTS_FRAME_SIZE > TTS_FRAME_SIZE) {
		return;
	}

	//缓区剩余空间不够，返回
	if ((TTS_BUFFER_SIZE - tTTSFSM.u16AvailableBytes) < u8FrameLen) {
		return;
	}

	//帧写入发送缓区
	for (i = 0; i < u8FrameLen; i++) {
		//转储数据
		*(tTTSFSM.au8Buffer + tTTSFSM.u16Write++) = *pFrame++;

		//写指针回头
		if (tTTSFSM.u16Write == TTS_BUFFER_SIZE) {
			tTTSFSM.u16Write = 0;
		}
	}

	//更新缓区待发送字节数
	tTTSFSM.u16AvailableBytes += u8FrameLen;

	//设置位发送状态
	tTTSFSM.eCMD_Type = T1_CMD_VOICE;

	//已发送位数
	tTTSFSM.u8SendBitsCount = 0;

	//打开定时器开始发送
	TIM_ITConfig(TIMTts, TIM_IT_CC1, ENABLE);
}

/**
 * T1中断，语音芯片数据发送
 *
 */
void T1_Voice_Interrupt(void) {
	//等待1字节发送完毕
	if (tTTSFSM.eCMD_Type != T1_CMD_IDLE) {
		//数据位发送完成
		if (tTTSFSM.u8SendBitsCount == 9) {
			//发送停止位
			GPIO_SetBits(TTS_TX_PORT, TTS_TX_PIN);

			//累加已发送位数
			tTTSFSM.u8SendBitsCount++;
		}

		//如果有数据位需要发送
		else if (tTTSFSM.u8SendBitsCount < 9) {
			//发送起始位
			if (tTTSFSM.u8SendBitsCount == 0) {
				//拉低数据线，发送起始位
				GPIO_ResetBits(TTS_TX_PORT, TTS_TX_PIN);
			} else {
				//开始发送数据位
				if (tTTSFSM.u8SendingData & 0x01) {
					//拉高数据线，发送数据“1”
					GPIO_SetBits(TTS_TX_PORT, TTS_TX_PIN);
				} else {
					//拉低数据线，发送数据“0”
					GPIO_ResetBits(TTS_TX_PORT, TTS_TX_PIN);
				}

				//取下一位
				tTTSFSM.u8SendingData >>= 1;
			}

			//累计已发送位数
			tTTSFSM.u8SendBitsCount++;
		}
		//一字节数据已发送完毕
		else {
			//拉高数据线
			GPIO_SetBits(TTS_TX_PORT, TTS_TX_PIN);

			//复位位发送计数器
			tTTSFSM.u8SendBitsCount = 0;

			//位发送状态 = 空闲
			tTTSFSM.eCMD_Type = T1_CMD_IDLE;
			return;
		}
	} else {
		//取下一字节发送
		if (tTTSFSM.u8CurSendByte <= tTTSFSM.u8MsgLen - 1) {
			tTTSFSM.u8SendingData = tTTSFSM.u8Msg[tTTSFSM.u8CurSendByte++];
			tTTSFSM.u8SendBitsCount = 0;
			tTTSFSM.eCMD_Type = T1_CMD_VOICE;
		}
		//字符串发送完毕，
		else {
			//已朗读完一次，递减朗读次数
			tTTSFSM.u8ReadTimes--;

			tTTSFSM.u8CurSendByte = 0;

			//关闭定时器1
			TIM_ITConfig(TIMTts, TIM_IT_CC1, DISABLE);
		}
	}
}

/**
 * 10MS超时处理函数
 *
 */
void Isr_TTS_Timeout10ms(void) {
	if (tTTSFSM.u16TimeElapsed_Reset) {
		--tTTSFSM.u16TimeElapsed_Reset;
	}
}

/**
 * 延时关闭TTS音源
 *
 */
void Isr_TTS_Timeout1s(void) {
	if (tTTSFSM.u8TimeElapsed_TTSAudioOff > 0) {
		tTTSFSM.u8TimeElapsed_TTSAudioOff--;
	}
}
