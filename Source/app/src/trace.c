/**   
* @Title: trace.c  
* @Description: 通过串口打印调试信息
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-4 上午11:36:47 
* @version V1.0   
*/ 

#include "trace.h"
#include <stm32f2xx_conf.h>
#include "bsp.h"
#include "utility.h"

/*

//输出单个字符
#define PUTCHAR(c, p) 	{ 	\
							do {	\
								*p = c;	\
							} while(0);	\
						}

//输出字符串
#define PUTSTRING(s, p) {	\
							do {	\
								memcpy(p, (u8*)&s[0], strlen_(s);	\
//								p+= strlen_(s);	\
							} while(0);	\
						}
*/



void PUTCHAR(char c, char *p)
{
	*p = c;
}

void PUTSTRING(char *s, char* p)
{
	memcpy_(p, (u8*)&s[0], strlen_(s));
//	p+= strlen_(s);
}

/*可变长参数相关宏定义，摘自stdarg.h*/
typedef char* va_list;

#define _INTSIZEOF(n) \
						((sizeof(n)+sizeof(int)-1)&~(sizeof(int) - 1) )

#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )

#define va_arg(ap,t) \
						( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )

#define va_end(ap) ( ap = (va_list)0 )

#define PRECISION 6
#define PRE_CORRECT 0.0000001

int ineg_to_pos(int n, char *p)
{
	if (n >= 0)
		return n;
	else
		PUTCHAR('-', p);
	return (-n);
}

double dneg_to_pos(double n, char *p)
{
	if (n >= 0)
		return n;
	else
		PUTCHAR('-', p);
	return (-n);
}

char itoc(unsigned int i)
{
	if (i < 10)
		return (char)(i + 48);
	else
		return (char)(i + 87);
}

void putstr_reverse(char n[], int i, char *p)
{
	for (i--; i >= 0; i--)
	{
		PUTCHAR(n[i], p);
		p++;
	}
}

char *frac_to_str(char n[], double f)
{
	int i = 0, j, ch;
	f += PRE_CORRECT;    /* 修正精度 */
	n[i++] = '.';
	for(j = 0; j < PRECISION; j++)
	{
		if (f)
		{
			ch = (int)(f * 10);
			n[i++] = itoc(ch);
			f = f * 10 - ch;
		}
		else
			n[i++] = '0';
	}
	n[i] = '\0';
	return n;
}

/* 处理无符号数，根据sys将其转换成相应进制的字符数组再输出 */
u8 put_dox(unsigned int num, int sys, char *p)
{
	u8 i = 0, j = 0;
	char n[20];

	if (num == 0)
	{
		PUTCHAR('0', p);
		return 1;
	}
	else
	{
		while (num) {
			j = num % sys;
			n[i++] = itoc(j);
			num /= sys;
		}
//		putstr_reverse(n, i-1, p);
//		return i-1;
		putstr_reverse(n, i, p);
		return i;
	}
}

#if 0
/* 处理浮点型数 */
void put_f(double num, char *p)
{
	char n[20];
	int in = num;           /* in代表浮点数的整数部分 */
	double fn = num - in;   /* fn代表浮点数的小数部分 */

	/* 处理整数部分 */
	put_dox(in, 10, p);
	/* 处理小数部分 */
	PUTSTRING(n);
}
#endif

/**
 * 格式化字符串
 *
 * @param pBuf
 * @param format
 */
