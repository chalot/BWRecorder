/*
 * ir.c
 *
 *  Created on: 2014��4��25��
 *      Author: ThinkPad User
 */
#include <stm32f2xx_conf.h>
#include <board.h>
#include <ir.h>

static tIRFSMType tIRFSM;		//����ģ�����״̬��

/**
 * ң����Ӳ����ʼ��
 *
 */
void IR_Lowlevel_Init() {

}

/***********************************************************************************************************
 * ����������	���ⰴ�����������̣������������
 * ��    �ڣ�	��
 * ��    �ڣ�	��
 * ��	�ƣ�
 ***********************************************************************************************************/
extern void IR_Process() {
	u8 u8IRData = 0;

	//������ջ����������ݣ�
	if (tIRFSM.u8Available_Rx) {
		//��ȡ������
		u8IRData = *(tIRFSM.pu8Buffer_Rx + tIRFSM.u8Read_Rx++);

		//ָ���ͷ
		if (tIRFSM.u8Read_Rx >= _BUFFER_LENGTH_IR_Rx_) {
			tIRFSM.u8Read_Rx = 0;
		}

		//��Ч���ݼ���
		--tIRFSM.u8Available_Rx;

		//ִ�в�ͬ�Ĳ�����
		switch (u8IRData) {
		//�����̰��� ѡ̨��/��һ��
		case _IR_CMD_DIRECTION_UP_:
		case _IR_CMD_PREV:

			break;

			//�����̰��� ѡ̨+/��һ��
		case _IR_CMD_DIRECTION_DOWN_:
		case _IR_CMD_NEXT:

			break;

			//MUTE
		case _IR_CMD_DIRECTION_MUTE_:

			break;

			//��Դ
		case _IR_CMD_POWER:

			break;

			//����
		case _IR_CMD_DISP:

			break;

			//MUTE
		case _IR_CMD_MUTE:

			break;

			//����+
		case _IR_CMD_VOLUME_PLUS:

			break;

			//����-
		case _IR_CMD_VOLUME_MINUS:

			break;

		case _IR_CMD_PLAY:

			break;
		default:
			break;
		}
	}
}

/***********************************************************************************************************
 * ����������	��������жϺ���
 * ��    �ڣ�	��
 * ��    �ڣ�	��
 * ��	�ƣ�
 ***********************************************************************************************************/
