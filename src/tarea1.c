/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h" 

void pisar(unsigned int m);

void task() {
    /* Tarea 1 */
  char aa[512];
  
  syscall_misil(2, 2, (unsigned int)aa, 512);
  syscall_misil(3, 3, (unsigned int)aa, 512);
  syscall_misil(4, 4, (unsigned int)aa, 512);
  syscall_misil(5, 5, (unsigned int)aa, 512);
  syscall_misil(6, 6, (unsigned int)aa, 512);
  syscall_misil(7, 7, (unsigned int)aa, 512);
  syscall_misil(8, 8, (unsigned int)aa, 512);
  syscall_misil(9, 9, (unsigned int)aa, 512);
  
  syscall_misil( -1,  -1, (unsigned int)aa, 512);
  syscall_misil(-10, -10, (unsigned int)aa, 512);
  syscall_misil( 10, -10, (unsigned int)aa, 512);
  syscall_misil(-10,  10, (unsigned int)aa, 512);
  syscall_misil( -8,  -8, (unsigned int)aa, 512);
  syscall_misil(  8,   8, (unsigned int)aa, 512);
  syscall_misil(  8,  -8, (unsigned int)aa, 512);
  syscall_misil( -8,   8, (unsigned int)aa, 512);
  
  unsigned int i;
  for(i=0;i<10;i++) {
    unsigned int mapn = syscall_mover(N);
    pisar(mapn-0x1000+1);
    unsigned int mapo = syscall_mover(O);
    pisar(mapo-0x1000+1);
  }
  
  unsigned int maps = syscall_mover(S);
  pisar(maps-0x1000+1);
  
  for(i=0;i<100;i++) {
    unsigned int mapno = syscall_mover(NO);
    pisar(mapno-0x1000+1);
  }
  
  while(1) { __asm __volatile("mov $1, %%eax":::"eax"); }
}


void pisar(unsigned int m) {
  unsigned int i;
  char* p = (char*)m;
  for(i=0;i<PAGE_SIZE;i++) {
    p[i]=0xcd;
  }
}
