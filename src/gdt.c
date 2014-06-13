/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"


gdt_entry gdt[GDT_COUNT] = {
  /* Descriptor nulo*/
  /* Offset = 0x00 */
  [GDT_IDX_NULL_DESC] = (gdt_entry) {
    (unsigned short)    0x0000,         /* limit[0:15]  */
    (unsigned short)    0x0000,         /* base[0:15]   */
    (unsigned char)     0x00,           /* base[23:16]  */
    (unsigned char)     0x00,           /* type         */
    (unsigned char)     0x00,           /* s            */
    (unsigned char)     0x00,           /* dpl          */
    (unsigned char)     0x00,           /* p            */
    (unsigned char)     0x00,           /* limit[16:19] */
    (unsigned char)     0x00,           /* avl          */
    (unsigned char)     0x00,           /* l            */
    (unsigned char)     0x00,           /* db           */
    (unsigned char)     0x00,           /* g            */
    (unsigned char)     0x00,           /* base[31:24]  */
  },
  
  [GDT_IDX_CD_0] = (gdt_entry) {
    (unsigned short)    0xdcff,         /* limit[0:15]  */
    (unsigned short)    0x0000,         /* base[0:15]   */
    (unsigned char)     0x00,           /* base[23:16]  */
    (unsigned char)     0x0a,           /* type         */
    (unsigned char)     0x01,           /* s            */
    (unsigned char)     0x00,           /* dpl          */
    (unsigned char)     0x01,           /* p            */
    (unsigned char)     0x02,           /* limit[16:19] */
    (unsigned char)     0x00,           /* avl          */
    (unsigned char)     0x00,           /* l            */
    (unsigned char)     0x01,           /* db           */
    (unsigned char)     0x01,           /* g            */
    (unsigned char)     0x00,           /* base[31:24]  */
  },
  
  [GDT_IDX_CD_1] = (gdt_entry) {
    (unsigned short)    0xdcff,         /* limit[0:15]  */
    (unsigned short)    0x0000,         /* base[0:15]   */
    (unsigned char)     0x00,           /* base[23:16]  */
    (unsigned char)     0x0a,           /* type         */
    (unsigned char)     0x01,           /* s            */
    (unsigned char)     0x03,           /* dpl          */
    (unsigned char)     0x01,           /* p            */
    (unsigned char)     0x02,           /* limit[16:19] */
    (unsigned char)     0x00,           /* avl          */
    (unsigned char)     0x00,           /* l            */
    (unsigned char)     0x01,           /* db           */
    (unsigned char)     0x01,           /* g            */
    (unsigned char)     0x00,           /* base[31:24]  */
  },
  
  [GDT_IDX_DD_0] = (gdt_entry) {
    (unsigned short)    0xdcff,         /* limit[0:15]  */
    (unsigned short)    0x0000,         /* base[0:15]   */
    (unsigned char)     0x00,           /* base[23:16]  */
    (unsigned char)     0x02,           /* type         */
    (unsigned char)     0x01,           /* s            */
    (unsigned char)     0x00,           /* dpl          */
    (unsigned char)     0x01,           /* p            */
    (unsigned char)     0x02,           /* limit[16:19] */
    (unsigned char)     0x00,           /* avl          */
    (unsigned char)     0x00,           /* l            */
    (unsigned char)     0x01,           /* db           */
    (unsigned char)     0x01,           /* g            */
    (unsigned char)     0x00,           /* base[31:24]  */
  },
  
  [GDT_IDX_DD_1] = (gdt_entry) {
    (unsigned short)    0xdcff,         /* limit[0:15]  */
    (unsigned short)    0x0000,         /* base[0:15]   */
    (unsigned char)     0x00,           /* base[23:16]  */
    (unsigned char)     0x02,           /* type         */
    (unsigned char)     0x01,           /* s            */
    (unsigned char)     0x03,           /* dpl          */
    (unsigned char)     0x01,           /* p            */
    (unsigned char)     0x02,           /* limit[16:19] */
    (unsigned char)     0x00,           /* avl          */
    (unsigned char)     0x00,           /* l            */
    (unsigned char)     0x01,           /* db           */
    (unsigned char)     0x01,           /* g            */
    (unsigned char)     0x00,           /* base[31:24]  */
  },
  
  [GDT_IDX_SD] = (gdt_entry) {
    (unsigned short)    0x04ff,         /* limit[0:15]  */
    (unsigned short)    0x8000,         /* base[0:15]   */
    (unsigned char)     0x0b,           /* base[23:16]  */
    (unsigned char)     0x02,           /* type         */
    (unsigned char)     0x01,           /* s            */
    (unsigned char)     0x00,           /* dpl          */
    (unsigned char)     0x01,           /* p            */
    (unsigned char)     0x03,           /* limit[16:19] */
    (unsigned char)     0x00,           /* avl          */
    (unsigned char)     0x00,           /* l            */
    (unsigned char)     0x01,           /* db           */
    (unsigned char)     0x01,           /* g            */
    (unsigned char)     0x00,           /* base[31:24]  */
  },
  
  [GDT_TSS_TAREA_INICIAL] = (gdt_entry) {
    (unsigned short)    0x0067,         /* limit[0:15]  */
    (unsigned short)    0x0000,         /* base[0:15]   */
    (unsigned char)     0x00,           /* base[23:16]  */
    (unsigned char)     0x09,           /* type         */
    (unsigned char)     0x00,           /* s            */
    (unsigned char)     0x00,           /* dpl          */
    (unsigned char)     0x01,           /* p            */
    (unsigned char)     0x00,           /* limit[16:19] */
    (unsigned char)     0x00,           /* avl          */
    (unsigned char)     0x00,           /* l            */
    (unsigned char)     0x01,           /* db           */
    (unsigned char)     0x00,           /* g            */
    (unsigned char)     0x00,           /* base[31:24]  */
  },
  
  [GDT_TSS_1] = (gdt_entry) {
    (unsigned short)    0x0067,         /* limit[0:15]  */
    (unsigned short)    0x0000,         /* base[0:15]   */  /* &tss_idle */
    (unsigned char)     0x00,           /* base[23:16]  */
    (unsigned char)     0x09,           /* type         */
    (unsigned char)     0x00,           /* s            */
    (unsigned char)     0x00,           /* dpl          */
    (unsigned char)     0x01,           /* p            */
    (unsigned char)     0x00,           /* limit[16:19] */
    (unsigned char)     0x00,           /* avl          */
    (unsigned char)     0x00,           /* l            */
    (unsigned char)     0x01,           /* db           */
    (unsigned char)     0x00,           /* g            */
    (unsigned char)     0x00,           /* base[31:24]  */
  },
  
  [GDT_TSS_2] = (gdt_entry) {
    (unsigned short)    0x0067,         /* limit[0:15]  */
    (unsigned short)    0x0000,         /* base[0:15]   */
    (unsigned char)     0x00,           /* base[23:16]  */
    (unsigned char)     0x09,           /* type         */
    (unsigned char)     0x00,           /* s            */
    (unsigned char)     0x00,           /* dpl          */
    (unsigned char)     0x01,           /* p            */
    (unsigned char)     0x00,           /* limit[16:19] */
    (unsigned char)     0x00,           /* avl          */
    (unsigned char)     0x00,           /* l            */
    (unsigned char)     0x01,           /* db           */
    (unsigned char)     0x00,           /* g            */
    (unsigned char)     0x00,           /* base[31:24]  */
  },

};

gdt_descriptor GDT_DESC = {
  sizeof(gdt) - 1,
  (unsigned int) &gdt    
};
