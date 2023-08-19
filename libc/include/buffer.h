#ifndef OS_BUFFER_H
#define OS_BUFFER_H

#include <stdint.h>

// must be kept up to date with kernel's buffer.h
typedef struct {
    int readHead;
    int writeHead;
    int length;
    uint8_t data[];
} FileBuffer;

/**
 * Determine how many  bytes are available for reading
 * @param buffer
 */
int bufferAvailable(FileBuffer* buffer);

/**
 * Determine how many  bytes are available for writing
 */
int bufferRemaining(FileBuffer* buffer);

/**
 * Write a byte to the buffer
 * @param buffer
 * @param byte
 * @return whether the data was successfully written
 */
int writeByte(FileBuffer* buffer, uint8_t byte);

/**
 * Read a byte from the buffer
 * @param buffer
 * @param result the byte read
 * @return
 */
int readByte(FileBuffer* buffer, uint8_t* result);

/**
 * Write a character to the buffer
 * @param buffer
 * @param c
 * @return whether the character was successfully written
 */
int writeChar(FileBuffer* buffer, char c);
int readChar(FileBuffer* buffer, char* result);

#endif //OS_BUFFER_H
