
#ifndef OS_STDIO_H
#define OS_STDIO_H

#include <buffer.h>
#include "libc_name.h"

extern FileBuffer* LIBC_SYMBOL(stdin);
extern FileBuffer* LIBC_SYMBOL(stdout);

int LIBC_SYMBOL(printf)(const char* __restrict, ...);
int LIBC_SYMBOL(putchar)(char);
int LIBC_SYMBOL(puts)(const char*);

#endif //OS_STDIO_H
