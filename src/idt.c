/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
  sizeof(idt) - 1,
  (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero, _segsel, _attr)                                                                      \
  idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xffff);        \
  idt[numero].segsel = (unsigned short) _segsel;                                                               \
  idt[numero].attr = (unsigned short) _attr;                                                                   \
  idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xffff);



unsigned int idt_inicializar() {
  IDT_ENTRY(   0, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(   1, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(   2, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(   3, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(   4, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(   5, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(   6, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(   7, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(   8, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(   9, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  10, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  11, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  12, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  13, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  14, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  15, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  16, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  17, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  18, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  19, 0b1001000, 0b1000111000000000);
  
  IDT_ENTRY(  32, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(  33, 0b1001000, 0b1000111000000000);
  IDT_ENTRY(0x52, 0b1001000, 0b1110111000000000); 
  /* 0x52 -> cs de nivel 0, attr.dpl en 3 para poder ser llamado por tareas. */
  
  return (unsigned int)tss_tanques;
}
