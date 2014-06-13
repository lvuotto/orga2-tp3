/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"


sched_ctx_t ctx_tanques[CANT_TANQUES];
sched_ctx_t ctx_idle;


unsigned short sched_proximo_indice () {
  
  unsigned short r;
  
  if (gdt[GDT_TSS_1].type & 0x2) {  /* gdt[GDT_TSS_1] busy? */
    r = GDT_TSS_2;
  } else {
    r = GDT_TSS_1;
  }
  
  return r;
  
}
