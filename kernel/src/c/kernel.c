#include <stdio.h>
#include <terminal.h>

void kernel_main(void) {
    terminal_initialize();
    /* Newline support is left as an exercise. */
    puts("First Line\nSecond Line\nA very long line that will certainly overflow the 80 character limit of this text mode, causing it to wrap to the next line\nAnother Line");
}
