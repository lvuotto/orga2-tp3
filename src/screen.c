/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"


void pintar_posicion (char id,
                      unsigned int x,
                      unsigned int y,
                      unsigned char c)
{
  unsigned short *p;
  
  p = (unsigned short *) 0xb8000 + (80*y + x);
  *p = (c << 8) | id;
}


void pintar_posiciones_iniciales () {
  unsigned short i, pos, coord_x, coord_y; 
  
  for (i = 0; i < CANT_TANQUES; i++) {
    pos = obtener_posicion_tanque(i);
    coord_y = pos >> 8;
    coord_x = pos & 0xff;
    
    posiciones_ocupadas[coord_y][coord_x    ] = TRUE;
    posiciones_ocupadas[coord_y][coord_x - 1] = TRUE;
    
    pintar_posicion(i + '1', coord_x - 1, coord_y, 0x7f);
    pintar_posicion(i + '1', coord_x    , coord_y, 0x7f);
  }
}

