#include "ide.h"
#include "system.h"

void ideWrite(uint8_t channel, uint8_t reg, uint8_t value) {
    uint8_t base = ide.channels[channel].base;
    outportByte(base+reg, value);
}

void ideWriteControl(uint8_t channel, uint8_t reg, uint8_t value) {
    uint8_t base = ide.channels[channel].controlBase;
    outportByte(base+reg, value);
}
