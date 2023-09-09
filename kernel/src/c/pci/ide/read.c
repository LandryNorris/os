#include "ide.h"
#include "system.h"

uint8_t ideRead(uint8_t channel, uint8_t reg) {
    uint16_t base = ide.channels[channel].base;
    return inportByte(base + reg);
}

uint8_t ideReadControl(uint8_t channel, uint8_t reg) {
    uint16_t base = ide.channels[channel].controlBase;
    return inportByte(base + reg);
}
