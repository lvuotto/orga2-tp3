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



tss  *idle_ctx;

void          desalojar_tarea_actual();
tss           *tss_tarea_anterior();
void          scheduler_inicializar();
unsigned int sched_proximo_indice();
unsigned int sched_proxima_tarea();
void          actualizar_tss();
 



#endif	/* !__SCHED_H__ */
