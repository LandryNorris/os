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

#endif //OS_EXT2_H
