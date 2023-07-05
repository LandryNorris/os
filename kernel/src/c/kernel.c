#include <stdio.h>
#include <terminal.h>
#include <gdt.h>

__attribute__((unused)) void kernel_main(void) {
    terminal_initialize();
    initializeGdt();

    printf("Hello, World");
}
