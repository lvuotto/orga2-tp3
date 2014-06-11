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
}


void tss_inicializar_tarea_idle () {
  tss_idle.ptl = 0x0;
  tss_idle.unused0 = 0x0;
  tss_idle.esp0 = 0x27000;
  tss_idle.ss0 = 0b1011000;
  tss_idle.unused1 = 0x0;
  tss_idle.esp1 = 0x0;
  tss_idle.ss1 = 0x0;
  tss_idle.unused2 = 0x0;
  tss_idle.esp2 = 0x0;
  tss_idle.ss2 = 0x0;
  tss_idle.unused3 = 0x0;
  tss_idle.cr3 = 0x27000;
  tss_idle.eip = 0x0;
  tss_idle.eflags = 0x0; /* ??? */
  tss_idle.eax = 0x0;
  tss_idle.ecx = 0x0;
  tss_idle.edx = 0x0;
  tss_idle.ebx = 0x0;
  tss_idle.esp = 0x27000;
  tss_idle.ebp = 0x27000;
  tss_idle.esi = 0x0;
  tss_idle.edi = 0x0;
  tss_idle.es = 0b1011000;
  tss_idle.unused4 = 0x0;
  tss_idle.cs = 0x;
  tss_idle.unused5 = 0x0;
  tss_idle.ss = 0b1011000;
  tss_idle.unused6 = 0x0;
  tss_idle.ds = 0b1011000;
  tss_idle.unused7 = 0x0;
  tss_idle.fs = 0b1101000;
  tss_idle.unused8 = 0x0;
  tss_idle.gs = 0x0;
  tss_idle.unused9 = 0x0;
  tss_idle.ldt = 0x0;
  tss_idle.unused10 = 0x0;
  tss_idle.dtrap = 0x0; /* ??? */
  tss_idle.iomap = 0xffff;
}
