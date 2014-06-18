/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#define SWAP(a, b) a^=b;b^=a;a^=b;


unsigned int anterior;                 /* indice de la tarea anterior */
unsigned int actual;                   /* indice de la tarea actual */
unsigned int indice_tarea_anterior;
unsigned int indice_tarea_actual;
unsigned int tareas_vivas[CANT_TANQUES];
unsigned int actual_prox   = 0;     /* xq nunca se ejecuta la tarea inicial */
unsigned int anterior_prox = 1;


//~ void swap(unsigned int a, unsigned int b) {
  //~ 
  //~ unsigned int temp = b;
  //~ b = a;
  //~ a = temp;
  //~ 
//~ }


void scheduler_inicializar () {
  unsigned int i = 0;
  for (i = 0; i < CANT_TANQUES; i++) {
    tareas_vivas[i]   = 1;
  }
  anterior              = GDT_TSS_2;
  actual                = GDT_TSS_1;
  indice_tarea_actual   = 0;
  indice_tarea_anterior = 0xdeadc0de;
  tss_copy(&tss_idle, &tss_next_1);  /* la primer tarea que cargamos es la IDLE */
}


void desalojar_tarea_actual () {
  tareas_vivas[indice_tarea_actual] = 0;
}


tss *tss_tarea_anterior () {
  if (actual_prox == 0) {
    return &tss_next_1;
  } else {
    return &tss_next_2;
  }
}


unsigned int sched_tarea_actual () {
  return indice_tarea_actual;
}


unsigned short sched_proximo_indice () {
  if (actual_prox) {
    actual_prox   = 0;
    anterior_prox = 1;
    return (actual << 3) | 3; /* devolvemos el selector */
  } else {
    actual_prox   = 1;
    anterior_prox = 0;
    return (anterior << 3) | 3;
  }
}


unsigned int sched_proxima_tarea () {
  unsigned int i = indice_tarea_actual;
  while (tareas_vivas[i] == 0) {
    if (i >= CANT_TANQUES) {
      i = 0;
    } else {
      i++;
    }
    if (i == indice_tarea_actual) {
      return 69;
    }
  }
  
  
  return i;
}


void sched_cambiar_a_idle () {
  tss_copy(&tss_next_1, &tss_idle); 
}


//TENEMOS QUE ACTUALIZAR tss_next_1 y tss_next_2
void actualizar_tss () {
  /* guardamos el contexto de la tarea anterior */
  tss *contexto_anterior = tss_tarea_anterior();
  
  /* en la pos correspondiente correspondiente del array */
  if (indice_tarea_anterior != 0xdeadc0de) {
    tss_copy(contexto_anterior, &tss_tanques[indice_tarea_anterior]);
  }
                                                   
  /* calculamos cuál es la próxima tarea y la cargamos */
  unsigned int i        = sched_proxima_tarea();
  tss_copy(&tss_tanques[i], contexto_anterior); 
  indice_tarea_anterior = indice_tarea_actual;
  indice_tarea_actual   = i;
  
  SWAP(anterior, actual);
}
