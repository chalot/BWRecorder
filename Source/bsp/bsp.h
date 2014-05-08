/*****************************************************************************
 * Product: DPP with networking example, Board Support Package for STM3220G-eval
 * with Quantum Leaps (www.state-machine.com) kernel.
 * Copyright Datacard Corporation 1999-2012. All rights reserved.
 * Author: Harry Rostovtsev
 * Email:  harry_rostovtsev at datacard.com, rost0031 at gmail.com
 ****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
// Product: DPP with networking example, Board Support Package
// Last Updated for Version: 4.0.01
// Date of the Last Update:  Jul 29, 2008
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
//
// This software may be distributed and modified under the terms of the GNU
// General Public License version 2 (GPL) as published by the Free Software
// Foundation and appearing in the file GPL.TXT included in the packaging of
// this file. Please note that GPL Section 2[b] requires that all works based
// on this software must also be made publicly available under the terms of
// the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GPL and are specifically designed for licensees interested in
// retaining the proprietary status of their code.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// e-mail:                  info@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#ifndef bsp_h
#define bsp_h

#include <stm32f2xx.h>
#include <qp_port.h>
                                                // System clock tick rate [Hz]
#define BSP_TICKS_PER_SEC 100



#ifdef Q_SPY
//�����ض�Ӧ�õĵ��Լ�¼
enum AppRecords {                 /* application-specific trace records */
	SESSION_STAT = QS_USER,
	SESSIONMGR_STAT,
	GPRS_STAT,
	CAN_STAT,
	SYSTEM_STAT,

};
#endif

//�����豸���ں�
//#define	UART_GPS	USART3	//GPS����
//#define	UART_GPRS	USART2	//GPRS����
//#define	UART_UPG	USART1	//��������


/*ģ������̬��־*/
#define MODULE_SLEEP_STATE_GPS	0x01
#define MODULE_SLEEP_STATE_CAN	0x02
#define MODULE_SLEEP_STATE_GPRS	0x04

void BSP_init(void);
//void BSP_ButtonAndLED_Init(void);		        // Initialize LEDs on STM3220G
void BSP_USART_Init(void);                 // Initialize the USART on STM3220G
void BSP_EXTI_Init(void);
void BSP_Display_Init(void);		                 // initialize the display
void BSP_busyDelay(void);             // to artificially extend RTC processing
//void BSP_displyPhilStat(uint8_t n, char const *stat);
void BSP_Trace(u8 eState, QActive* pAO, char* const str);

/**
 * ����/��������
 */
void BSP_Vechical_Lock();
void BSP_Vechical_UnLock();

#define ACC_ON		1	//ACC�ϵ�
#define ACC_OFF		0	//ACC�ص�

///����/��ȡACC״̬
u8 GetACCState();
void SetACCState(u8 state);

///ģ������̬��־
#define MODULE_SLEEP_STATE_GPS	0x01
#define MODULE_SLEEP_STATE_CAN	0x02
#define MODULE_SLEEP_STATE_GPRS	0x04

///����ģ������̬
void SetSysSleepState(u8 state);

///���ģ������̬
void ClearSysSleepState(u8 state);


/**��ȡϵͳ��ǰTICK*/
u32 GetCurTicks();

/**��ǰʱ������*/
void ResetCurTicks();



#endif                                                                // bsp_h


