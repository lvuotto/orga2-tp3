/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#ifndef __GDT_H__
#define __GDT_H__

#define GDT_COUNT 17
#define GDT_IDX_CD_0  9
#define GDT_IDX_CD_1 10
#define GDT_IDX_DD_0 11
#define GDT_IDX_DD_1 12
#define GDT_IDX_SD   13
#define GDT_TSS_TAREA_INICIAL 14
#define GDT_TSS_1 15
#define GDT_TSS_2 16

#include "defines.h"


typedef struct str_gdt_descriptor {
  unsigned short  gdt_length;
  unsigned int    gdt_addr;
} __attribute__((__packed__)) gdt_descriptor;

typedef struct str_gdt_entry {
  unsigned short  limit_0_15;
  unsigned short  base_0_15;
  unsigned char   base_23_16;
  unsigned char   type:4;
  unsigned char   s:1;
  unsigned char   dpl:2;
  unsigned char   p:1;
  unsigned char   limit_16_19:4;
  unsigned char   avl:1;
  unsigned char   l:1;
  unsigned char   db:1;
  unsigned char   g:1;
  unsigned char   base_31_24;
} __attribute__((__packed__, aligned (8))) gdt_entry;

/* Tabla GDT */
extern gdt_entry gdt[];
extern gdt_descriptor GDT_DESC;


#endif  /* !__GDT_H__ */
