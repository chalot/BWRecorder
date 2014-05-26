/*
 * ir.h
 *
 *  Created on: 2014年4月25日
 *      Author: ThinkPad User
 */

#ifndef IR_H_
#define IR_H_

#define		_BUFFER_LENGTH_IR_Rx_					20

//方向盘快捷键
#define		_IR_CMD_DIRECTION_UP_	0xB0		//方向盘按键  UP
#define		_IR_CMD_DIRECTION_DOWN_	0x88		//方向盘按键 DOWN
#define	    _IR_CMD_DIRECTION_MUTE_	0x80		//方向盘按键 MUTE

//First Colume
#define		_IR_CMD_POWER			0x20		//电源按键
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
#define		_IR_CMD_DISP			0xC8		//DISPLAY键
#define		_IR_CMD_STOP			0xE8		//STOP
#define		_IR_CMD_FF				0xD8		//FF
#define		_IR_CMD_VOLUME_MINUS	0xF8		//音量-
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
#define		_IR_CMD_VOLUME_PLUS		0xBA		//音量+
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
#define		_IR_CMD_NEXT			0x1A		//下一曲
#define		_IR_CMD_MUTE			0x3A		//静音键
#define		_IR_CMD_RESET			0xFA		//RESET

//红外遥控器码型格式时间长度（单位：us）
#define 	_IR_CODE_TIME_START					13500		//启动码-----9ms低电平+4.5ms高电平
#define 	_IR_CODE_TIME_SEQUENTIAL			11500		//连发码-----9ms低电平+2.5ms高电平
#define 	_IR_CODE_TIME_0						1125		//"0"-----0.625ms低电平+0.5ms高电平
#define		_IR_CODE_TIME_1						2250		//"1"-----0.625ms低电平+1.625ms高电平
#define		_IR_CODE_TIME_BIAS					300			//码型格式时长偏移 = 0.3ms

#define		_IR_DATA_LIMITATION_TIME			20000		//启动码、数据码红外极限时间

//如果超过这个时间，有错，主动复位
#define		_IR_INTERVAL_LIMITATION_TIME		120000		//数据接收完到连发码之间的时间间隔、以及多个连发码之间的时间间隔


//红外遥控器编码接收状态
#define		_IR_RECEIVEDSTATUS_IDLE				0			//接收空闲
#define		_IR_RECEIVEDSTATUS_START			1			//接收启动码
#define		_IR_RECEIVEDSTATUS_32BITSDATA		2			//接收32位数据
#define		_IR_RECEIVEDSTATUS_CONTINUE			3			//接收连发码
#define		_IR_RECEIVEDSTATUS_INTERVAL			4			//接收数据结束
#define		_IR_RECEIVE_TIMEOUT_				20			//接收超时，200ms

//红外遥控器接收状态机
typedef struct {
	u8 u8ReceivedStatus;					//当前的接收状态
	u8 fCodeTimeEn :1;						//码型时长使能
	u8 fFirstContinue :1;					//第一个连发码
	u8 fReserved :6;
	u32 u32Recieved32Bits;					//接收的32位数据

	u8 u8BitsLength;						//接收的数据位长度
	u8 u8RecievedCode;						//接收到的操作码

	u32 u32CodeTime;						//码型格式时长

	u8 u8Write_Rx;							//接收缓冲区写指针
	u8 u8Read_Rx;							//接收缓冲区读指针
	u8 u8Available_Rx;						//接收缓冲区有效字节数
	u8 *pu8Buffer_Rx;						//接收缓冲区首指针

	u8 u8ReceiveTimeout;					//接收超时，500ms接收超时
} tIRFSMType;

extern void IR_Process(void);
void IR_Lowlevel_Init();

extern void Isr_IR_Timeout10ms(void);
extern void Isr_IR_Timeout100us(void);
extern void ISR_IRReceive(void);

#endif /* IR_H_ */
