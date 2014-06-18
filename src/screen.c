/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"


void pintar_posicion_tanque (unsigned int id, unsigned int pos) {
  unsigned short *p;
  unsigned int coord_x, coord_y;
  
  coord_y = ((pos - 0x400000) / PAGE_SIZE) / 50;
  coord_x = ((pos - 0x400000) / PAGE_SIZE) % 50;
  
  p = (unsigned short *) 0xb8000 + (80*coord_y + coord_x);
  /* le saco el offset del mapa, lo divido por PAGE_SIZE asi queda en bytes,
   * y lo multiplico por 2, asi queda como unidades de la memoria de video. */
  *p = (0x70 << 8) | ((char) id + '1');
}

