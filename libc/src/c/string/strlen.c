
#include <stddef.h>
#include "libc_name.h"

__attribute__((unused)) size_t LIBC_SYMBOL(strlen)(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}
