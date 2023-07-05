#include <stdio.h>
#include <terminal.h>

__attribute__((unused)) void kernel_main(void) {
    terminal_initialize();

    printf("Hello, World");
}
