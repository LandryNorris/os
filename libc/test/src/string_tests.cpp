#include "gtest/gtest.h"
#include "libc_modified.h"
#include "libk/include/buffer.h"

FileBuffer* allocateBuffer(int size) {
    // we need to allocate an extra byte, so we can distinguish
    // empty (read == write) vs full (write+1 == read)
    auto* buffer = (FileBuffer*) malloc(sizeof(FileBuffer) + size+1);
    buffer->readHead = 0;
    buffer->writeHead = 0;
    buffer->length = size+1;

    return buffer;
}

TEST(PrintTests, PutChar) {
    char c;

    FileBuffer* buffer = allocateBuffer(5);
    initializeBuffers(nullptr, buffer);

    os_libc_putchar('a');
    readChar(buffer, &c);
    ASSERT_EQ(c, 'a');

    os_libc_putchar('b');
    readChar(buffer, &c);
    ASSERT_EQ(c, 'b');

    free(buffer);
}

TEST(PrintTests, PrintfLiteralTest) {
    FileBuffer* buffer = allocateBuffer(100);
    initializeBuffers(nullptr, buffer);

    os_libc_printf("Printing literal string");

    char value[100] = {0};

    char c;
    for(int i = 0; readChar(buffer, &c); i++) {
        value[i] = c;
    }

    ASSERT_STREQ(value, "Printing literal string");

    free(buffer);
}

TEST(PrintTests, PrintfIntConversionTest) {
    FileBuffer* buffer = allocateBuffer(100);
    initializeBuffers(nullptr, buffer);

    int x = 4;
    int largeNumber = INT32_MAX - 1;
    int negativeNumber = -1;
    int largeNegativeNumber = INT32_MIN;
    os_libc_printf("Value: %d %d %d %d %d\n", 0, x, largeNumber, negativeNumber, largeNegativeNumber);

    char value[100] = {0};

    char c;
    for(int i = 0; readChar(buffer, &c); i++) {
        value[i] = c;
    }

    ASSERT_STREQ(value, "Value: 0 4 2147483646 -1 -2147483648\n");

    free(buffer);
}

TEST(PrintTests, PrintfFloatConversionTest) {
    FileBuffer* buffer = allocateBuffer(100);
    initializeBuffers(nullptr, buffer);

    float f = 1.0;
    float largeFloat = 1234567890.0;
    float negativeFloat = -2.0;
    float largeNegativeFloat = -largeFloat;
    float fractionalFloat = 123.45678;
    os_libc_printf("Value: %f %f %f %f %f %f\n", 0.0, f, largeFloat, negativeFloat, largeNegativeFloat, fractionalFloat);

    char value[100] = {0};

    char c;
    for(int i = 0; readChar(buffer, &c); i++) {
        value[i] = c;
    }

    ASSERT_STREQ(value, "Value: 0 1.000000 1234567936.000000 -2.000000 -1234567936.000000 123.456779\n");

    free(buffer);
}

TEST(PrintTests, PrintfHexTests) {
    FileBuffer* buffer = allocateBuffer(100);
    initializeBuffers(nullptr, buffer);

    int value1 = 1;
    int value2 = 0x123ABC;
    int value3 = 0xABC;
    int value4 = 0xDEF1;
    os_libc_printf("Value: %x %x %x %x\n", value1, value2, value3, value4);

    char value[100] = {0};

    char c;
    for(int i = 0; readChar(buffer, &c); i++) {
        value[i] = c;
    }

    ASSERT_STREQ(value, "Value: 00000001 00123ABC 00000ABC 0000DEF1\n");

    free(buffer);
}

TEST(PrintTests, PrintfMultiLine) {
    FileBuffer* buffer = allocateBuffer(100);
    initializeBuffers(nullptr, buffer);

    os_libc_printf("Multiline\nString\nWith\nSeveral\nLines");

    char value[100] = {0};

    char c;
    for(int i = 0; readChar(buffer, &c); i++) {
        value[i] = c;
    }

    ASSERT_STREQ(value, "Multiline\nString\nWith\nSeveral\nLines");

    free(buffer);
}

TEST(PrintTests, PrintfMultiCall) {
    FileBuffer* buffer = allocateBuffer(100);
    initializeBuffers(nullptr, buffer);

    os_libc_printf("Multiline\n");
    os_libc_printf("String\n");
    os_libc_printf("Value: %d %f\n", 100, -30.0);

    char value[100] = {0};

    char c;
    for(int i = 0; readChar(buffer, &c); i++) {
        value[i] = c;
    }

    ASSERT_STREQ(value, "Multiline\nString\nValue: 100 -30.000000\n");

    free(buffer);
}
