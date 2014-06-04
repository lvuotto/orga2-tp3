
; La GDT, descripta a lo cabeza :D

gdt:
.GDT_IDX_NULL_DESC: dd 0x0, 0x0
dd 0x0, 0x0     ; 8 descriptores reservados.
dd 0x0, 0x0     ; 8 descriptores reservados.
dd 0x0, 0x0     ; 8 descriptores reservados.
dd 0x0, 0x0     ; 8 descriptores reservados.
dd 0x0, 0x0     ; 8 descriptores reservados.
dd 0x0, 0x0     ; 8 descriptores reservados.
dd 0x0, 0x0     ; 8 descriptores reservados.
dd 0x0, 0x0     ; 8 descriptores reservados.
.GDT_IDX_CD_0:  dd 0x0000dd00,
                dd 0x00329a00
.GDT_IDX_CD_1:  dd 0x0000dd00,
                dd 0x0032fa00
.GDT_IDX_DD_0:  dd 0x0000dd00,
                dd 0x00329200
.GDT_IDX_DD_1:  dd 0x0000dd00,
                dd 0x0032f200
.GDT_IDX_SD:    dd 0x80000500,
                dd 0x0033920b


GDT_DESC: dw $ - gdt
          dd gdt

