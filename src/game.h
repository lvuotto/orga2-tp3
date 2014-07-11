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
#include "i386.h"


struct posicion_s {
  unsigned char x;
  unsigned char y;
};


struct contexto_desalojo_s {
  unsigned int   eax;
  unsigned int   ebx;
  unsigned int   ecx;
  unsigned int   edx;
  unsigned int   esi;
  unsigned int   edi;
  unsigned int   ebp;
  unsigned int   esp;
  unsigned int   eip;
  unsigned short cs;
  unsigned short ds;
  unsigned short es;
  unsigned short fs;
  unsigned short gs;
  unsigned short ss;
  unsigned int   eflags;
  unsigned int   cr3;
  
  /**
   * cr0, cr2, y cr4 son globales. No hay necesidad de
   * definirlos localmente en cada tanque. Ademas, cr2
   * solo se seteara en caso de un #PF.
   **/
} __attribute__((__packed__, aligned (8)));


typedef struct posicion_s          posicion_t;
typedef struct contexto_desalojo_s contexto_desalojo_t;


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
