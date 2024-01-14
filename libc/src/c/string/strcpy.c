
#include "libc_name.h"

char* LIBC_SYMBOL(strcpy)(char* dst, const char* src) {
    char* s = src;
    char* d = dst;

    for (char c = *s; c != '\0'; c = *++s) {
        *d = c;
        d++;
    }
    *d = '\0';

    return dst;
}
