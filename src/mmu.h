/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

void mmu_inicializar();


struct page_directory_entry_s {
  unsigned char p:1;
  unsigned char rw:1;
  unsigned char us:1;
  unsigned char pwt:1;
  unsigned char pcd:1;
  unsigned char a:1;
  unsigned char cero:1;
  unsigned char ps:1;
  unsigned char g:1;
  unsigned char disponible:3;
  unsigned int base:20;
} __attribute__((__packed__, aligned (4)));

struct page_table_entry_s {
  unsigned char p:1;
  unsigned char rw:1;
  unsigned char us:1;
  unsigned char pwt:1;
  unsigned char pcd:1;
  unsigned char a:1;
  unsigned char d:1;
  unsigned char pat:1;
  unsigned char g:1;
  unsigned char disponible:3;
  unsigned int base:20;
} __attribute__((__packed__, aligned (4)));


typedef struct page_directory_entry_s page_directory_entry_t;
typedef struct page_table_entry_s page_table_entry_t;


extern page_directory_entry_t *page_dir;


#endif	/* !__MMU_H__ */




