; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"
%include "registros.mac"


BITS 32


sched_tarea_offset:   dd 0x00
sched_tarea_selector: dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proxima_tarea
extern sched_montar_idle
extern sched_tarea_actual
extern sched_estado_tarea
extern sched_desalojar_tarea
extern esta_corriendo_la_idle
extern mostrar_contexto
extern _estan_todas_muertas

;; Game
extern game_mover
extern game_misil
extern game_minar
extern poner_pausa
extern pausa_on
extern pise_una_mina
extern fallos_tanques

;; TSS
extern tss_tanques

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

texto_pausa: db "(paused)"
texto_pausa_len: equ $ - texto_pausa

texto_vacio: db 0,0,0,0,0,0,0,0
texto_vacio_len: equ $ - texto_vacio

_isr_msg_0:  db ' Atendimos interrupcion 0 [#DE - divide error]. '
_isr_len_0:  equ $ - _isr_msg_0
_isr_msg_1:  db ' Atendimos interrupcion 1 [#DB - reserved]. '
_isr_len_1:  equ $ - _isr_msg_1
_isr_msg_2:  db ' Atendimos interrupcion 2 [! nmi]. '
_isr_len_2:  equ $ - _isr_msg_2
_isr_msg_3:  db ' Atendimos interrupcion 3 [#BP - breakpoint]. '
_isr_len_3:  equ $ - _isr_msg_3
_isr_msg_4:  db ' Atendimos interrupcion 4 [#OF - overflow]. '
_isr_len_4:  equ $ - _isr_msg_4
_isr_msg_5:  db ' Atendimos interrupcion 5 [#BR - bound range exceeded]. '
_isr_len_5:  equ $ - _isr_msg_5
_isr_msg_6:  db ' Atendimos interrupcion 6 [#UD - invalid opcode]. '
_isr_len_6:  equ $ - _isr_msg_6
_isr_msg_7:  db ' Atendimos interrupcion 7 [#NM - device not available]. '
_isr_len_7:  equ $ - _isr_msg_7
_isr_msg_8:  db ' Atendimos interrupcion 8 [#DF - ! - double fault]. '
_isr_len_8:  equ $ - _isr_msg_8
_isr_msg_9:  db ' Atendimos interrupcion 9 [reserved - segment overrun]. '
_isr_len_9:  equ $ - _isr_msg_9
_isr_msg_10: db ' Atendimos interrupcion 10 [#TS - invalid tss]. '
_isr_len_10: equ $ - _isr_msg_10
_isr_msg_11: db ' Atendimos interrupcion 11 [#NP - segment not present]. '
_isr_len_11: equ $ - _isr_msg_11
_isr_msg_12: db ' Atendimos interrupcion 12 [#SS - stack-segment fault]. '
_isr_len_12: equ $ - _isr_msg_12
_isr_msg_13: db ' Atendimos interrupcion 13 [#GP - general protection]. '
_isr_len_13: equ $ - _isr_msg_13
_isr_msg_14: db ' Atendimos interrupcion 14 [#PF - page fault]. '
_isr_len_14: equ $ - _isr_msg_14
_isr_msg_15: db ' Atendimos interrupcion 15 [Intel reserved]. '
_isr_len_15: equ $ - _isr_msg_15
_isr_msg_16: db ' Atendimos interrupcion 16 [#MF - fpu fp exception]. '
_isr_len_16: equ $ - _isr_msg_16
_isr_msg_17: db ' Atendimos interrupcion 17 [#AC - alignment check]. '
_isr_len_17: equ $ - _isr_msg_17
_isr_msg_18: db ' Atendimos interrupcion 18 [#MC - machine check]. '
_isr_len_18: equ $ - _isr_msg_18
_isr_msg_19: db ' Atendimos interrupcion 19 [#XM - simd fp exception]. '
_isr_len_19: equ $ - _isr_msg_19


_isr_desalojo_0:  db '0 - #DE          ', 0   ;
_isr_desalojo_1:  db '1 - #DB          ', 0   ;
_isr_desalojo_2:  db '2 - NMI          ', 0   ;
_isr_desalojo_3:  db '3 - #BP          ', 0   ;
_isr_desalojo_4:  db '4 - #OF          ', 0   ;
_isr_desalojo_5:  db '5 - #BR          ', 0   ;
_isr_desalojo_6:  db '6 - #UD          ', 0   ;
_isr_desalojo_7:  db '7 - #NM          ', 0   ;
_isr_desalojo_8:  db '8 - #DF          ', 0   ;
_isr_desalojo_9:  db '9 - reserved     ', 0   ;
_isr_desalojo_10: db '10 - #TS         ', 0   ;
_isr_desalojo_11: db '11 - #NP         ', 0   ; ALTA CABECEADA,
_isr_desalojo_12: db '12 - #SS         ', 0   ;
_isr_desalojo_13: db '13 - #GP         ', 0   ; AMEEGOOOOO!
_isr_desalojo_14: db '14 - #PF         ', 0   ;
_isr_desalojo_15: db '15 - reserved    ', 0   ;
_isr_desalojo_16: db '16 - #MF         ', 0   ;
_isr_desalojo_17: db '17 - #AC         ', 0   ;
_isr_desalojo_18: db '18 - #MC         ', 0   ;
_isr_desalojo_19: db '19 - #XM         ', 0   ;
                                              ;
