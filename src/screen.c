/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"


void pintar_posicion (char c,
                      unsigned int x,
                      unsigned int y,
                      unsigned char color)
{
  unsigned short *p;
  
  p = (unsigned short *) 0xb8000 + (80*y + x);
  *p = (color << 8) | c;
}


void pintar_posiciones_iniciales () {
  unsigned short i;
  posicion_t pos;
  
  for (i = 0; i < CANT_TANQUES; i++) {
    pos = obtener_posicion_tanque(i);
    
    posiciones_ocupadas[pos.y][pos.x                        ] = TRUE;
    posiciones_ocupadas[pos.y][(pos.x + 50 - 1) % CAMPO_SIZE] = TRUE;
    
    pintar_posicion(i + '1', (pos.x + 50 - 1) % CAMPO_SIZE, pos.y, 0x7f);
    pintar_posicion(i + '1', pos.x                        , pos.y, 0x7f);
  }
}

