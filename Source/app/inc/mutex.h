/** @file mutex.h
 * @brief brief description
 * @author <gao zhengdong, zhengdgao@163.com>
 * @date 2014��4��9������8:51:22
 * @version V1.0
 * @note
 * detailed description
 */
#ifndef MUTEX_H_
#define MUTEX_H_

#include "qp_port.h"

#if 0
#define PRIO_CEILING	2

/*������ʿ��ƣ�����QP��ܵ�QMutex����ʵ��*/
#define DECLARE_MUTEX(mux)	QMutex mux;
#define MUTEX_LOCK(mux) 	{ mux = QK_mutexLock(PRIO_CEILING); }
#define MUTEX_UNLOCK(mux)	{ QK_mutexUnlock(mux); }
#endif

#define DECLARE_MUTEX(mux)
#define MUTEX_LOCK(mux) 	{ }
#define MUTEX_UNLOCK(mux)	{ }

#endif /* MUTEX_H_ */
