/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

char campo_minado[CAMPO_SIZE][CAMPO_SIZE];
extern unsigned int codigo_virtual_tanques[CANT_TANQUES];

void game_inicializar() {
  unsigned int i;
  
  for (i = 0; i < CAMPO_SIZE*CAMPO_SIZE; i++) {
    campo_minado[i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
  }
}

unsigned int game_mover (unsigned int id, direccion d) {
  unsigned int pos, coord_x, coord_y, cr3;
  
  //~ breakpoint();
  pos = obtener_posicion_tanque(id);
  
  switch (d) {
    case NO:
      pos = pos - 50*PAGE_SIZE - PAGE_SIZE;
      break;
    case N:
      pos = pos - 50*PAGE_SIZE;
      break;
    case NE:
      pos = pos - 50*PAGE_SIZE + PAGE_SIZE;
      break;
    case O:
      pos = pos - PAGE_SIZE;
      break;
    case C:
      pos = pos;
      break;
    case E:
      pos = pos + PAGE_SIZE;
      break;
    case SO:
      pos = pos + 50*PAGE_SIZE - PAGE_SIZE;
      break;
    case S:
      pos = pos + 50*PAGE_SIZE;
      break;
    case SE:
      pos = pos + 50*PAGE_SIZE + PAGE_SIZE;
      break;
    default:
      break;
  }
  
  if (pos >= 0x400000 + 50*50*PAGE_SIZE) pos -= 50*50*PAGE_SIZE;
  if (pos <  0x400000)                   pos += 50*50*PAGE_SIZE;
  
  coord_y = ((pos - 0x400000) / PAGE_SIZE) / 50;
  coord_x = ((pos - 0x400000) / PAGE_SIZE) % 50;
  
  if (campo_minado[coord_y][coord_x]) {
    desalojar_tarea(id);
    campo_minado[coord_y][coord_x] = FALSE;
    return FALSE;
  }
  
  cr3 = tss_get_cr3(id);
  codigo_virtual_tanques[id] += PAGE_SIZE;
  mmu_mapear_pagina(codigo_virtual_tanques[id], cr3, pos, 3);
  pintar_posicion_tanque(id, pos);
  
  
  return TRUE;
}

unsigned int game_misil (unsigned int id,
                         int val_x,
                         int val_y,
                         unsigned int misil,
                         unsigned int size)
{
  unsigned int pos;
  
  if (size > PAGE_SIZE) {
    desalojar_tarea(id);
    return FALSE;
  }
  
  if (val_x < -50 || val_x > 50) {
    desalojar_tarea(id);
    return FALSE;
  }
  
  if (val_y < -50 || val_y > 50) {
    desalojar_tarea(id);
    return FALSE;
  }
  
  pos = obtener_posicion_tanque(id);
  pos = pos + 50*val_y*PAGE_SIZE + val_x*PAGE_SIZE;
  copiar_memoria(pos, misil, size);
  
  return TRUE;
}

unsigned int game_minar (unsigned int id, direccion d) {
  unsigned int pos, coord_x, coord_y;
  
  pos = obtener_posicion_tanque(id);
  
  switch (d) {
    case NO:
      pos = pos - 50*PAGE_SIZE - PAGE_SIZE;
      break;
    case N:
      pos = pos - 50*PAGE_SIZE;
      break;
    case NE:
      pos = pos - 50*PAGE_SIZE + PAGE_SIZE;
      break;
    case O:
      pos = pos - PAGE_SIZE;
      break;
    case C:
      pos = pos;
      break;
    case E:
      pos = pos + PAGE_SIZE;
      break;
    case SO:
      pos = pos + 50*PAGE_SIZE - PAGE_SIZE;
      break;
    case S:
      pos = pos + 50*PAGE_SIZE;
      break;
    case SE:
      pos = pos + 50*PAGE_SIZE + PAGE_SIZE;
      break;
    default:
      break;
  }
  
  if (pos >= 0x400000 + 50*50*PAGE_SIZE) pos -= 50*50*PAGE_SIZE;
  if (pos <  0x400000)                   pos += 50*50*PAGE_SIZE;
  
  coord_y = ((pos - 0x400000) / PAGE_SIZE) / 50;
  coord_x = ((pos - 0x400000) / PAGE_SIZE) % 50;
  
  campo_minado[coord_y][coord_x] = TRUE;
  
  return TRUE;
}



