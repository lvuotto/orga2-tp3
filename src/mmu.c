/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

page_directory_entry_t *page_dir = (page_directory_entry_t *) 0x27000;

void mmu_inicializar() {
  
  int i;
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
    p->p = 1;
    p->rw = 1;
    p->us = 0;
    p->pwt = 0;
    p->pcd = 0;
    p->a = 0;
    p->d = 0;
    p->pat = 0;
    p->g = 0;
    p->disponible = 0;
    p->base = i;
  }
  
  p = (page_table_entry_t *) (page_dir[1].base << 12);
  for (i = 0; i < 1024; i++) {
    p->p = 1;
    p->rw = 1;
    p->us = 0;
    p->pwt = 0;
    p->pcd = 0;
    p->a = 0;
    p->d = 0;
    p->pat = 0;
    p->g = 0;
    p->disponible = 0;
    p->base = i;
  }
  
  p = (page_table_entry_t *) (page_dir[2].base << 12);
  for (i = 0; i < 1024; i++) {
    p->p = 1;
    p->rw = 1;
    p->us = 0;
    p->pwt = 0;
    p->pcd = 0;
    p->a = 0;
    p->d = 0;
    p->pat = 0;
    p->g = 0;
    p->disponible = 0;
    p->base = i;
  }
  
  p = (page_table_entry_t *) (page_dir[3].base << 12);
  for (i = 0; i < 1024; i++) {
    p->p = 1;
    p->rw = 1;
    p->us = 0;
    p->pwt = 0;
    p->pcd = 0;
    p->a = 0;
    p->d = 0;
    p->pat = 0;
    p->g = 0;
    p->disponible = 0;
    p->base = i;
  }
  
  
}







