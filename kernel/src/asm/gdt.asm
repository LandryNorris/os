
[GLOBAL setGdt]
setGdt:
    mov eax, [esp + 4]
    lgdt [eax]

    ; 0x10 = 0000000100000 0 00 (GDT, ring 0)
    mov ax, 0x10 ; 0x10 represents the kernel's data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; 0x08 = 0000000000010 0 00 (GDT, ring 0)
    jmp 0x08:.reload_cs ; set the CS register. 0x08 represents the code segment.
.reload_cs:
    ret