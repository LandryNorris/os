
#include <stdio.h>
#include "system.h"
#include "isr.h"

char* exceptionMessages[32] = {
        "Divide By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",
        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
};

void finalExceptionHandler(Register reg) {
    if (reg.interruptNumber < 32) {
        printf("EXCEPTION: %s (err code is %d)\n", exceptionMessages[reg.interruptNumber], reg.errno);
        isr handler = interruptHandlers[reg.interruptNumber];
        if (handler) handler(&reg);
        for (;;);
    }
    if (interruptHandlers[reg.interruptNumber] != 0) {
        isr handler = interruptHandlers[reg.interruptNumber];
        handler(&reg);
    }
}
