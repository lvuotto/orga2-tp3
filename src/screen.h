/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80


#include "defines.h"
#include "i386.h"
#include "mmu.h"
#include "game.h"


void pintar_posicion             (char c,
                                  unsigned int x,
                                  unsigned int y,
                                  unsigned char color);
void pintar_string               (char *msg,
                                  unsigned int x,
                                  unsigned int y,
                                  unsigned char color);
void pintar_posiciones_iniciales ();
void mostrar_contexto (task_id_t tid);


#endif  /* !__SCREEN_H__ */
