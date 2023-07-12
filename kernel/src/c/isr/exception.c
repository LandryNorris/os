
#include <stdio.h>
#include "system.h"
#include "isr.h"

char *exception_messages[32] = {
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

void final_exception_handler(Register reg) {
    if(reg.int_no < 32) {
        printf("EXCEPTION: %s (err code is %d)\n", exception_messages[reg.int_no], reg.err_code);
        isr handler = interrupt_handlers[reg.int_no];
        if(handler) handler(&reg);
        for(;;);
    }
    if(interrupt_handlers[reg.int_no] != 0) {
        isr handler = interrupt_handlers[reg.int_no];
        handler(&reg);
    }

}
