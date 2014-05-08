/*
 * tts.h
 *
 *  Created on: 2014��4��25��
 *      Author: ThinkPad User
 */

#ifndef TTS_H_
#define TTS_H_

#include <type.h>

#pragma	pack(1)

//���ű��뷽ʽ
#define CODE_GB2312								0x00				//GB2312#define CODE_UNICODE							0x03				//Unicode
#define	TTS_BUFFER_SIZE							500					//TTS������󳤶�#define	TTS_FRAME_SIZE							204					//һ֡��󳤶�
#define	TIMEOUT_TTS_RESET						1					//оƬ��λ�͵�ƽ����ʱ�䣬10ms#define	TIMEOUT_TTS_SLEEP						30//120					//оƬ�������߳���ʱ�䣬2����
#define	TTS_AUDIO_ON							1					//���������#define	TTS_AUDIO_OFF							0					//�ر��������
//ģ���Դ״̬
typedef enum {
	_ePOWER_ON = 0,													//�����ϵ�
	_ePOWER_SLEEP,													//����
	_ePOWER_OFF,													//�ϵ�
	_ePOWER_RESET,													//��λ

} _ePOWER_STATE;

typedef enum {
	T1_CMD_IDLE = 0, T1_CMD_VOICE
} T1_CMD_TYPE;

//TTS֡��ʽ
typedef struct {
	u8 u8Head;												//֡ͷ��0xFE
	u8 u8Cmd;												//������
	u8 u8Reserved;											//���ȸ��ֽ�
	u8 u8Len;												//���ȵ��ֽ�
	u8 u8Code;												//�ı���������
	u8 *pData;												//���ϳ��ı�

} _tTTSFRAMETYPE;

#define		MSG_LEN_MAX			256				//������󳤶�
//TTS����״̬��
typedef struct {
	u8 ePowerState;										//��Դ״̬
	u8 u8ResetReq :1;										//��λ����
	u8 u8ResetOk :1;										//��λ�ɹ�
	u8 fReserved :6;
	u8 u8ResetTimeout;										//��λ��ʱʱ��
	u16 u16TimeElapsed_Reset;								//��λ�͵�ƽά�ּ�ʱ��
	u16 u16TimeElapsed_Sleep;								//�������߼�ʱ��
	u8 bAudioState;										//�������״̬��0=�رգ�1=����

	u8 eCMD_Type;
	u8 u8SendStatus;										//��ǰ����״̬
	u8 u8SendBitsCount;									//����λ����
	u8 u8SendingData;										//��ǰ���͵��ֽ�

	u8 bCommingMsg;										//�·����ű�־
	u8 u8Msg[MSG_LEN_MAX];									//�·���������
	u8 u8MsgLen;											//�·����ų���
	u8 u8ReadTimes;										//�ʶ�����
	u8 u8RepeatInterval;									//�ʶ�ʱ����
	u8 u8TimeElapsed_RepeatInterval;						//�ʶ���ʱ����
	u8 u8CurSendByte;										//�ѷ����ֽ���
	u8 u8TimeElapsed_TTSAudioOff;							//��ʱ�ر���Դ

	u16 u16Read;											//��λ��
	u16 u16Write;											//дλ��
	u16 u16AvailableBytes;									//ʣ���ֽ���
	u8 au8Buffer[TTS_BUFFER_SIZE];							//������֡����

} tTTSFSMType;

/**�ⲿ�ӿ� **************************************************************************************/
extern void TTS_Init(void);
extern void TTS_Speak(u8 u8CodeType, u8 *pTxt, u8 u8TxtLen, u8 u8RepeatInterval,
		u8 u8ReadTimes);
extern void TTS_EnableSound(void);							//�����������
extern void TTS_DisableSound(void);							//��ֹ�������

/**�ڲ��ӿ� **************************************************************************************/
void TTS_Process(void);
void TTS_ReadMsgProcess(void);
void TTS_SendByte(u8 u8Data);
void TTS_Stop(void);
void TTS_Sleep(void);
void TTS_Wakeup(void);		//����
BOOL TTS_IsChipIdle(void);	//��������״̬
void TTS_SendFrame(u8 *pFrame, u8 u8FrameLen);
void TIM_Tts_Configuration();

/**ISR *******************************************************************************************/
extern void Isr_TTS_Timeout10ms(void);
extern void Isr_TTS_Timeout1s(void);	//��ʱ�ر�TTS��Դ
extern void T1_Voice_Interrupt(void);

void TTS_Lowlevel_Init();
#pragma	pack()

#endif /* TTS_H_ */
