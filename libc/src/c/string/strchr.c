
#include "libc_name.h"

char* LIBC_SYMBOL(strchr)(const char* str, int ch) {
    char* s = str;
    for (char c = *s; c; c = *++s) {
        if (c == ch) {
            return s;
        }
    }

    return 0;
}
