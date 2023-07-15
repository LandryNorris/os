#include "serial.h"
#include <system.h>

int initializeSerial(Serial* serial, int port) {
    //https://wiki.osdev.org/Serial_Ports
    outportByte(port + 1, 0x00);    // Disable all interrupts
    outportByte(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outportByte(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outportByte(port + 1, 0x00);    //                  (hi byte)
    outportByte(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outportByte(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outportByte(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outportByte(port + 4, 0x1E);    // Set in loopback mode
    outportByte(port + 0, 0xAE);    // Send a byte to test the serial chip

    // Check if serial is faulty
    if(inportByte(port + 0) != 0xAE) {
        return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outportByte(port + 4, 0x0F);
    serial->port = port;
    serial->isInitialized = 1;
    return 0;
}

int updateLineStatusRegister(Serial* serial) {
    //Read 'Line status register'
    uint8_t line = inportByte(serial->port + 5);
    serial->lineRegister = line;
    return line;
}

int serialAvailable(Serial* serial) {
    if(!serial->isInitialized) return 0;

    //Read the LSB of the status register to see if data is available
    return updateLineStatusRegister(serial) & 1;
}

uint8_t serialRead(Serial* serial) {
    while(!serialAvailable(serial));

    return inportByte(serial->port);
}

int isSerialReadyToSend(Serial* serial) {
    return updateLineStatusRegister(serial) & 0x20;
}

void serialWrite(Serial* serial, uint8_t byte) {
    while (!isSerialReadyToSend(serial));

    outportByte(serial->port, byte);
}

void serialPrint(Serial* serial, const char* text) {
    for(char c = *text; c; c = *++text) {
        serialWrite(serial, c);
    }
}
