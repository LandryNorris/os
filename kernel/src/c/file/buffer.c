#include "mem.h"
#include "buffer.h"

/*
 * Buffer design:
 * We hold two indices: one for read and one for write.
 * On each read/write operation, we increment the pointer with
 * wraparound.
 * We need to have the write pointer lag behind the read pointer when writing
 * to ensure that we can distinguish full vs empty. If the write pointer is
 * one spot before the read pointer, we are full. If the read pointer is
 * at the same spot as the write pointer, we are empty.
 */

FileBuffer* allocFileBuffer(int size) {
    // we need to allocate an extra byte, so we can distinguish
    // empty (read == write) vs full (write+1 == read)
    FileBuffer* buffer = malloc(sizeof(FileBuffer) + size+1);
    buffer->readHead = 0;
    buffer->writeHead = 0;
    buffer->length = size+1;

    return buffer;
}

int bufferAvailable(FileBuffer* buffer) {
    if(buffer->writeHead >= buffer->readHead) {
        return buffer->writeHead - buffer->readHead;
    } else {
        return buffer->length - (buffer->readHead - buffer->writeHead);
    }
}

int bufferRemaining(FileBuffer* buffer) {
    //we need to subtract one because of our unwritable space
    return (buffer->length - 1) - bufferAvailable(buffer);
}

int writeByte(FileBuffer* buffer, uint8_t byte) {
    //check if the write head has caught up with the read head.
    int nextWriteIndex = (buffer->writeHead + 1) % (buffer->length);
    if(nextWriteIndex == buffer->readHead) return 0;

    buffer->data[buffer->writeHead++] = byte;
    if(buffer->writeHead == buffer->length) buffer->writeHead = 0;

    return 1;
}

int readByte(FileBuffer* buffer, uint8_t* result) {
    //check if the read head has caught up with the write head.
    if(buffer->readHead == buffer->writeHead) return 0;

    *result = buffer->data[buffer->readHead++];

    if(buffer->readHead == buffer->length) buffer->readHead = 0;

    return 1;
}

int writeChar(FileBuffer* buffer, char c) {
    //check if the write head has caught up with the read head.
    if(buffer->writeHead == buffer->readHead) return 0;

    buffer->data[buffer->writeHead++] = c;
    if(buffer->writeHead == (buffer->length)) buffer->writeHead = 0;

    return 1;
}

int readChar(FileBuffer* buffer, char* result) {
    //check if the read head has caught up with the write head.
    if(buffer->readHead == buffer->writeHead) return 0;

    *result = (char)buffer->data[buffer->readHead++];

    if(buffer->readHead == buffer->length) buffer->readHead = 0;

    return 1;
}
