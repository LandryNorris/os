#include "buffer.h"

#include <mem.h>

FileBuffer* allocFileBuffer(int size) {
    // we need to allocate an extra byte, so we can distinguish
    // empty (read == write) vs full (write+1 == read)
    FileBuffer* buffer = malloc(sizeof(FileBuffer) + size+1);
    buffer->readHead = 0;
    buffer->writeHead = 0;
    buffer->length = size+1;

    return buffer;
}
