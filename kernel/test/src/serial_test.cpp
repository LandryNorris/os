#include "gtest/gtest.h"

extern "C" {
#include "serial.h"
#include "portio_mock.h"
}

TEST(SerialTest, initializeSerial) {
    Serial serial;
    int port = 1;

    putPortData(port, 0xAE); // test value read during initialization
    int result = initializeSerial(&serial, port);

    ASSERT_EQ(0, result);
    ASSERT_EQ(port, serial.port);
    ASSERT_TRUE(serial.isInitialized);

    // Assert data written to port
    ASSERT_EQ(0x00, getPortData(port+1));
    ASSERT_EQ(0x80, getPortData(port+3));
    ASSERT_EQ(0x03, getPortData(port+0));
    ASSERT_EQ(0x00, getPortData(port+1));
    ASSERT_EQ(0x03, getPortData(port+3));
    ASSERT_EQ(0xC7, getPortData(port+2));
    ASSERT_EQ(0x0B, getPortData(port+4));
    ASSERT_EQ(0x1E, getPortData(port+4));
    ASSERT_EQ(0xAE, getPortData(port+0));
    ASSERT_EQ(0x0F, getPortData(port+4));
}

TEST(SerialTest, faultySerialTest) {
    Serial serial;
    int port = 1;
    serial.port = 5;
    serial.isInitialized = 0;

    putPortData(port, 0x00); // test value read during initialization
    int result = initializeSerial(&serial, port);

    ASSERT_EQ(1, result);
    ASSERT_EQ(5, serial.port);
    ASSERT_FALSE(serial.isInitialized);
}

TEST(SerialTest, serialAvailable) {
    Serial serial;
    int port = 5;
    serial.isInitialized = 0;

    ASSERT_FALSE(serialAvailable(&serial));

    putPortData(port, 0xAE); // test value read during initialization
    initializeSerial(&serial, port);

    putPortData(port+5, 1);
    ASSERT_TRUE(serialAvailable(&serial));
    ASSERT_EQ(1, serial.lineRegister);

    putPortData(port+5, 0);
    ASSERT_FALSE(serialAvailable(&serial));
    ASSERT_EQ(0, serial.lineRegister);
}

TEST(SerialTest, readyToSend) {
    Serial serial;
    int port = 5;

    putPortData(port, 0xAE); // test value read during initialization
    initializeSerial(&serial, port);

    putPortData(port+5, 0x20);
    ASSERT_TRUE(isSerialReadyToSend(&serial));
    ASSERT_EQ(0x20, serial.lineRegister);

    putPortData(port+5, 0);
    ASSERT_FALSE(isSerialReadyToSend(&serial));
    ASSERT_EQ(0, serial.lineRegister);
}

TEST(SerialTest, serialWrite) {
    Serial serial;
    int port = 5;

    putPortData(port, 0xAE); // test value read during initialization
    initializeSerial(&serial, port);
    drainPort(port);

    putPortData(port+5, 0x20); //indicate ready to send
    serialWrite(&serial, 10);

    ASSERT_EQ(10, getPortData(port));

    // TODO: Add test for when the port is temporarily busy
}

TEST(SerialTest, printTest) {
    Serial serial;
    int port = 5;

    putPortData(port, 0xAE); // test value read during initialization
    initializeSerial(&serial, port);
    drainPort(port);

    char* text = (char*)"Hello, World\n";
    size_t len = strlen(text);

    for(int i = 0; i < len; i++) {
        putPortData(port+5, 0x20); //indicate ready to send
    }

    serialPrint(&serial, text);

    for(int i = 0; i < len; i++) {
        ASSERT_EQ(text[i], getPortData(port));
    }
}
