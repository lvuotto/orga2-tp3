/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"


#define NULL 0
#define SWAAAAP()               \
  swaaaap      = tss_tarea_1;    \
  tss_tarea_1   = tss_tarea_2;  \
  tss_tarea_2 = swaaaap


tss *tss_tarea_1, *tss_tarea_2, *swaaaap;
unsigned int _tarea_actual;
unsigned int tareas_vivas[CANT_TANQUES];
unsigned int _esta_corriendo_la_idle,
             gdt_tss_1_busy,
             guardar_tanquecito,
             primera_vez;


void sched_inicializar () {
  unsigned int i;
  
  for (i = 0; i < CANT_TANQUES; i++) {
    tareas_vivas[i] = TRUE;
  }
  
  tss_tarea_1 = &tss_next_1;
  tss_tarea_2 = &tss_next_2;
  
  tss_copy(tss_tarea_1, &tss_idle);
  _esta_corriendo_la_idle = TRUE;
  guardar_tanquecito = TRUE;
  
  gdt_tss_1_busy = TRUE;
  _tarea_actual = CANT_TANQUES - 1;
  
  primera_vez = TRUE;
}


unsigned int esta_corriendo_la_idle () {
  return _esta_corriendo_la_idle;
}


unsigned int sched_tarea_actual () {
  return _tarea_actual;
}


void sched_desalojar_tarea (unsigned int id) {
  tareas_vivas[id] = FALSE;
}


unsigned int proximo_indice_vivo () {
  unsigned int i;
  
  i = (_tarea_actual + 1) % CANT_TANQUES;
  while (tareas_vivas[i] == FALSE && i != _tarea_actual) {
    i++;
    i %= CANT_TANQUES;
  }
  
  return i == _tarea_actual ? 0xdeadc0de : i;
}


tss * proxima_tarea () {
  unsigned int p;
  tss *r;
  
  p = proximo_indice_vivo();
  //~ breakpoint();
  
  if (p == 0xdeadc0de) {
    r = NULL;
  } else {
    r = &tss_tanques[p];
    _tarea_actual = p;
  }
  
  return r;
}


unsigned short sched_montar_idle () {
  unsigned short r;
  tss *anterior;
  
  anterior = guardar_tanquecito ? &tss_tanques[_tarea_actual] : &tss_idle;
  
  if (gdt_tss_1_busy) {
    tss_copy(anterior, tss_tarea_2);
    tss_copy(tss_tarea_2, &tss_idle);
  } else {
    tss_copy(anterior, tss_tarea_1);
    tss_copy(tss_tarea_1, &tss_idle);
  }
  
  r = gdt_tss_1_busy ? GDT_TSS_2 << 3 : GDT_TSS_1 << 3;
  gdt_tss_1_busy = 1 - gdt_tss_1_busy;
  _esta_corriendo_la_idle = TRUE;
  
  return r;
}


unsigned short sched_proxima_tarea () {
  
  unsigned short r;
  tss *proximo, *anterior;
  
  anterior = guardar_tanquecito ? &tss_tanques[_tarea_actual] : &tss_idle;
  proximo = proxima_tarea();
  
  if (proximo == NULL && !primera_vez) {
    return 0;
  }
  
  if (gdt_tss_1_busy) {
    if (primera_vez) {
      tss_copy(tss_tarea_2, &tss_tanques[0]);
    } else {
      tss_copy(anterior, tss_tarea_2);
      tss_copy(tss_tarea_2, proximo);
    }
  } else {
    if (primera_vez) {
      tss_copy(tss_tarea_1, &tss_tanques[0]);
    } else {
      tss_copy(anterior, tss_tarea_1);
      tss_copy(tss_tarea_1, proximo);
    }
  }
  
  r = gdt_tss_1_busy ? GDT_TSS_2 << 3 : GDT_TSS_1 << 3;
  gdt_tss_1_busy = 1 - gdt_tss_1_busy;
  guardar_tanquecito = 1 - _esta_corriendo_la_idle;
  _esta_corriendo_la_idle = FALSE;
  primera_vez = FALSE;
  
  return r;
  
}

