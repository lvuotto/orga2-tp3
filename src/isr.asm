; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"
%include "registros.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice
<<<<<<< HEAD
extern sched_tarea_actual
extern desalojar_tarea
=======
extern actualizar_tss
extern sched_proxima_tarea

>>>>>>> ej7
;; Game
extern game_mover
extern game_misil
extern game_minar

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;


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
_isr_msg_15: db ' Atendimos interrupcion 15 [intel reserved]. '
_isr_len_15: equ $ - _isr_msg_15
_isr_msg_16: db ' Atendimos interrupcion 16 [#MF - fpu fp exception]. '
_isr_len_16: equ $ - _isr_msg_16
_isr_msg_17: db ' Atendimos interrupcion 17 [#AC - alignment check]. '
_isr_len_17: equ $ - _isr_msg_17
_isr_msg_18: db ' Atendimos interrupcion 18 [#MC - machine check]. '
_isr_len_18: equ $ - _isr_msg_18
_isr_msg_19: db ' Atendimos interrupcion 19 [#XM - simd fp exception]. '
_isr_len_19: equ $ - _isr_msg_19


%macro ISR 1

global _isr%1

_isr%1:
  imprimir_texto_mp _isr_msg_%1, _isr_len_%1, 0x4f, 0, 0
  
  pushad
  imprimir_registros
  popad
  
  ; TODO:
  ; - [ ] Dar de baja una tarea (remover del scheduler) ante una interrupcion.
  ;       - Deberiamos poner ON a la tarea idle? Si.
  ; - [ ] Remover el loop infinito y agregar un iret (no hacerlo hasta terminar
  ;       el punto anterior).
  
  .loopear:
    ; To Infinity And Beyond!!
    mov eax, 0xffff
    mov ebx, 0xffff
    mov ecx, 0xffff
    mov edx, 0xffff
    jmp $
; FIN _isr%1

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

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
  call sched_proxima_tarea
  
  cmp ax, 69
  je .nojump
  
    call sched_proximo_indice
    call actualizar_tss
    mov [sched_tarea_selector], ax
    call fin_intr_pic1
    jmp far [sched_tarea_offset]
    jmp .end
    
    .nojump:
      call fin_intr_pic1
    
    .end:
      popad
      sti
  
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
  jnz .fin
  
  ; atiendo cuando se suelta.
  cmp al, 0xa
  jg .fin
  cmp al, 0x2
  jl .fin
  mov bx, 0x2f00
  add al, '0' - 1
  or bx, ax
  mov word [0xb8000], bx
  
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

_isr0x52:
  cli
  pushad
  
  mov ebx, eax
  call sched_tarea_actual
  
  cmp ebx, SYS_MOVER
  je .mover
  
  cmp ebx, SYS_MISIL
  je .misil
  
  cmp ebx, SYS_MINAR
  je .minar
  
  push eax
  call desalojar_tarea
  pop eax
  jmp .fin
  
  
  .mover:
  push dword [esp + 4*4 +  0] ; ebx
  push eax
  call game_mover
  add esp, 2*4
  
  
  .misil:
  push dword [esp + 1*4 +  0] ; esi
  push dword [esp + 5*4 +  4] ; edx
  push dword [esp + 6*4 +  8] ; ecx
  push dword [esp + 4*4 + 12] ; ebx
  push eax
  call game_misil
  add esp, 5*4
  
  
  .minar:
  push dword [esp + 4*4 +  0] ; ebx
  push eax
  call game_minar
  add esp, 2*4
  
  
  .fin:
  popad
  sti
  iret
; FIN _isr0x52  


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
  inc DWORD [isrnumero]
  mov ebx, [isrnumero]
  cmp ebx, 0x4
  jl .ok
    mov DWORD [isrnumero], 0x0
    xor ebx, ebx
  .ok:
    add ebx, isrClock
    imprimir_texto_mp ebx, 1, 0x0f, 49, 79
  ret
  
