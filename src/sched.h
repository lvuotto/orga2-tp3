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



void            desalojar_tarea_actual();
void            scheduler_inicializar();
unsigned short sched_proximo_indice();
unsigned int   sched_proxima_tarea();
unsigned int   sched_tarea_actual();
void            sched_cambiar_a_idle(); 
void            actualizar_tss();



#endif	/* !__SCHED_H__ */
