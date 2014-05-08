/*
 * tts.c
 *
 *  Created on: 2014��4��25��
 *      Author: ThinkPad User
 */

#include "stm32f2xx_conf.h"
#include "tts.h"
#include "utility.h"
#include <board.h>

static volatile tTTSFSMType tTTSFSM;

/**
 * TTSӲ����ʼ��
 *
 */
void TTS_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(
	RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);

	////����ģ��IO�ڶ���
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
 * 104uS��ʱ�����ã���Ҫ�ǲ���TTS�������ŵ�λʱ������������9600��ÿ������λΪ104us��
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
 * ����ģ���ʼ��
 *
 */
void TTS_Init(void) {
	ZeroMem((u8*) &tTTSFSM, sizeof(tTTSFSMType));

	tTTSFSM.u8ResetReq = 1;										//��λ����
	tTTSFSM.u8ResetOk = 0;										//��λ�ɹ�

	//TTSģ�⴮�ڳ�ʼ��ƽ�ø�
	GPIO_SetBits(TTS_TX_PORT, TTS_TX_PIN);

}

/**
 * �������ʹ���
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
	//����Ҫ�ظ��ʶ����򵽴���ʱ����ظ��ʶ�
	if(tTTSFSM.u8ReadTimes > 0)
	{
		if(tTTSFSM.u8TimeElapsed_TTSAudioOff == 0)
		{
			//�ȴ���Դͨ������ʱ�ſ��Բ���
			if(tSYSFSM.u16VoiceSel == _AUDIO_PHONE_)
			{
				return;
			}

			//��λ�ʶ��ֽ���ָ��
			tTTSFSM.u8CurSendByte = 0;

			//׼�����͵�һ���ֽ�
			tTTSFSM.u8SendingData = tTTSFSM.u8Msg[tTTSFSM.u8CurSendByte++];

			//�ѷ���λ��
			tTTSFSM.u8SendBitsCount = 0;

			//����λ����״̬
			tTTSFSM.eCMD_Type = T1_CMD_VOICE;

			//��TTS��Դ
			PERI_AudioSel(_AUDIO_TTS_);

			//�򿪶�ʱ����ʼ����
			TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);

			//�����ر���Դ��ʱ
			tTTSFSM.u8TimeElapsed_TTSAudioOff = tTTSFSM.u8MsgLen >> 1;
		}
	}
	//�ʶ���ϣ��ر�TTS��Դ
	else
	{
		if(tTTSFSM.u8TimeElapsed_TTSAudioOff == 0)
		{
			if(tSYSFSM.u16VoiceSel == _AUDIO_TTS_)
			{
				//�ر�TTS��Դ
				PERI_AudioSel(_AUDIO_NONE_);
			}
		}
	}

	/*
	 //���Ų�������
	 //	TTS_ReadMsgProcess();

	 //�д��ʶ����ţ��ʶ�����
	 if(tTTSFSM.bCommingMsg)
	 {
	 //�ʶ�������ϣ�����ʶ���־
	 if(tTTSFSM.u8ReadTimes >= 2)
	 {
	 tTTSFSM.bCommingMsg = 0;
	 tTTSFSM.u8ReadTimes = 0;
	 return;
	 }

	 //�ȴ���ǰ�������У�
	 if(	(tTTSFSM.eCMD_Type == T1_CMD_IDLE) 	&&
	 (tTTSFSM.u16AvailableBytes == 0) 	&&
	 (tTTSFSM.u8RepeatInterval == 0)
	 )
	 {
	 //������������
	 TTS_Speak(CODE_GB2312, tTTSFSM.u8CommingMsg, tTTSFSM.u8CommingMsgLen);

	 //�ۼ��ʶ�����
	 tTTSFSM.u8ReadTimes++;
	 }
	 }

	 //��δ�����λ���ȴ��������
	 if(tTTSFSM.eCMD_Type != T1_CMD_IDLE)
	 {
	 return;
	 }

	 //�޴������ֽڣ�����
	 if(tTTSFSM.u16AvailableBytes == 0)
	 {
	 return;
	 }

	 //��ʼ������һ�ֽ�
	 u8SendByte = *(tTTSFSM.au8Buffer + tTTSFSM.u16Read++);

	 //��ָ���ͷ
	 if(tTTSFSM.u16Read == TTS_BUFFER_SIZE)
	 {
	 tTTSFSM.u16Read = 0;
	 }

	 //����ʣ���ֽ���
	 if(tTTSFSM.u16AvailableBytes > 0)
	 {
	 tTTSFSM.u16AvailableBytes --;
	 }

	 //����ģ�⴮�ڣ�����һ���ֽ�
	 TTS_SendByte(u8SendByte);
	 */
