#include <stdio.h>
#include <terminal.h>
#include <string.h>

void kernel_main(void) {
    terminal_initialize();

    char s1[100] = "Hello, ";
    char* s2 = "World";

    strcat(s1, s2);
    puts(s1);
    putchar('\n');

    char first[100] = "Here ";
    char* second = "is ";
    char* third = "some ";
    char* fourth = "text\n";

    strcat(first, second);
    strcat(first, third);
    strcat(first, fourth);

    puts(first);
}