void ISR_IRReceive(void) {
	u8 u8Err = 0;											//�����Ƿ����
	u8 u8Bit;												//���յ���bitλ
	u8 u8IDData, u8IDAntiData;								//���յ���ʶ���뼰�䷴��
	u8 u8OperateData, u8OperateAntiData;					//���յ������ݲ����뼰�䷴��

	//���ݽ���״̬����
	switch (tIRFSM.u8ReceivedStatus) {
	//����̬
	case _IR_RECEIVEDSTATUS_IDLE:
		//ת�����������״̬
		tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_START;

		//����ʱ��ʹ��
		tIRFSM.fCodeTimeEn = 1;

		//����ʱ����0
		tIRFSM.u32CodeTime = 0;

		//���ճ�ʱ
		tIRFSM.u8ReceiveTimeout = _IR_RECEIVE_TIMEOUT_;

		//���յ��Ĳ�������0
		tIRFSM.u8RecievedCode = 0;

		//������ʱ��0
		TIM_ClearFlag(TIMIr, TIM_FLAG_Update);

		TIM_ITConfig(TIMIr, TIM_IT_Update, ENABLE);

		TIM_Cmd(TIMIr, ENABLE);

		break;

		//���յ�������
	case _IR_RECEIVEDSTATUS_START:
		//�ϴν��յ�����������
		if ((tIRFSM.u32CodeTime >= _IR_CODE_TIME_START - _IR_CODE_TIME_BIAS)
				&& (tIRFSM.u32CodeTime <= _IR_CODE_TIME_START + _IR_CODE_TIME_BIAS)) {
			//����״̬תΪ����32λ����
			tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_32BITSDATA;

			//����ʱ����0
			tIRFSM.u32CodeTime = 0;

			//���ݳ���
			tIRFSM.u8BitsLength = 32;

			//32λ������0
			tIRFSM.u32Recieved32Bits = 0;
		}
		//����
		else {
			u8Err = 1;
		}
		break;

		//����32λ����
	case _IR_RECEIVEDSTATUS_32BITSDATA:
		//�յ���0��
		if ((tIRFSM.u32CodeTime >= _IR_CODE_TIME_0 - _IR_CODE_TIME_BIAS)
				&& (tIRFSM.u32CodeTime <= _IR_CODE_TIME_0 + _IR_CODE_TIME_BIAS)) {
			u8Bit = 0;
		}
		//�յ���1��?
		else if ((tIRFSM.u32CodeTime >= _IR_CODE_TIME_1 - _IR_CODE_TIME_BIAS)
				&& (tIRFSM.u32CodeTime <= _IR_CODE_TIME_1 + _IR_CODE_TIME_BIAS)) {
			u8Bit = 1;
		}
		//����
		else {
			u8Err = 1;
		}

		if (!u8Err) {
			//32λ��������
			tIRFSM.u32Recieved32Bits <<= 1;

			//��¼��ǰ���յ���0/1ֵ
			tIRFSM.u32Recieved32Bits |= u8Bit;

			//��������ʱ����0
			tIRFSM.u32CodeTime = 0;

			//�������ݳ���--
			tIRFSM.u8BitsLength--;

			//�Ѿ����յ�32λ���ݣ�
			if (!tIRFSM.u8BitsLength) {
				//��ȡ��ַ�����ݡ����ݷ���
				u8IDData = ((tIRFSM.u32Recieved32Bits >> 24) & 0x000000FF);
				u8IDAntiData = ((tIRFSM.u32Recieved32Bits >> 16) & 0x000000FF);
				u8OperateData = ((tIRFSM.u32Recieved32Bits >> 8) & 0x000000FF);
				u8OperateAntiData = tIRFSM.u32Recieved32Bits & 0x000000FF;

				//��ַ�����ݲ���ȷ
				if ((0xFF != u8IDData + u8IDAntiData) || (0xFF != u8OperateData + u8OperateAntiData)) {
					u8Err = 1;
				}
				//���յ���ȷ������
				else {
					//������ջ������Ƿ�������
					if (tIRFSM.u8Available_Rx < _BUFFER_LENGTH_IR_Rx_) {
						//���ݻ�������������
						tIRFSM.u8Available_Rx++;

						//�����յ��ĺ��������������ݻ�����
						*(tIRFSM.pu8Buffer_Rx + tIRFSM.u8Write_Rx++) = u8OperateData;

						//��¼���յ��Ĳ�����
						tIRFSM.u8RecievedCode = u8OperateData;

						//���ջ�����дָ���ͷ
						if (tIRFSM.u8Write_Rx >= _BUFFER_LENGTH_IR_Rx_) {
							tIRFSM.u8Write_Rx = 0;
						}
					}

					//״̬תΪ���ռ��
					tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_INTERVAL;

					//�ڼ������յ����������ǵ�һ�������룬��һ��������Ҫ����
					tIRFSM.fFirstContinue = 1;

					//����ʱ����0
					tIRFSM.u32CodeTime = 0;
				}
			}
		}

		break;

		//�������ݽ�������������ɺ���һ���½�������
	case _IR_RECEIVEDSTATUS_INTERVAL:
		//IR����״̬תΪ����������
		tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_CONTINUE;

		//����ʱ����0
		tIRFSM.u32CodeTime = 0;

		//���ý��ճ�ʱ
		tIRFSM.u8ReceiveTimeout = _IR_RECEIVE_TIMEOUT_;

		break;

		//����������
	case _IR_RECEIVEDSTATUS_CONTINUE:
		//������
		if ((tIRFSM.u32CodeTime >= _IR_CODE_TIME_SEQUENTIAL - _IR_CODE_TIME_BIAS)
				&& (tIRFSM.u32CodeTime <= _IR_CODE_TIME_SEQUENTIAL + _IR_CODE_TIME_BIAS)) {
			//�ǵ�һ�������룿
			if (tIRFSM.fFirstContinue == 1) {
				//������һ��������

				//����������յ�������Ļ������ǵ�һ����������
				tIRFSM.fFirstContinue = 0;
			}
			//���ǵ�һ��������,ң����֧����������
			else if ((_IR_CMD_VOLUME_PLUS == tIRFSM.u8RecievedCode)
					|| (_IR_CMD_VOLUME_MINUS == tIRFSM.u8RecievedCode)) {
				//������ջ������Ƿ�������
				if (tIRFSM.u8Available_Rx < _BUFFER_LENGTH_IR_Rx_) {
					//���ջ�����+1
					tIRFSM.u8Available_Rx++;

					//���ϴν��յ��Ĳ����������ջ�����
					*(tIRFSM.pu8Buffer_Rx + tIRFSM.u8Write_Rx++) = tIRFSM.u8RecievedCode;

					//���ջ�����дָ���ͷ
					if (tIRFSM.u8Write_Rx >= _BUFFER_LENGTH_IR_Rx_) {
						tIRFSM.u8Write_Rx = 0;
					}
				}
			}

			//ת�����������֮��ļ��״̬
			tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_INTERVAL;

			//����ʱ����0
			tIRFSM.u32CodeTime = 0;

			//���ý��ճ�ʱ
			tIRFSM.u8ReceiveTimeout = _IR_RECEIVE_TIMEOUT_;
		}
		//���ճ���
		else {
			u8Err = 1;
		}

		break;

	default:
		break;
	}

	//��������¿�ʼ
	if (u8Err) {
		//����״̬תΪ����
		tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_IDLE;

		//��ֹ��������ʱ����ʱ
		tIRFSM.fCodeTimeEn = 0;

		//�������ͼ�ʱ��0
		tIRFSM.u32CodeTime = 0;

		//����������0
		tIRFSM.u32Recieved32Bits = 0;

		//�������ݳ�����0
		tIRFSM.u8BitsLength = 0;

		//��ֹ���ճ�ʱ
		tIRFSM.u8ReceiveTimeout = 0;

		//ֹͣ��ʱ��
		TIM_Cmd(TIMIr, DISABLE);
	}
}

