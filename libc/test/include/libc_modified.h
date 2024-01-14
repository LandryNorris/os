#ifndef KERNEL_LIBC_MODIFIED_H
#define KERNEL_LIBC_MODIFIED_H

extern "C" {
#include "libk/include/buffer.h"

extern FileBuffer* os_libc_stdout;

void initializeBuffers(FileBuffer* in, FileBuffer* out);

void os_libc_putchar(char c);
void os_libc_printf(const char* __restrict s, ...);

}

#endif //KERNEL_LIBC_MODIFIED_H
