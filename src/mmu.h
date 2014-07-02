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


enum task_id_e {
  TAREA_1,
  TAREA_2,
  TAREA_3,
  TAREA_4,
  TAREA_5,
  TAREA_6,
  TAREA_7,
  TAREA_8
};


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
typedef enum task_id_e task_id_t;


extern page_directory_entry_t *page_dir;


void         mmu_inicializar            ();
unsigned int mmu_crear_pagina           ();
void         mmu_inicializar_dir_kernel ();
unsigned int mmu_inicializar_dir_tarea  (task_id_t id_tarea);
void         mmu_mapear_pagina          (unsigned int virtual,
                                         unsigned int cr3,
                                         unsigned int fisica,
                                         unsigned int attr);
void         mmu_unmapear_pagina        (unsigned int virtual,
                                         unsigned int cr3);
void         copiar_memoria             (unsigned int dst,
                                         unsigned int src,
                                         unsigned int size);


void desalojar_tarea (unsigned int t);

#endif  /* !__MMU_H__ */




