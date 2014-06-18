/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"


void pintar_posicion_tanque (unsigned int id, unsigned int x, unsigned int y) {
  unsigned short *p;
  
  p = (unsigned short *) 0xb8000 + (80*y + x);
  *p = (0x70 << 8) | ((char) id + '1');
}

