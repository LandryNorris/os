
[GLOBAL setPageDirectoryLowLevel]
setPageDirectoryLowLevel:
    mov eax, [esp + 4]
    mov cr3, eax
    ret;
