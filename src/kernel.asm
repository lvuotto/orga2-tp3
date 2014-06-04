; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

extern GDT_DESC
extern IDT_DESC
extern idt_inicializar
extern mmu_inicializar

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
    mov ax, 0x0003
    int 0x10 ; set mode 0x03
    xor bx, bx
    mov ax, 0x1112
    int 0x10 ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
    
    
    ; Habilitar A20
    ;~ ¿?¿?
    call habilitar_A20
    
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
    call pintar_pantalla
    
    ; Imprimir mensaje de bienvenida
    
    
    ; Inicializar pantalla
    
    
    ; Inicializar el manejador de memoria
    
    
    ; Inicializar el directorio de paginas
    call mmu_inicializar      ; creamos la tabla.
    
    ; Cargar directorio de paginas
    mov eax, 0x27000
    mov cr3, eax
    
    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    ; Inicializar tss
    
    
    ; Inicializar tss de la tarea Idle
    
    
    ; Inicializar tss de las tanques
    
    
    ; Inicializar el scheduler
    
    
    ; Inicializar la IDT
    call idt_inicializar
    
    
    ; Inicializar Game
    
    
    ; Cargar IDT
    lidt [IDT_DESC]
    int 19
    
    ; Configurar controlador de interrupciones
    
    
    ; pintar posiciones inciales de tanques
    
    
    ; Cargar tarea inicial
    
    
    ; Habilitar interrupciones
    
    
    ; Saltar a la primera tarea: Idle
    
    
    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xffff
    mov ebx, 0xffff
    mov ecx, 0xffff
    mov edx, 0xffff
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;


limpiar_pantalla:
  
  xor eax, eax
  mov ecx, 50
  
  .ciclo_fila:
    
    xor esi, esi
    .ciclo_columna:
      
      and dword [fs:eax + 2*esi], 0xff00ff00
      add esi, 2
      cmp esi, 80
      jl .ciclo_columna
      
    ; FIN .ciclo_columna
    
    add eax, 80*2
    loop .ciclo_fila
    
  ; FIN .ciclo_fila
  
  ret
  
; FIN limpiar_pantalla


pintar_pantalla:
  
  xor eax, eax
  mov ecx, 50
  
  .ciclo_fila:
    
    xor esi, esi
    .ciclo_columna:
      
      and dword [fs:eax + 2*esi], 0x8fff8fff
      or  dword [fs:eax + 2*esi], 0x20002000
      
      add esi, 2
      cmp esi, 50
      jl .ciclo_columna
      
    ; FIN .ciclo_columna
    
    add eax, 80*2
    loop .ciclo_fila
    
  ; FIN .ciclo_fila
  
  ret
  
; FIN pintar_pantalla


%include "a20.asm"
