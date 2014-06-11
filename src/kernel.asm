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
    
    
    ; Limpiar buffer de video
    limpiar_buffer_video
    

    ; Inicializar pantalla
    call pintar_info
    imprimir_texto_mp tag_eax, tag_eax_len, 0x70, 8, 55
    imprimir_texto_mp tag_ebx, tag_ebx_len, 0x70, 10, 55
    imprimir_texto_mp tag_ecx, tag_ecx_len, 0x70, 12, 55
    imprimir_texto_mp tag_edx, tag_edx_len, 0x70, 14, 55
    imprimir_texto_mp tag_esi, tag_esi_len, 0x70, 16, 55
    imprimir_texto_mp tag_edi, tag_edi_len, 0x70, 18, 55
    imprimir_texto_mp tag_ebp, tag_ebp_len, 0x70, 20, 55
    imprimir_texto_mp tag_esp, tag_esp_len, 0x70, 22, 55
    imprimir_texto_mp tag_eip, tag_eip_len, 0x70, 24, 55
    imprimir_texto_mp tag_cs, tag_cs_len, 0x70, 26, 55
    imprimir_texto_mp tag_ds, tag_ds_len, 0x70, 28, 55
    imprimir_texto_mp tag_es, tag_es_len, 0x70, 30, 55
    imprimir_texto_mp tag_fs, tag_fs_len, 0x70, 32, 55
    imprimir_texto_mp tag_gs, tag_gs_len, 0x70, 34, 55
    imprimir_texto_mp tag_ss, tag_ss_len, 0x70, 36, 55
    imprimir_texto_mp tag_eflags, tag_eflags_len, 0x70, 36, 55
    
    imprimir_texto_mp tag_cr0, tag_cr0_len, 0x70, 8, 66
    imprimir_texto_mp tag_cr2, tag_cr2_len, 0x70, 10, 66
    imprimir_texto_mp tag_cr3, tag_cr3_len, 0x70, 12, 66
    imprimir_texto_mp tag_cr4, tag_cr4_len, 0x70, 14, 66
    imprimir_texto_mp tag_stack, tag_stack_len, 0x70, 25, 66
    
    
    ; Inicializar el manejador de memoria
    
    
    ; Inicializar el directorio de paginas
    call mmu_inicializar
    
    ; Cargar directorio de paginas
    ;~ mov eax, 0x27000       DESCOMENTAR ESTO
    mov cr3, eax
    
    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    xchg bx, bx
    
    imprimir_texto_mp nombre, nombre_len, 0x0c, 1, 80 - nombre_len
    
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

pintar_info:
  
  xor eax, eax
  add eax, 80*10
  mov ecx, 35
  
  mov esi, 54
  .ciclo_columna0:
      
      and dword [fs:eax + 2*esi], 0xff00ff00
      or  dword [fs:eax + 2*esi], 0x40004000
      add esi, 2
      cmp esi, 80
      jl .ciclo_columna0
  
  add eax, 80*2
  .ciclo_fila:
    
    mov esi, 54
    .ciclo_columna:
      
      and dword [fs:eax + 2*esi], 0xff00ff00
      or  dword [fs:eax + 2*esi], 0x70007000
      add esi, 2
      cmp esi, 80
      jl .ciclo_columna
      
      ; FIN .ciclo_columna
    
    add eax, 80*2
    loop .ciclo_fila
    
    ; FIN .ciclo_fila
    
    
    add eax, 80*2
    mov ecx, 3
    mov esi, 54
    
    .ciclo_columna2:
      
      and dword [fs:eax + 2*esi], 0xff00ff00
      or  dword [fs:eax + 2*esi], 0x40004000
      
      mov ecx, eax
      add ecx, 80*2
      and dword [fs:ecx + 2*esi], 0xff00ff00
      or  dword [fs:ecx + 2*esi], 0x40004000
      add ecx, 80*2
      and dword [fs:ecx + 2*esi], 0xff00ff00
      or  dword [fs:ecx + 2*esi], 0x40004000
      
      add esi, 2
      cmp esi, 80
      jl .ciclo_columna2
     
    add eax, 80*10
    mov ecx, 3
    mov esi, 55
    
    .ciclo_columna3:
      
      and dword [fs:eax + 2*esi], 0xff00ff00
      or  dword [fs:eax + 2*esi], 0x70007000
      
      mov ecx, eax
      add ecx, 80*2
      and dword [fs:ecx + 2*esi], 0xff00ff00
      or  dword [fs:ecx + 2*esi], 0x70007000
      add ecx, 80*2
      and dword [fs:ecx + 2*esi], 0xff00ff00
      or  dword [fs:ecx + 2*esi], 0x70007000
      
      add esi, 2
      cmp esi, 73
      jl .ciclo_columna3
      
  
  ret


%macro limpiar_buffer_video 0
  mov ecx, 0xa0000
  .ciclo:
  and dword [ecx], 0xff00ff00
  add ecx, 4
  cmp ecx, 0xc0000
  jl .ciclo
%endmacro



tag_eax        db 'eax '
tag_eax_len    equ $ - tag_eax
tag_ebx        db 'ebx '
tag_ebx_len    equ $ - tag_ebx
tag_ecx        db 'ecx '
tag_ecx_len    equ $ - tag_ecx
tag_edx        db 'edx '
tag_edx_len    equ $ - tag_edx
tag_esi        db 'esi '
tag_esi_len    equ $ - tag_esi
tag_edi        db 'edi '
tag_edi_len    equ $ - tag_edi
tag_ebp        db 'ebp '
tag_ebp_len    equ $ - tag_ebp
tag_esp        db 'esp '
tag_esp_len    equ $ - tag_esp
tag_eip        db 'eip '
tag_eip_len    equ $ - tag_eip
tag_cs         db 'cs '
tag_cs_len     equ $ - tag_cs
tag_ds         db 'ds '
tag_ds_len     equ $ - tag_ds
tag_es         db 'es '
tag_es_len     equ $ - tag_es
tag_fs         db 'fs '
tag_fs_len     equ $ - tag_fs
tag_gs         db 'gs '
tag_gs_len     equ $ - tag_gs
tag_ss         db 'ss '
tag_ss_len     equ $ - tag_ss
tag_eflags     db 'eflags '
tag_eflags_len equ $ - tag_eflags
tag_cr0        db 'cr0 '
tag_cr0_len    equ $ - tag_cr0
tag_cr2        db 'cr2 '
tag_cr2_len    equ $ - tag_cr2
tag_cr3        db 'cr3 '
tag_cr3_len    equ $ - tag_cr3
tag_cr4        db 'cr4 '
tag_cr4_len    equ $ - tag_cr4
tag_stack      db 'stack '
tag_stack_len  equ $ - tag_stack
nombre:      db 'Alemania / Vollkornbrot '
nombre_len:  equ $ - nombre


%include "a20.asm"
