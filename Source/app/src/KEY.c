/***********************************************************************************************************
 * ��    ����	Key.c
 * ��    ����	��������
 * ��  	�ڣ�
 * �� 	����	V1.0
 ***********************************************************************************************************/
#include "stm32f2xx.h"
#include <board.h>
#include <key.h>
#include <qactiveobjs.h>
#include <qp_port.h>

#define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)

__IO uint16_t u16Key_ADC3ConvertedValue = 0; 	///��ԴAD���ֵ

/*
 ������ѹVref = 5.40V����������Rp = 2K��

 ���㹫ʽ��Vadc = (Rp / (Rp + 10)) * Vref

 �����������Щ����������Щ��������������Щ�������������������
 ��  ����  �� ����(��) �� ʵ���ѹ(mV) ��  �ο�ֵ(12bits)  ��
 �����������੤���������੤�������������੤������������������
 ��  MENU  ��   220    ��     537      ��     0x0074       ��
 �����������੤���������੤�������������੤������������������
 ��   OK   ��   440    ��     972      ��     0x00D2       ��
 �����������੤���������੤�������������੤������������������
 ��   ��   ��   1370   ��    2191      ��     0x01DB       ��
 �����������੤���������੤�������������੤������������������
 ��   ��   ��   1709   ��    2489      ��     0x021C       ��
 �����������੤���������੤�������������੤������������������
 ��  ���  ��   5760   ��    3999      ��     0x0365       ��
 �����������ة����������ة��������������ة�������������������
 */

/**
 * ���AD����ֵ�Ƿ���Ч
 *
 * @param u16Sample
 * @return	BOOL,TRUE:��ʾ����ֵ����Ч��Χ�ڣ�FALSE:��ʾ����ֵ��Ч
 */
BOOL Key_ADInrange(u16 u16Sample) {
	if ((( _KEY_AD_OK_ + _KEY_AD_OFFSET_ > u16Sample)
			&& ( _KEY_AD_OK_ - _KEY_AD_OFFSET_ < u16Sample))
			|| (( _KEY_AD_UP_ + _KEY_AD_OFFSET_ > u16Sample)
					&& ( _KEY_AD_UP_ - _KEY_AD_OFFSET_ < u16Sample))
			|| (( _KEY_AD_DOWN_ + _KEY_AD_OFFSET_ > u16Sample)
					&& ( _KEY_AD_DOWN_ - _KEY_AD_OFFSET_ < u16Sample))
			|| (( _KEY_AD_MENU_ + _KEY_AD_OFFSET_ > u16Sample)
					&& ( _KEY_AD_MENU_ - _KEY_AD_OFFSET_ < u16Sample))) {
		return TRUE;
	}

	return FALSE;
}

/**
 * ��ð���������
 *
 * @param u16Sample			������AD����ֵ
 * @return	eKEY			������Ϣ
 */
eKEY Key_GetCode(u16 u16Sample) {

	if (( _KEY_AD_OK_ + _KEY_AD_OFFSET_ > u16Sample)
			&& ( _KEY_AD_OK_ - _KEY_AD_OFFSET_ < u16Sample)) {
		return KEY_OK;
	} else if (( _KEY_AD_UP_ + _KEY_AD_OFFSET_ > u16Sample)
			&& ( _KEY_AD_UP_ - _KEY_AD_OFFSET_ < u16Sample)) {
		return KEY_UP;
	} else if (( _KEY_AD_DOWN_ + _KEY_AD_OFFSET_ > u16Sample)
			&& ( _KEY_AD_DOWN_ - _KEY_AD_OFFSET_ < u16Sample)) {
		return KEY_DOWN;
	} else if (( _KEY_AD_MENU_ + _KEY_AD_OFFSET_ > u16Sample)
			&& ( _KEY_AD_MENU_ - _KEY_AD_OFFSET_ < u16Sample)) {
		return KEY_MENU;
	} else {
		return KEY_UNKNOWN;
	}
}
//#define	TTS_TEST
/***********************************************************************************************************
 * ����������	�������ʵ�ʴ�������
 * ��    �ڣ�	��
 * ��    �ڣ�	��
 ***********************************************************************************************************/
void Key_Process() {
#if 0
	u8 u8Tmp = 0;
	u32 u32SampleValue = 0;
	u8 u8KeyCode = 0;

	//AD��������м�⵽�а�������
	if(tKEYFSM.eDetect_Status == _eKEY_DETECTIONSTATUS_KEYUP_)
	{

		tKEYFSM.eDetect_Status = _eKEY_DETECTIONSTATUS_IDEL_;

		//�����β���ƽ��ֵ
		for(u8Tmp = 0; u8Tmp < _KEY_SAMPLE_TIMES_; u8Tmp++)
		{
			//��������ۼ�ֵ
			u32SampleValue += tKEYFSM.u16Key_SmpRslt[u8Tmp];

			//����ͬʱ���
			tKEYFSM.u16Key_SmpRslt[u8Tmp] = 0;
		}

		//����ֵȡƽ��							
		u32SampleValue = u32SampleValue / _KEY_SAMPLE_TIMES_;

		//����ƽ��ֵҲ�ں���������ȡֵ��Χ��
		if(Key_ADInrange((u16)u32SampleValue))
		{
			//�õ�������
			u8KeyCode = Key_GetCode((u16)u32SampleValue);

			//����������100ms
			tSYSFSM.u16SysBeepTimeout = 8;

			//ʹ�ܷ�����
			Beep_Enable();

			//ִ�а����������
			Key_Execute(u8KeyCode);
		}
	}
#endif
}

