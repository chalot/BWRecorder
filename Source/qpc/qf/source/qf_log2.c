/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.1.0
* Date of the Last Update:  Oct 07, 2013
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
#include "qf_pkg.h"

/**
* \file
* \ingroup qf
* \brief QF_log2Lkup[] definition.
*/

#ifdef QF_LOG2LKUP

/* Global objects ----------------------------------------------------------*/
uint8_t const Q_ROM Q_ROM_VAR QF_log2Lkup[256] = {
    (uint8_t)0, (uint8_t)1, (uint8_t)2, (uint8_t)2,
    (uint8_t)3, (uint8_t)3, (uint8_t)3, (uint8_t)3,
    (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4,
    (uint8_t)4, (uint8_t)4, (uint8_t)4, (uint8_t)4,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)5, (uint8_t)5, (uint8_t)5, (uint8_t)5,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)6, (uint8_t)6, (uint8_t)6, (uint8_t)6,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)7, (uint8_t)7, (uint8_t)7, (uint8_t)7,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8,
    (uint8_t)8, (uint8_t)8, (uint8_t)8, (uint8_t)8
};

#endif                                                /* #ifdef QF_LOG2LKUP */

