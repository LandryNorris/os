#include <stdio.h>
#include <terminal.h>
#include <string.h>

void kernel_main(void) {
    terminal_initialize();

    char* text = "The quick brown fox jumped over the lazy dog";

    char* subtext = strchr(text, 'o');
    char* empty = strchr(text, '@');

    printf("%s\n%s\n%s", text, subtext, empty);
}