#endif
}

/**
 * �������ţ�ѭ����������
 *
 */
void TTS_ReadMsgProcess(void) {
	/*	//�д��ʶ����ţ��ʶ�����
	 if(tTTSFSM.bCommingMsg)
	 {
	 //�ʶ�������ϣ�����ʶ���־
	 if(tTTSFSM.u8ReadTimes >= 2)
	 {
	 tTTSFSM.bCommingMsg = 0;
	 tTTSFSM.u8ReadTimes = 0;
	 return;
	 }

	 //�ȴ���ǰ�������У�
	 if(	(tTTSFSM.eCMD_Type == T1_CMD_IDLE) 	&&
	 (tTTSFSM.u16AvailableBytes == 0) 	&&
	 (tTTSFSM.u8RepeatInterval == 0)
	 )
	 {
	 //��TTS��Դ
	 PERI_AudioSel(_AUDIO_TTS_);

	 //������������
	 TTS_Speak(CODE_GB2312, tTTSFSM.u8CommingMsg, tTTSFSM.u8CommingMsgLen);

	 //�趨�ʶ�ʱ���������ݶ�����������
	 tTTSFSM.u8RepeatInterval = tTTSFSM.u8CommingMsgLen >> 1;

	 //�ۼ��ʶ�����
	 tTTSFSM.u8ReadTimes++;
	 }
	 }
	 */
}

/**
 * ������оƬ����һ�ֽ�����
 *
 * @param u8Data		�ֽ�����
 */
void TTS_SendByte(u8 u8Data) {
	//����������ֽ�
	tTTSFSM.u8SendingData = u8Data;

	//����λ����״̬
	tTTSFSM.eCMD_Type = T1_CMD_VOICE;

	//�ѷ���λ��
	tTTSFSM.u8SendBitsCount = 0;

	//�򿪶�ʱ����ʼ����
	TIM_ITConfig(TIMTts, TIM_IT_CC1, ENABLE);
}

/**
 * �ʶ��ַ���
 *
 * @param u8CodeType		��������
 * @param pTxt				�ı�
 * @param u8TxtLen			�ı�����
 * @param u8RepeatInterval	�ظ����
 * @param u8ReadTimes		�ظ��ʶ�����
 */