void sprintf_(u8 *pBuf, const char *format, ...)
{
	if(NULL == pBuf)
		return;

	va_list ap;
	char c;
	char *pMsg;
	BOOL bTag = FALSE;

	pMsg = (char*)pBuf;

	va_start(ap, format);
	c = *format;

	while (c != '\0') {
//	while(c != '\0') {
		switch (c) {
			case '%': {
				if (*format == '%') {
					bTag = TRUE;
				}
				break;
			}
			case 'c': {
				/* char is promoted to int when passed through '...' */
				if(bTag) {
					char ch = va_arg(ap, int);
					PUTCHAR(ch, pMsg);
					pMsg++;
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 's': {
				if(bTag) {
					char *s = va_arg(ap, char *);
					PUTSTRING(s, pMsg);
					pMsg += strlen_(s);
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 'd': {
				if(bTag) {
					int n = va_arg(ap, int);
					int m = ineg_to_pos(n, pMsg);
					u8 len;
					if(n < 0)
						pMsg++;
					len = put_dox(m, 10, pMsg);
					pMsg += len;
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 'o': {
				if(bTag) {
					unsigned int n = va_arg(ap, int);
					u8 len = put_dox(n, 8, pMsg);
					pMsg += len;
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 'x': {
				if(bTag) {
					unsigned int n = va_arg(ap, int);
					u8 len = put_dox(n, 16, pMsg);
					pMsg += len;
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 'f': { //不考虑浮点数
/*
				double n = va_arg(ap, double);
				put_f(dneg_to_pos(n));
*/
				if(bTag) {
					double n = va_arg(ap, double);

					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			default:
				*pMsg++ = c;
				bTag = FALSE;
				break;
		}

		format++;
		c = *format;
	} //end of while

	*pMsg = '\0'; //添加字符串结束标志

	va_end(ap);
}

/**
 * @Func	打印调试信息
 * @Param	eState		[in]	记录类型
 * 			pAO			[in]	状态机
 * 			format		[in]	格式化字符串
 * @Ret		无
 */
void TRACE_(u8 eState, QActive* pAO, const char *format, ...)
{
static u8 trace_buf[TRACE_BUFSIZE]; //打印缓区
	va_list ap;
	char c;
	char *pMsg;
	BOOL bTag = FALSE;

	pMsg = (char*)trace_buf;
	ZeroMem(pMsg, TRACE_BUFSIZE);

	va_start(ap, format);
	c = *format;

	while (c != '\0') {
//	while(c != '\0') {
		switch (c) {
			case '%': {
				if (*format == '%') {
					bTag = TRUE;
				}
				break;
			}
			case 'c': {
				/* char is promoted to int when passed through '...' */
				if(bTag) {
					char ch = va_arg(ap, int);
					PUTCHAR(ch, pMsg);
					pMsg++;
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 's': {
				if(bTag) {
					char *s = va_arg(ap, char *);
					PUTSTRING(s, pMsg);
					pMsg += strlen_(s);
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 'd': {
				if(bTag) {
					int n = va_arg(ap, int);
					int m = ineg_to_pos(n, pMsg);
					u8 len;
					if(n < 0)
						pMsg++;
					len = put_dox(m, 10, pMsg);
					pMsg += len;
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 'o': {
				if(bTag) {
					unsigned int n = va_arg(ap, int);
					u8 len = put_dox(n, 8, pMsg);
					pMsg += len;
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 'x': {
				if(bTag) {
					unsigned int n = va_arg(ap, int);
					u8 len = put_dox(n, 16, pMsg);
					pMsg += len;
					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			case 'f': { //不考虑浮点数
/*
				double n = va_arg(ap, double);
				put_f(dneg_to_pos(n));
*/
				if(bTag) {
					double n = va_arg(ap, double);

					bTag = !bTag;
				}
				else
				{
					*pMsg++ = c;
				}
				break;
			}
			default:
				*pMsg++ = c;
				bTag = FALSE;
				break;
		}

		format++;
		c = *format;
	} //end of while

	*pMsg = '\0';

	va_end(ap);

	BSP_Trace(eState, pAO, trace_buf);
}


/**
 * 打印原始数据，十六进制
 *
 * @param pMsg
 * @param msgLen
 */
void TRACE_RAW_BUFFER(u8 *pMsg, u16 msgLen)
{

}

/*

int main(void)
{
	myprintf("%c, %s, %%, %d, %f\n", 'a', "hello", 123, 12.34);
	return 0;
}


*/

