/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#define SWAP(a, b) a^=b;b^=a;a^=b;


unsigned int anterior;                 // indice de las tarea anterior
unsigned int actual;                   // indice de las tarea actual
unsigned int indice_tarea_anterior;
unsigned int indice_tarea_actual;
unsigned int tareas_vivas[CANT_TANQUES];
unsigned int actual_prox   = 0;        // xq nunca se ejecuta la tarea inicial
unsigned int anterior_prox = 1;
unsigned int i             = 0;


//~ void swap(unsigned int a, unsigned int b) {
  //~ 
  //~ unsigned int temp = b;
  //~ b = a;
  //~ a = temp;
  //~ 
//~ }


void desalojar_tarea_actual() {
    tareas_vivas[indice_tarea_actual] = 0;
    //"tendria que pasar a la IDLE"
    
}


tss *tss_tarea_anterior() {
    
    if(actual_prox == 0) {
        return &(tss_next_1);
    } else {
        return &(tss_next_2);
    }
    
}


//~ tss *tss_tarea_actual() {
    //~ 
    //~ if(actual_prox == false) {
        //~ return &(tss_next_2);
    //~ } else {
        //~ return &(tss_next_1);
    //~ }
    //~ 
//~ }


void scheduler_inicializar() {
  
  for(i = 0; i < CANT_TANQUES; i++) {
      tareas_vivas[i]   = 1;
  }
  anterior              = GDT_TSS_1;
  actual                = GDT_TSS_2;
  indice_tarea_actual   = GDT_TSS_TAREA_INICIAL;
  indice_tarea_anterior = -1;
  
}


unsigned int sched_proximo_indice() {
  
  if(actual_prox) {
    actual_prox   = 0;
    anterior_prox = 1;
  return actual;
  } else {
    actual_prox   = 1;
    anterior_prox = 0;
    return anterior;
  }
  
}


unsigned int sched_proxima_tarea() {
  
  i = actual;
  while(tareas_vivas[i] != 0) {
    if(i >= CANT_TANQUES) {
      i = 0;
    }
    i++;
  }
  
  if(i == indice_tarea_actual){
    return 69;
  }
  return i;
    
}


void actualizar_tss() {
  
  /* guardamos el contexto de la tarea anterior */
  tss *contexto_anterior = tss_tarea_anterior();
    
  /* en la pos correspondiente correspondiente del array */
  if(indice_tarea_anterior != -1) {
    int j = indice_tarea_anterior;
    tss_copy(contexto_anterior, &tss_tanques[j]);
  }
                                                   
  /* calculamos cuál es la próxima tarea y la cargamos */
  i = sched_proxima_tarea();
  tss_copy(&tss_tanques[i], contexto_anterior); 
                                                        
  indice_tarea_anterior = indice_tarea_actual;
  indice_tarea_actual   = sched_proxima_tarea();
  
  SWAP(anterior, actual);
  
}



