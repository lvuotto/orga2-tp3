/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"


#define C2H(x) ( (x) < 10 ? '0' + (x) : 'A' - 10 + (x) )
#define IMPRIMER_HEX_8(v, col, fila)                                      \
  video = (unsigned short *) 0xb8000 + (80*(col) + (fila));               \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >> 28) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >> 24) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >> 20) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >> 16) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >> 12) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >>  8) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >>  4) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(  (v)        & 0xf)
#define IMPRIMER_HEX_4(v, col, fila)                                      \
  video = (unsigned short *) 0xb8000 + (80*(col) + (fila));               \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >> 12) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >>  8) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(( (v) >>  4) & 0xf);  \
  *video++ = ((C_BG_BLACK | C_FG_WHITE) << 8) | C2H(  (v)        & 0xf)


extern unsigned char posiciones_ocupadas_tanques[CANT_TANQUES][CAMPO_SIZE][CAMPO_SIZE];
extern unsigned char posiciones_ocupadas[CAMPO_SIZE][CAMPO_SIZE];
extern informe_de_fallos_t fallos_tanques[CANT_TANQUES];


void pintar_posicion (char c,
                      unsigned int x,
                      unsigned int y,
                      unsigned char color)
{
  static unsigned short *p;
  
  p = (unsigned short *) 0xb8000 + (80*y + x);
  *p = (color << 8) | c;
}


void pintar_string (char* msg,
                    unsigned int x,
                    unsigned int y,
                    unsigned char color)
{
  static unsigned short *p;
  
  p = (unsigned short *) 0xb8000 + (80*y + x);
  while (*msg){
    *p++ = (color << 8) | *msg++;
  }
  
}


void pintar_posiciones_iniciales () {
  unsigned short i;
  posicion_t pos;
  
  for (i = 0; i < CANT_TANQUES; i++) {
    pos = obtener_posicion_tanque(i);
    
    posiciones_ocupadas_tanques[i][pos.y][pos.x                                ] = TRUE;
    posiciones_ocupadas_tanques[i][pos.y][(pos.x + CAMPO_SIZE - 1) % CAMPO_SIZE] = TRUE;
    
    posiciones_ocupadas[pos.y][pos.x                                ] = TRUE;
    posiciones_ocupadas[pos.y][(pos.x + CAMPO_SIZE - 1) % CAMPO_SIZE] = TRUE;
    
    pintar_posicion(i + '1', (pos.x + CAMPO_SIZE - 1) % CAMPO_SIZE, pos.y, 0x7f);
    pintar_posicion(i + '1', pos.x                                , pos.y, 0x7f);
  }
}




void mostrar_contexto (task_id_t tid) {
  static unsigned short *video;
  
  pintar_string("Tanque", 52, 6, C_BG_BLACK | C_FG_WHITE);
  pintar_posicion('1' + tid, 59, 6, C_BG_BLACK | C_FG_WHITE);
  
  IMPRIMER_HEX_8(tss_tanques[tid].eax   ,  8, 57);
  IMPRIMER_HEX_8(tss_tanques[tid].ebx   , 10, 57);
  IMPRIMER_HEX_8(tss_tanques[tid].ecx   , 12, 57);
  IMPRIMER_HEX_8(tss_tanques[tid].edx   , 14, 57);
  IMPRIMER_HEX_8(tss_tanques[tid].esi   , 16, 57);
  IMPRIMER_HEX_8(tss_tanques[tid].edi   , 18, 57);
  IMPRIMER_HEX_8(tss_tanques[tid].ebp   , 20, 57);
  IMPRIMER_HEX_4(tss_tanques[tid].ds    , 28, 57);
  IMPRIMER_HEX_4(tss_tanques[tid].es    , 30, 57);
  IMPRIMER_HEX_4(tss_tanques[tid].fs    , 32, 57);
  IMPRIMER_HEX_4(tss_tanques[tid].gs    , 34, 57);
  IMPRIMER_HEX_8(tss_tanques[tid].eflags, 38, 60);
  IMPRIMER_HEX_8(fallos_tanques[tid].cr0,  8, 71);
  IMPRIMER_HEX_8(fallos_tanques[tid].cr2, 10, 71);
  IMPRIMER_HEX_8(tss_tanques[tid].cr3   , 12, 71);
  IMPRIMER_HEX_8(fallos_tanques[tid].cr4, 14, 71);
  
  if (sched_estado_tarea(tid)) {
    IMPRIMER_HEX_8(tss_tanques[tid].esp   , 22, 57);
    IMPRIMER_HEX_8(tss_tanques[tid].eip   , 24, 57);
    IMPRIMER_HEX_4(tss_tanques[tid].cs    , 26, 57);
    IMPRIMER_HEX_4(tss_tanques[tid].ss    , 36, 57);
  } else {
    IMPRIMER_HEX_8(fallos_tanques[tid].esp, 22, 57);
    IMPRIMER_HEX_8(fallos_tanques[tid].eip, 24, 57);
    IMPRIMER_HEX_4(fallos_tanques[tid].cs , 26, 57);
    IMPRIMER_HEX_4(fallos_tanques[tid].ss , 36, 57);
  }
  
  pintar_posicion(tid + '1', 79, 0, C_BG_RANDOM | C_FG_WHITE);
  pintar_string(fallos_tanques[tid].mensaje, 52, 42, C_BG_BLACK | C_FG_LIGHT_RED);
}

