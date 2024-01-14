#include <stddef.h>
#include "libc_name.h"

//Todo(landry): Use word copying where possible.

__attribute__((unused)) int LIBC_SYMBOL(memcmp)(const void* p1, const void* p2, size_t size) {
    const unsigned char* a = (const unsigned char*) p1;
    const unsigned char* b = (const unsigned char*) p2;
    for (size_t i = 0; i < size; i++) {
        int diff = a[i] - b[i];

        if (diff != 0) return diff;
    }
    return 0;
}

__attribute__((unused)) void* LIBC_SYMBOL(memcpy)(void* __restrict p1, const void* __restrict p2, size_t size) {
    unsigned char* dst = (unsigned char*) p1;
    const unsigned char* src = (const unsigned char*) p2;

    for (size_t i = 0; i < size; i++) {
        dst[i] = src[i];
    }

    return p1;
}

__attribute__((unused)) void* LIBC_SYMBOL(memmove)(void* p1, const void* p2, size_t size) {
    unsigned char* dst = (unsigned char*) p1;
    const unsigned char* src = (const unsigned char*) p2;
    if (dst < src) {
        for (size_t i = 0; i < size; i++)
            dst[i] = src[i];
    } else {
        for (size_t i = size; i != 0; i--)
            dst[i - 1] = src[i - 1];
    }
    return p1;
}

__attribute__((unused)) void* LIBC_SYMBOL(memset)(void* ptr, int value, size_t size) {
    unsigned char* dst = (unsigned char*) ptr;
    for (size_t i = 0; i < size; i++)
        dst[i] = (unsigned char) value;
    return ptr;
}
