/*
 * ir.h
 *
 *  Created on: 2014��4��25��
 *      Author: ThinkPad User
 */

#ifndef IR_H_
#define IR_H_

#define		_BUFFER_LENGTH_IR_Rx_					20

//�����̿�ݼ�
#define		_IR_CMD_DIRECTION_UP_	0xB0		//�����̰���  UP
#define		_IR_CMD_DIRECTION_DOWN_	0x88		//�����̰��� DOWN
#define	    _IR_CMD_DIRECTION_MUTE_	0x80		//�����̰��� MUTE

//First Colume
#define		_IR_CMD_POWER			0x20		//��Դ����
#define		_IR_CMD_AUDIO			0x00		//AUDIO
#define		_IR_CMD_SUBTITLE		0x40		//SUBTITLE
#define		_IR_CMD_TITLE			0x60		//TITLE
#define		_IR_CMD_REPEAT			0x50		//REPEAT
#define		_IR_CMD_A_B				0x70		//A-B
#define		_IR_CMD_SETUP			0x48		//SETUP
#define		_IR_CMD_SLOW			0x68		//SLOW
#define		_IR_CMD_FB				0x58		//FB
#define		_IR_CMD_MENU_PBC		0x78		//MENU/PBC
#define		_IR_CMD_I_P				0x08		//I/P

//Second Colume
#define		_IR_CMD_1				0xA0		//1
#define		_IR_CMD_4				0x80		//4
#define		_IR_CMD_7				0xC0		//7
#define		_IR_CMD_STEP			0xD0		//STEP
#define		_IR_CMD_10_PLUS			0xE0		//10+
#define		_IR_CMD_LEFT			0xF0		//LEFT
#define		_IR_CMD_DISP			0xC8		//DISPLAY��
#define		_IR_CMD_STOP			0xE8		//STOP
#define		_IR_CMD_FF				0xD8		//FF
#define		_IR_CMD_VOLUME_MINUS	0xF8		//����-
#define		_IR_CMD_N_P				0xA8		//N/P

//Third Colume
#define		_IR_CMD_2				0x62		//2
#define		_IR_CMD_5				0x42		//5
#define		_IR_CMD_8				0x82		//8
#define		_IR_CMD_0				0xA2		//0
#define		_IR_CMD_UP				0x92		//UP
#define		_IR_CMD_ENTER			0xB2		//ENTER
#define		_IR_CMD_DOWN			0x8A		//DOWN
#define		_IR_CMD_PLAY			0xAA		//PLAY\PAUSE
#define		_IR_CMD_PREV			0x9A		//PREV
#define		_IR_CMD_VOLUME_PLUS		0xBA		//����+
#define		_IR_CMD_SEARCH			0x5A		//SEARCH

//Fourth Colume
#define		_IR_CMD_3				0xE2		//3
#define		_IR_CMD_6				0xC2		//6
#define		_IR_CMD_9				0x02		//9
#define		_IR_CMD_CLEAR			0x22		//CLEAR
#define		_IR_CMD_ZOOM			0x12		//ZOOM
#define		_IR_CMD_RIGHT			0x32		//RIGHT
#define		_IR_CMD_ANGLE			0x0A		//ANGLE
#define		_IR_CMD_PROG			0x2A		//RPOG
#define		_IR_CMD_NEXT			0x1A		//��һ��
#define		_IR_CMD_MUTE			0x3A		//������
#define		_IR_CMD_RESET			0xFA		//RESET

//����ң�������͸�ʽʱ�䳤�ȣ���λ��us��
#define 	_IR_CODE_TIME_START					13500		//������-----9ms�͵�ƽ+4.5ms�ߵ�ƽ
#define 	_IR_CODE_TIME_SEQUENTIAL			11500		//������-----9ms�͵�ƽ+2.5ms�ߵ�ƽ
#define 	_IR_CODE_TIME_0						1125		//"0"-----0.625ms�͵�ƽ+0.5ms�ߵ�ƽ
#define		_IR_CODE_TIME_1						2250		//"1"-----0.625ms�͵�ƽ+1.625ms�ߵ�ƽ
#define		_IR_CODE_TIME_BIAS					300			//���͸�ʽʱ��ƫ�� = 0.3ms

#define		_IR_DATA_LIMITATION_TIME			20000		//�����롢��������⼫��ʱ��

//����������ʱ�䣬�д�������λ
#define		_IR_INTERVAL_LIMITATION_TIME		120000		//���ݽ����굽������֮���ʱ�������Լ����������֮���ʱ����


//����ң�����������״̬
#define		_IR_RECEIVEDSTATUS_IDLE				0			//���տ���
#define		_IR_RECEIVEDSTATUS_START			1			//����������
#define		_IR_RECEIVEDSTATUS_32BITSDATA		2			//����32λ����
#define		_IR_RECEIVEDSTATUS_CONTINUE			3			//����������
#define		_IR_RECEIVEDSTATUS_INTERVAL			4			//�������ݽ���
#define		_IR_RECEIVE_TIMEOUT_				20			//���ճ�ʱ��200ms

//����ң��������״̬��
typedef struct {
	u8 u8ReceivedStatus;					//��ǰ�Ľ���״̬
	u8 fCodeTimeEn :1;						//����ʱ��ʹ��
	u8 fFirstContinue :1;					//��һ��������
	u8 fReserved :6;
	u32 u32Recieved32Bits;					//���յ�32λ����

	u8 u8BitsLength;						//���յ�����λ����
	u8 u8RecievedCode;						//���յ��Ĳ�����

	u32 u32CodeTime;						//���͸�ʽʱ��

	u8 u8Write_Rx;							//���ջ�����дָ��
	u8 u8Read_Rx;							//���ջ�������ָ��
	u8 u8Available_Rx;						//���ջ�������Ч�ֽ���
	u8 *pu8Buffer_Rx;						//���ջ�������ָ��

	u8 u8ReceiveTimeout;					//���ճ�ʱ��500ms���ճ�ʱ
} tIRFSMType;

extern void IR_Process(void);
void IR_Lowlevel_Init();

extern void Isr_IR_Timeout10ms(void);
extern void Isr_IR_Timeout100us(void);
extern void ISR_IRReceive(void);

#endif /* IR_H_ */
