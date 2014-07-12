/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


#define MASK_22_BAJOS 0x3fffff
#define ESTA_OCUPADA(p)  \
  codigo_mapa[0] == p || codigo_mapa[0] + 1 == p || \
  codigo_mapa[1] == p || codigo_mapa[1] + 1 == p || \
  codigo_mapa[2] == p || codigo_mapa[2] + 1 == p || \
  codigo_mapa[3] == p || codigo_mapa[3] + 1 == p || \
  codigo_mapa[4] == p || codigo_mapa[4] + 1 == p || \
  codigo_mapa[5] == p || codigo_mapa[5] + 1 == p || \
  codigo_mapa[6] == p || codigo_mapa[6] + 1 == p || \
  codigo_mapa[7] == p || codigo_mapa[7] + 1 == p


unsigned int pagina_libre = BASE_AREA_LIBRE;
unsigned int codigo_virtual = BASE_TAREA_VIRTUAL;
unsigned int codigo_virtual_tanques[CANT_TANQUES];
unsigned int codigo_mapa[CANT_TANQUES];


/* ==========================================================================
 * RANDOMNEEEESSSS!!
 * ========================================================================== */

unsigned int _x = 1;
const unsigned int _a = 0x41c64e6d,
                   _c = 0x00003039,
                   _m = 0x7fffffff;

void srand (unsigned int seed) {
  _x = seed;
}

unsigned int rand () {
  _x = (_a*_x + _c) & _m;
  return _x;
}

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
  page_directory_entry_t *pd;
  page_table_entry_t *p;
  
  pd = (page_directory_entry_t *) 0x27000;
  for(i = 0; i < 1024; i++) {
    pd[i].p = 0;
    pd[i].rw = 1;
    pd[i].us = 0;
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
  pd[0].base = 0x28;
  pd[1].base = 0x29;
  pd[2].base = 0x2a;
  pd[3].base = 0x2b;
  
  
  p = (page_table_entry_t *) (pd[0].base << 12);
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
  
  p = (page_table_entry_t *) (pd[1].base << 12);
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
  
  p = (page_table_entry_t *) (pd[2].base << 12);
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
  
  p = (page_table_entry_t *) (pd[3].base << 12);
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
  for (i = 452; i < 1024; i++) {
    p[i].p = 0;
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
}


unsigned int mmu_inicializar_dir_tarea (task_id_t tid) {
  
  page_directory_entry_t *pd;
  page_table_entry_t *p;
  unsigned int i, cr3, codigo, pos;
  /* cr3 y codigo funcionan como punteros. */
  
  
  cr3 = mmu_crear_pagina();
  pd = (page_directory_entry_t *) cr3;
  
  for(i = 0; i < 1024; i++) {
    pd[i].p = 0;
    pd[i].rw = 1;
    pd[i].us = 0;
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
  
  pd[0].base = mmu_crear_pagina() >> 12;
  pd[1].base = mmu_crear_pagina() >> 12;
  pd[2].base = mmu_crear_pagina() >> 12;
  pd[3].base = mmu_crear_pagina() >> 12;
  
  
  p = (page_table_entry_t *) (pd[0].base << 12);
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
  
  p = (page_table_entry_t *) (pd[1].base << 12);
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
  
  p = (page_table_entry_t *) (pd[2].base << 12);
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
  
  p = (page_table_entry_t *) (pd[3].base << 12);
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
  for (i = 452; i < 1024; i++) {
    p[i].p = 0;
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
      codigo = 0xdeadc0de;
      break;
  }
  
  /**
   * TODO:
   *  - [X] Consultar el tema de copiado.
   *        Si, hay que copiar, a algun lado de _el_mapa_. En principio, lo
   *        hacemos lineal, despues se va randomizar.
   *  - [X] Consultar si el mapeo esta todo liso.
   *        No. Estamos mapeando solo una pagina. Debe mapear tooooodas las
   *        paginas.
   **/
  
  do {
    pos = rand() % (CAMPO_SIZE*CAMPO_SIZE);
  } while (ESTA_OCUPADA(pos));
  codigo_mapa[tid] = pos;
  pintar_posicion(tid + '1',
                  pos % CAMPO_SIZE,
                  pos / CAMPO_SIZE,
                  C_BG_GREEN | C_FG_LIGHT_BLUE);
  pintar_posicion(tid + '1',
                  (pos + 1) % CAMPO_SIZE,
                  (pos + 1) / CAMPO_SIZE,
                  C_BG_GREEN | C_FG_LIGHT_BLUE);
  copiar_memoria(BASE_EL_MAPA + PAGE_SIZE*pos, codigo, 2*PAGE_SIZE);
 
  mmu_mapear_pagina(codigo_virtual,
                    cr3,
                    BASE_EL_MAPA + PAGE_SIZE*pos,
                    3);
  mmu_mapear_pagina(codigo_virtual + PAGE_SIZE,
                    cr3,
                    BASE_EL_MAPA + PAGE_SIZE*(pos + 1),
                    3);
  /* 3 = 0b11 => r/w = 1, u/s = 1 */
  
  return cr3;
  
}


void mmu_inicializar () {
  
  unsigned int i;
  
  mmu_inicializar_dir_kernel();
  
  for (i = 0; i < CANT_TANQUES; i++) {
    codigo_virtual_tanques[i] = BASE_TAREA_VIRTUAL + 2*PAGE_SIZE;
    codigo_mapa[i] = 0;
  }
  
}


void mmu_mapear_pagina (unsigned int virtual,
                        unsigned int cr3,
                        unsigned int fisica,
                        unsigned int attr)
{
  
  unsigned int i,dir_idx, tabla_idx, us, rw;
  page_directory_entry_t *dir;
  page_table_entry_t *tabla;
  
  rw =  attr & 1;
  us = (attr & 2) >> 1;
  
  dir = (page_directory_entry_t *) cr3;
  dir_idx = virtual >> 22;
  
  if (!dir[dir_idx].p) {
    dir[dir_idx].base = (mmu_crear_pagina() >> 12);
    dir[dir_idx].p = 1;
    dir[dir_idx].rw = rw;
    dir[dir_idx].us = us;
    
    tabla = (page_table_entry_t *) (dir[dir_idx].base << 12);
    for (i = 0; i < 1024; i++) {
      tabla[i].p = 0;
      tabla[i].rw = rw;
      tabla[i].us = us;
      tabla[i].pwt = 0;
      tabla[i].pcd = 0;
      tabla[i].a = 0;
      tabla[i].d = 0;
      tabla[i].pat = 0;
      tabla[i].g = 0;
      tabla[i].disponible = 0;
      tabla[i].base = 0;
    }
  } else {
    tabla = (page_table_entry_t *) (dir[dir_idx].base << 12);
  }
  
  tabla_idx = (virtual & MASK_22_BAJOS) >> 12;
  
  tabla[tabla_idx].p = 1;
  tabla[tabla_idx].rw = rw;
  tabla[tabla_idx].us = us;
  
  tabla[tabla_idx].base = fisica >> 12;
  
  tlbflush();
  
}



void mmu_unmapear_pagina (unsigned int virtual, unsigned int cr3) {
  
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


void copiar_memoria (unsigned int dst, unsigned int src, unsigned int size) {
  unsigned char *d, *f;
  
  d = (unsigned char *) dst;
  f = (unsigned char *) src;
  
  while (size--)
    *d++ = *f++;
}
