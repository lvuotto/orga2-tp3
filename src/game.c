/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"


char campo_minado[CAMPO_SIZE][CAMPO_SIZE];
unsigned char posiciones_ocupadas_tanques[CANT_TANQUES][CAMPO_SIZE][CAMPO_SIZE];
unsigned char posiciones_ocupadas[CAMPO_SIZE][CAMPO_SIZE];
posicion_t posicion[CANT_TANQUES];
extern unsigned int codigo_virtual_tanques[CANT_TANQUES];
char poner_pausa;
contexto_desalojo_t ctx_tanques[CANT_TANQUES];


void game_inicializar() {
  unsigned int i;
  
  for (i = 0; i < CAMPO_SIZE*CAMPO_SIZE; i++) {
    campo_minado[i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas[i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas_tanques[0][i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas_tanques[1][i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas_tanques[2][i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas_tanques[3][i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas_tanques[4][i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas_tanques[5][i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas_tanques[6][i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
    posiciones_ocupadas_tanques[7][i/CAMPO_SIZE][i % CAMPO_SIZE] = FALSE;
  }
  
  for (i = 0; i < CANT_TANQUES; i++) {
    posicion[i].x = 5 + 5*i;
    posicion[i].y = 5 + 5*i;
    ctx_tanques[i].eax   = tss_tanques[i].eax;
    ctx_tanques[i].ebx   = tss_tanques[i].ebx;
    ctx_tanques[i].ecx   = tss_tanques[i].ecx;
    ctx_tanques[i].edx   = tss_tanques[i].edx;
    ctx_tanques[i].esi   = tss_tanques[i].esi;
    ctx_tanques[i].edi   = tss_tanques[i].edi;
    ctx_tanques[i].ebp   = tss_tanques[i].ebp;
    ctx_tanques[i].esp   = tss_tanques[i].esp;
    ctx_tanques[i].eip   = tss_tanques[i].eip;
    ctx_tanques[i].cs    = tss_tanques[i].cs;
    ctx_tanques[i].ds    = tss_tanques[i].ds;
    ctx_tanques[i].es    = tss_tanques[i].es;
    ctx_tanques[i].fs    = tss_tanques[i].fs;
    ctx_tanques[i].gs    = tss_tanques[i].gs;
    ctx_tanques[i].ss    = tss_tanques[i].ss;
    ctx_tanques[i].cr3   = tss_tanques[i].cr3;
  }
  
  poner_pausa = 0;
}


posicion_t obtener_posicion_tanque (unsigned int id) {
  return posicion[id];
}


unsigned int game_mover (unsigned int id, direccion d) {
  unsigned int cr3;
  posicion_t *pos;
  
  pos = &posicion[id];
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
  
  cr3 = tss_get_cr3(id);
  if (!posiciones_ocupadas_tanques[id][pos->y][pos->x]) {
    mmu_mapear_pagina(codigo_virtual_tanques[id],
                      cr3,
                      BASE_EL_MAPA + CAMPO_SIZE*pos->y*PAGE_SIZE + pos->x*PAGE_SIZE,
                      3);
    codigo_virtual_tanques[id] += PAGE_SIZE;
  }
  if (posiciones_ocupadas[pos->y][pos->x]) {
    pintar_posicion('X', pos->x, pos->y, C_BG_BROWN | C_FG_BLACK);
  } else {
    pintar_posicion(id + '1', pos->x, pos->y, C_BG_LIGHT_GREY | C_FG_BLACK);
    posiciones_ocupadas_tanques[id][pos->y][pos->x] = TRUE;
    posiciones_ocupadas[pos->y][pos->x] = TRUE;
  }
  
  return codigo_virtual_tanques[id] - 1;
}

unsigned int game_misil (unsigned int id,
                         int val_x,
                         int val_y,
                         unsigned int misil,
                         unsigned int size)
{
  posicion_t pos;
  
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
  
  pos = obtener_posicion_tanque(id);
  pos.x = (pos.x + CAMPO_SIZE + val_x) % CAMPO_SIZE;
  pos.y = (pos.y + CAMPO_SIZE + val_y) % CAMPO_SIZE;
  
  if (campo_minado[pos.y][pos.x]) {
    campo_minado[pos.y][pos.x] = FALSE;
  } else {
    copiar_memoria(BASE_EL_MAPA + CAMPO_SIZE*pos.y*PAGE_SIZE + pos.x*PAGE_SIZE,
                   misil,
                   size);
  }
  
  pintar_posicion('X', pos.x, pos.y, C_BG_BROWN | C_FG_GREEN);
  
  return TRUE;
}


unsigned int game_minar (unsigned int id, direccion d) {
  posicion_t pos;
  
  pos = obtener_posicion_tanque(id);
  pos.x += 50;
  pos.y += 50;
  
  switch (d) {
    case NO:
      pos.y--;
      pos.x--;
      break;
    case N:
      pos.y--;
      break;
    case NE:
      pos.y--;
      pos.x++;
      break;
    case O:
      pos.x--;
      break;
    case C:
      break;
    case E:
      pos.x++;
      break;
    case SO:
      pos.y++;
      pos.x--;
      break;
    case S:
      pos.y++;
      break;
    case SE:
      pos.y++;
      pos.x++;
      break;
    default:
      break;
  }
  
  pos.x %= CAMPO_SIZE;
  pos.y %= CAMPO_SIZE;
  
  campo_minado[pos.y][pos.x] = (id << 1) | TRUE;
  pintar_posicion('X', pos.x, pos.y, C_BLINK | C_BG_GREEN | C_FG_RED);
  
  return TRUE;
}


void actualizar_contexto (unsigned int i) {
  ctx_tanques[i].eax   = tss_tanques[i].eax;
  ctx_tanques[i].ebx   = tss_tanques[i].ebx;
  ctx_tanques[i].ecx   = tss_tanques[i].ecx;
  ctx_tanques[i].edx   = tss_tanques[i].edx;
  ctx_tanques[i].esi   = tss_tanques[i].esi;
  ctx_tanques[i].edi   = tss_tanques[i].edi;
  ctx_tanques[i].ebp   = tss_tanques[i].ebp;
  ctx_tanques[i].esp   = tss_tanques[i].esp;
  ctx_tanques[i].eip   = tss_tanques[i].eip;
  ctx_tanques[i].cs    = tss_tanques[i].cs;
  ctx_tanques[i].ds    = tss_tanques[i].ds;
  ctx_tanques[i].es    = tss_tanques[i].es;
  ctx_tanques[i].fs    = tss_tanques[i].fs;
  ctx_tanques[i].gs    = tss_tanques[i].gs;
  ctx_tanques[i].ss    = tss_tanques[i].ss;
  ctx_tanques[i].cr3   = tss_tanques[i].cr3;
}
