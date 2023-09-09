#include <stdio.h>
#include "ide.h"
#include "system.h"

void ideWrite(uint8_t channel, uint8_t reg, uint8_t value) {
    uint16_t base = ide.channels[channel].base;
    //printf("Writing port %x\n", base+reg);
    outportByte(base+reg, value);
}

void ideWriteControl(uint8_t channel, uint8_t reg, uint8_t value) {
    uint16_t base = ide.channels[channel].controlBase;
    outportByte(base+reg, value);
}
