/*****************************************************************************
 * Product: DPP example
 * Last Updated for Version: 4.5.02
 * Date of the Last Update:  Jul 04, 2012
 *
 *                    Q u a n t u m     L e a P s
 *                    ---------------------------
 *                    innovating embedded systems
 *
 * Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
 *
 * This program is open source software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
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

#include "qp_port.h"
#include "qactiveobjs.h"
#include "bsp.h"
#include "mempool.h"
#include "parameter.h"
#include <acc.h>
#include <audio.h>
#include <beep.h>
#include <Can_Interface.h>
#include <can.h>
#include <digital_in.h>
#include <fram.h>
#include <gprs.h>
#include <gps.h>
#include <iccard.h>
#include <ir.h>
#include <lcd.h>
#include <nandflash.h>
#include <printer.h>
#include <sensor.h>
#include <speedpulse.h>
#include <tts.h>
#include <usb.h>
#include <video.h>
#include <qevents.h>

/*ģ��ʹ��*/
#define GPRS
#define GPS
#define CAN
#define EXTSIGNALS
#define LCD
#define SENSOR
#define VIDEO
#define UPGRADE
#define TTS
#define ACCDET
#define SYSTICK
#define RECORDER
#define ICCARD
#define PRINTER
#define IR
#define DIGITAL
#define USB
#define AUDIO

/**
 * AO���ȼ������ȼ���ֵԽ��״̬�����ȼ�Խ�ߣ�ע��AO���ȼ�����>0
 */
enum {
	AO_PRIO_GPRS = 1,
	AO_PRIO_GPS,
	AO_PRIO_CAN,
	AO_PRIO_LCD,
	AO_PRIO_SENSOR,
	AO_PRIO_VIDEO,
	AO_PRIO_UPGRADE,
	AO_PRIO_TTS,
	AO_PRIO_ACCDET,
	AO_PRIO_SYSTICK,
	AO_PRIO_RECORDER,
	AO_PRIO_IC,
	AO_PRIO_IR,
	AO_PRIO_DIGITAL,
	AO_PRIO_USB,
	AO_PRIO_AUDIO,
	AO_PRIO_PRINTER,

};

/* Local-scope objects -----------------------------------------------------*/

static QEvt const *l_ACCQueueSto[20];
static QEvt const *l_GPRSQueueSto[20];
static QEvt const *l_GPSQueueSto[20];
static QEvt const *l_ExtSignalQueueSto[20];
static QEvt const *l_CANQueueSto[20];
static QEvt const *l_LCDQueueSto[20];
static QEvt const *l_SENSORQueueSto[20];
static QEvt const *l_VIDEOQueueSto[20];
static QEvt const *l_UPGRADEQueueSto[20];
static QEvt const *l_TTSQueueSto[20];
static QEvt const *l_ACCQueueSto[20];
static QEvt const *l_SYSTICKQueueSto[20];
static QEvt const *l_RECORDQueueSto[20];
static QEvt const *l_ICCARDQueueSto[20];
static QEvt const *l_PRINTERQueueSto[20];
static QEvt const *l_IRQueueSto[20];
static QEvt const *l_DIGITALQueueSto[20];
static QEvt const *l_USBQueueSto[20];
static QEvt const *l_AUDIOQueueSto[20];

static QSubscrList l_subscrSto[MAX_PUB_SIG];

static union SmallEvents {
	void *e0; /* minimum event size */
	uint8_t e1[sizeof(QEvt)];
/* ... other event types to go into this pool */
} l_smlPoolSto[200]; /* storage for the small event pool */

static union MediumEvents {
	void *e0; /* minimum event size */
//    uint8_t e1[sizeof(ServiceReqEvt)];
//    uint8_t e2[sizeof(CANMsgEvt)];
//    uint8_t e3[sizeof(SessionDoneEvt)];
//    uint8_t e4[sizeof(DiagServiceResponseEvt)];
//    uint8_t e5[sizeof(DiagnoseReqEvt)];
	uint8_t e6[sizeof(TaskEvt)];
/* ... other event types to go into this pool */
} l_medPoolSto[200]; /* storage for the medium event pool */

#if 0
static union LargeEvents {
	void *e0; /* minimum event size */
	uint8_t e1[sizeof(TaskEvt)];

}l_largePoolSto[20]; /* storage for the medium event pool */
#endif

#if 1
/*..........................................................................*/
int main(void) {

	/**��ʼ��BSP*/
	BSP_init();

//	/**����ϵͳ��������ʧ��ϵͳֹͣ����*/
//	if(!Load_Parameters())
//	{
//		//ϵͳ������ʼ��ʧ�ܣ�������ֹ
//		return -1;
//	}

	/**��ʼ��QP���*/
	QF_init();

	/** ��ʼ���¼��أ�����С�ߴ��¼��غ��гߴ��¼��أ�δ�ô�ߴ��¼��أ���Ϊ��û���ֽ����ر����¼� */
	QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));
	QF_poolInit(l_medPoolSto, sizeof(l_medPoolSto), sizeof(l_medPoolSto[0]));
