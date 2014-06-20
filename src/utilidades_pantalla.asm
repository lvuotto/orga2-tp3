
%include "imprimir.mac"

BITS 32

; Asume que fs apunta al comienzo del video (0xb8000).
; Tenerlo en cuenta a la hora de utilizarse.

global limpiar_pantalla
global limpiar_buffer_video
global inicializar_pantalla
global pintar_pantalla


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
  
  mov eax, 80*10
  mov ecx, 35
  
  mov esi, 52
  .ciclo_columna0:
      
      and dword [fs:eax + 2*esi], 0xff00ff00
      or  dword [fs:eax + 2*esi], 0x40004000
      add esi, 2
      cmp esi, 80
      jl .ciclo_columna0
  
  add eax, 80*2
  .ciclo_fila:
    
    mov esi, 52
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
    mov esi, 52
    
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
    mov esi, 53
    
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
      cmp esi, 71
      jl .ciclo_columna3
      
  
  ret


limpiar_buffer_video
  mov ecx, 0xa0000
  .ciclo:
  and dword [ecx], 0xff00ff00
  add ecx, 4
  cmp ecx, 0xc0000
  jl .ciclo
  ret

inicializar_pantalla:
  
  call pintar_info
  imprimir_texto_mp tag_eax,    tag_eax_len,    0x70,  8, 53
  imprimir_texto_mp tag_ebx,    tag_ebx_len,    0x70, 10, 53
  imprimir_texto_mp tag_ecx,    tag_ecx_len,    0x70, 12, 53
  imprimir_texto_mp tag_edx,    tag_edx_len,    0x70, 14, 53
  imprimir_texto_mp tag_esi,    tag_esi_len,    0x70, 16, 53
  imprimir_texto_mp tag_edi,    tag_edi_len,    0x70, 18, 53
  imprimir_texto_mp tag_ebp,    tag_ebp_len,    0x70, 20, 53
  imprimir_texto_mp tag_esp,    tag_esp_len,    0x70, 22, 53
  imprimir_texto_mp tag_eip,    tag_eip_len,    0x70, 24, 53
  imprimir_texto_mp tag_cs,     tag_cs_len,     0x70, 26, 53
  imprimir_texto_mp tag_ds,     tag_ds_len,     0x70, 28, 53
  imprimir_texto_mp tag_es,     tag_es_len,     0x70, 30, 53
  imprimir_texto_mp tag_fs,     tag_fs_len,     0x70, 32, 53
  imprimir_texto_mp tag_gs,     tag_gs_len,     0x70, 34, 53
  imprimir_texto_mp tag_ss,     tag_ss_len,     0x70, 36, 53
  imprimir_texto_mp tag_eflags, tag_eflags_len, 0x70, 38, 53
  
  imprimir_texto_mp tag_cr0,    tag_cr0_len,    0x70,  8, 67
  imprimir_texto_mp tag_cr2,    tag_cr2_len,    0x70, 10, 67
  imprimir_texto_mp tag_cr3,    tag_cr3_len,    0x70, 12, 67
  imprimir_texto_mp tag_cr4,    tag_cr4_len,    0x70, 14, 67
  imprimir_texto_mp tag_stack,  tag_stack_len,  0x70, 25, 67

ret


tag_eax        db 'eax'
tag_eax_len    equ $ - tag_eax
tag_ebx        db 'ebx'
tag_ebx_len    equ $ - tag_ebx
tag_ecx        db 'ecx'
tag_ecx_len    equ $ - tag_ecx
tag_edx        db 'edx'
tag_edx_len    equ $ - tag_edx
tag_esi        db 'esi'
tag_esi_len    equ $ - tag_esi
tag_edi        db 'edi'
tag_edi_len    equ $ - tag_edi
tag_ebp        db 'ebp'
tag_ebp_len    equ $ - tag_ebp
tag_esp        db 'esp'
tag_esp_len    equ $ - tag_esp
tag_eip        db 'eip'
tag_eip_len    equ $ - tag_eip
tag_cs         db 'cs'
tag_cs_len     equ $ - tag_cs
tag_ds         db 'ds'
tag_ds_len     equ $ - tag_ds
tag_es         db 'es'
tag_es_len     equ $ - tag_es
tag_fs         db 'fs'
tag_fs_len     equ $ - tag_fs
tag_gs         db 'gs'
tag_gs_len     equ $ - tag_gs
tag_ss         db 'ss'
tag_ss_len     equ $ - tag_ss
tag_eflags     db 'eflags'
tag_eflags_len equ $ - tag_eflags
tag_cr0        db 'cr0'
tag_cr0_len    equ $ - tag_cr0
tag_cr2        db 'cr2'
tag_cr2_len    equ $ - tag_cr2
tag_cr3        db 'cr3'
tag_cr3_len    equ $ - tag_cr3
tag_cr4        db 'cr4'
tag_cr4_len    equ $ - tag_cr4
tag_stack      db 'stack'
tag_stack_len  equ $ - tag_stack

