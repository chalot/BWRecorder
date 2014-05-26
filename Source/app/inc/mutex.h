/** @file mutex.h
 * @brief brief description
 * @author <gao zhengdong, zhengdgao@163.com>
 * @date 2014年4月9日上午8:51:22
 * @version V1.0
 * @note
 * detailed description
 */
#ifndef MUTEX_H_
#define MUTEX_H_

#include "qp_port.h"

#if 0
#define PRIO_CEILING	2

/*互斥访问控制，利用QP框架的QMutex机制实现*/
#define DECLARE_MUTEX(mux)	QMutex mux;
#define MUTEX_LOCK(mux) 	{ mux = QK_mutexLock(PRIO_CEILING); }
#define MUTEX_UNLOCK(mux)	{ QK_mutexUnlock(mux); }
#endif

#define DECLARE_MUTEX(mux)
#define MUTEX_LOCK(mux) 	{ }
#define MUTEX_UNLOCK(mux)	{ }

#endif /* MUTEX_H_ */
