/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"


typedef enum direccion_e { NE = 12, N  = 11, NO = 14,
                           E  = 22, C  = 0,  O  = 44,
                           SE = 32, S  = 33, SO = 34 } direccion;


void game_inicializar();

unsigned int game_mover(unsigned int id, direccion d);

unsigned int game_misil(unsigned int id, int val_x, int val_y, unsigned int misil, unsigned int size);

unsigned int game_minar(unsigned int id, direccion d);

#endif  /* !__GAME_H__ */
