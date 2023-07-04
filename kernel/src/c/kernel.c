#include <stdio.h>
#include <terminal.h>

__attribute__((unused)) void kernel_main(void) {
    terminal_initialize();

    puts("First Line\nSecond Line\nA very long line that will certainly overflow the 80 character limit of this text mode, causing it to wrap to the next line\nAnother Line");
}