/**
 * ����������
 *
 * @param u8KeyOp	�����õ��İ���������ֵ
 */
void Key_Execute(u8 u8KeyOp) {
//	//������Ч
//	if((u8KeyOp >= _eKEYOK_SHORT_ &&  u8KeyOp <= _eKEYCANCEL_SHORT_) ||
//	   (u8KeyOp >= _eKEYOK_LONG_  &&  u8KeyOp <= _eKEYCANCEL_LONG_ )  )
//	{
//		//��������
////		DISP_OLED_ExecuteCommand(u8KeyOp);
//
//	}
}

/**
 * ADC3�жϴ�����
 *
 */
void ISR_ADC_Key(void) {
#if 0
	//�����ǰ��IDLE״̬����˵�������а���������
	if(tKEYFSM.eDetect_Status == _eKEY_DETECTIONSTATUS_IDEL_ )
	{
		//׼������
		tKEYFSM.eDetect_Status = _eKEY_DETECTIONSTATUS_JITTERY_;

		//����ʱ��20ms												   	
		tKEYFSM.u8JitterTimeout = KEY_DETECT_JITTER_TIME_;
	}
	//����״̬
	else if(tKEYFSM.eDetect_Status == _eKEY_DETECTIONSTATUS_JITTERY_)
	{
		if( tKEYFSM.u8JitterTimeout == 0)
		{
			tKEYFSM.eDetect_Status = _eKEY_DETECTIONSTATUS_KEYDOWN_DET_;

			tKEYFSM.u8SampleTimes = 0;
		}
	}
	//��ⰴ��
	else if(tKEYFSM.eDetect_Status == _eKEY_DETECTIONSTATUS_KEYDOWN_DET_)
	{
		while(tKEYFSM.u8SampleTimes < _KEY_SAMPLE_TIMES_)
		{
			tKEYFSM.u16Key_SmpRslt[tKEYFSM.u8SampleTimes] = ADC_GetConversionValue(ADC3);

			tKEYFSM.u8SampleTimes++;
		}
		tKEYFSM.u8SampleTimes = 0;

		tKEYFSM.eDetect_Status = _eKEY_DETECTIONSTATUS_KEYDOWN_;

		/* Configure high and low analog watchdog thresholds */
		ADC_AnalogWatchdogThresholdsConfig(ADC3, 0xE30, 0x0320);
	}
	//����̧��
	else if(tKEYFSM.eDetect_Status == _eKEY_DETECTIONSTATUS_KEYDOWN_)
	{
		if(0x0E20 < ADC_GetConversionValue(ADC3))
		{
			/* Configure high and low analog watchdog thresholds */
			ADC_AnalogWatchdogThresholdsConfig(ADC3, 0xFFF, 0x0E20);

			tKEYFSM.eDetect_Status = _eKEY_DETECTIONSTATUS_KEYUP_;
		}

	}
#endif
}

/**
 * �����豸��ʼ��,PF8
 *
 */
void Key_LowLevel_Init() {
	/* ADC3 configuration *******************************************************/
	/*  - Enable peripheral clocks                                              */
	/*  - DMA2_Stream0 channel2 configuration                                   */
	/*  - Configure ADC Channel6 pin as analog input                            */
	/*  - Configure ADC3 Channel6                                               */
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* DMA2 Stream0 channel2 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr =
			(uint32_t) &u16Key_ADC3ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* Configure ADC3 Channel7 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = KEY_DET_PIN; 	//GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC3 regular channel7 configuration *************************************/
	ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);

	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);

	/* Start ADC3 Software Conversion */
	ADC_SoftwareStartConv(ADC3);

#if 0
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	//	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	//	TIM_OCInitTypeDef	TIM_OCInitStructure;

	ADC_DeInit();

	/* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(/*RCC_AHB1Periph_DMA2 |*/RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* Configure ADC3 Channel6 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = KEY_DET_PIN;	///GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	/* ADC3 Configuration ------------------------------------------------------*/
	//ADC3ͨ�ó�ʼ��
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC3 regular channel6 configuration */
	ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_TwoSamplingDelay_10Cycles);

	/* Configure high and low analog watchdog thresholds */
	ADC_AnalogWatchdogThresholdsConfig(ADC3, 0xFFF, 0x0E20);

	/* Configure channel14 as the single analog watchdog guarded channel */
	ADC_AnalogWatchdogSingleChannelConfig(ADC3, ADC_Channel_6);

	/* Enable analog watchdog on one regular channel */
	ADC_AnalogWatchdogCmd(ADC3, ADC_AnalogWatchdog_SingleRegEnable);

	/* Enable AWD interupt */
	ADC_ITConfig(ADC3, ADC_IT_AWD, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC3, ENABLE);

	/* Start ADC3 Software Conversion */
	ADC_SoftwareStartConv(ADC3);
#endif
}