void TTS_Speak(u8 u8CodeType, u8 *pTxt, u8 u8TxtLen, u8 u8RepeatInterval,
		u8 u8ReadTimes) {
	_tTTSFRAMETYPE tFrame;

	//�޷������ݣ�����
	if (pTxt == NULL || u8TxtLen == 0) {
		return;
	}
//
//	//����ͨ����ռ�ã����ʶ�
//	if(_AUDIO_PHONE_ == tSYSFSM.u16VoiceSel)
//	{
//		return;
//	}

	//��TTS��Դ
//	PERI_AudioSel(_AUDIO_TTS_);

	/*
	 //�жϵ�ǰTTS״̬���Ƿ���Ҫ����
	 if(tTTSFSM.ePowerState == _ePOWER_SLEEP)
	 {
	 //����
	 TTS_Wakeup();
	 }

	 //���������
	 TTS_DisableSound();
	 */

	//�������
	tFrame.u8Head = 0xFE;
	tFrame.u8Cmd = 0x05;
	tFrame.u8Code = u8CodeType;
	tFrame.u8Reserved = 0;
	tFrame.u8Len = u8TxtLen + 1;
	tFrame.pData = pTxt;

	//����֡ͷ
	memcpy_(tTTSFSM.u8Msg, (u8*) &tFrame, 5);

	//��������
	memcpy_(tTTSFSM.u8Msg + 5, pTxt, u8TxtLen);

	//���泤��
	tTTSFSM.u8MsgLen = u8TxtLen + 5;

	//�趨�ʶ�ʱ���������ݶ�����������
	tTTSFSM.u8TimeElapsed_TTSAudioOff = (u8) (u8TxtLen * 0.7);

	//�����ʶ�������ʶ�����
	tTTSFSM.u8ReadTimes = u8ReadTimes;

	//��λ�ʶ��ֽ���ָ��
	tTTSFSM.u8CurSendByte = 0;

	//׼�����͵�һ���ֽ�
	tTTSFSM.u8SendingData = tTTSFSM.u8Msg[tTTSFSM.u8CurSendByte++];

	//�ѷ���λ��
	tTTSFSM.u8SendBitsCount = 0;

	//����λ����״̬
	tTTSFSM.eCMD_Type = T1_CMD_VOICE;

	//�򿪶�ʱ����ʼ����
	TIM_ITConfig(TIMTts, TIM_IT_CC1, ENABLE);

}

/**
 * ֹͣ�ϳ�
 *
 */
void TTS_Stop(void) {
	u8 aFrame[] = { 0xFE, 0x02 };

	//֡д�뵽���ͻ���
	TTS_SendFrame(aFrame, 2);
}

/**
 * ����оƬ����
 *
 */
void TTS_Sleep(void) {
	u8 aFrame[] = { 0xFE, 0x88 };

	if (tTTSFSM.ePowerState == _ePOWER_SLEEP) {
		return;
	}

	//֡д�뵽���ͻ���
	TTS_SendFrame(aFrame, 2);
}

/**
 * �����������
 *
 */
void TTS_EnableSound(void) {
	GPIO_SetBits(GPIOE, TTS_SHUNT_PIN);

}

/**
 * ��ֹ�������
 *
 */
void TTS_DisableSound(void) {
	if (tTTSFSM.bAudioState == TTS_AUDIO_ON) {
		GPIO_ResetBits(GPIOE, TTS_SHUNT_PIN);

		tTTSFSM.bAudioState = TTS_AUDIO_OFF;
	}
}

/**
 * ����
 *
 */
void TTS_Wakeup(void) {
	//��λ�źţ��͵�ƽ=10ms�����ߺ����50ms
	GPIO_ResetBits(GPIOF, TTS_RST_PIN);

	//��Դ״̬ = ��λ
	tTTSFSM.ePowerState = _ePOWER_RESET;

	//��ʼ��λ��ʱ
	tTTSFSM.u16TimeElapsed_Reset = 0;
}

/**
 * ��ѯ��ǰ��������״̬
 *
 * @return	TRUE=���У�FALSE=æ
 */
BOOL TTS_IsChipIdle(void) {
	return TRUE;
}

/**
 * ����֡
 *
 * @param pFrame		֡����
 * @param u8FrameLen	֡����
 */
