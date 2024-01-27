#include "gtest/gtest.h"

extern "C" {
#include "buffer.h"
}

FileBuffer* allocateBuffer(int size) {
    // we need to allocate an extra byte, so we can distinguish
    // empty (read == write) vs full (write+1 == read)
    auto* buffer = (FileBuffer*) malloc(sizeof(FileBuffer) + size+1);
    buffer->readHead = 0;
    buffer->writeHead = 0;
    buffer->length = size+1;

    return buffer;
}

TEST(BufferTest, emptyBuffer) {
    FileBuffer* buffer = allocateBuffer(10);

    ASSERT_EQ(0, bufferAvailable(buffer));
    ASSERT_EQ(10, bufferRemaining(buffer));

    free(buffer);
}

TEST(BufferTest, write) {
    FileBuffer* buffer = allocateBuffer(10);

    for(int i = 0; i < 10; i++) {
        ASSERT_EQ(1, writeByte(buffer, i));

        ASSERT_EQ(1+i, bufferAvailable(buffer));
        ASSERT_EQ(9-i, bufferRemaining(buffer));

        ASSERT_EQ(i, buffer->data[i]);
    }

    ASSERT_EQ(0, writeByte(buffer, 1));

    free(buffer);
}

TEST(BufferTest, read) {
    FileBuffer* buffer = allocateBuffer(10);

    for(int i = 0; i < 10; i++) {
        writeByte(buffer, i*2);
    }

    for(int i = 0; i < 10; i++) {
        uint8_t result;
        ASSERT_EQ(1, readByte(buffer, &result));

        ASSERT_EQ(9-i, bufferAvailable(buffer));
        ASSERT_EQ(1+i, bufferRemaining(buffer));
        ASSERT_EQ(i*2, result);
    }

    uint8_t result;
    ASSERT_EQ(0, readByte(buffer, &result));

    free(buffer);
}

TEST(BufferTest, longCharTest) {
    FileBuffer* buffer = allocateBuffer(10);
    for(int i = 1; i < 50; i++) {
        int length = i;
        if(length > 10) length = 10;
        for(int j = 0; j < length; j++) {
            ASSERT_EQ(1, writeChar(buffer, j*3));
        }

        for(int j = 0; j < length; j++) {
            char result;
            ASSERT_EQ(1, readChar(buffer, &result));
            ASSERT_EQ(j*3, result);

            int notInTest = 10 - length;
            ASSERT_EQ(length-j-1, bufferAvailable(buffer));
            ASSERT_EQ(notInTest+j+1, bufferRemaining(buffer));
        }
    }

    free(buffer);
}

TEST(BufferTest, longByteTest) {
    FileBuffer* buffer = allocateBuffer(10);
    for(int i = 1; i < 50; i++) {
        int length = i;
        if(length > 10) length = 10;
        for(int j = 0; j < length; j++) {
            ASSERT_EQ(1, writeByte(buffer, j*3));
        }

        for(int j = 0; j < length; j++) {
            uint8_t result;
            ASSERT_EQ(1, readByte(buffer, &result));
            ASSERT_EQ(j*3, result);

            int notInTest = 10 - length;
            ASSERT_EQ(length-j-1, bufferAvailable(buffer));
            ASSERT_EQ(notInTest+j+1, bufferRemaining(buffer));
        }
    }

    free(buffer);
}
