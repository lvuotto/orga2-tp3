/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"


#define NULL 0
#define SWAAAAP()               \
  swaaaap      = tss_actual;    \
  tss_actual   = tss_anterior;  \
  tss_anterior = swaaaap


tss *tss_actual, *tss_anterior, *swaaaap;
unsigned int _tarea_actual;
unsigned int tareas_vivas[CANT_TANQUES];
unsigned int _esta_corriendo_la_idle,
             gdt_tss_1_busy,
             guardar_tanquecito,
             primera_vez;


void sched_inicializar () {
  unsigned int i;
  
  for (i = 0; i < CANT_TANQUES + 1; i++) {
    tareas_vivas[i] = TRUE;
  }
  
  tss_actual = &tss_next_1;
  tss_anterior = &tss_next_2;
  
  tss_copy(tss_actual, &tss_idle);
  _esta_corriendo_la_idle = TRUE;
  
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
  tareas_vivas[id % CANT_TANQUES] = FALSE;
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


tss * estructura_de_resguardo () {
  tss *r;
  
  r = guardar_tanquecito ? &tss_tanques[_tarea_actual] : &tss_idle;
  guardar_tanquecito = !_esta_corriendo_la_idle;
  
  return r;
}


tss * proxima_tarea () {
  unsigned int p;
  tss *r;
  
  p = proximo_indice_vivo();
  
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
  tss *backup;
  
  backup = estructura_de_resguardo();
  r = gdt_tss_1_busy ? GDT_TSS_2 << 3 : GDT_TSS_1 << 3;
  
  if (!gdt_tss_1_busy) {
    SWAAAAP();
  }
  
  tss_copy(backup, tss_anterior);
  tss_copy(tss_anterior, &tss_idle);
  
  gdt_tss_1_busy = !gdt_tss_1_busy;
  
  _esta_corriendo_la_idle = TRUE;
  
  return r;
}


unsigned short sched_proxima_tarea () {
  
  unsigned short r;
  tss *proximo;
  
  proximo = proxima_tarea();
  
  primera_vez = FALSE;
  
  if (proximo == NULL) {
    return 0;
  }
  
  breakpoint();
  if (!primera_vez) {
    if (guardar_tanquecito)
      tss_copy(&tss_tanques[_tarea_actual], tss_anterior);
    else
      tss_copy(&tss_idle, tss_anterior);
  }
  tss_copy(tss_anterior, proximo);
  SWAAAAP();
  
  r = gdt_tss_1_busy ? GDT_TSS_2 << 3 : GDT_TSS_1 << 3;
  gdt_tss_1_busy = !gdt_tss_1_busy;
  guardar_tanquecito = !_esta_corriendo_la_idle;
  
  return r;
  
}