//	QF_poolInit(l_largePoolSto, sizeof(l_largePoolSto), sizeof(l_largePoolSto[0]));

	/**��ʼ��publish-subscribe����*/
	QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));

	/**�����ڴ�أ�Ԥ����һ���顣ֻ�ܵ���һ��*/
	if (MP_Create() < 0) {
		/**�ڴ�ش���ʧ�ܣ�������ֹ*/
		return -1;
	}

#ifdef GPRS
	/**GPRS״̬����ʼ��*/
	QGprs_ctor();

	/**����GPRS״̬��*/
	QActive_start(AO_Gprs, AO_PRIO_GPRS, l_GPRSQueueSto, Q_DIM(l_GPRSQueueSto),
			(void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef GPS
	/**GPS״̬����ʼ��*/
	QGps_ctor();

	/**����״̬��*/
	QActive_start(AO_Gps, AO_PRIO_GPS, l_GPSQueueSto, Q_DIM(l_GPSQueueSto),
			(void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef ACCDET
	/**ACC���״̬����ʼ��*/
	QACCDetector_ctor();

	/**����ACC���״̬��*/
	QActive_start(AO_ACCDetector, AO_PRIO_ACCDET, l_ACCQueueSto,
			Q_DIM(l_ACCQueueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef CAN
	/**CAN״̬����ʼ��*/
	QCAN_ctor();

	/**����CAN״̬��*/
	QActive_start(AO_Can, AO_PRIO_CAN, l_CANQueueSto, Q_DIM(l_CANQueueSto),
			(void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef LCD
	/**LCD��״̬����ʼ�� */
	QLCD_ctor();

	/**����״̬��*/
	QActive_start(AO_LCD, AO_PRIO_LCD, l_LCDQueueSto, Q_DIM(l_LCDQueueSto),
			(void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef SENSOR
	/**LCD��״̬����ʼ�� */
	QSensors_ctor();

	/**����״̬��*/
	QActive_start(AO_Sensors, AO_PRIO_SENSOR, l_SENSORQueueSto,
			Q_DIM(l_SENSORQueueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef VIDEO
	/**����ͷ״̬����ʼ�� */
	QVideo_ctor();

	/**����״̬��*/
	QActive_start(AO_Video, AO_PRIO_VIDEO, l_VIDEOQueueSto,
			Q_DIM(l_VIDEOQueueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef TTS
	/**TTS״̬����ʼ�� */
	QTTS_ctor();

	/**����״̬��*/
	QActive_start(AO_TTS, AO_PRIO_TTS, l_TTSQueueSto, Q_DIM(l_TTSQueueSto),
			(void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef RECORDER
	/**TTS״̬����ʼ�� */
	QRecorder_ctor();

	/**����״̬��*/
	QActive_start(AO_Recorder, AO_PRIO_RECORDER, l_RECORDQueueSto,
			Q_DIM(l_RECORDQueueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef ICCARD
	/**IC��״̬����ʼ�� */
	QICcard_ctor();

	/**����״̬��*/
	QActive_start(AO_ICCard, AO_PRIO_IC, l_ICCARDQueueSto,
			Q_DIM(l_ICCARDQueueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef PRINTER
	/**IC��״̬����ʼ�� */
	QPrinter_ctor();

	/**����״̬��*/
	QActive_start(AO_Printer, AO_PRIO_PRINTER, l_PRINTERQueueSto,
			Q_DIM(l_PRINTERQueueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef IR
	/**IC��״̬����ʼ�� */
	QIR_ctor();

	/**����״̬��*/
	QActive_start(AO_IR, AO_PRIO_IR, l_IRQueueSto, Q_DIM(l_IRQueueSto),
			(void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef USB
	/**IC��״̬����ʼ�� */
	QUSB_ctor();

	/**����״̬��*/
	QActive_start(AO_USB, AO_PRIO_USB, l_USBQueueSto, Q_DIM(l_USBQueueSto),
			(void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef AUDIO
	/**IC��״̬����ʼ�� */
	QAudio_ctor();

	/**����״̬��*/
	QActive_start(AO_Audio, AO_PRIO_AUDIO, l_AUDIOQueueSto,
			Q_DIM(l_AUDIOQueueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

#ifdef UPGRADE
	/**SD������״̬����ʼ��*/
	QUpgrade_ctor();

	/**��������״̬��*/
	QActive_start(AO_Upgrade, AO_PRIO_UPGRADE, l_UPGRADEQueueSto,
			Q_DIM(l_UPGRADEQueueSto), (void *) 0, 0U, (QEvt *) 0);
#endif

	/**ϵͳʱ��״̬����ʼ��*/
	QSystick_ctor();

	/**����ϵͳʱ��״̬��*/
	QActive_start(AO_Systick, AO_PRIO_SYSTICK, l_SYSTICKQueueSto,
			Q_DIM(l_SYSTICKQueueSto), (void *) 0, 0U, (QEvt *) 0);

	/**����ϵͳ��������ʧ��ϵͳֹͣ����*/
//	if (!Load_Parameters()) {
//		/**ϵͳ������ʼ��ʧ�ܣ�������ֹ*/
//		return -1;
//	}

	/**Ƕ���ж�������ʼ��*/
	NVIC_Configuration();

	/**����QP��ܣ�����ʼ����*/
	return QF_run();
}
#endif

