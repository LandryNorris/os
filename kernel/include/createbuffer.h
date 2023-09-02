#ifndef OS_CREATEBUFFER_H
#define OS_CREATEBUFFER_H

#include <stdint.h>
#include <buffer.h>

/**
 * Allocate a file ideBuffer
 * @param size size of the ideBuffer to allocate
 */
FileBuffer* allocFileBuffer(int size);

#endif //OS_CREATEBUFFER_H
