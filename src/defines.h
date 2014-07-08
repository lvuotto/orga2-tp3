/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__


/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_TANQUES            8
#define PAGE_SIZE               0x00001000
#define TASK_SIZE               2 * 4096
#define CAMPO_SIZE              50


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC           0


/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)


/* Direcciones de memoria utiles */
/* -------------------------------------------------------------------------- */
#define BASE_AREA_LIBRE    0x00100000
#define BASE_EL_MAPA       0x00400000
#define BASE_TAREA_VIRTUAL 0x08000000

#define DIR_TAREA_1 0x10000
#define DIR_TAREA_2 0x12000
#define DIR_TAREA_3 0x14000
#define DIR_TAREA_4 0x16000
#define DIR_TAREA_5 0x18000
#define DIR_TAREA_6 0x1a000
#define DIR_TAREA_7 0x1c000
#define DIR_TAREA_8 0x1e000


#endif  /* !__DEFINES_H__ */
