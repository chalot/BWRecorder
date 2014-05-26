/*****************************************************************************
 * DPP with networking example for STM3220G-eval
 * with Quantum Leaps (www.state-machine.com) kernel.
 * Copyright Datacard Corporation 1999-2012. All rights reserved.
 * Author: Harry Rostovtsev
 * Email:  harry_rostovtsev at datacard.com, rost0031 at gmail.com
 ****************************************************************************/
/*****************************************************************************
* Last Updated for Version: 4.3.00
* Date of the Last Update:  Nov 01, 2011
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2011 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "stm32f2xx_conf.h"
#include "qp_port.h"
#include "bsp.h"
#include "qs.h"
#include <qevt.h>
#include <stm32_eval.h>
#include <trace.h>
#include <type.h>

Q_DEFINE_THIS_FILE

enum KernelUnawareISRs { /* see NOTE00 */
/* ... */
	MAX_KERNEL_UNAWARE_CMSIS_PRI /* keep always last */
};

/* "kernel-unaware" interrupts can't overlap "kernel-aware" interrupts */
Q_ASSERT_COMPILE(MAX_KERNEL_UNAWARE_CMSIS_PRI <= QF_AWARE_ISR_CMSIS_PRI);

enum KernelAwareISRs {
	GPIOPORTA_PRI = QF_AWARE_ISR_CMSIS_PRI, /* see NOTE00 */
	SYSTICK_PRIO,
	EXTI0_PRIO,

	/* ... */
	MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};

/* "kernel-aware" interrupts should not overlap the PendSV priority */
Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF>>(8-__NVIC_PRIO_BITS)));


static uint32_t l_delay = 0UL; /* limit for the loop counter in busyDelay() */
static uint32_t l_nTicks;

#ifdef Q_SPY
    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;
    static uint8_t l_SysTick_Handler;

    #define QS_BUF_SIZE   (1*1024)
    #define QS_BAUD_RATE  115200

//    enum AppRecords {                 /* application-specific trace records */
//        PHILO_STAT = QS_USER
//    };
#endif

static u8 g_bReadyForSleepMode = 0; //是否准备好进入休眠模式
static u32 s_Ticks = 0; //当前时刻数

/**
 * 获取系统当前TICK数
 *
 * @return
 */
u32 GetCurTicks()
{
	u32 ticks = 0;

	QF_INT_DISABLE();
	ticks = s_Ticks;
	QF_INT_ENABLE();

	return ticks;
}

/**
 * 当前时刻清零
 */
void ResetCurTicks()
{
	QF_INT_DISABLE();
	s_Ticks = 0;
	QF_INT_ENABLE();
}

/** 休眠控制 ******************************************************************************/
static u8 SysSleepState	= 0;  //系统休眠状态

//设置模块休眠态
void SetSysSleepState(u8 state)
{
	SysSleepState |= state;
}

//清除模块休眠态
void ClearSysSleepState(u8 state)
{
	SysSleepState &= ~state;
}

//检测系统是否满足休眠条件，必须所有模块都进入休眠态
BOOL IsSystemReadyForSleep()
{
	if ((SysSleepState & MODULE_SLEEP_STATE_GPS) &&
		(SysSleepState & MODULE_SLEEP_STATE_CAN) &&
		(SysSleepState & MODULE_SLEEP_STATE_GPRS) )
		{
			return TRUE;
		}

	return FALSE;
}

static volatile u32 ticks = 0;

/*..........................................................................*/
void SysTick_Handler(void) __attribute__((__interrupt__));
void SysTick_Handler(void) {
    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */
#ifdef Q_SPY
    uint32_t dummy = SysTick->CTRL;        /* clear NVIC_ST_CTRL_COUNT flag */
    QS_tickTime_ += QS_tickPeriod_;       /* account for the clock rollover */
#endif

    QF_TICK(&l_SysTick_Handler);           /* process all armed time events */

    if(ticks++ % 100 == 0)
    {
    	TRACE_(QS_USER, NULL, "cur tick = %d", ticks);
    }
    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}

/*..........................................................................*/
void BSP_init(void) {
	SystemInit();         /* initialize STM32 system (clock, PLL and Flash) */

#ifdef Q_SPY
	//初始化QS
    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
#endif

    RCC_APBClockEnable();


	BSP_USART_Init(); ///使能调试串口，开发板上是USART3

	Periph_Lowlevel_Init();	///所有外设初始化


	EXTI_Configuration();

	NVIC_Configuration();

}

