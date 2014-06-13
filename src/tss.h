/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "defines.h"
#include "i386.h"
#include "gdt.h"
#include "mmu.h"

typedef struct str_tss {
  unsigned short  ptl;
  unsigned short  unused0;
  unsigned int    esp0;
  unsigned short  ss0;
  unsigned short  unused1;
  unsigned int    esp1;
  unsigned short  ss1;
  unsigned short  unused2;
  unsigned int    esp2;
  unsigned short  ss2;
  unsigned short  unused3;
  unsigned int    cr3;
  unsigned int    eip;
  unsigned int    eflags;
  unsigned int    eax;
  unsigned int    ecx;
  unsigned int    edx;
  unsigned int    ebx;
  unsigned int    esp;
  unsigned int    ebp;
  unsigned int    esi;
  unsigned int    edi;
  unsigned short  es;
  unsigned short  unused4;
  unsigned short  cs;
  unsigned short  unused5;
  unsigned short  ss;
  unsigned short  unused6;
  unsigned short  ds;
  unsigned short  unused7;
  unsigned short  fs;
  unsigned short  unused8;
  unsigned short  gs;
  unsigned short  unused9;
  unsigned short  ldt;
  unsigned short  unused10;
  unsigned short  dtrap;
  unsigned short  iomap;
} __attribute__((__packed__, aligned (8))) tss;

extern tss tss_inicial;
extern tss tss_idle;

extern tss tss_tanques[];
extern tss tss_next_1;
extern tss tss_next_2;

void tss_inicializar();
void tss_inicializar_tarea_idle();
void tss_inicializar_tarea_tanque(unsigned int id);
void tss_inicializar_tareas_tanques();
void tss_copy(tss* tss_src, tss* tss_dst);
unsigned int tss_get_cr3(unsigned int id);

#endif  /* !__TSS_H__ */
