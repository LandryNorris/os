
#ifndef OS_STDIO_H
#define OS_STDIO_H

#include "buffer.h"

extern FileBuffer* stdin;
extern FileBuffer* stdout;

int printf(const char* __restrict, ...);
int putchar(char);
int puts(const char*);

#endif //OS_STDIO_H
