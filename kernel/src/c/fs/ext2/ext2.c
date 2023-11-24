#include <stdio.h>
#include <mem.h>
#include "ide.h"
#include "ext2.h"

typedef struct {
    uint32_t blockUsageBitmapAddress;
    uint32_t inodeUsageBitmapAddress;
    uint32_t inodeTableAddress;
    uint16_t numUnallocatedBlocks;
    uint16_t numUnallocatedInodes;
    uint16_t numDirectories;
    uint8_t reserved[14];
} __attribute__((packed)) Ext2RawBlockDescriptor;

inline uint32_t readUint32(void* data, int offset) {
    void* ptr = ((uint8_t*) data) + offset;
    return *((uint32_t*) ptr);
}

inline uint16_t readUint16(void* data, int offset) {
    void* ptr = ((uint8_t*) data) + offset;
    return *((uint16_t*) ptr);
}

void readBlock(IdeDevice* device, Ext2Fs* fs, int block, uint8_t* buffer) {
    uint32_t address = fs->superBlock.blockSize * block;
    uint32_t numSectors = fs->superBlock.blockSize/512;

    for(uint32_t sector = 0; sector < numSectors; sector++) {
        // TODO: Figure out why we need to call this twice
        ataReadSectors(device, address/512 + sector, buffer + sector*512, 512);
        ataReadSectors(device, address/512 + sector, buffer + sector*512, 512);
    }
}

int readSuperblock(IdeDevice* device, Ext2Fs* fs) {
    printf("Initializing ext2 drive\n");

    uint8_t data[1024];
    ataReadSectors(device, 2, data, 1024);

    uint16_t signature = readUint16(data, 56);

    if(signature != 0xef53) {
        printf("Signature does not match! this is not an ext2 drive\n");
        printf("Signature is %x\n", signature);
        return 0;
    }

    fs->superBlock.inodeCount = readUint32(data, 0);
    fs->superBlock.blockCount = readUint32(data, 4);
    fs->superBlock.blockSize = 1024 << (readUint32(data, 24));
    fs->superBlock.numBlocksInGroup = readUint32(data, 32);

    return 1;
}

void readBlockGroupDescriptor(IdeDevice* device, Ext2Fs* fs) {
    fs->numGroups = fs->superBlock.blockCount / fs->superBlock.numBlocksInGroup;
    if(fs->superBlock.numBlocksInGroup*fs->superBlock.blockCount < fs->numGroups) {
        fs->numGroups++;
    }

    fs->blockDescriptors = malloc(fs->numGroups * sizeof(Ext2BlockDescriptor));
    // SuperBlock starts at 1024, and the block group descriptor is the block after
    int blockDescriptorSize = 32;

    uint32_t blockTableSize = (blockDescriptorSize*fs->numGroups);

    uint32_t numBlocksTaken = blockTableSize/fs->superBlock.blockSize;
    if(numBlocksTaken*fs->superBlock.blockSize < blockTableSize) {
        numBlocksTaken++;
    }

    uint8_t descriptorData[fs->superBlock.blockSize*numBlocksTaken];
    Ext2RawBlockDescriptor* rawDescriptors = (Ext2RawBlockDescriptor*) descriptorData;
    for(uint32_t j = 0; j < numBlocksTaken; j++) {
        readBlock(device, fs, 1+j, (uint8_t*) rawDescriptors + j*fs->superBlock.blockSize);
    }

    for(uint32_t i = 0; i < fs->numGroups; i++) {
        fs->blockDescriptors[i].numDirectories = rawDescriptors[i].numDirectories;
        fs->blockDescriptors[i].inodeTableAddress = rawDescriptors[i].inodeTableAddress;
        fs->blockDescriptors[i].inodeUsageBitmapAddress = rawDescriptors[i].inodeUsageBitmapAddress;
        fs->blockDescriptors[i].numUnallocatedInodes = rawDescriptors[i].numUnallocatedInodes;
        fs->blockDescriptors[i].blockUsageBitmapAddress = rawDescriptors[i].blockUsageBitmapAddress;
        fs->blockDescriptors[i].numUnallocatedBlocks = rawDescriptors[i].numUnallocatedBlocks;
    }
}

void readInode(SuperBlock* superBlock, int inodeAddress, Ext2Inode* inode) {
    int group = inodeAddress * superBlock->numInodesInGroup;
    //int block =
}
