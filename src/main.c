
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define CANT_TANQUES 8
#define FALSE 0x00000000
#define TRUE  0x00000001
#define GDT_TSS_1 0xe
#define GDT_TSS_2 0xf

#define SWAAAAP()               \
  swaaaap      = tss_tarea_1;    \
  tss_tarea_1   = tss_tarea_2;  \
  tss_tarea_2 = swaaaap


int *tss_tarea_1, *tss_tarea_2, *swaaaap, tss_idle, tss_tanques[CANT_TANQUES];
int t1 = 1, t2 = 2;
unsigned int _tarea_actual;
unsigned int tareas_vivas[CANT_TANQUES];
unsigned int _esta_corriendo_la_idle,
             gdt_tss_1_busy,
             guardar_tanquecito,
             primera_vez;


void sched_inicializar () {
  unsigned int i;
  
  for (i = 0; i < CANT_TANQUES; i++) {
    tareas_vivas[i] = TRUE;
    tss_tanques[i] = i;
  }
  
  tss_tarea_1 = &t1;
  tss_tarea_2 = &t2;
  
  tss_idle = 4321;
  tss_tarea_1 = &tss_idle;
  _esta_corriendo_la_idle = 1;
  guardar_tanquecito = 1;
  
  gdt_tss_1_busy = 1;
  _tarea_actual = CANT_TANQUES - 1;
  
  primera_vez = 1;
}


unsigned int esta_corriendo_la_idle () {
  return _esta_corriendo_la_idle;
}


unsigned int sched_tarea_actual () {
  return _tarea_actual;
}


void sched_desalojar_tarea (unsigned int id) {
  tareas_vivas[id] = FALSE;
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


int * proxima_tarea () {
  unsigned int p;
  int *r;
  
  p = proximo_indice_vivo();
  printf("~ p = %#x\n", p);
  
  if (p == 0xdeadc0de) {
    r = NULL;
  } else {
    r = &tss_tanques[p];
    _tarea_actual = p;
  }
  
  return r;
}


unsigned short sched_montar_idle () {
  unsigned short r;
  int *anterior;
  
  anterior = guardar_tanquecito ? &tss_tanques[_tarea_actual] : &tss_idle;
  
  if (gdt_tss_1_busy) {
    anterior = tss_tarea_2;
    tss_tarea_2 = &tss_idle;
  } else {
    anterior = tss_tarea_1;
    tss_tarea_1 = &tss_idle;
  }
  
  printf("~ tss_tarea_1 = %d, tss_tarea_2 = %d\n", *tss_tarea_1, *tss_tarea_2);
  printf("~ anterior = %d\n", *anterior);
  printf("~ guardar_tanquecito = %d, _esta_corriendo_la_idle = %d\n",
         guardar_tanquecito,
         _esta_corriendo_la_idle);
  printf("~ gtd_tss_1_busy = %d, primera_vez = %d\n",
         gdt_tss_1_busy,
         primera_vez);
  
  r = gdt_tss_1_busy ? GDT_TSS_2 << 3 : GDT_TSS_1 << 3;
  gdt_tss_1_busy = 1 - gdt_tss_1_busy;
  _esta_corriendo_la_idle = 1;
  printf("[_esta_corriendo_la_idle ? %d]\n", _esta_corriendo_la_idle);
  
  return r;
}


unsigned short sched_proxima_tarea () {
  
  unsigned short r;
  int *proximo, *anterior;
  
  anterior = guardar_tanquecito ? &tss_tanques[_tarea_actual] : &tss_idle;
  proximo = proxima_tarea();
  
  if (proximo == NULL && !primera_vez) {
    return 0;
  }
  
  if (gdt_tss_1_busy) {
    if (primera_vez) {
      tss_tarea_2 = &tss_tanques[0];
    } else {
      anterior = tss_tarea_2;
      tss_tarea_2 = proximo;
    }
  } else {
    if (primera_vez) {
      tss_tarea_1 = &tss_tanques[0];
    } else {
      anterior = tss_tarea_1;
      tss_tarea_1 = proximo;
    }
  }
  
  printf("~ tss_tarea_1 = %d, tss_tarea_2 = %d\n", *tss_tarea_1, *tss_tarea_2);
  printf("~ anterior = %d, proximo = %d\n", *anterior, *proximo);
  printf("~ anterior = %p, &tss_idle = %p\n", (void *) anterior, (void *) &tss_idle);
  printf("~ guardar_tanquecito = %d, _esta_corriendo_la_idle = %d\n",
         guardar_tanquecito,
         _esta_corriendo_la_idle);
  printf("~ gtd_tss_1_busy = %d, primera_vez = %d\n",
         gdt_tss_1_busy,
         primera_vez);
  
  r = gdt_tss_1_busy ? GDT_TSS_2 << 3 : GDT_TSS_1 << 3;
  gdt_tss_1_busy = 1 - gdt_tss_1_busy;
  guardar_tanquecito = 1 - _esta_corriendo_la_idle;
  _esta_corriendo_la_idle = 0;
  primera_vez = 0;
  
  return r;
  
}


/* ========================================================================== */
/* ========================================================================== */


int main () {
  
  int c;
  unsigned short i, t = 0;
  
  sched_inicializar();
  srand(time(NULL));
  
  while (1) {
    t = rand() % CANT_TANQUES;
    printf("> ");
    c = getchar();
    
    if (c == 'q') break;
    
    i = sched_proxima_tarea();
    printf("Proximo indice: %#x\n", i >> 3);
    if (t == 7) {
      printf("\n--- IDLE ---\n");
      i = sched_montar_idle();
      printf("Proximo indice: %#x\n", i >> 3);
    }
  }
  
  return 0;
  
}