void TTS_SendFrame(u8 *pFrame, u8 u8FrameLen) {
	u8 i = 0;

	//����У����󣬷���
	if (pFrame == NULL || u8FrameLen == 0 || TTS_FRAME_SIZE > TTS_FRAME_SIZE) {
		return;
	}

	//����ʣ��ռ䲻��������
	if ((TTS_BUFFER_SIZE - tTTSFSM.u16AvailableBytes) < u8FrameLen) {
		return;
	}

	//֡д�뷢�ͻ���
	for (i = 0; i < u8FrameLen; i++) {
		//ת������
		*(tTTSFSM.au8Buffer + tTTSFSM.u16Write++) = *pFrame++;

		//дָ���ͷ
		if (tTTSFSM.u16Write == TTS_BUFFER_SIZE) {
			tTTSFSM.u16Write = 0;
		}
	}

	//���»����������ֽ���
	tTTSFSM.u16AvailableBytes += u8FrameLen;

	//����λ����״̬
	tTTSFSM.eCMD_Type = T1_CMD_VOICE;

	//�ѷ���λ��
	tTTSFSM.u8SendBitsCount = 0;

	//�򿪶�ʱ����ʼ����
	TIM_ITConfig(TIMTts, TIM_IT_CC1, ENABLE);
}

/**
 * T1�жϣ�����оƬ���ݷ���
 *
 */
void T1_Voice_Interrupt(void) {
	//�ȴ�1�ֽڷ������
	if (tTTSFSM.eCMD_Type != T1_CMD_IDLE) {
		//����λ�������
		if (tTTSFSM.u8SendBitsCount == 9) {
			//����ֹͣλ
			GPIO_SetBits(TTS_TX_PORT, TTS_TX_PIN);

			//�ۼ��ѷ���λ��
			tTTSFSM.u8SendBitsCount++;
		}

		//���������λ��Ҫ����
		else if (tTTSFSM.u8SendBitsCount < 9) {
			//������ʼλ
			if (tTTSFSM.u8SendBitsCount == 0) {
				//���������ߣ�������ʼλ
				GPIO_ResetBits(TTS_TX_PORT, TTS_TX_PIN);
			} else {
				//��ʼ��������λ
				if (tTTSFSM.u8SendingData & 0x01) {
					//���������ߣ��������ݡ�1��
					GPIO_SetBits(TTS_TX_PORT, TTS_TX_PIN);
				} else {
					//���������ߣ��������ݡ�0��
					GPIO_ResetBits(TTS_TX_PORT, TTS_TX_PIN);
				}

				//ȡ��һλ
				tTTSFSM.u8SendingData >>= 1;
			}

			//�ۼ��ѷ���λ��
			tTTSFSM.u8SendBitsCount++;
		}
		//һ�ֽ������ѷ������
		else {
			//����������
			GPIO_SetBits(TTS_TX_PORT, TTS_TX_PIN);

			//��λλ���ͼ�����
			tTTSFSM.u8SendBitsCount = 0;

			//λ����״̬ = ����
			tTTSFSM.eCMD_Type = T1_CMD_IDLE;
			return;
		}
	} else {
		//ȡ��һ�ֽڷ���
		if (tTTSFSM.u8CurSendByte <= tTTSFSM.u8MsgLen - 1) {
			tTTSFSM.u8SendingData = tTTSFSM.u8Msg[tTTSFSM.u8CurSendByte++];
			tTTSFSM.u8SendBitsCount = 0;
			tTTSFSM.eCMD_Type = T1_CMD_VOICE;
		}
		//�ַ���������ϣ�
		else {
			//���ʶ���һ�Σ��ݼ��ʶ�����
			tTTSFSM.u8ReadTimes--;

			tTTSFSM.u8CurSendByte = 0;

			//�رն�ʱ��1
			TIM_ITConfig(TIMTts, TIM_IT_CC1, DISABLE);
		}
	}
}

/**
 * 10MS��ʱ������
 *
 */
void Isr_TTS_Timeout10ms(void) {
	if (tTTSFSM.u16TimeElapsed_Reset) {
		--tTTSFSM.u16TimeElapsed_Reset;
	}
}

/**
 * ��ʱ�ر�TTS��Դ
 *
 */
void Isr_TTS_Timeout1s(void) {
	if (tTTSFSM.u8TimeElapsed_TTSAudioOff > 0) {
		tTTSFSM.u8TimeElapsed_TTSAudioOff--;
	}
}
