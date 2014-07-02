/*****************************************************************************
* Model: bwgpsrecorder.qm
* File:  qmcode/qspeedpulse.c
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/* @(/3/20) ................................................................*/
#include "qp_port.h"
#include "qevents.h"
#include "bsp.h"
#include "utility.h"
#include "error.h"
#include "type.h"

Q_DEFINE_THIS_MODULE("qspeedpulse.c")


/* Active object class -----------------------------------------------------*/
/* @(/1/19) ................................................................*/
typedef struct QSpeedPulseTag {
/* protected: */
    QActive super;
} QSpeedPulse;

/* protected: */
static QState QSpeedPulse_initial(QSpeedPulse * const me, QEvt const * const e);
static QState QSpeedPulse_state1(QSpeedPulse * const me, QEvt const * const e);



/* Local objects -----------------------------------------------------------*/
static QSpeedPulse l_QSpeedPulse; /* the single instance of the Table active object */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_SpeedPulse = &l_QSpeedPulse.super; /* "opaque" AO pointer */

/*..........................................................................*/
/* @(/1/41) ................................................................*/
void QSpeedPulse_ctor(void) {
    QSpeedPulse *me = &l_QSpeedPulse;
    QActive_ctor(&me->super, Q_STATE_CAST(&QSpeedPulse_initial));
}
/* @(/1/19) ................................................................*/
/* @(/1/19/0) ..............................................................*/
/* @(/1/19/0/0) */
static QState QSpeedPulse_initial(QSpeedPulse * const me, QEvt const * const e) {
    return Q_TRAN(&QSpeedPulse_state1);
}
/* @(/1/19/0/1) ............................................................*/
static QState QSpeedPulse_state1(QSpeedPulse * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

