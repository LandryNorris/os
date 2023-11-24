#include <stdio.h>
#include "ide.h"
#include "ext2.h"

inline uint32_t readUint32(void* data, int offset) {
    void* ptr = ((uint8_t*) data) + offset;
    return *((uint32_t*) ptr);
}

inline uint16_t readUint16(void* data, int offset) {
    void* ptr = ((uint8_t*) data) + offset;
    return *((uint16_t*) ptr);
}

int readSuperblock(IdeDevice* device, SuperBlock* superBlock) {
    printf("Initializing ext2 drive\n");

    uint8_t data[1024];
    ataReadSectors(device, 2, data, 1024);

    uint16_t signature = readUint16(data, 56);

    if(signature != 0xef53) {
        printf("Signature does not match! this is not an ext2 drive\n");
        printf("Signature is %x\n", signature);
        return 0;
    }

    superBlock->inodeCount = readUint32(data, 0);
    superBlock->blockCount = readUint32(data, 4);
    superBlock->blockSize = 1024 << (readUint32(data, 24));

    return 1;
}
