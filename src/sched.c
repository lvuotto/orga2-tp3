/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"


/**
 * El bit busy es el 2 del tipo del descriptor de tss. El resto es constante.
 * Lo unico que se hace es checkear si ese bit es 1 o 0. VER DIAPOS.
 **/
#define GDT_TSS_1_BUSY() (gdt[GDT_TSS_1].type == 0b1011)


#define NULL 0


extern void actualizar_contexto ();


tss *tss_tarea_1, *tss_tarea_2;     /* Necesario para context switch. */
extern gdt_entry gdt[GDT_COUNT];    /* Referencia a la gdt,
                                       para GDT_TSS_1_BUSY */
unsigned int _tarea_actual;               /* \                  */
unsigned int tareas_vivas[CANT_TANQUES];  /*  \                 */
unsigned int _esta_corriendo_la_idle,     /*   \ bastante auto- */
             guardar_tanquecito,          /*   / descriptivos.  */
             primera_vez;                 /*  /                 */
extern char poner_pausa;                  /* /                  */


void sched_inicializar () {
  unsigned int i;
  
  for (i = 0; i < CANT_TANQUES; i++) {
    tareas_vivas[i] = TRUE;
  }
  
  tss_tarea_1 = &tss_next_1;    /* Tienen que apuntar a algo asi no */
  tss_tarea_2 = &tss_next_2;    /* frutean al ser sobreescritas.    */
  
  tss_copy(tss_tarea_1, &tss_idle);
  _esta_corriendo_la_idle = TRUE;
  guardar_tanquecito = TRUE;
  
  _tarea_actual = CANT_TANQUES - 1;
  
  primera_vez = TRUE;
}


unsigned int esta_corriendo_la_idle () {
  return _esta_corriendo_la_idle;
}


unsigned int sched_tarea_actual () {
  return _tarea_actual;
}


unsigned int sched_estado_tarea (unsigned int id) {
  return tareas_vivas[id];
}


void sched_desalojar_tarea (unsigned int id) {
  posicion_t pos;
  
  tareas_vivas[id] = FALSE;
  pos = obtener_posicion_tanque(id);
  
  pintar_posicion('X', pos.x, pos.y, C_BG_RED | C_FG_WHITE);
}


unsigned int proximo_indice_vivo () {
  unsigned int i;
  
  i = (_tarea_actual + 1) % CANT_TANQUES;
  while (tareas_vivas[i] == FALSE && i != _tarea_actual) {
    i++;
    i %= CANT_TANQUES;
  }
  
  return i == _tarea_actual ? 0xdeadc0de : i;
}


tss * proxima_tarea () {
  unsigned int p;
  tss *r;
  
  p = proximo_indice_vivo();
  
  if (p == 0xdeadc0de) {
    r = NULL;
  } else {
    r = &(tss_tanques[p]);
    _tarea_actual = p;
  }
  
  return r;
}


unsigned short sched_montar_idle () {
  unsigned short r;
  tss *anterior;
  
  anterior = guardar_tanquecito ? &tss_tanques[_tarea_actual] : &tss_idle;
  
  /* Se entiende leyendo sched_proxima_tarea. */
  if (GDT_TSS_1_BUSY()) {
    tss_copy(anterior, tss_tarea_2);
    tss_copy(tss_tarea_2, &tss_idle);
  } else {
    tss_copy(anterior, tss_tarea_1);
    tss_copy(tss_tarea_1, &tss_idle);
  }
  
  r = GDT_TSS_1_BUSY() ? GDT_TSS_2 << 3 : GDT_TSS_1 << 3;
  guardar_tanquecito = 1 - _esta_corriendo_la_idle;
  _esta_corriendo_la_idle = TRUE;
  
  return r;
}


unsigned short sched_proxima_tarea () {
  
  unsigned short r;
  unsigned int indice_anterior;
  static tss *proximo, *anterior;
  
  /**
   * Si hay que guardar el contexto de un tanque, lo hacemos.
   * Si no, hay que guardar el contexto de la idle.
   **/
  indice_anterior = _tarea_actual;
  anterior = guardar_tanquecito ? &(tss_tanques[_tarea_actual]) : &tss_idle;
  
  proximo = proxima_tarea();
  /* Si proximo devolvio NULL, no hay que saltar a otra tarea. */
  if (proximo == NULL && !primera_vez) {
    return 0;
  }
  
  if (GDT_TSS_1_BUSY()) {
    if (primera_vez) {
      /**
       * Como es la primera vez, no hace falta guardar el contexto
       * de la tarea anterior.
       **/
      tss_copy(tss_tarea_2, &tss_tanques[0]);
      primera_vez = FALSE;
    } else {
      /**
       * Se copia en anterior (que apunta a la estructura correcta)
       * el contexto viejo, y se sobreescribe ese contexto en la gdt
       * con el contexto de la proxima tarea a ejecutar.
       **/
      tss_copy(anterior, tss_tarea_2);
      tss_copy(tss_tarea_2, proximo);
    }
  } else {
    /* Comentarios idem. */
    if (primera_vez) {
      tss_copy(tss_tarea_1, &tss_tanques[0]);
      primera_vez = FALSE;
    } else {
      tss_copy(anterior, tss_tarea_1);
      tss_copy(tss_tarea_1, proximo);
    }
  }
  
  /**
   * Si la entrada del selector de la tss 1 en la gdt esta ocupada,
   * devuelvo el selector de segmento de la entrada del selector de
   * la tss 2. Si no, devuelvo el de la tss 1.
   **/
  r = GDT_TSS_1_BUSY() ? GDT_TSS_2 << 3 : GDT_TSS_1 << 3;
  
  /**
   * guardar_tanquecito va a valer 1 si no se esta corriendo la idle,
   * 0 en caso contrario.
   * Supongamos que viene corriendo la tarea 1 en la tss 1 y hace un syscall.
   * Se monta la idle [-> _esta_corriendo_la_idle = 1, y su contexto esta en
   * la tss 2]. Llega la interrupcion del clock, y hay que hacer context
   * switch. Se hace toda la saraza de arriba, y se llega a este cacho de
   * codigo. El codigo de la tarea 2 esta montado en la tss 1. En el proximo
   * clock, voy a tener que guardar el contexto de la idle. Como todavia
   * _esta_corriendo_la_idle vale 1, guardar_tanquecito va a pasar a valer 0,
   * y en el proximo clock, efectivamente anterior va a apuntar a la tss de
   * la tarea idle.
   **/
  guardar_tanquecito = 1 - _esta_corriendo_la_idle;
  
  /* Como cambie de tarea, _NO_ esta corriendo la idle. */
  _esta_corriendo_la_idle = FALSE;
  
  actualizar_contexto(indice_anterior);
  
  return r;
  
}

