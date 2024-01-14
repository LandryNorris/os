
#include "libc_name.h"

int LIBC_SYMBOL(strcmp)(const char* str1, const char* str2) {
    const char* p1 = str1;
    const char* p2 = str2;

    while(*p1 && *p2) {
        int diff = *p1++ - *p2++;
        if(diff != 0) return diff;
    }

    return *p1 - *p2;
}
