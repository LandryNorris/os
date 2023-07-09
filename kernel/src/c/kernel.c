#include <stdio.h>
#include <terminal.h>
#include <gdt.h>
#include "idt.h"
#include "system.h"
#include "isr.h"
#include "interrupts.h"
#include "paging.h"
#include "pmm.h"

__attribute__((unused)) void kernel_main(void) {
    terminal_initialize();

    printf("Initializing Gdt/Idt\n");
    initializeGdt();
    initializeIdt();

    printf("Initializing PMM\n");
    initializePmm(1024*1024*1024);

    printf("Initializing Paging\n");
    initPaging();

    registerInterruptHandler(0x20 + 1, handleKeyboard);

    printf("Hello, World\n");

    printf("Size of Page Table Entry: %d\nSize of Page table: %d\n", sizeof(PageTableEntry), sizeof(PageTable));

    //We don't want kernel_main to exit, so we need to loop.
    for(;;);
}
