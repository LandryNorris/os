#ifndef KERNEL_LIBC_MODIFIED_H
#define KERNEL_LIBC_MODIFIED_H

extern "C" {
#include "libk/include/buffer.h"

extern FileBuffer* os_libc_stdout;

void initializeBuffers(FileBuffer* in, FileBuffer* out);

void os_libc_putchar(char c);
void os_libc_printf(const char* __restrict s, ...);

int os_libc_memcmp(const void*, const void*, size_t);
void* os_libc_memcpy(void* __restrict, const void* __restrict, size_t);
void* os_libc_memmove(void*, const void*, size_t);
void* os_libc_memset(void*, int, size_t);
size_t os_libc_strlen(const char*);
char* os_libc_strcat(char*, char*);
char* os_libc_strchr(const char*, int);
char* os_libc_strcpy(char*, const char*);
int os_libc_strcmp(const char* str1, const char* str2);

}

#endif //KERNEL_LIBC_MODIFIED_H
