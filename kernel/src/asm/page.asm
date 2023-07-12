
[GLOBAL setPageDirectoryLowLevel]
setPageDirectoryLowLevel:
    mov eax, [esp + 4]
    and eax, 0xFFFFF000
    mov cr3, eax
    ret;
