
%define _NE 12
%define _N  11
%define _NO 14
%define _SE 32
%define _S  33
%define _SO 34
%define _MOVER 0x83d
%define _MISIL 0x911
%define _MINAR 0x355
%define _CODIGO 0x8000000

global task

task:
  
  push dword 0xdeadc0de
  push dword 1103515245
  
  .ciclo:
  
  mov edi, 49
  .moverX:
    mov eax, _MISIL
    mov ebx, 1
    mov ecx, -1
    mov edx, _CODIGO
    mov esi, 3072
    int 0x52
    
    mov eax, _MOVER
    mov ebx, _SE
    int 0x52
    
    call escribir_basura
    
    mov eax, _MINAR
    mov ebx, _NO
    int 0x52
    
    mov eax, [esp + 4]
    call random
    mov ebx, eax
    call random
    mov ecx, eax
    
    mov eax, _MISIL
    and ebx, 31
    and ecx, 31
    mov edx, _CODIGO
    mov esi, 3072
    int 0x52
    
    dec edi
    jne .moverX
  ; FIN .mover
  
  mov eax, _MISIL
  mov ebx, 0
  mov ecx, -1
  mov edx, _CODIGO
  mov esi, 1
  int 0x52
  
  mov eax, _MOVER
  mov ebx, _S
  int 0x52
  
  mov eax, _MINAR
  mov ebx, _N
  int 0x52
  
  mov edi, 49
  .moverY:
    mov eax, _MISIL
    mov ebx, -1
    mov ecx, -1
    mov edx, _CODIGO
    mov esi, 3072
    int 0x52
    
    mov eax, _MOVER
    mov ebx, _SO
    int 0x52
    
    mov eax, _MINAR
    mov ebx, _NE
    int 0x52
    
    dec edi
    jne .moverY
  ; FIN .mover
  
  mov eax, _MISIL
  mov ebx, 0
  mov ecx, -1
  mov edx, _CODIGO
  mov esi, 1
  int 0x52
  
  mov eax, _MOVER
  mov ebx, _S
  int 0x52
  
  mov eax, _MINAR
  mov ebx, _N
  int 0x52
  
  jmp .ciclo
  
; FIN task

random:
  mul dword [esp + 4]
  add eax, 12345
  and eax, 0x7fffffff
  mov [esp + 8], eax
  ret
; FIN random

escribir_basura:
  mov ecx, 512+256
  .ciclo:
    mov dword [eax - 0xfff + 4*ecx], 0xdeadc0de
    loop .ciclo
  ret