/*..........................................................................*/
void BSP_USART_Init(void) {

	/* USART resources configuration (Clock, GPIO pins and USART registers) ----*/
	/* USART configured as follow:
	    	  - BaudRate = 115200 baud
	    	  - Word Length = 8 Bits
	    	  - One Stop Bit
	    	  - No parity
	    	  - Hardware flow control disabled (RTS and CTS signals)
	    	  - Receive and transmit enabled
	 */
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	STM_EVAL_COMInit(COM1, &USART_InitStructure);
}
/*..........................................................................*/
void BSP_EXTI_Init(void) {
	EXTI_InitTypeDef exti_init;

	/* initialize the EXTI Line0 interrupt used for testing */
//	exti_init.EXTI_Mode    = EXTI_Mode_Interrupt;
//	exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
//	exti_init.EXTI_Line    = EXTI_Line0;
//	exti_init.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&exti_init);
}

/*..........................................................................*/
void QF_onStartup(void) {
    NVIC_InitTypeDef nvic_init, nvic_eth_init;

    /* assing all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

    /* Set up and enable the SysTick timer.  It will be used as a reference
    * for delay loops in the interrupt handlers.  The SysTick timer period
    * will be set up for BSP_TICKS_PER_SEC.
    */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

//    /* Enable the EXTI0 Interrupt used for testing preemptions */
//    nvic_init.NVIC_IRQChannel                   = EXTI0_IRQn;
//    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
//    nvic_init.NVIC_IRQChannelSubPriority        = 0;
//    nvic_init.NVIC_IRQChannelCmd                = ENABLE;
//    NVIC_Init(&nvic_init);/* enables the device and sets interrupt priority */
//
//    nvic_eth_init.NVIC_IRQChannel                   = ETH_IRQn;
//    nvic_eth_init.NVIC_IRQChannelPreemptionPriority = 2;
//    nvic_eth_init.NVIC_IRQChannelSubPriority        = 0;
//    nvic_eth_init.NVIC_IRQChannelCmd                = ENABLE;
//    NVIC_Init(&nvic_eth_init);/* enables the device and sets interrupt priority */
//

    /* set priorities of all interrupts in the system... */
    NVIC_SetPriority(SysTick_IRQn, 	SYSTICK_PRIO);
    NVIC_SetPriority(EXTI0_IRQn,   	EXTI0_PRIO);
//    NVIC_SetPriority(ETH_IRQn,   	ETH_PRIO);
    /* ... */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_onIdle(void) {
                         /* toggle the blue LED on and then off, see NOTE01 */
    QF_INT_DISABLE();
//    STM_EVAL_LEDOn (LED4);                                  /* blue LED on  */
//    STM_EVAL_LEDOff(LED4);                                  /* blue LED off */
    QF_INT_ENABLE();

#ifdef Q_SPY

    if ((USART3->SR & USART_FLAG_TXE) != 0) {              /* is TXE empty? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {                              /* not End-Of-Data? */
           USART3->DR = (b & 0xFF);             /* put into the DR register */
        }
    }

#elif defined NDEBUG
    __WFI();                                          /* wait for interrupt */
#endif
}
/*..........................................................................*/
/* error routine that is called if the STM32 library encounters an error    */
void assert_failed(char const *file, int line) {
    Q_onAssert(file, line);
}
/*..........................................................................*/
void Q_onAssert(char const * const file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();         /* make sure that all interrupts are disabled */
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
}

