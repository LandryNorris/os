#include <stdio.h>
#include <terminal.h>
#include <gdt.h>
#include "idt.h"
#include "system.h"
#include "isr.h"
#include "interrupts.h"
#include "paging.h"
#include "pmm.h"
#include "serial.h"
#include "mem.h"

__attribute__((unused)) void kernel_main(void) {
    terminal_initialize();

    printf("Initializing COM1\n");
    Serial com1;
    initializeSerial(&com1, COM1_PORT);
    serialPrint(&com1, "Printing to COM1\n");

    printf("Initializing Gdt/Idt\n");
    initializeGdt();
    initializeIdt();

    printf("Initializing PMM\n");
    initializePmm(1024 * 1024 * 1024);

    printf("Initializing Paging\n");
    initPaging();

    initializeMalloc(1024 * 1024 * 1024);

    registerInterruptHandler(0x20 + 1, handleKeyboard);

    printf("Hello, World\n");

    //We don't want kernel_main to exit, so we need to loop.
    for (;;);
}
