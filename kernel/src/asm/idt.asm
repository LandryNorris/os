[GLOBAL setIdt]
setIdt:
    mov eax, [esp + 4]
    lidt [eax]
    ret
