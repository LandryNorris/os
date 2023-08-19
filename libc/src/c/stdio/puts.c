#include <stdio.h>

int puts(const char* s) {
    int i = 0;
    for(; s[i]; i++) {
        putchar(s[i]);
    }
    return 0;
}
