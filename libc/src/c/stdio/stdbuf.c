
#include "buffer.h"
#include "libc_name.h"

FileBuffer* LIBC_SYMBOL(stdin) = 0;
FileBuffer* LIBC_SYMBOL(stdout) = 0;

void initializeBuffers(FileBuffer* in, FileBuffer* out) {
    LIBC_SYMBOL(stdin) = in;
    LIBC_SYMBOL(stdout) = out;
}
