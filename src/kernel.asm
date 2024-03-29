; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

extern GDT_DESC
extern IDT_DESC

extern idt_inicializar
extern mmu_inicializar
extern srand
extern tss_inicializar
extern tss_inicializar_tarea_idle
extern resetear_pic
extern habilitar_pic
extern deshabilitar_pic
extern sched_inicializar
extern game_inicializar
extern pintar_posiciones_iniciales

extern limpiar_buffer_video
extern limpiar_pantalla
extern pintar_info
extern pintar_pantalla
extern inicializar_pantalla

extern tss_idle


global start


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg


offset:   dd 0x0
selector: dw 0x0



;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.

BITS 16
start:
  ; Deshabilitar interrupciones
  cli

  ; Cambiar modo de video a 80x50
  mov ax, 0x0003
  int 0x10 ; set mode 0x03
  xor bx, bx
  mov ax, 0x1112
  int 0x10 ; load 8x8 font

  ; Imprimir mensaje de bienvenida
  imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
  
  ; Habilitar A20
  call habilitar_A20
  
  ; Cargar la GDT
  lgdt [GDT_DESC]
  
  ; Setear el bit PE del registro CR0
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  
  ; Saltar a modo protegido
  jmp 0b1001000:protected_mode

BITS 32
protected_mode:
  
  ; Establecer selectores de segmentos
  mov ax, 0b1011000     ; index 11 | gdt 0 | rpl 00
  mov ds, ax
  mov es, ax
  mov gs, ax
  mov ss, ax
  mov ax, 0b1101000     ; index 13 | gdt 0 | rpl 00
  mov fs, ax            ; video en fs.
  
  ; Establecer la base de la pila
  mov esp, 0x27000      ; setear la pila del kernel en la dirección 0x27000.
  
  ; rutina para limpiar la pantalla
  call limpiar_pantalla
  
  ; pintar el area "del mapa"
  call pintar_pantalla
  
  ; Imprimir mensaje de bienvenida
  imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x27, 0, 0
  
  ; Limpiar buffer de video
  call limpiar_buffer_video

  ; Inicializar pantalla
  call inicializar_pantalla
  mov ax, 0b1011000     ; index 11 | gdt 0 | rpl 00
  mov fs, ax
  
  ; Inicializar el generador de numeros pseudoaleatorios
  rdtsc
  push eax
  call srand
  add esp, 4
  
  ; Inicializar el directorio de paginas
  call mmu_inicializar
  
  ; Cargar directorio de paginas
  mov eax, 0x27000
  mov cr3, eax
  
  ; Habilitar paginacion
  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax
  
  imprimir_texto_mp nombre, nombre_len, 0x0c, 3, 50 + (30 - nombre_len)/2
  
  ; Inicializar tss
  call tss_inicializar
  
  ; Inicializar tss de la tarea Idle
  call tss_inicializar_tarea_idle
  
  ; Inicializar tss de los tanques
  ; Se realiza en game_inicializar
  
  ; Inicializar el scheduler
  call sched_inicializar
  
  ; Inicializar la IDT
  call idt_inicializar
  
  ; Cargar IDT
  lidt [IDT_DESC]
  
  ; Inicializar Game
  call game_inicializar
  
  ; Configurar controlador de interrupciones
  call deshabilitar_pic
  call resetear_pic
  call habilitar_pic
  
  ; pintar posiciones inciales de tanques
  call pintar_posiciones_iniciales
  
  ; Cargar tarea inicial
  mov ax, 0b1110000
  ltr ax
  
  ; Habilitar interrupciones
  sti
  
  ; Saltar a la primera tarea: Idle
  mov ax, 0b1111000
  mov [selector], ax
  jmp far [offset]
  
  ; Ciclar infinitamente (por si algo sale mal...)
  mov eax, 0xffff
  mov ebx, 0xffff
  mov ecx, 0xffff
  mov edx, 0xffff
  jmp $
  jmp $

;; -------------------------------------------------------------------------- ;;

nombre      db 'Alemania / Vollkornbrot'
nombre_len  equ $ - nombre

%include "a20.asm"