/*..........................................................................*/
void BSP_busyDelay(void) {
    uint32_t volatile i = l_delay;
    while (i-- > 0UL) {                                   /* busy-wait loop */
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE];            /* buffer for Quantum Spy */
    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* USART3 and GPIOA/AFIO clocks are already enabled by this point */
    QS_tickPeriod_ = (QSTimeCtr)(SystemCoreClock / BSP_TICKS_PER_SEC);
    QS_tickTime_ = QS_tickPeriod_;        /* to start the timestamp at zero */

                                                 /* setup the QS filters... */
    QS_FILTER_ON(QS_ALL_RECORDS);

 //    QS_FILTER_OFF(QS_QEP_STATE_EMPTY);
     QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
     QS_FILTER_OFF(QS_QEP_STATE_EXIT);
     QS_FILTER_OFF(QS_QEP_STATE_INIT);
     QS_FILTER_OFF(QS_QEP_INIT_TRAN);
     QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
     QS_FILTER_OFF(QS_QEP_TRAN);
 //    QS_FILTER_OFF(QS_QEP_IGNORED);
     QS_FILTER_OFF(QS_QEP_DISPATCH);

     QS_FILTER_OFF(QS_QF_ACTIVE_ADD);
     QS_FILTER_OFF(QS_QF_ACTIVE_REMOVE);
     QS_FILTER_OFF(QS_QF_ACTIVE_SUBSCRIBE);
     QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
     QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
     QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
     QS_FILTER_OFF(QS_QF_ACTIVE_GET);
     QS_FILTER_OFF(QS_QF_ACTIVE_GET_LAST);
     QS_FILTER_OFF(QS_QF_EQUEUE_INIT);
     QS_FILTER_OFF(QS_QF_EQUEUE_POST_FIFO);
     QS_FILTER_OFF(QS_QF_EQUEUE_POST_LIFO);
     QS_FILTER_OFF(QS_QF_EQUEUE_GET);
     QS_FILTER_OFF(QS_QF_EQUEUE_GET_LAST);
     QS_FILTER_OFF(QS_QF_MPOOL_INIT);
     QS_FILTER_OFF(QS_QF_MPOOL_GET);
     QS_FILTER_OFF(QS_QF_MPOOL_PUT);
     QS_FILTER_OFF(QS_QF_PUBLISH);
     QS_FILTER_OFF(QS_QF_NEW);
     QS_FILTER_OFF(QS_QF_GC_ATTEMPT);
     QS_FILTER_OFF(QS_QF_GC);
     QS_FILTER_OFF(QS_QF_TICK);
     QS_FILTER_OFF(QS_QF_TIMEEVT_ARM);
     QS_FILTER_OFF(QS_QF_TIMEEVT_AUTO_DISARM);
     QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM_ATTEMPT);
     QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM);
     QS_FILTER_OFF(QS_QF_TIMEEVT_REARM);
     QS_FILTER_OFF(QS_QF_TIMEEVT_POST);
     QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
     QS_FILTER_OFF(QS_QF_CRIT_EXIT);
     QS_FILTER_OFF(QS_QF_ISR_ENTRY);
     QS_FILTER_OFF(QS_QF_ISR_EXIT);

     QS_FILTER_OFF(QS_QK_MUTEX_LOCK);
     QS_FILTER_OFF(QS_QK_MUTEX_UNLOCK);
     QS_FILTER_OFF(QS_QK_SCHEDULE);

    return (uint8_t)1;                                    /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {            /* invoked with interrupts locked */
    if ((SysTick->CTRL & 0x00010000) == 0) {               /* COUNT no set? */
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else {     /* the rollover occured, but the SysTick_ISR did not run yet */
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) {      /* while not End-Of-Data... */
        while ((USART3->SR & USART_FLAG_TXE) == 0) { /* while TXE not empty */
        }
        USART3->DR = (b & 0xFF);                /* put into the DR register */
    }
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*..........................................................................*/
/* sys_now() is used in the lwIP stack
*/
uint32_t sys_now(void) {
    return l_nTicks * (1000 / BSP_TICKS_PER_SEC);
}

/*AO内部打印调试信息*/
void BSP_Trace(u8 eState, QActive* pAO, char* const str)
{
	QS_BEGIN(eState, pAO)  /* application-specific record begin */
		QS_STR(str);    /* Philosopher status */
	QS_END()
}

/*****************************************************************************/
/**
 * 车辆控制-锁车
 */
void BSP_Vechical_Lock()
{

}

/**
 * 车辆控制-解锁
 */
void BSP_Vechical_UnLock()
{

}


static u8 acc_state = 0;

/**
 * @Func 	取ACC状态
 * @Param	无
 * @Ret		0 未上电 1 上电
 */
u8 GetACCState()
{
	return acc_state;
}

/**
 * @Func 	设置ACC状态
 * @Param	state 		[IN]	0 未上电 1 上电
 * @Ret		无
 */
void SetACCState(u8 state)
{
	acc_state = state;
}

