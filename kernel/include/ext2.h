#ifndef OS_EXT2_H
#define OS_EXT2_H

#include <stdint.h>

typedef struct {
    uint32_t inodeCount;
    uint32_t blockCount;
    uint32_t superuserBlockCount;
    uint32_t unallocatedBlocks;
    uint32_t unallocatedInodes;
    uint32_t superblockLocation;
    uint32_t blockSize;
    uint32_t fragmentSize;
    uint32_t numBlocksInGroup;
    uint32_t numInodesInGroup;
    //Todo: add consistency check fields
    uint16_t fileSystemState;
    uint16_t errorDetectionBehavior;
    uint32_t version;
    //Todo: add time-based consistency check fields
    uint32_t osId;
    uint16_t userIdForReservedBlocks;
    uint16_t groupIdForReservedBlocks;
} SuperBlock;

typedef enum {
    Clean = 1,
    Errored
} Ext2State;

typedef enum {
    Ignore = 1,
    RemountReadOnly,
    Panic
} Ext2ErrorBehavior;

typedef struct {
    uint32_t blockUsageBitmapAddress;
    uint32_t inodeUsageBitmapAddress;
    uint32_t inodeTableAddress;
    uint16_t numUnallocatedBlocks;
    uint16_t numUnallocatedInodes;
    uint16_t numDirectories;
} Ext2BlockDescriptor;

typedef struct {
    uint8_t type;
    uint8_t permissions;
    uint64_t size;
    uint32_t directPointer1;
    uint32_t directPointer2;
    uint32_t directPointer3;
    uint32_t directPointer4;
    uint32_t directPointer5;
    uint32_t directPointer6;
    uint32_t directPointer7;
    uint32_t directPointer8;
    uint32_t directPointer9;
    uint32_t directPointer10;
    uint32_t directPointer11;
} Ext2Inode;

typedef struct {
    SuperBlock superBlock;
    uint32_t numGroups;
    Ext2BlockDescriptor* blockDescriptors;
} Ext2Fs;

int readSuperblock(IdeDevice* device, Ext2Fs* fs);
void readBlockGroupDescriptor(IdeDevice* device, Ext2Fs* fs);

#endif //OS_EXT2_H
