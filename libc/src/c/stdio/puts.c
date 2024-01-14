#include <stdio.h>
#include "libc_name.h"

int LIBC_SYMBOL(puts)(const char* s) {
    int i = 0;
    for(; s[i]; i++) {
        LIBC_SYMBOL(putchar)(s[i]);
    }
    return 0;
}
