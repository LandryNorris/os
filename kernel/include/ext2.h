#ifndef OS_EXT2_H
#define OS_EXT2_H

#include "ide.h"
#include "vfs.h"

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

    uint16_t inodeSize;
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
    uint16_t permissions;
    uint64_t size;
    uint32_t creationTime;
    uint32_t directPointers[12];
    uint32_t singlyIndirectPointer;
    uint32_t doublyIndirectPointer;
    uint32_t triplyIndirectPointer;
    uint32_t tableAddress;
} Ext2Inode;

typedef struct {
    SuperBlock superBlock;
    uint32_t numGroups;
    Ext2BlockDescriptor* blockDescriptors;
} Ext2Fs;

int readSuperblock(IdeDevice* device, Ext2Fs* fs);
void readBlockGroupDescriptor(IdeDevice* device, Ext2Fs* fs);
void readInode(IdeDevice* device, Ext2Fs* fs, int inodeAddress, Ext2Inode* inode);
void readDirectoryName(IdeDevice* device, Ext2Fs* fs, Ext2Inode* inode, char* buffer, int length);
void mountExt2(IdeDevice* device, Ext2Fs* fs, VfsNode* vfsNode);

#endif //OS_EXT2_H
