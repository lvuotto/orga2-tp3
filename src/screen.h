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


void pintar_posicion_tanque (unsigned int id, unsigned int x, unsigned int y);


#endif  /* !__SCREEN_H__ */
