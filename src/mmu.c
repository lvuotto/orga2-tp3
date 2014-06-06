/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


#define BASE_AREA_LIBRE    0x100000
#define BASE_EL_MAPA       0x400000
#define BASE_TAREA_VIRTUAL 0x08000000

#define DIR_TAREA_1 0x10000
#define DIR_TAREA_2 0x12000
#define DIR_TAREA_3 0x14000
#define DIR_TAREA_4 0x16000
#define DIR_TAREA_5 0x18000
#define DIR_TAREA_6 0x1a000
#define DIR_TAREA_7 0x1c000
#define DIR_TAREA_8 0x1e000

#define MASK_22_BAJOS 0x3fffff


page_directory_entry_t *page_dir = (page_directory_entry_t *) 0x27000;
unsigned int pagina_libre = BASE_AREA_LIBRE;
unsigned int memoria_mapa = BASE_EL_MAPA;
unsigned int codigo_virtual = BASE_TAREA_VIRTUAL;


/* ==========================================================================
 * FUNCIONES AUXILIARES PARA MMU
 * ========================================================================== */


unsigned int mmu_crear_pagina () {
  unsigned int r;
  
  r = pagina_libre;
  pagina_libre += PAGE_SIZE;
  
  return r;
}


/* ==========================================================================
 * FUNCIONES POSTA PARA EL TP
 * ========================================================================== */


void mmu_inicializar_dir_kernel () {
  
  unsigned int i;
  page_table_entry_t *p;
  
  for(i = 0; i < 1024; i++) {
    page_dir[i].p = 0;
    page_dir[i].rw = 1;
    page_dir[i].us = 0;
    page_dir[i].pwt = 0;
    page_dir[i].pcd = 0;
    page_dir[i].a = 0;
    page_dir[i].cero = 0;
    page_dir[i].ps = 0;
    page_dir[i].g = 0;
    page_dir[i].disponible = 0;
    page_dir[i].base = 0;
  }
  
  
  /* Para hacer identity mapping de 0x00000000 a 0x00dc3fff */
  page_dir[0].p = 1;
  page_dir[1].p = 1;
  page_dir[2].p = 1;
  page_dir[3].p = 1;
  
  page_dir[0].base = 0x28;
  page_dir[1].base = 0x29;
  page_dir[2].base = 0x2a;
  page_dir[3].base = 0x2b;
  
  /* Marco: se puede (y debe) hacer casteo a la zabeca.       */
  /* shift de 12 porque las direcciones estan alineadas a 4k. */
  p = (page_table_entry_t *) (page_dir[0].base << 12);
  for (i = 0; i < 1024; i++) {
    p[i].p = 1;
    p[i].rw = 1;
    p[i].us = 0;
    p[i].pwt = 0;
    p[i].pcd = 0;
    p[i].a = 0;
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = i;
  }
  
  p = (page_table_entry_t *) (page_dir[1].base << 12);
  for (i = 0; i < 1024; i++) {
    p[i].p = 1;
    p[i].rw = 1;
    p[i].us = 0;
    p[i].pwt = 0;
    p[i].pcd = 0;
    p[i].a = 0;
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = 1024 + i;
  }
  
  p = (page_table_entry_t *) (page_dir[2].base << 12);
  for (i = 0; i < 1024; i++) {
    p[i].p = 1;
    p[i].rw = 1;
    p[i].us = 0;
    p[i].pwt = 0;
    p[i].pcd = 0;
    p[i].a = 0;
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = 1024*2 + i;
  }
  
  p = (page_table_entry_t *) (page_dir[3].base << 12);
  for (i = 0; i < 452; i++) {
    p[i].p = 1;
    p[i].rw = 1;
    p[i].us = 0;
    p[i].pwt = 0;
    p[i].pcd = 0;
    p[i].a = 0;
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = 1024*3 + i;
  }
  /*for (i = 452; i < 1024; i++) {
    p[i].p = 0;
    p[i].rw = 0;
    p[i].us = 0;
    p[i].pwt = 0;
    p[i].pcd = 0;
    p[i].a = 0;
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = 0;
  }*/
  
}


