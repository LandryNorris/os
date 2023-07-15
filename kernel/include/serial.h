#ifndef OS_SERIAL_H
#define OS_SERIAL_H

#include <stdint.h>

#define COM1_PORT 0x3f8          // COM1

typedef struct {
    int port;
    int isInitialized;
    uint8_t lineRegister;
} Serial;

int initializeSerial(Serial* serial, int port);
int serialAvailable(Serial* serial);
uint8_t serialRead(Serial* serial);
int isSerialReadyToSend(Serial* serial);
void serialWrite(Serial* serial, uint8_t byte);
void serialPrint(Serial* serial, const char* text);

#endif //OS_SERIAL_H
