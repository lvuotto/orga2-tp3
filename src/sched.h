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



void           sched_inicializar      ();
unsigned short sched_proxima_tarea    ();
unsigned int   sched_estado_tarea     (unsigned int id);
unsigned int   sched_tarea_actual     ();
unsigned int   sched_tarea_anterior   ();
unsigned int   esta_corriendo_la_idle ();
void           sched_desalojar_tarea  (unsigned int id);
unsigned short sched_montar_idle      ();



#endif	/* !__SCHED_H__ */
