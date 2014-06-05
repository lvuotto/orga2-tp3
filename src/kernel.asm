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

_eax:  	   db 'eax '
_eax_len:  equ $ - _eax
_ebx:  	   db 'ebx '
_ebx_len:  equ $ - _ebx
_ecx:  	   db 'ecx '
_ecx_len:  equ $ - _ecx
_edx:  	   db 'edx '
_edx_len:  equ $ - _edx
_esi:  	   db 'esi '
_esi_len:  equ $ - _esi
_edi:  	   db 'edi '
_edi_len:  equ $ - _edi
_ebp:  	   db 'ebp '
_ebp_len:  equ $ - _ebp
_esp:  	   db 'esp '
_esp_len:  equ $ - _esp
_eip:  	   db 'eip '
_eip_len:  equ $ - _eip
_cs:  	   db 'cs '
_cs_len:  equ $ - _cs
_ds:  	   db 'ds '
_ds_len:  equ $ - _ds
_es:  	   db 'es '
_es_len:  equ $ - _es
_fs:  	   db 'fs '
_fs_len:  equ $ - _fs
_gs:  	   db 'gs '
_gs_len:  equ $ - _gs
_ss:  	   db 'ss '
_ss_len:  equ $ - _ss
_eflags:  	   db 'eflags '
_eflags_len:  equ $ - _eflags
_cr0:  	    db 'cr0 '
_cr0_len:  equ $ - _cr0
_cr2:  	    db 'cr2 '
_cr2_len:  equ $ - _cr2
_cr3:  	    db 'cr3 '
_cr3_len:  equ $ - _cr3
_cr4:  	    db 'cr4 '
_cr4_len:  equ $ - _cr4
_st:  	   db '_st '
_st_len:  equ $ - _st

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
    
    
    ; Inicializar pantalla
    
    call pintar_info
    imprimir_texto_mp _eax, _eax_len, 0x70, 8, 55
    imprimir_texto_mp _ebx, _ebx_len, 0x70, 10, 55
    imprimir_texto_mp _ecx, _ecx_len, 0x70, 12, 55
    imprimir_texto_mp _edx, _edx_len, 0x70, 14, 55
    imprimir_texto_mp _esi, _esi_len, 0x70, 16, 55
    imprimir_texto_mp _edi, _edi_len, 0x70, 18, 55
    imprimir_texto_mp _ebp, _ebp_len, 0x70, 20, 55
    imprimir_texto_mp _esp, _esp_len, 0x70, 22, 55
    imprimir_texto_mp _eip, _eip_len, 0x70, 24, 55
    imprimir_texto_mp _cs, _cs_len, 0x70, 26, 55
    imprimir_texto_mp _ds, _ds_len, 0x70, 28, 55
    imprimir_texto_mp _es, _es_len, 0x70, 30, 55
    imprimir_texto_mp _fs, _fs_len, 0x70, 32, 55
    imprimir_texto_mp _gs, _gs_len, 0x70, 34, 55
    imprimir_texto_mp _ss, _ss_len, 0x70, 36, 55
    imprimir_texto_mp _eflags, _eflags_len, 0x70, 36, 55
    
    imprimir_texto_mp _cr0, _cr0_len, 0x70, 8, 66
    imprimir_texto_mp _cr2, _cr2_len, 0x70, 10, 66
    imprimir_texto_mp _cr3, _cr3_len, 0x70, 12, 66
    imprimir_texto_mp _cr4, _cr4_len, 0x70, 14, 66
    imprimir_texto_mp _st, _st_len, 0x70, 28, 66
    
    
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


%include "a20.asm"
