
; Exeption handlers (some with code and some without)

%macro EXCEPTION_NO_ERROR 1
    global exception%1
exception%1:
    cli
    push byte 0 ; push a dummy value so the stack is the same as error with code.
    push %1
    jmp temp_exception_handler
%endmacro

%macro EXCEPTION_ERROR 1
    global exception%1
exception%1:
    cli
    push %1
    jmp temp_exception_handler
%endmacro

EXCEPTION_NO_ERROR 0
EXCEPTION_NO_ERROR 1
EXCEPTION_NO_ERROR 2
EXCEPTION_NO_ERROR 3
EXCEPTION_NO_ERROR 4
EXCEPTION_NO_ERROR 5
EXCEPTION_NO_ERROR 6
EXCEPTION_NO_ERROR 7
EXCEPTION_ERROR 8
EXCEPTION_NO_ERROR 9
EXCEPTION_ERROR 10
EXCEPTION_ERROR 11
EXCEPTION_ERROR 12
EXCEPTION_ERROR 13
EXCEPTION_ERROR 14
EXCEPTION_NO_ERROR 15
EXCEPTION_NO_ERROR 16
EXCEPTION_NO_ERROR 17
EXCEPTION_NO_ERROR 18
EXCEPTION_NO_ERROR 19
EXCEPTION_NO_ERROR 20
EXCEPTION_NO_ERROR 21
EXCEPTION_NO_ERROR 22
EXCEPTION_NO_ERROR 23
EXCEPTION_NO_ERROR 24
EXCEPTION_NO_ERROR 25
EXCEPTION_NO_ERROR 26
EXCEPTION_NO_ERROR 27
EXCEPTION_NO_ERROR 28
EXCEPTION_NO_ERROR 29
EXCEPTION_NO_ERROR 30
EXCEPTION_NO_ERROR 31
EXCEPTION_NO_ERROR 128

extern finalExceptionHandler

temp_exception_handler:
    pusha                           ; push eax, ebx, ecx, edx, esi, edi, ebp, esp
    mov ax, ds
    push eax                        ; save ds register

    mov ax, 0x10                    ; load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call finalExceptionHandler

    pop ebx                         ; restore original data segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 0x8                    ; rebalance stack(pop err code and exception number)

    sti                             ; re-enable interrupts
    iret
