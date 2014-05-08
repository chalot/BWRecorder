/*
 * syscalls.c
 *
 *  @brief: Come descritto qui:
 *
 *  			 http://embdev.net/topic/177530
 *  			 http://www.utasker.com/forum/index.php?topic=748.0 (terzo post)
 *  			 file:///C:/Programmi/CodeSourcery/Sourcery%20G++%20Lite/share/doc/arm-arm-none-eabi/html/libc/Syscalls.html (man CodeSourcery)
 *
 *  questo codice serve a linkare correttamente la libreria newlib degli ARM.
 *  Il codice risultante qui sotto e' un collage delle varie versioni di syscall.c trovate in giro
 *  per la rete e fornisce una versione minimale che consente di compilare il progetto
 *  correttamente.
 */
// TODO: Da verificare se non crea problemi di sorta a runtime.
#include <reent.h>
#include <errno.h>
#include <stdlib.h> /* abort */
#include <sys/types.h>
#include <sys/stat.h>


#include "stm32f2xx.h" /* for _get_PSP() from core_cm3.h*/

#undef errno
extern int errno;

extern int errno;
extern int  _end;
caddr_t _sbrk(int incr) {

	  static unsigned char *heap = NULL;
	  unsigned char *prev_heap;
	  if (heap == NULL) {
		  heap = (unsigned char *)&_end;
	  }

	  prev_heap = heap;
	  heap += incr;
	  return (caddr_t) prev_heap;
}

void _exit(int status)
{
	// xprintf("_exit called with parameter %d\n", status);
	while(1) {;} // Blocca tutto!
}

int _getpid(void)
{
	return 1;
}

int _kill(int pid, int sig)
{
	pid = pid; sig = sig; /* avoid warnings */
	errno = EINVAL;
	return -1;
}

int link(char *old, char *new) {
	return -1;
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

int _read(int file, char *ptr, int len)
{
	return 0;
}

int _write(int file, char *ptr, int len)
{
	return len;
}

// extern char _end; /* Defined by the linker (c'è gia' sopra)*/
static char *heap_end;

char* get_heap_end(void)
{
	return (char*) heap_end;
}

char* get_stack_top(void)
{
	return (char*) __get_MSP();
}