unsigned int mmu_inicializar_dir_tarea (task_id_t tid) {
  
  page_directory_entry_t *pd;
  page_table_entry_t *p;
  unsigned int i, cr3, codigo;
  /* cr3 y codigo funcionan como punteros. */
  
  
  cr3 = pagina_libre;
  pd = (page_directory_entry_t *) pagina_libre;
  pagina_libre += PAGE_SIZE;
  
  
  for(i = 0; i < 1024; i++) {
    pd[i].p = 0;
    pd[i].rw = 1;
    pd[i].us = 1;
    pd[i].pwt = 0;
    pd[i].pcd = 0;
    pd[i].a = 0;
    pd[i].cero = 0;
    pd[i].ps = 0;
    pd[i].g = 0;
    pd[i].disponible = 0;
    pd[i].base = 0;
  }
  
  pd[0].p = 1;
  pd[1].p = 1;
  pd[2].p = 1;
  pd[3].p = 1;
  
  pd[0].base = pagina_libre >> 12;
  pagina_libre += PAGE_SIZE;
  pd[1].base = pagina_libre >> 12;
  pagina_libre += PAGE_SIZE;
  pd[2].base = pagina_libre >> 12;
  pagina_libre += PAGE_SIZE;
  pd[3].base = pagina_libre >> 12;
  pagina_libre += PAGE_SIZE;
  
  
  p = (page_table_entry_t *) (pd[0].base << 12);
  for (i = 0; i < 1024; i++) {
    p[i].p = 1;
    p[i].rw = 1;
    p[i].us = 1;
    p[i].pwt = 0;
    p[i].pcd = 0;
    p[i].a = 0;
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = i;
  }
  
  p = (page_table_entry_t *) (pd[1].base << 12);
  for (i = 0; i < 1024; i++) {
    p[i].p = 1;
    p[i].rw = 1;
    p[i].us = 1;
    p[i].pwt = 0;
    p[i].pcd = 0;
    p[i].a = 0;
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = 1024 + i;
  }
  
  p = (page_table_entry_t *) (pd[2].base << 12);
  for (i = 0; i < 1024; i++) {
    p[i].p = 1;
    p[i].rw = 1;
    p[i].us = 1;
    p[i].pwt = 0;
    p[i].pcd = 0;
    p[i].a = 0;
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = 1024*2 + i;
  }
  
  p = (page_table_entry_t *) (pd[3].base << 12);
  for (i = 0; i < 452; i++) {
    p[i].p = 1;
    p[i].rw = 1;
    p[i].us = 1;
    p[i].pwt = 0;
    p[i].pcd = 0;
    p[i].a = 0;
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = 1024*3 + i;
  }
  /*for (i = 452; i < 1024; i++) {
    p[i].p = 0;
    p[i].rw = 1;
    p[i].us = 1;
    p[i].pwt = 0;       PREGUNTAR
    p[i].pcd = 0;       PREGUNTAR
    p[i].a = 0;         PREGUNTAR
    p[i].d = 0;
    p[i].pat = 0;
    p[i].g = 0;
    p[i].disponible = 0;
    p[i].base = PAGE_SIZE*3 + i;
  }*/
  
  
  switch (tid) {
    case TAREA_1:
      codigo = DIR_TAREA_1;
      break;
    case TAREA_2:
      codigo = DIR_TAREA_2;
      break;
    case TAREA_3:
      codigo = DIR_TAREA_3;
      break;
    case TAREA_4:
      codigo = DIR_TAREA_4;
      break;
    case TAREA_5:
      codigo = DIR_TAREA_5;
      break;
    case TAREA_6:
      codigo = DIR_TAREA_6;
      break;
    case TAREA_7:
      codigo = DIR_TAREA_7;
      break;
    case TAREA_8:
      codigo = DIR_TAREA_8;
      break;
    default:
      break;
  }
  
  mmu_mapear_pagina(codigo_virtual, cr3, codigo, 0);
  codigo_virtual += 2*PAGE_SIZE;
  
  return cr3;
  
}


unsigned int mmu_inicializar () {
  
  /* CAMBIAR ESTO A VOID */
  
  unsigned int cr3;
  
  mmu_inicializar_dir_kernel();
  
  cr3 = mmu_inicializar_dir_tarea(TAREA_1);
  mmu_inicializar_dir_tarea(TAREA_2);
  mmu_inicializar_dir_tarea(TAREA_3);
  mmu_inicializar_dir_tarea(TAREA_4);
  mmu_inicializar_dir_tarea(TAREA_5);
  mmu_inicializar_dir_tarea(TAREA_6);
  mmu_inicializar_dir_tarea(TAREA_7);
  mmu_inicializar_dir_tarea(TAREA_8);
  
  return cr3;
  
}


void mmu_mapear_pagina (unsigned int virtual,
                        unsigned int cr3,
                        unsigned int fisica,
                        unsigned int attr)
{
  
  unsigned int dir_idx, tabla_idx;
  page_directory_entry_t *dir;
  page_table_entry_t *tabla;
  
  dir = (page_directory_entry_t *) cr3;
  dir_idx = virtual >> 22;
  
  if (!dir[dir_idx].p) {
    dir[dir_idx].base = (mmu_crear_pagina() >> 12) & MASK_22_BAJOS;
    dir[dir_idx].p = 1;
    dir[dir_idx].us = 1;   /* CONSULTAAAAAAAAAAAAAAR */
    dir[dir_idx].rw = 1;
  }
  
  tabla = (page_table_entry_t *) (dir[dir_idx].base << 12);
  tabla_idx = (virtual & MASK_22_BAJOS) >> 12;
  
  if (!tabla[tabla_idx].p) {
    tabla[tabla_idx].p = 1;
    tabla[tabla_idx].us = 1;
    tabla[tabla_idx].rw = 1;
  }
  
  tabla[tabla_idx].base = fisica >> 12;
  
  tlbflush();
  
}



void mmu_unmapear_pagina (unsigned int virtual,
                          unsigned int cr3)
{
  
  unsigned int dir_idx, tabla_idx;
  page_directory_entry_t *dir;
  page_table_entry_t *tabla;
  
  dir = (page_directory_entry_t *) cr3;
  dir_idx = virtual >> 22;
  
  if (dir[dir_idx].p) {
    tabla = (page_table_entry_t *) (dir[dir_idx].base << 12);
    tabla_idx = (virtual & MASK_22_BAJOS) >> 12;
    tabla[tabla_idx].p = 0;
    
    tlbflush();
  }
  
}







