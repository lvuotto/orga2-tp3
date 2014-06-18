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



tss *idle_ctx;
void scheduler_inicializar ();

unsigned short sched_proximo_indice ();

unsigned short sched_tarea_actual ();


#endif	/* !__SCHED_H__ */
