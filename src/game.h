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
#include "sched.h"
#include "colors.h"


struct posicion_s {
  unsigned char x;
  unsigned char y;
};

typedef struct posicion_s posicion_t;


typedef enum direccion_e { NO = 14, N  = 11, NE = 12,
                           O  = 44, C  = 0,  E  = 22,
                           SO = 34, S  = 33, SE = 32 } direccion;


void           game_inicializar        ();
unsigned int   game_mover              (unsigned int id, direccion d);
unsigned int   game_misil              (unsigned int id,
                                        int val_x,
                                        int val_y,
                                        unsigned int misil,
                                        unsigned int size);
unsigned int   game_minar              (unsigned int id, direccion d);
posicion_t     obtener_posicion_tanque (unsigned int id);

#endif  /* !__GAME_H__ */
