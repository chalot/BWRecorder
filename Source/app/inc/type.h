/*
 * type.h
 *
 *  Created on: 2013-5-15
 *      Author: gaozhengdong
 */

#ifndef TYPE_H_
#define TYPE_H_

#include <stm32f2xx.h>

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef signed long long  s64;
typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;

typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

typedef unsigned long long u64;
typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */


typedef unsigned long  const cu32;  /* Read Only */
typedef unsigned short const cu16;  /* Read Only */
typedef unsigned char  const cu8;   /* Read Only */


typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

//#define TRUE 1
//#define FALSE 0

//typedef enum {FALSE = 0, TRUE = 1} BOOL;

//typedef enum {FALSE = 0, TRUE = !FALSE} BOOL;
typedef enum {CLOSE = 0, OPEN = !CLOSE}CONTROL;

#ifndef NULL
#define  NULL	(void*)0
#endif

#endif /* TYPE_H_ */
