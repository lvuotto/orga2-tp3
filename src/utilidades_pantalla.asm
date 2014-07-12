
%include "imprimir.mac"

BITS 32

; Asume que fs apunta al comienzo del video (0xb8000).
; Tenerlo en cuenta a la hora de utilizarse.

%define nro_tarea_1 0xb8000 + 46*160 + 2*54
%define nro_tarea_2 0xb8000 + 46*160 + 2*56
%define nro_tarea_3 0xb8000 + 46*160 + 2*58
%define nro_tarea_4 0xb8000 + 46*160 + 2*60
%define nro_tarea_5 0xb8000 + 46*160 + 2*62
%define nro_tarea_6 0xb8000 + 46*160 + 2*64
%define nro_tarea_7 0xb8000 + 46*160 + 2*66
%define nro_tarea_8 0xb8000 + 46*160 + 2*68


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
      
      and dword [fs:eax + 2*esi], 0xff00ff00 ; eax <- fila
      add esi, 2                     
      cmp esi, 80                            ; esi <- columna
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


limpiar_buffer_video
  mov ecx, 0xa0000
  .ciclo:
  and dword [ecx], 0xff00ff00
  add ecx, 4
  cmp ecx, 0xc0000
  jl .ciclo
  ret



%define COLOR_TAG_REG       0x0e


inicializar_pantalla:
  
  imprimir_texto_mp tag_eax,    tag_eax_len,    COLOR_TAG_REG,  8, 52
  imprimir_texto_mp tag_ebx,    tag_ebx_len,    COLOR_TAG_REG, 10, 52
  imprimir_texto_mp tag_ecx,    tag_ecx_len,    COLOR_TAG_REG, 12, 52
  imprimir_texto_mp tag_edx,    tag_edx_len,    COLOR_TAG_REG, 14, 52
  imprimir_texto_mp tag_esi,    tag_esi_len,    COLOR_TAG_REG, 16, 52
  imprimir_texto_mp tag_edi,    tag_edi_len,    COLOR_TAG_REG, 18, 52
  imprimir_texto_mp tag_ebp,    tag_ebp_len,    COLOR_TAG_REG, 20, 52
  imprimir_texto_mp tag_esp,    tag_esp_len,    COLOR_TAG_REG, 22, 52
  imprimir_texto_mp tag_eip,    tag_eip_len,    COLOR_TAG_REG, 24, 52
  imprimir_texto_mp tag_cs,     tag_cs_len,     COLOR_TAG_REG, 26, 52
  imprimir_texto_mp tag_ds,     tag_ds_len,     COLOR_TAG_REG, 28, 52
  imprimir_texto_mp tag_es,     tag_es_len,     COLOR_TAG_REG, 30, 52
  imprimir_texto_mp tag_fs,     tag_fs_len,     COLOR_TAG_REG, 32, 52
  imprimir_texto_mp tag_gs,     tag_gs_len,     COLOR_TAG_REG, 34, 52
  imprimir_texto_mp tag_ss,     tag_ss_len,     COLOR_TAG_REG, 36, 52
  imprimir_texto_mp tag_eflags, tag_eflags_len, COLOR_TAG_REG, 38, 52
  
  imprimir_texto_mp tag_cr0,    tag_cr0_len,    COLOR_TAG_REG,  8, 66
  imprimir_texto_mp tag_cr2,    tag_cr2_len,    COLOR_TAG_REG, 10, 66
  imprimir_texto_mp tag_cr3,    tag_cr3_len,    COLOR_TAG_REG, 12, 66
  imprimir_texto_mp tag_cr4,    tag_cr4_len,    COLOR_TAG_REG, 14, 66
  imprimir_texto_mp tag_stack,  tag_stack_len,  COLOR_TAG_REG, 25, 66
  
  mov word [nro_tarea_1], 0xb << 8 | '1'
  mov word [nro_tarea_2], 0xb << 8 | '2'
  mov word [nro_tarea_3], 0xb << 8 | '3'
  mov word [nro_tarea_4], 0xb << 8 | '4'
  mov word [nro_tarea_5], 0xb << 8 | '5'
  mov word [nro_tarea_6], 0xb << 8 | '6'
  mov word [nro_tarea_7], 0xb << 8 | '7'
  mov word [nro_tarea_8], 0xb << 8 | '8'

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

