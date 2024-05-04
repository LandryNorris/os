
%define CPUID_FLAG 0x00200000

; Written in assembly, since C can change EFLAGS
global hasCpuId
hasCpuId:
    ; Save old state of FLAGS in eax
    pushfd
    pop eax

    ; Push FLAGS to the stack again for modification.
    pushfd
    xor dword [esp], CPUID_FLAG ; Invert the ID flag in EFLAGS on the stack
    popfd

    ; Copy new FLAGS value to ebx
    pushfd
    pop ebx

    ; Restore old FLAGS value
    push eax
    popfd

    xor eax, ebx ; set eax to a bitmask of bits that are different
    and eax, 0x00200000 ; check that the ID bit, and only the ID bit is changed

    ret

global callCpuId
callCpuId:
    mov eax, [esp+4]
    cpuid
