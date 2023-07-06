#include <stdio.h>
#include <terminal.h>
#include <gdt.h>
#include "idt.h"

__attribute__((unused)) void kernel_main(void) {
    terminal_initialize();
    initializeGdt();
    initializeIdt();

    printf("Hello, World");
}
