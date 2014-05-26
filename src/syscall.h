/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define LS_INLINE static __inline __attribute__((always_inline))

#define SYS_MOVER     0x83D
#define SYS_MISIL     0x911
#define SYS_MINAR     0x355

/*
 * Syscalls
 */

LS_INLINE unsigned int syscall_mover(direccion d) {
    int ret;

    __asm __volatile(
        "mov %0, %%eax \n"
        "mov %1, %%ebx \n"
        "int $0x52     \n"
        : /* no output*/
        : "r" (SYS_MOVER), "m" (d)
        : "eax", "ebx"
    );

    __asm __volatile("mov %%eax, %0" : "=r" (ret));

    return ret;
}

LS_INLINE unsigned int syscall_misil(int x, int y, unsigned int addr, unsigned int size) {
    int ret;

    __asm __volatile(
        "mov %0, %%eax \n"
        "mov %1, %%ebx \n"
        "mov %2, %%ecx \n"
        "mov %3, %%edx \n"
        "mov %4, %%esi \n"
        "int $0x52     \n"
        : /* no output*/
        : "r" (SYS_MISIL), "m" (x), "m" (y), "m" (addr) , "m" (size)
        : "eax", "ebx", "ecx", "edx", "esi"
    );

    __asm __volatile("mov %%eax, %0" : "=r" (ret));

    return ret;
}

LS_INLINE unsigned int syscall_minar(direccion d) {
    int ret;

    __asm __volatile(
        "mov %0, %%eax \n"
        "mov %1, %%ebx \n"
        "int $0x52     \n"
        : /* no output*/
        : "r" (SYS_MINAR), "m" (d)
        : "eax", "ebx"
    );

    __asm __volatile("mov %%eax, %0" : "=r" (ret));

    return ret;
}

#endif  /* !__SYSCALL_H__ */
