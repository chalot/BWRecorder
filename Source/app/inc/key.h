#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f2xx_conf.h"

///#define ADC3_DR_ADDRESS    ((u32)0x4001224C)

//���尴������ģʽ��̧����������ǰ��¾Ͳ���
//��ǰ������ǰ��²���
//�������²���
#define	_KEY_DOWN_OPERATE_		0
//����̧�����
#define	_KEY_UP_OPERATE_		1
//�������²���
#define	_KEY_OPERATE_MODE		_KEY_UP_OPERATE_ ///_KEY_DOWN_OPERATE_ ///����̧��ʱ��Ӧ������ʱ����BEEP��
#define	_KEY_BUF_LEN_			5		//������������С����#define	_KEY_SAMPLE_TIMES_		8		//����������������
//�����Ĳ���ADֵ
#define	_KEY_AD_OK_			0xA90		//OK���ο�ֵ#define	_KEY_AD_UP_			0x98E		//UP���ο�ֵ#define	_KEY_AD_DOWN_		0x7FA		//DOWN���ο�ֵ#define	_KEY_AD_MENU_		0x555		//CANCEL���ο�ֵ
#define	_KEY_AD_OFFSET_		0x00B0		//�ο�ֵ����ƫ��
///�������ֳ����̰���
//#define	_KEY_ACTION_SHORT_DOWN_		0x01	//�����̰�
//#define	_KEY_ACTION_LONG_DOWN_		0x02	//��������

//������Ϣ����
typedef enum {
	KEY_OK = 0,		//ȷ����
	KEY_UP,			//���ϼ�
	KEY_DOWN,		//���¼�
	KEY_MENU,		//�˵���
	KEY_UNKNOWN,
} eKEY;

/**
 //������Ҫ�������Ĳ���
 typedef enum
 {
 _eKEYOK_SHORT_ = 1,				//ȷ�����̰�
 _eKEYSELUP_SHORT_,				//ѡ�����ϼ��̰�	
 _eKEYSELDOWN_SHORT_,			//ѡ�����¼��̰�
 _eKEYCANCEL_SHORT_,				//ȡ���̰���
 
 _eKEYOK_LONG_= 0x81,			//ȷ��������			
 _eKEYSELUP_LONG_,				//ѡ�����ϼ�����
 _eKEYSELDOWN_LONG_,				//ѡ�����¼�����
 _eKEYCANCEL_LONG_,				//ȡ��������	
 
 _eKEY_NULL = 0xFF				//�հ���
 }_eKEY_OPERATOR_;
 */

#define	_KEY_CODE_OFFSET  	0x80	//��������̰��ͳ�����ƫ��ֵ��0x80	
//����ʱ�䶨��
#define	KEY_DETECT_JITTER_TIME_			5		//����ʱ��20ms
//����������ʱ�䶨��2s
#define	KEY_DETECT_LONGDOWN_TIME_       100		//����ʱ��2s
//��������ʱLED����ʱ�䶨��5s
#define		KEY_LED_TIME_       500		//��������LED����ʱ��5s//������ⲽ��
typedef enum {
	_eKEY_DETECTIONSTATUS_IDEL_ = 0,		//������
	_eKEY_DETECTIONSTATUS_JITTERY_,					//�������
	_eKEY_DETECTIONSTATUS_KEYDOWN_DET_,				//��ⰴ������
	_eKEY_DETECTIONSTATUS_KEYDOWN_,					//��⵽��������
	_eKEY_DETECTIONSTATUS_KEYLONGDOWN_DET_,			//��ⰴ������
	_eKEY_DETECTIONSTATUS_KEYLONGDOWN_,				//��⵽��������
	_eKEY_DETECTIONSTATUS_KEYUP_DET_,				//��ⰴ��̧��
	_eKEY_DETECTIONSTATUS_KEYUP_					//��⵽����̧��
} eKEYDETECTIONSTATUSType;

//����״̬��
typedef struct {
	eKEYDETECTIONSTATUSType eDetect_Status;		//���״̬
	u8 u8KeyAction;		//��������

	u16 u16Key_SmpRslt[_KEY_SAMPLE_TIMES_];		//��β�����ȡƽ��ֵ
	u8 u8SampleTimes;			//��������
	u8 u8JitterTimeout;
	u8 u8LedlightTimeout;		//��������ʱLED����ʱ�䳬ʱֵ
	u8 u8KeyBuffer[_KEY_BUF_LEN_];		//����������
	u8 u8Read;		//��ָ��
	u8 u8Write;	//дָ��

} tKEYFSMType;

#define Key_GetADCValue()	ADC_GetConversionValue(ADC3)	///ȡ�������ֵ
void Key_Put2Buffer(u8 u8KeyCode);
BOOL Key_ADInrange(u16 u16Sample);

eKEY Key_GetCode(u16 u16SmpValue);
void ISR_ADC_Key(void);

extern void Key_Process(void);

void Key_LowLevel_Init();

#endif