syscall_invalido: db 'Syscall invalido ', 0   ;
mensaje_mina:     db "KILL'D BY MINA!  ", 0   ;
;       en C  -->    'La tarea aun vive'      ;


%define FALLOS_SIZE             28
%define FALLOS_OFFSET_MENSAJE   12
%define FALLOS_OFFSET_EIP       16
%define FALLOS_OFFSET_ESP       20
%define FALLOS_OFFSET_CS        24
%define FALLOS_OFFSET_SS        26


%macro ISR 1

global _isr%1

_isr%1:
    
  pushad
  
  mov ax, word [esp + 4*2]
  test ax, 0x3    ; [esp + 4*2] <- cs antes de la interrupcion.
  jne .loopear
  
  call sched_tarea_actual
  mov edi, eax
  mov ecx, FALLOS_SIZE
  mul ecx  ; en eax esta la id de la tarea
  mov ebx, eax
  mov eax, edi
  add ebx, fallos_tanques
  mov dword [ebx + FALLOS_OFFSET_MENSAJE], _isr_desalojo_%1
  mov ecx, [esp + 4*1]  ; eip
  mov dword [ebx + FALLOS_OFFSET_EIP], ecx
  mov ecx, [esp + 4*2]  ; cs
  mov word  [ebx + FALLOS_OFFSET_CS] , cx
  mov ecx, [esp + 4*4]  ; esp
  mov dword [ebx + FALLOS_OFFSET_ESP], ecx
  mov ecx, [esp + 4*5]  ; ss
  mov word  [ebx + FALLOS_OFFSET_SS] , cx
  
  push eax
  call sched_desalojar_tarea
  pop eax
  call sched_montar_idle
  mov [sched_tarea_selector], ax
  jmp far [sched_tarea_offset]
  ;jmp .fin
  
  ; TODO:
  ; - [X] Dar de baja una tarea (remover del scheduler) ante una interrupcion.
  ;       - Deberiamos poner ON a la tarea idle? Si.
  ; - [X] Remover el loop infinito y agregar un iret (no hacerlo hasta terminar
  ;       el punto anterior).
  ;       - Falso. No hay que hacer eso.
  
  .loopear:
    imprimir_texto_mp _isr_msg_%1, _isr_len_%1, 0x4f, 0, 0
    ; To Infinity And Beyond!!
    mov eax, 0xffff
    mov ebx, 0xffff
    mov ecx, 0xffff
    mov edx, 0xffff
    jmp $
  
  .fin:
  popad
  iret
  
; FIN _isr%1

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'
tarea_muerta:        db 'X'
relojes:             dd 0, 0, 0, 0, 0, 0, 0, 0

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;

ISR  0
ISR  1
ISR  2
ISR  3
ISR  4
ISR  5
ISR  6
ISR  7
ISR  8
ISR  9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;


global _isr32

_isr32:
  cli
  pushad
  
  call proximo_reloj
  
  cmp byte [poner_pausa], 1
  jne .saltar_a_tarea
  
  imprimir_texto_mp texto_pausa, texto_pausa_len, 0xa, 1, 54
  call esta_corriendo_la_idle
  cmp eax, 1
  je .no_salto
  
  call sched_montar_idle
  mov [sched_tarea_selector], ax
  call fin_intr_pic1
  sti
  jmp far [sched_tarea_offset]
  jmp .fin
  
  .saltar_a_tarea:
  imprimir_texto_mp texto_vacio, texto_vacio_len, 0x0, 1, 54
  call sched_proxima_tarea
  call proximo_reloj_tarea_actual   ; conserva eax.
  
  cmp ax, 0                         ; sched_proxima_tarea == 0 sii tengo que   
  je .no_salto                      ; volver a saltar a la tarea actual.
  
  mov [sched_tarea_selector], ax
  call fin_intr_pic1
  sti
  jmp far [sched_tarea_offset]
  jmp .fin
  
  .no_salto:
  call fin_intr_pic1
  sti
  
  .fin:
  popad
  iret
