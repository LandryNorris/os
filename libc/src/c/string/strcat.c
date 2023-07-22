
#include <string.h>

char* strcat(char* dst, char* src) {
    char* s = src;
    char* d = dst;
    for (; *d; d++);

    for (char c = *s; c != '\0'; c = *++s) {
        *d = c;
        d++;
    }

    return dst;
}
