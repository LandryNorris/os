#include <stdio.h>
#include "libc_name.h"

int LIBC_SYMBOL(putchar)(char c) {
    return writeChar(LIBC_SYMBOL(stdout), c);
}