; FIN _isr32

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33

_isr33:
  cli
  pushad
  
  xor eax, eax
  in al, 0x60
  test al, 0x80
  jne .fin
  
  ; atiendo cuando se presiona.
  cmp al, 0x19
  je .pausar
  
  cmp al, 0xa
  jge .fin
  cmp al, 0x2
  jl .fin
  
  sub al, 2
  push eax
  call mostrar_contexto
  add esp, 4
  jmp .fin
  
  .pausar:
  xor byte [poner_pausa], 1
  
  .fin:
  call fin_intr_pic1
  
  popad
  sti
  iret
; FIN _isr33

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

%define SYS_MOVER     0x83d
%define SYS_MISIL     0x911
%define SYS_MINAR     0x355

global _isr0x52

_isr0x52:     ; MALTRATA LOS REGISTROS :D
  
  push ebp
  mov ebp, esp
  push ebx
  push ecx
  push edx
  push edi
  push esi
  ; ebx, edi, esi no tocados por convencion C.
 
  mov edi, eax
  call sched_tarea_actual
  
  cmp edi, SYS_MOVER
  je .mover
 
  cmp edi, SYS_MISIL
  je .misil
 
  cmp edi, SYS_MINAR
  je .minar
 
  mov edi, eax
  jmp .desalojar
 
 
  .mover:
  mov edi, eax
  push dword [ebp -  4] ; ebx
  push edi
  call game_mover
  add esp, 2*4
  cmp eax, 0
  je .desalojar
  jmp .fin
 
 
  .misil:
  mov edi, eax
  push dword [ebp - 20] ; esi
  push dword [ebp - 12] ; edx
  push dword [ebp -  8] ; ecx
  push dword [ebp -  4] ; ebx
  push edi
  call game_misil
  add esp, 5*4
  cmp eax, 0
  je .desalojar
  jmp .fin
 
 
  .minar:
  mov edi, eax
  push dword [ebp -  4] ; ebx
  push edi
  call game_minar
  add esp, 2*4
  cmp eax, 0
  je .desalojar
  jmp .fin
 
 
  .desalojar:
  push edi
  mov esi, eax
  mov eax, edi
  mov ecx, FALLOS_SIZE
  mul ecx
  mov ebx, eax
  mov eax, esi
  add ebx, fallos_tanques
  mov ecx, [esp + 4*1 + 4*6]  ; eip
  mov dword [ebx + FALLOS_OFFSET_EIP], ecx
  mov ecx, [esp + 4*2 + 4*6]  ; cs
  mov word  [ebx + FALLOS_OFFSET_CS] , cx
  mov ecx, [esp + 4*4 + 4*6]  ; esp
  mov dword [ebx + FALLOS_OFFSET_ESP], ecx
  mov ecx, [esp + 4*5 + 4*6]  ; ss
  mov word  [ebx + FALLOS_OFFSET_SS] , cx
  
  cmp byte [pise_una_mina], 1
  jne .no_pise
  mov byte [pise_una_mina], 0
  mov dword [ebx + FALLOS_OFFSET_MENSAJE], mensaje_mina
  jmp ._desalojar
  
  .no_pise:
  mov dword [ebx + FALLOS_OFFSET_MENSAJE], syscall_invalido
  
  ._desalojar:
  call sched_desalojar_tarea
  pop edi
 
  .fin:
  mov ebx, eax
  
  call sched_montar_idle
  mov [sched_tarea_selector], ax
  jmp far [sched_tarea_offset]
  
  mov eax, ebx
  pop esi
  pop edi
  pop edx
  pop ecx
  pop ebx
  pop ebp
  
  iret
  
; FIN _isr0x52


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
  inc dword [isrnumero]
  mov ebx, [isrnumero]
  cmp ebx, 0x4
  jl .ok
    mov dword [isrnumero], 0x0
    xor ebx, ebx
  .ok:
    add ebx, isrClock
    imprimir_texto_mp ebx, 1, 0x0f, 49, 79
  ret

proximo_reloj_tarea_actual:
  
  push eax
  
  cmp dword [_estan_todas_muertas], 1
  je .fin
  
  call sched_tarea_actual
  
  inc dword [relojes + 4*eax]
  and dword [relojes + 4*eax], 0x3
  mov edx, [relojes + 4*eax]
  add edx, isrClock
  shl eax, 1
  add eax, 54
  
  imprimir_texto_mp edx, 1, 0x0b, 48, eax
  
  .fin:
  pop eax
  ret
  
; FIN proximo_reloj_tarea_actual
