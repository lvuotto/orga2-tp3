/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"


#define OBTENER_POSICION_TANQUE(id) &posicion[id];


char campo_minado[CAMPO_SIZE][CAMPO_SIZE];
unsigned char posiciones_ocupadas[CAMPO_SIZE][CAMPO_SIZE];
posicion_t posicion[CANT_TANQUES];
extern unsigned int codigo_virtual_tanques[CANT_TANQUES];


void game_inicializar() {
  unsigned int i;
  
  for (i = 0; i < CAMPO_SIZE*CAMPO_SIZE; i++) {
    campo_minado[i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas[i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
  }
  
  for (i = 0; i < CANT_TANQUES; i++) {
    posicion[i].x = 5 + 5*i;
    posicion[i].y = 5 + 5*i;
  }
}


posicion_t obtener_posicion_tanque (unsigned int id) {
  return posicion[id];
}


unsigned int game_mover (unsigned int id, direccion d) {
  unsigned int cr3;
  posicion_t *pos;
  
  pos = OBTENER_POSICION_TANQUE(id);
  breakpoint();
  pos->x += CAMPO_SIZE;
  pos->y += CAMPO_SIZE;
  
  switch (d) {
    case NO:
      pos->y--;
      pos->x--;
      break;
    case N:
      pos->y--;
      break;
    case NE:
      pos->y--;
      pos->x++;
      break;
    case O:
      pos->x--;
      break;
    case C:
      break;
    case E:
      pos->x++;
      break;
    case SO:
      pos->y++;
      pos->x--;
      break;
    case S:
      pos->y++;
      break;
    case SE:
      pos->y++;
      pos->x++;
      break;
    default:
      break;
  }
  
  pos->y %= CAMPO_SIZE;
  pos->x %= CAMPO_SIZE;
  
  if (campo_minado[pos->y][pos->x] &&
      (campo_minado[pos->y][pos->x] >> 1) != id)
  {
    sched_desalojar_tarea(id);
    campo_minado[pos->y][pos->x] = FALSE;
    return FALSE;
  }
  
  if (posiciones_ocupadas[pos->y][pos->x]) {
    pintar_posicion('X', pos->x, pos->y, C_BG_BROWN | C_FG_BLACK);
  } else {
    cr3 = tss_get_cr3(id);
    codigo_virtual_tanques[id] += PAGE_SIZE;
    mmu_mapear_pagina(codigo_virtual_tanques[id],
                      cr3,
                      0x400000 + CAMPO_SIZE*pos->y*PAGE_SIZE + pos->x*PAGE_SIZE,
                      3);
    pintar_posicion(id + '1', pos->x, pos->y, C_BG_LIGHT_GREY | C_FG_BLACK);
    posiciones_ocupadas[pos->y][pos->x] = TRUE;
  }
  
  return codigo_virtual_tanques[id];
}

unsigned int game_misil (unsigned int id,
                         int val_x,
                         int val_y,
                         unsigned int misil,
                         unsigned int size)
{
  posicion_t *pos;
  
  if (size > PAGE_SIZE) {
    sched_desalojar_tarea(id);
    return FALSE;
  }
  
  if (val_x < -CAMPO_SIZE || val_x > CAMPO_SIZE) {
    sched_desalojar_tarea(id);
    return FALSE;
  }
  
  if (val_y < -CAMPO_SIZE || val_y > CAMPO_SIZE) {
    sched_desalojar_tarea(id);
    return FALSE;
  }
  
  pos = OBTENER_POSICION_TANQUE(id);
  pos->x = (pos->x + CAMPO_SIZE + val_x) % CAMPO_SIZE;
  pos->y = (pos->y + CAMPO_SIZE + val_y) % CAMPO_SIZE;
  
  if (campo_minado[pos->y][pos->x]) {
    campo_minado[pos->y][pos->x] = FALSE;
    pintar_posicion(0, pos->x, pos->y, C_BG_GREEN | C_FG_BLACK);
  } else {
    copiar_memoria(0x400000 + CAMPO_SIZE*pos->y*PAGE_SIZE + pos->x*PAGE_SIZE,
                   misil,
                   size);
    pintar_posicion(id + '1',
                    pos->x,
                    pos->y,
                    C_BLINK | C_BG_MAGENTA | C_FG_LIGHT_RED);
  }
  
  return TRUE;
}

unsigned int game_minar (unsigned int id, direccion d) {
  posicion_t *pos;
  
  pos = OBTENER_POSICION_TANQUE(id);
  
  switch (d) {
    case NO:
      pos->y--;
      pos->x--;
      break;
    case N:
      pos->y--;
      break;
    case NE:
      pos->y--;
      pos->x++;
      break;
    case O:
      pos->x--;
      break;
    case C:
      break;
    case E:
      pos->x++;
      break;
    case SO:
      pos->y++;
      pos->x--;
      break;
    case S:
      pos->y++;
      break;
    case SE:
      pos->y++;
      pos->x++;
      break;
    default:
      break;
  }
  
  pos->y %= CAMPO_SIZE;
  pos->x %= CAMPO_SIZE;
  
  campo_minado[pos->y][pos->x] = (id << 1) | TRUE;
  pintar_posicion('X', pos->x, pos->y, C_BG_GREEN | C_FG_RED);
  
  return TRUE;
}



