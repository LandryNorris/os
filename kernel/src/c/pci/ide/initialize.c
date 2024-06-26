#include "ide.h"
#include "system.h"

#include <stdio.h>

Ide ide;
uint8_t ideBuffer[2048];

void disableAtaInterrupts() {
    ideWriteControl(ATA_PRIMARY, ATA_REG_CONTROL, 2);
    ideWriteControl(ATA_SECONDARY, ATA_REG_CONTROL, 2);
}

void ataIoWait(int channel) {
    for(int i = 0; i < 50; i++){
        ideReadControl(channel, ATA_REG_ALTSTATUS);
    }
}

/**
 * wait until the ATA channel is not busy
 * @param channel
 * @return 0 if successful, 1 if error
 */
int ataWaitUntilReady(int channel) {
    //todo: handle error flag
    while(1) {
        uint8_t status = ideRead(channel, ATA_REG_STATUS);
        if(status & ATA_SR_ERR) return 1;
        if(!(status & ATA_SR_BSY)) return 0;
    }
}

void ataSelect(int channel, int isSecondary) {
    ideWrite(channel, ATA_REG_HDDEVSEL, 0xA0 | (isSecondary << 4));
}

void initializeDrive(int channel, int isSecondary) {
    int index = channel*2 + isSecondary;
    int type = IDE_ATA;

    ataSelect(channel, isSecondary);
    ataIoWait(channel);

    ideWrite(channel, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    ataIoWait(channel);

    uint8_t status = ideRead(channel, ATA_REG_STATUS);
    if(status == 0 || status == 0xFF) { //no drive or floating bus
        ide.devices[index].exists = 0;
        return;
    }

    int error = ataWaitUntilReady(channel);

    if(error) {

        uint8_t low = ideRead(channel, ATA_REG_LBA1);
        uint8_t high = ideRead(channel, ATA_REG_LBA2);

        if(low == 0x14 && high == 0xEB) {
            type = IDE_ATAPI;
        } else if(low == 0x69 && high == 0x96) {
            type = IDE_ATAPI;
        } else {
            printf("Device is neither ATA nor ATAPI!\n");
            return;
        }

        ideWrite(channel, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
        ataIoWait(channel);
    }

    uint16_t* shortBuffer = (uint16_t*) ideBuffer;
    for(int i = 0; i < 256; i++) {
        uint16_t base = ide.channels[channel].base;
        uint16_t value = inportShort(base + ATA_REG_DATA);
        shortBuffer[i] = value;
    }

    IdeDevice* device = &ide.devices[index];

    device->exists = 1;
    device->type = type;
    device->channel = channel;
    device->drive = isSecondary;
    device->signature = *(uint16_t*)(ideBuffer + ATA_IDENT_DEVICE_TYPE);
    device->capabilities = *(uint16_t*)(ideBuffer + ATA_IDENT_CAPABILITIES);
    device->commandSets = *(uint32_t*)(ideBuffer + ATA_IDENT_COMMAND_SETS);

    if(device->commandSets & (1 << 26)) {
        device->size = *(uint32_t*)(ideBuffer + ATA_IDENT_MAX_LBA_EXT);
    } else {
        device->size = *(uint32_t*)(ideBuffer + ATA_IDENT_MAX_LBA);
    }

    for(int i = 0; i < 40; i += 2) {
        device->model[i] = (char)ideBuffer[ATA_IDENT_MODEL + i + 1];
        device->model[i+1] = (char)ideBuffer[ATA_IDENT_MODEL + i];
    }
    device->model[40] = 0;
}

void initializeIdeCompatibility() {
    initializeIde(0x1F0, 0x3F64, 0x170, 0x374, 0x000);
}

void initializeIde(uint32_t bar0, uint32_t bar1, uint32_t bar2, uint32_t bar3,
                   uint32_t bar4) {
    ide.channels[ATA_PRIMARY].base = bar0 & 0xFFFFFFFC;
    ide.channels[ATA_PRIMARY].controlBase = bar1 & 0xFFFFFFFC;
    ide.channels[ATA_SECONDARY].base = bar2 & 0xFFFFFFFC;
    ide.channels[ATA_SECONDARY].controlBase = bar3 & 0xFFFFFFFC;
    ide.channels[ATA_PRIMARY].busController = bar4 & 0xFFFFFFFC;
    ide.channels[ATA_SECONDARY].busController = (bar4 & 0xFFFFFFFC) + 8;

    disableAtaInterrupts();

    for(int channel = 0; channel < 2; channel++) {
        initializeDrive(channel, 0);
        initializeDrive(channel, 1);
    }
}
