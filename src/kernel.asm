; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

extern GDT_DESC

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

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.

BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
    

    ; Habilitar A20
    
    ; Cargar la GDT
    cli
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
    mov ss, ax
    mov ax, 0b1101000     ; index 13 | gdt 0 | rpl 00
    mov fs, ax            ; video en fs.
    
    ; Establecer la base de la pila
    mov esp, 0x27000      ; setear la pila del kernel en la dirección 0x27000.
    
    ; rutina para limpiar la pantalla
    call limpiar_pantalla
    
    ; pintar el area "del mapa"

    
    ; Imprimir mensaje de bienvenida
    
    
    ; Inicializar pantalla
    
    ; Inicializar el manejador de memoria
    
    ; Inicializar el directorio de paginas
    
    ; Cargar directorio de paginas
    
    ; Habilitar paginacion
    
    ; Inicializar tss
    
    ; Inicializar tss de la tarea Idle
    
    ; Inicializar tss de las tanques
    
    ; Inicializar el scheduler
    
    ; Inicializar la IDT
    
    ; Inicializar Game
    
    ; Cargar IDT
    
    ; Configurar controlador de interrupciones
    
    ; pintar posiciones inciales de tanques
    
    ; Cargar tarea inicial

    ; Habilitar interrupciones
 
    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;


limpiar_pantalla:
  
  
  ret

%include "a20.asm"
