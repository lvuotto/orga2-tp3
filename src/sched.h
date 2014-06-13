/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"


struct sched_ctx_s {
  unsigned int    alive;
  tss            *la_tss;
};


typedef struct sched_ctx_s sched_ctx_t;


unsigned short sched_proximo_indice ();


#endif	/* !__SCHED_H__ */
