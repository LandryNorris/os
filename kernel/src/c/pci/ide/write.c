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

void writePortShorts(int port, void* data, int size) {
    __asm__ volatile ("rep outsw" : "+S" (data), "+c" (size) : "d" (port));
}

void ataWriteSectors(IdeDevice* device, uint32_t blockAddress, uint8_t* buffer, int length) {
    ideWrite(device->channel, ATA_REG_CONTROL, 0x02);
    int numSectors = length/512 + 1;

    ataWaitUntilReady(device->channel);

    uint32_t head = (blockAddress & 0x0f000000) >> 24;
    uint32_t devSel = 0xE0 | (device->drive << 4) | head;

    ideWrite(device->channel, ATA_REG_HDDEVSEL, devSel);

    ideWrite(device->channel, ATA_REG_HDDEVSEL, devSel);
    ideWrite(device->channel, ATA_REG_FEATURES, 0x00);
    ideWrite(device->channel, ATA_REG_SECCOUNT0, numSectors);
    ideWrite(device->channel, ATA_REG_LBA0, (blockAddress & 0x000000FF) >> 0);
    ideWrite(device->channel, ATA_REG_LBA1, (blockAddress & 0x0000FF00) >> 8);
    ideWrite(device->channel, ATA_REG_LBA2, (blockAddress & 0x00FF0000) >> 16);
    ideWrite(device->channel, ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);
    ataIoWait(device->channel);

    int bus = ide.channels[device->channel].base;

    for(int i = 0; i < numSectors; i++) {
        writePortShorts(bus, buffer, 256); //512 bytes = 256 shorts
        buffer += 512;
    }
}
