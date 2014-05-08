/*****************************************************************************
* Product: QEP/C
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Jul 30, 2013
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
#ifndef qep_pkg_h
#define qep_pkg_h

/**
* \file
* \ingroup qep
* \brief Internal (package scope) QEP/C interface.
*/

#include "qep_port.h"                                           /* QEP port */

extern QEvt const QEP_reservedEvt_[4];    /**< preallocated reserved events */

/** internal QEP constants */
enum QEPConst {
    QEP_EMPTY_SIG_ = 0,    /**< reserved empty signal for internal use only */

    /** maximum depth of state nesting (including the top level),
     * must be >= 3
     */
    QEP_MAX_NEST_DEPTH_ = 6
};

/** helper macro to trigger reserved event in an HSM */
#define QEP_TRIG_(state_, sig_) \
    ((*(state_))(me, &QEP_reservedEvt_[(sig_)]))

/** helper macro to trigger exit action in an HSM */
#define QEP_EXIT_(state_) do { \
    if (QEP_TRIG_((state_), Q_EXIT_SIG) == (QState)Q_RET_HANDLED) { \
        QS_BEGIN_(QS_QEP_STATE_EXIT, QS_priv_.smObjFilter, me) \
            QS_OBJ_(me); \
            QS_FUN_(state_); \
        QS_END_() \
    } \
} while (0)

/** helper macro to trigger entry action in an HSM */
#define QEP_ENTER_(state_) do { \
    if (QEP_TRIG_((state_), Q_ENTRY_SIG) == (QState)Q_RET_HANDLED) { \
        QS_BEGIN_(QS_QEP_STATE_ENTRY, QS_priv_.smObjFilter, me) \
            QS_OBJ_(me); \
            QS_FUN_(state_); \
        QS_END_() \
    } \
} while (0)

/** \brief Internal QEP macro to increment the given action table \a act_
*
* \note Incrementing a pointer violates the MISRA-C 2004 Rule 17.4(req),
* pointer arithmetic other than array indexing. Encapsulating this violation
* in a macro allows to selectively suppress this specific deviation.
*/
#define QEP_ACT_PTR_INC_(act_) (++(act_))

#endif                                                         /* qep_pkg_h */

/****************************************************************************/
#ifdef Q_SPY                                /* QS software tracing enabled? */
    #include "qs_port.h"                                 /* include QS port */
#else
    #include "qs_dummy.h"                /* disable the QS software tracing */
#endif                                                             /* Q_SPY */

