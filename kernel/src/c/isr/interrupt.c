#include "system.h"
#include "isr.h"
#include "pic.h"

isr interrupt_handlers[256];

/*
 * Register a function as the handler for a certian interrupt number, both exception and irq interrupt can change their handler using this function
 * */
void registerInterruptHandler(int num, isr handler) {
    if(num < 256)
        interrupt_handlers[num] = handler;
}

void final_irq_handler(Register * reg) {
    if(interrupt_handlers[reg->int_no] != 0) {
        isr handler = interrupt_handlers[reg->int_no];
        handler(reg);
    }
    sendEoi(reg->int_no);
}