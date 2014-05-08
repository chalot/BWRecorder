/**
 * @file 	stm32_hardfault_handler.S
 * @brief   This file contains code to get the data in registers when a hard
 *			fault happens and to call its C counterpart (in stm32f2xx_it.c)
 *			to print it out.  This is very useful for debugging.  Blatantly
 *			stolen from the Joseph Yiu book on Arm Cortex M3 processors.  See
 *			http://blog.frankvh.com/2011/12/07/cortex-m3-m4-hard-fault-handler/
 *
 * @date   	09/27/2012
 * @author 	Harry Rostovtsev (Actually, it's Joseph Yiu.  See link).
 * @email  	harry_rostovtsev@datacard.com
 * Copyright (C) 2012 Datacard. All rights reserved.
 */

.syntax unified
.cpu cortex-m3
.thumb

.global HardFault_Handler
.extern hard_fault_handler_c

HardFault_Handler:
  TST LR, #4
  ITE EQ
  MRSEQ R0, MSP
  MRSNE R0, PSP
  B hard_fault_handler_c

/*********** Copyright (C) 2012 Datacard. All rights reserved *****END OF FILE****/
