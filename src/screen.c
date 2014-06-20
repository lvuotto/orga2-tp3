/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"


void pintar_posicion_tanque (unsigned int id,
                             unsigned int x,
                             unsigned int y,
                             unsigned char c)
{
  unsigned short *p;
  
  p = (unsigned short *) 0xb8000 + (80*y + x);
  *p = (c << 8) | ((char) id + '1');
}


void pintar_posiciones_iniciales () {
  unsigned int i, pos, coord_x, coord_y;
  
  pos = 0x400000 + 0x1000;
  
  for (i = 0; i < CANT_TANQUES; i++) {
    pos = obtener_posicion_tanque(i);
    coord_y = ((pos - 0x400000) / PAGE_SIZE) / 50;
    coord_x = ((pos - 0x400000) / PAGE_SIZE) % 50;
    
    pintar_posicion_tanque(i, coord_x - 1, coord_y, 0x7f);
    pintar_posicion_tanque(i, coord_x    , coord_y, 0x7f);
  }
}

