/*****************************************************************************
* Product: QP/C
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Aug 10, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qp_port_h
#define qp_port_h

#include "qf_port.h"                   /* QF/C port from the port directory */
#include "qassert.h"                                       /* QP assertions */

#ifdef Q_SPY                                   /* software tracing enabled? */
    #include "qs_port.h"               /* QS/C port from the port directory */
#else
    #include "qs_dummy.h"                /* QS/C dummy (inactive) interface */
#endif

/**
* \file
* \ingroup qep qf qk qs
* \brief QP/C public interface.
*
* This header file must be included directly or indirectly
* in all application modules (*.c files) that use QP/C.
*/

              /* device driver signal offset at the top of the signal range */
#if (Q_SIGNAL_SIZE == 1)
    #define Q_DEV_DRIVER_SIG  ((QSignal)(0xFFU - 8U))
#elif (Q_SIGNAL_SIZE == 2)
    #define Q_DEV_DRIVER_SIG  ((QSignal)(0xFFFFU - 32U))
#elif (Q_SIGNAL_SIZE == 4)
    #define Q_DEV_DRIVER_SIG  ((QSignal)(0xFFFFFFFFU - 256U))
#else
    #error "Q_SIGNAL_SIZE not defined or incorrect"
#endif

#endif                                                         /* qp_port_h */
