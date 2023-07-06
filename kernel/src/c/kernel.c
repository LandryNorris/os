#include <stdio.h>
#include <terminal.h>
#include <gdt.h>
#include "idt.h"
#include "system.h"
#include "isr.h"
#include "interrupts.h"

__attribute__((unused)) void kernel_main(void) {
    terminal_initialize();
    initializeGdt();
    initializeIdt();

    registerInterruptHandler(0x20 + 1, handleKeyboard);

    printf("Hello, World\n");

    //We don't want kernel_main to exit, so we need to loop.
    for(;;);
}
