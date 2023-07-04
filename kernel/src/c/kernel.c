#include <stdio.h>
#include <terminal.h>
#include <string.h>

void kernel_main(void) {
    terminal_initialize();

    char* text = "some text we're about to overwrite.";
    char* t2 = "Hello, World";

    strcpy(text, t2);

    printf("%s\n%s\n", text, t2);
}
