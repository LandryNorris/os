#include <stdio.h>
#include <terminal.h>

void kernel_main(void) {
    terminal_initialize();
    /* Newline support is left as an exercise. */
    printf("First Line\nSecond Line\nA very long line that will certainly overflow the 80 character limit of this text mode, causing it to wrap to the next line\nAnother Line\n");

    printf("size of int: %d\n", sizeof(int));
    printf("size of long: %d\n", sizeof(long));
    printf("size of long long: %d\n", sizeof(long long));
    printf("size of double: %d\n", sizeof(double));
    printf("Long number: %d\n", 100000000000);
    printf("Testing numbers: %d %f %f %f %f\n", 100, 1.0, 0.0, 12345678.901234567890, -12345.6789);
}
