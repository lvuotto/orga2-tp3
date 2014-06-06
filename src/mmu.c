/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


#define BASE_pagina_libre 0x100000


page_directory_entry_t *page_dir = (page_directory_entry_t *) 0x27000;
unsigned int pagina_libre = BASE_pagina_libre;


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
    p[i].base = 1024 * 2 + i;
  }
  
  p = (page_table_entry_t *) (page_dir[3].base << 12);
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
    p[i].base = 1024 * 3 + i;
  }
  
}


unsigned int mmu_inicializar_dir_tarea (unsigned int id_tarea) {
  
  page_directory_entry_t *pd;
  page_table_entry_t *p;
  unsigned int i, cr3;
  
  cr3 = pagina_libre;
  pd = (page_directory_entry_t *) pagina_libre;
  pagina_libre += 0x1000;
  
  
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
  
  pd[0].base = pagina_libre;
  pagina_libre += 0x1000;
  pd[1].base = pagina_libre;
  pagina_libre += 0x1000;
  pd[2].base = pagina_libre;
  pagina_libre += 0x1000;
  pd[3].base = pagina_libre;
  pagina_libre += 0x1000;
  
  
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
    p[i].base = i;
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
    p[i].base = i;
  }
  
  p = (page_table_entry_t *) (pd[3].base << 12);
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
  
  return cr3;
  
}


void mmu_inicializar () {
  
  unsigned int cr3;
  
  mmu_inicializar_dir_kernel();
  
  cr3 = mmu_inicializar_dir_tarea(1);
  lcr3(cr3);
  //~ breakpoint();
  
  cr3 = mmu_inicializar_dir_tarea(2);
  lcr3(cr3);
  //~ breakpoint();
  
  cr3 = mmu_inicializar_dir_tarea(3);
  lcr3(cr3);
  //~ breakpoint();
  
  cr3 = mmu_inicializar_dir_tarea(4);
  lcr3(cr3);
  //~ breakpoint();
  
  cr3 = mmu_inicializar_dir_tarea(5);
  lcr3(cr3);
  //~ breakpoint();
  
  cr3 = mmu_inicializar_dir_tarea(6);
  lcr3(cr3);
  //~ breakpoint();
  
  cr3 = mmu_inicializar_dir_tarea(7);
  lcr3(cr3);
  //~ breakpoint();
  
  cr3 = mmu_inicializar_dir_tarea(8);
  lcr3(cr3);
  //~ breakpoint();
  
}


void mmu_mapear_pagina (unsigned int virtual,
                        unsigned int cr3,
                        unsigned int fisica)
{
  
  
  
}







