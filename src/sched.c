/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

tss *tss_tarea_1, *tss_tarea_2, *swaaaap;
unsigned int _tarea_actual, _tarea_anterior;
unsigned int tareas_vivas[CANT_TANQUES + 1];
unsigned int _esta_corriendo_la_idle;
unsigned int tss_tarea_1_busy;
unsigned char primera_vez;


void sched_inicializar () {
  unsigned int i;
  
  for (i = 0; i < CANT_TANQUES + 1; i++) {
    tareas_vivas[i] = TRUE;
  }
  tss_copy(tss_tarea_1, &tss_idle);
  _esta_corriendo_la_idle = TRUE;
  
  tss_tarea_1_busy = TRUE;
  _tarea_actual = 0;
  _tarea_anterior = 0;
  
  primera_vez = TRUE;
}


unsigned int esta_corriendo_la_idle () {
  return _esta_corriendo_la_idle;
}


unsigned int sched_tarea_actual () {
  return _tarea_actual;
}


unsigned int tarea_anterior () {
  return _tarea_anterior;
}


void sched_desalojar_tarea (unsigned int id) {
  tareas_vivas[id % CANT_TANQUES] = FALSE;
}


unsigned short sched_montar_idle () {
  unsigned int r;
  
  if (tss_tarea_1_busy) {
    tss_copy(&tss_tanques[_tarea_anterior], tss_tarea_2);
    tss_copy(tss_tarea_2, &tss_idle);
    r = GDT_TSS_2 << 3;
    tss_tarea_1_busy = FALSE;
  } else {
    tss_copy(&tss_tanques[_tarea_anterior], tss_tarea_1);
    tss_copy(tss_tarea_1, &tss_idle);
    r = GDT_TSS_1 << 3;
    tss_tarea_1_busy = TRUE;
  }
  
  swaaaap     = tss_tarea_1;
  tss_tarea_1 = tss_tarea_2;
  tss_tarea_2 = swaaaap;
  
  return r;
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


unsigned short sched_proxima_tarea () {
  
  unsigned int r, p;
  
  p = proximo_indice_vivo();
  
  if (primera_vez) {
    primera_vez = FALSE;
    tss_tarea_1_busy = FALSE;
    tss_copy(tss_tarea_2, &tss_tanques[0]);
    r = GDT_TSS_2 << 3;
    p = 0;
    _tarea_actual = 0;
  } else if (tss_tarea_1_busy) {
    p = proximo_indice_vivo();
    if (p == 0xdeadc0de) {
      r = 0;
    } else {
      tss_copy(&tss_tanques[_tarea_anterior], tss_tarea_2);
      tss_copy(tss_tarea_2, &tss_tanques[p]);
      r = GDT_TSS_2 << 3;
      tss_tarea_1_busy = FALSE;
    }
    /*tss_tarea_1_busy = FALSE;
    r = GDT_TSS_2 << 3;*/
  } else {
    if (p == 0xdeadc0de) {
      r = 0;
    } else {
      tss_copy(&tss_tanques[_tarea_anterior], tss_tarea_1);
      tss_copy(tss_tarea_1, &tss_tanques[p]);
      r = GDT_TSS_1 << 3;
      tss_tarea_1_busy = TRUE;
    }
    /*tss_tarea_1_busy = TRUE;
    r = GDT_TSS_1 << 3;*/
  }
  
  _esta_corriendo_la_idle = FALSE;
  
  _tarea_anterior = _tarea_actual;
  _tarea_actual = p;
  
  swaaaap     = tss_tarea_1;
  tss_tarea_1 = tss_tarea_2;
  tss_tarea_2 = swaaaap;
  
  return r;
  
}

