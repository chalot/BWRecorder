/*
 * ir.c
 *
 *  Created on: 2014年4月25日
 *      Author: ThinkPad User
 */
#include <stm32f2xx_conf.h>
#include <board.h>
#include <ir.h>

static tIRFSMType tIRFSM;		//红外模拟输出状态机

/**
 * 遥控器硬件初始化
 *
 */
void IR_Lowlevel_Init() {

}

/***********************************************************************************************************
 * 功能描述：	红外按键处理主流程，处理红外命令
 * 入    口：	无
 * 出    口：	无
 * 设	计：
 ***********************************************************************************************************/
extern void IR_Process() {
	u8 u8IRData = 0;

	//红外接收缓冲区有数据？
	if (tIRFSM.u8Available_Rx) {
		//获取操作码
		u8IRData = *(tIRFSM.pu8Buffer_Rx + tIRFSM.u8Read_Rx++);

		//指针回头
		if (tIRFSM.u8Read_Rx >= _BUFFER_LENGTH_IR_Rx_) {
			tIRFSM.u8Read_Rx = 0;
		}

		//有效数据减少
		--tIRFSM.u8Available_Rx;

		//执行不同的操作码
		switch (u8IRData) {
		//方向盘按键 选台－/上一曲
		case _IR_CMD_DIRECTION_UP_:
		case _IR_CMD_PREV:

			break;

			//方向盘按键 选台+/下一曲
		case _IR_CMD_DIRECTION_DOWN_:
		case _IR_CMD_NEXT:

			break;

			//MUTE
		case _IR_CMD_DIRECTION_MUTE_:

			break;

			//电源
		case _IR_CMD_POWER:

			break;

			//屏显
		case _IR_CMD_DISP:

			break;

			//MUTE
		case _IR_CMD_MUTE:

			break;

			//音量+
		case _IR_CMD_VOLUME_PLUS:

			break;

			//音量-
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
 * 功能描述：	红外接收中断函数
 * 入    口：	无
 * 出    口：	无
 * 设	计：
 ***********************************************************************************************************/
void ISR_IRReceive(void) {
	u8 u8Err = 0;											//接收是否出错？
	u8 u8Bit;												//接收到的bit位
	u8 u8IDData, u8IDAntiData;								//接收到的识别码及其反码
	u8 u8OperateData, u8OperateAntiData;					//接收到的数据操作码及其反码

	//根据接收状态处理
	switch (tIRFSM.u8ReceivedStatus) {
	//空闲态
	case _IR_RECEIVEDSTATUS_IDLE:
		//转入接收启动码状态
		tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_START;

		//码型时长使能
		tIRFSM.fCodeTimeEn = 1;

		//码型时长清0
		tIRFSM.u32CodeTime = 0;

		//接收超时
		tIRFSM.u8ReceiveTimeout = _IR_RECEIVE_TIMEOUT_;

		//接收到的操作码清0
		tIRFSM.u8RecievedCode = 0;

		//启动定时器0
		TIM_ClearFlag(TIMIr, TIM_FLAG_Update);

		TIM_ITConfig(TIMIr, TIM_IT_Update, ENABLE);

		TIM_Cmd(TIMIr, ENABLE);

		break;

		//接收到启动码
	case _IR_RECEIVEDSTATUS_START:
		//上次接收到的是启动码
		if ((tIRFSM.u32CodeTime >= _IR_CODE_TIME_START - _IR_CODE_TIME_BIAS)
				&& (tIRFSM.u32CodeTime <= _IR_CODE_TIME_START + _IR_CODE_TIME_BIAS)) {
			//接收状态转为接收32位数据
			tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_32BITSDATA;

			//码型时长清0
			tIRFSM.u32CodeTime = 0;

			//数据长度
			tIRFSM.u8BitsLength = 32;

			//32位数据清0
			tIRFSM.u32Recieved32Bits = 0;
		}
		//出错
		else {
			u8Err = 1;
		}
		break;

		//接收32位数据
	case _IR_RECEIVEDSTATUS_32BITSDATA:
		//收到“0”
		if ((tIRFSM.u32CodeTime >= _IR_CODE_TIME_0 - _IR_CODE_TIME_BIAS)
				&& (tIRFSM.u32CodeTime <= _IR_CODE_TIME_0 + _IR_CODE_TIME_BIAS)) {
			u8Bit = 0;
		}
		//收到“1”?
		else if ((tIRFSM.u32CodeTime >= _IR_CODE_TIME_1 - _IR_CODE_TIME_BIAS)
				&& (tIRFSM.u32CodeTime <= _IR_CODE_TIME_1 + _IR_CODE_TIME_BIAS)) {
			u8Bit = 1;
		}
		//出错
		else {
			u8Err = 1;
		}

		if (!u8Err) {
			//32位数据左移
			tIRFSM.u32Recieved32Bits <<= 1;

			//记录当前接收到的0/1值
			tIRFSM.u32Recieved32Bits |= u8Bit;

			//接收码型时长清0
			tIRFSM.u32CodeTime = 0;

			//接收数据长度--
			tIRFSM.u8BitsLength--;

			//已经接收到32位数据？
			if (!tIRFSM.u8BitsLength) {
				//获取地址、数据、数据反码
				u8IDData = ((tIRFSM.u32Recieved32Bits >> 24) & 0x000000FF);
				u8IDAntiData = ((tIRFSM.u32Recieved32Bits >> 16) & 0x000000FF);
				u8OperateData = ((tIRFSM.u32Recieved32Bits >> 8) & 0x000000FF);
				u8OperateAntiData = tIRFSM.u32Recieved32Bits & 0x000000FF;

				//地址、数据不正确
				if ((0xFF != u8IDData + u8IDAntiData) || (0xFF != u8OperateData + u8OperateAntiData)) {
					u8Err = 1;
				}
				//接收到正确的数据
				else {
					//红外接收缓冲区是否已满？
					if (tIRFSM.u8Available_Rx < _BUFFER_LENGTH_IR_Rx_) {
						//数据缓冲区数据增加
						tIRFSM.u8Available_Rx++;

						//将接收到的红外操作码放入数据缓冲区
						*(tIRFSM.pu8Buffer_Rx + tIRFSM.u8Write_Rx++) = u8OperateData;

						//记录接收到的操作码
						tIRFSM.u8RecievedCode = u8OperateData;

						//接收缓冲区写指针回头
						if (tIRFSM.u8Write_Rx >= _BUFFER_LENGTH_IR_Rx_) {
							tIRFSM.u8Write_Rx = 0;
						}
					}

					//状态转为接收间隔
					tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_INTERVAL;

					//在间隔后接收到的连发码是第一个连发码，第一个连发码要抛弃
					tIRFSM.fFirstContinue = 1;

					//码型时长清0
					tIRFSM.u32CodeTime = 0;
				}
			}
		}

		break;

		//接收数据结束，即数据完成后有一个下降沿脉冲
	case _IR_RECEIVEDSTATUS_INTERVAL:
		//IR接收状态转为接收连发码
		tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_CONTINUE;

		//接收时长清0
		tIRFSM.u32CodeTime = 0;

		//重置接收超时
		tIRFSM.u8ReceiveTimeout = _IR_RECEIVE_TIMEOUT_;

		break;

		//接收连发码
	case _IR_RECEIVEDSTATUS_CONTINUE:
		//连发码
		if ((tIRFSM.u32CodeTime >= _IR_CODE_TIME_SEQUENTIAL - _IR_CODE_TIME_BIAS)
				&& (tIRFSM.u32CodeTime <= _IR_CODE_TIME_SEQUENTIAL + _IR_CODE_TIME_BIAS)) {
			//是第一个连发码？
			if (tIRFSM.fFirstContinue == 1) {
				//抛弃第一个连发码

				//下面如果接收到连发码的话，则不是第一个连发码了
				tIRFSM.fFirstContinue = 0;
			}
			//不是第一个连发码,遥控器支持音量连发
			else if ((_IR_CMD_VOLUME_PLUS == tIRFSM.u8RecievedCode)
					|| (_IR_CMD_VOLUME_MINUS == tIRFSM.u8RecievedCode)) {
				//红外接收缓冲区是否已满？
				if (tIRFSM.u8Available_Rx < _BUFFER_LENGTH_IR_Rx_) {
					//接收缓冲区+1
					tIRFSM.u8Available_Rx++;

					//将上次接收到的操作码放入接收缓冲区
					*(tIRFSM.pu8Buffer_Rx + tIRFSM.u8Write_Rx++) = tIRFSM.u8RecievedCode;

					//接收缓冲区写指针回头
					if (tIRFSM.u8Write_Rx >= _BUFFER_LENGTH_IR_Rx_) {
						tIRFSM.u8Write_Rx = 0;
					}
				}
			}

			//转入接收连发码之间的间隔状态
			tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_INTERVAL;

			//接收时长清0
			tIRFSM.u32CodeTime = 0;

			//重置接收超时
			tIRFSM.u8ReceiveTimeout = _IR_RECEIVE_TIMEOUT_;
		}
		//接收出错
		else {
			u8Err = 1;
		}

		break;

	default:
		break;
	}

	//出错后重新开始
	if (u8Err) {
		//接收状态转为空闲
		tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_IDLE;

		//禁止接收码型时长计时
		tIRFSM.fCodeTimeEn = 0;

		//接收码型计时清0
		tIRFSM.u32CodeTime = 0;

		//接收数据清0
		tIRFSM.u32Recieved32Bits = 0;

		//接收数据长度置0
		tIRFSM.u8BitsLength = 0;

		//禁止接收超时
		tIRFSM.u8ReceiveTimeout = 0;

		//停止定时器
		TIM_Cmd(TIMIr, DISABLE);
	}
}

/***********************************************************************************************************
 * 功能描述：	10ms定时器函数，主要计数计时单位为10ms的定时变量
 * 入    口：	无
 * 出    口：	无
 * 设	计：
 ***********************************************************************************************************/
void Isr_IR_Timeout10ms(void) {
	//红外接收超时计时器
	if (tIRFSM.u8ReceiveTimeout) {
		tIRFSM.u8ReceiveTimeout--;

		//定时到
		if (0 == tIRFSM.u8ReceiveTimeout) {
			//停止接收，即停止定时器
			TIM_Cmd(TIMIr, DISABLE);

			//IR接收状态转为空闲
			tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_IDLE;

			//禁止码型时长计时
			tIRFSM.fCodeTimeEn = 0;
		}
	}
}

/***********************************************************************************************************
 * 功能描述：	10ous定时器函数，主要用来记录红外输入的脉冲宽度
 * 入    口：	无
 * 出    口：	无
 * 设	计：
 ***********************************************************************************************************/
void Isr_IR_Timeout100us(void) {
	//红外接收计时
	if (tIRFSM.fCodeTimeEn) {
		//码型时长+100us
		tIRFSM.u32CodeTime += 100;

		//接收超时
		if (((tIRFSM.u8ReceivedStatus <= _IR_RECEIVEDSTATUS_32BITSDATA)
				&& (tIRFSM.u32CodeTime >= _IR_DATA_LIMITATION_TIME))
				|| ((tIRFSM.u8ReceivedStatus == _IR_RECEIVEDSTATUS_INTERVAL)
						&& (tIRFSM.u32CodeTime >= _IR_INTERVAL_LIMITATION_TIME))) {
			//码型时长禁止
			tIRFSM.fCodeTimeEn = 0;

			//接收状态转为空闲
			tIRFSM.u8ReceivedStatus = _IR_RECEIVEDSTATUS_IDLE;

			//码型时长清0
			tIRFSM.u32CodeTime = 0;
		}
	}
}

