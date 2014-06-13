/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_next_1;
tss tss_next_2;

tss tss_inicial;
tss tss_idle;

tss tss_tanques[CANT_TANQUES];

void tss_inicializar() {
  unsigned int i, cr3;
  
  for (i = 0; i < CANT_TANQUES; i++) {
    cr3 = mmu_inicializar_dir_tarea(i); /* CONSULTAR */
    tss_tanques[i].ptl =      0x0;
    tss_tanques[i].unused0 =  0x0;
    tss_tanques[i].esp0 =     mmu_crear_pagina() + PAGE_SIZE;
    tss_tanques[i].ss0 =      0b1011000;
    tss_tanques[i].unused1 =  0x0;
    tss_tanques[i].esp1 =     0x0;
    tss_tanques[i].ss1 =      0x0;
    tss_tanques[i].unused2 =  0x0;
    tss_tanques[i].esp2 =     0x0;
    tss_tanques[i].ss2 =      0x0;
    tss_tanques[i].unused3 =  0x0;
    tss_tanques[i].cr3 =      cr3;  /* ??? */
    tss_tanques[i].eip =      0x8000000;
    tss_tanques[i].eflags =   0x00000202;
    tss_tanques[i].eax =      0x0;
    tss_tanques[i].ecx =      0x0;
    tss_tanques[i].edx =      0x0;
    tss_tanques[i].ebx =      0x0;
    tss_tanques[i].esp =      0x8002000;
    tss_tanques[i].ebp =      0x8002000;
    tss_tanques[i].esi =      0x0;
    tss_tanques[i].edi =      0x0;
    tss_tanques[i].es =       0b1100011;
    tss_tanques[i].unused4 =  0x0;
    tss_tanques[i].cs =       0b1010011;  /* desc lvl 3 */
    tss_tanques[i].unused5 =  0x0;
    tss_tanques[i].ss =       0b1100011;
    tss_tanques[i].unused6 =  0x0;
    tss_tanques[i].ds =       0b1100011;
    tss_tanques[i].unused7 =  0x0;
    tss_tanques[i].fs =       0b1101011;
    tss_tanques[i].unused8 =  0x0;
    tss_tanques[i].gs =       0b1100011;
    tss_tanques[i].unused9 =  0x0;
    tss_tanques[i].ldt =      0x0;
    tss_tanques[i].unused10 = 0x0;
    tss_tanques[i].dtrap =    0x0;  /* ??? */
    tss_tanques[i].iomap =    0xffff;
  }
}


void tss_inicializar_tarea_idle () {
  tss_idle.ptl =      0x0;
  tss_idle.unused0 =  0x0;
  tss_idle.esp0 =     0x27000;
  tss_idle.ss0 =      0b1011000;
  tss_idle.unused1 =  0x0;
  tss_idle.esp1 =     0x0;
  tss_idle.ss1 =      0x0;
  tss_idle.unused2 =  0x0;
  tss_idle.esp2 =     0x0;
  tss_idle.ss2 =      0x0;
  tss_idle.unused3 =  0x0;
  tss_idle.cr3 =      0x27000;
  tss_idle.eip =      0x20000;
  tss_idle.eflags =   0x00000202;  /* INTERRUPCIONES. CHEQUEAR MANUAL. */
  tss_idle.eax =      0x0;
  tss_idle.ecx =      0x0;
  tss_idle.edx =      0x0;
  tss_idle.ebx =      0x0;
  tss_idle.esp =      0x27000;
  tss_idle.ebp =      0x27000;
  tss_idle.esi =      0x0;
  tss_idle.edi =      0x0;
  tss_idle.es =       0b1011000;
  tss_idle.unused4 =  0x0;
  tss_idle.cs =       0b1001000;  /* IDLE VIVE EN KERNEL */
  tss_idle.unused5 =  0x0;
  tss_idle.ss =       0b1011000;
  tss_idle.unused6 =  0x0;
  tss_idle.ds =       0b1011000;
  tss_idle.unused7 =  0x0;
  tss_idle.fs =       0b1011000;
  tss_idle.unused8 =  0x0;
  tss_idle.gs =       0b1011000;
  tss_idle.unused9 =  0x0;
  tss_idle.ldt =      0x0;
  tss_idle.unused10 = 0x0;
  tss_idle.dtrap =    0x0;  /* ??? */
  tss_idle.iomap =    0xffff;
  
  gdt[GDT_TSS_1].base_31_24 =  (unsigned int) (&tss_idle) >> 24;
  gdt[GDT_TSS_1].base_23_16 = ((unsigned int) (&tss_idle) >> 16) & 0xff;
  gdt[GDT_TSS_1].base_0_15  =  (unsigned int) (&tss_idle)        & 0xffff;
}
