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


typedef struct nodo_sched_s nodo_sched_t;

struct nodo_sched_s {
  tss          *la_tss;
  nodo_sched_t *next;
  nodo_sched_t *prev;
} __attribute__((__packed__, aligned (4)));


unsigned short sched_proximo_indice ();


#endif	/* !__SCHED_H__ */
