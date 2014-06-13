/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

scheduler_t *sched_str;

void scheduler_inicializar () {
    
    //for (int i = 0; i < 7; i++) {
        //sched_str->(*sched_ctx_arr)[i] = tss_get_cr3(i);
    //}
    //sched_str->idle_ctx     = &tss_idle;
    //sched_str->tss1         = &tss_next_1;
    //sched_str->tss2         = &tss_next_2;
    //sched_str->tarea_actual = 0;
        
}


unsigned short sched_proximo_indice () {
  
  unsigned short r;
  
  if (gdt[GDT_TSS_1].type & 0x2) {  /* gdt[GDT_TSS_1] busy? */
    r = GDT_TSS_2;
  } else {
    r = GDT_TSS_1;
  }
  
  return r;
  
}