/***********************************************************************************************************
 * ����������	10ms��ʱ����������Ҫ������ʱ��λΪ10ms�Ķ�ʱ����
 * ��    �ڣ�	��
 * ��    �ڣ�	��
 * ��	�ƣ�
 ***********************************************************************************************************/
void Isr_IR_Timeout10ms(void) {
	//������ճ�ʱ��ʱ��
	if (tIRFSM.u8ReceiveTimeout) {
		tIRFSM.u8ReceiveTimeout--;

		//��ʱ��
		if (0 == tIRFSM.u8ReceiveTimeout) {
			//ֹͣ���գ���ֹͣ��ʱ��
			TIM_Cmd(TIMIr, DISABLE);

			//IR����״̬תΪ����
			tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_IDLE;

			//��ֹ����ʱ����ʱ
			tIRFSM.fCodeTimeEn = 0;
		}
	}
}

/***********************************************************************************************************
 * ����������	10ous��ʱ����������Ҫ������¼���������������
 * ��    �ڣ�	��
 * ��    �ڣ�	��
 * ��	�ƣ�
 ***********************************************************************************************************/
void Isr_IR_Timeout100us(void) {
	//������ռ�ʱ
	if (tIRFSM.fCodeTimeEn) {
		//����ʱ��+100us
		tIRFSM.u32CodeTime += 100;

		//���ճ�ʱ
		if (((tIRFSM.u8ReceivedStatus <= _IR_RECEIVEDSTATUS_32BITSDATA)
				&& (tIRFSM.u32CodeTime >= _IR_DATA_LIMITATION_TIME))
				|| ((tIRFSM.u8ReceivedStatus == _IR_RECEIVEDSTATUS_INTERVAL)
						&& (tIRFSM.u32CodeTime >= _IR_INTERVAL_LIMITATION_TIME))) {
			//����ʱ����ֹ
			tIRFSM.fCodeTimeEn = 0;

			//����״̬תΪ����
			tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_IDLE;

			//����ʱ����0
			tIRFSM.u32CodeTime = 0;
		}
	}
}

