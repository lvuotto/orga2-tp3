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



//typedef struct scheduler_s {
  //tss *sched_ctx_arr[];
  //tss *idle_ctx;
  //tss *tss1;
  //tss *tss2;
  //unsigned int tarea_actual;
//} __attribute__((__packed__, aligned (4))) scheduler_t;



tss *idle_ctx;
void scheduler_inicializar ();

unsigned short sched_proximo_indice ();


#endif	/* !__SCHED_H__ */
