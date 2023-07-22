#include "system.h"
#include "isr.h"
#include "pic.h"

isr interruptHandlers[256];

/*
 * Register a function as the handler for a certian interrupt number, both exception and irq interrupt can change their handler using this function
 * */
void registerInterruptHandler(int num, isr handler) {
    if(num < 256)
        interruptHandlers[num] = handler;
}

void final_irq_handler(Register * reg) {
    if(interruptHandlers[reg->interruptNumber] != 0) {
        isr handler = interruptHandlers[reg->interruptNumber];
        handler(reg);
    }
    sendEoi(reg->interruptNumber);
}