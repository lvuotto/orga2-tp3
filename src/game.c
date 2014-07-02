/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

char campo_minado[CAMPO_SIZE][CAMPO_SIZE];
unsigned char posiciones_ocupadas[CAMPO_SIZE][CAMPO_SIZE];
unsigned short posicion[CANT_TANQUES];
extern unsigned int codigo_virtual_tanques[CANT_TANQUES];


void game_inicializar() {
  unsigned int i;
  
  for (i = 0; i < CAMPO_SIZE*CAMPO_SIZE; i++) {
    campo_minado[i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas[i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
  }
  
  for (i = 0; i < CANT_TANQUES; i++) {
    posicion[i] = ((3 + 6*i) << 8) | (3 + 6*i);
  }
}


unsigned short obtener_posicion_tanque (unsigned int id) {
  return posicion[id % CANT_TANQUES];
}


unsigned int game_mover (unsigned int id, direccion d) {
  unsigned int cr3;
  unsigned short pos, coord_x, coord_y;
  
  pos = posicion[id];
  breakpoint();
  coord_x = (pos & 0xff) + 50;
  coord_y = (pos >> 8) + 50;
  
  switch (d) {
    case NO:
      coord_y--;
      coord_x--;
      break;
    case N:
      coord_y--;
      break;
    case NE:
      coord_y--;
      coord_x++;
      break;
    case O:
      coord_x--;
      break;
    case C:
      break;
    case E:
      coord_x++;
      break;
    case SO:
      coord_y++;
      coord_x--;
      break;
    case S:
      coord_y++;
      break;
    case SE:
      coord_y++;
      coord_x++;
      break;
    default:
      break;
  }
  
  coord_y %= CAMPO_SIZE;
  coord_x %= CAMPO_SIZE;
  
  if (campo_minado[coord_y][coord_x] &&
      (campo_minado[coord_y][coord_x] >> 1) != id)
  {
    sched_desalojar_tarea(id);
    campo_minado[coord_y][coord_x] = FALSE;
    return FALSE;
  }
  
  if (posiciones_ocupadas[coord_y][coord_x]) {
    pintar_posicion('X', coord_x, coord_y, C_BG_BROWN | C_FG_BLACK);
  } else {
    cr3 = tss_get_cr3(id);
    codigo_virtual_tanques[id] += PAGE_SIZE;
    mmu_mapear_pagina(codigo_virtual_tanques[id], cr3, pos, 3);
    pintar_posicion(id + '1', coord_x, coord_y, C_BG_LIGHT_GREY | C_FG_BLACK);
    posiciones_ocupadas[coord_y][coord_x] = TRUE;
  }
  
  posicion[id] = ((coord_y&0xFF) << 8) | (coord_x & 0xFF);
  
  return codigo_virtual_tanques[id];
}

unsigned int game_misil (unsigned int id,
                         int val_x,
                         int val_y,
                         unsigned int misil,
                         unsigned int size)
{
  unsigned short pos, coord_x, coord_y;
  
  if (size > PAGE_SIZE) {
    sched_desalojar_tarea(id);
    return FALSE;
  }
  
  if (val_x < -50 || val_x > 50) {
    sched_desalojar_tarea(id);
    return FALSE;
  }
  
  if (val_y < -50 || val_y > 50) {
    sched_desalojar_tarea(id);
    return FALSE;
  }
  
  pos = obtener_posicion_tanque(id);
  coord_x = pos & 0xff;
  coord_y = pos >> 8;
  
  coord_x = (coord_x + 50 + val_x) % CAMPO_SIZE;
  coord_y = (coord_y + 50 + val_y) % CAMPO_SIZE;
  
  if (campo_minado[coord_y][coord_x]) {
    campo_minado[coord_y][coord_x] = FALSE;
    pintar_posicion(0, coord_x, coord_y, C_BG_GREEN | C_FG_BLACK);
  } else {
    copiar_memoria(0x400000 + coord_x*PAGE_SIZE + 50*coord_y*PAGE_SIZE,
                   misil,
                   size);
    pintar_posicion(id + '1',
                    coord_x,
                    coord_y,
                    C_BLINK | C_BG_MAGENTA | C_FG_LIGHT_RED);
  }
  
  return TRUE;
}

unsigned int game_minar (unsigned int id, direccion d) {
  unsigned short pos, coord_x, coord_y;
  
  pos = posicion[id];
  coord_x = (pos & 0xff) + 50;
  coord_y = (pos >> 8) + 50;
  
  switch (d) {
    case NO:
      coord_y--;
      coord_x--;
      break;
    case N:
      coord_y--;
      break;
    case NE:
      coord_y--;
      coord_x++;
      break;
    case O:
      coord_x--;
      break;
    case C:
      break;
    case E:
      coord_x++;
      break;
    case SO:
      coord_y++;
      coord_x--;
      break;
    case S:
      coord_y++;
      break;
    case SE:
      coord_y++;
      coord_x++;
      break;
    default:
      break;
  }
  
  coord_y %= CAMPO_SIZE;
  coord_x %= CAMPO_SIZE;
  
  campo_minado[coord_y][coord_x] = (id << 1) | TRUE;
  pintar_posicion('X', coord_x, coord_y, C_BG_GREEN | C_FG_RED);
  
  return TRUE;
}



