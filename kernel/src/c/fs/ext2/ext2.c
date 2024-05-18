#include <stdio.h>
#include <mem.h>
#include <string.h>
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

typedef struct {
    uint32_t inode;
    int nameLength;
    char name[];
} Ext2DirectoryEntry;

int isExt2Directory(Ext2Inode* inode) {
    return inode->type == 4;
}

static inline uint32_t readUint32(void* data, int offset) {
    void* ptr = ((uint8_t*) data) + offset;
    return *((uint32_t*) ptr);
}

static inline uint16_t readUint16(void* data, int offset) {
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
    fs->superBlock.numInodesInGroup = readUint32(data, 40);

    uint32_t major = readUint32(data, 76);

    //TODO: read extended fields
    if(major >= 1) {
        fs->superBlock.inodeSize = readUint16(data, 88);
    } else {
        fs->superBlock.inodeSize = 128;
    }

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
        //NOLINTNEXTLINE(clang-analyzer-core.uninitialized.Assign)
        fs->blockDescriptors[i].numDirectories = rawDescriptors[i].numDirectories;
        fs->blockDescriptors[i].inodeTableAddress = rawDescriptors[i].inodeTableAddress;
        fs->blockDescriptors[i].inodeUsageBitmapAddress = rawDescriptors[i].inodeUsageBitmapAddress;
        fs->blockDescriptors[i].numUnallocatedInodes = rawDescriptors[i].numUnallocatedInodes;
        fs->blockDescriptors[i].blockUsageBitmapAddress = rawDescriptors[i].blockUsageBitmapAddress;
        fs->blockDescriptors[i].numUnallocatedBlocks = rawDescriptors[i].numUnallocatedBlocks;
    }
}

void readInode(IdeDevice* device, Ext2Fs* fs, int inodeAddress, Ext2Inode* inode) {
    uint32_t group = inodeAddress / fs->superBlock.numInodesInGroup;
    //NOLINTNEXTLINE(clang-analyzer-core.uninitialized.Assign)
    uint32_t tableAddress = fs->blockDescriptors[group].inodeTableAddress;

    // inode index is 1-indexed
    uint32_t indexInGroup = inodeAddress - group * fs->superBlock.numInodesInGroup - 1;
    uint32_t blockIndex = indexInGroup * fs->superBlock.inodeSize/fs->superBlock.blockSize;
    uint32_t blockOffset = indexInGroup - blockIndex * (fs->superBlock.blockSize/fs->superBlock.inodeSize);

    uint8_t* fileData = malloc(fs->superBlock.blockSize);
    readBlock(device, fs, tableAddress + blockIndex, fileData);

    uint8_t* data = fileData + blockOffset*fs->superBlock.inodeSize;

    uint16_t typeAndPermissions = readUint16(data, 0);
    inode->type = (typeAndPermissions >> 12) & 0xF;
    inode->permissions = typeAndPermissions & 0xFFF;
    inode->size = readUint32(data, 4);
    inode->creationTime = readUint32(data, 12);

    for(int i = 0; i < 12; i++) {
        inode->directPointers[i] = readUint32(data, 40 + 4*i);
    }

    inode->singlyIndirectPointer = readUint32(data, 88);
    inode->doublyIndirectPointer = readUint32(data, 92);
    inode->triplyIndirectPointer = readUint32(data, 96);

    inode->tableAddress = tableAddress;

    free(fileData);
}

uint32_t getBlockIndex(Ext2Inode* inode, uint32_t block) {
    if(block < 12) {
        return inode->directPointers[block];
    } else {
        //TODO: handle indirect blocks
        return -1;
    }
}

int isPseudoDirectory(char* name) {
    if(name[0] == '\0') return 0; //we should allow empty strings
    int nameIsDot = (name[0] == '.' && name[1] == '\0');
    int nameIsDotDot = (name[0] == '.' && name[1] == '.' && name[2] == '\0');

    return nameIsDot || nameIsDotDot;
}

void addChildNodes(IdeDevice* device, Ext2Fs* fs, VfsNode* parentVfsNode, Ext2Inode* parentExtNode) {
    if(parentVfsNode->numChildren > 0) {
        printf("Parent node already has children\n");
        return;
    }
    uint8_t* block = malloc(fs->superBlock.blockSize);
    memset(block, 0, fs->superBlock.blockSize);

    uint32_t address = getBlockIndex(parentExtNode, 0);
    readBlock(device, fs, (int)address, block);

    for(uint8_t* ptr = block; ptr < block + fs->superBlock.blockSize;) {
        uint32_t inodeIndex = readUint32(ptr, 0);
        VfsNode* vfsChild = malloc(sizeof(VfsNode));
        initializeVfsNode(vfsChild);

        Ext2Inode* ext2Inode = malloc(sizeof(Ext2Inode));
        readInode(device, fs, inodeIndex, ext2Inode);

        // Todo: handle empty directory node in middle
        if(inodeIndex == 0) {
            break;
        }

        // TODO: handle if directory entries have a second name byte
        uint16_t nameLength = ptr[6];
        uint16_t size = readUint16(ptr, 4);

        memcpy(vfsChild->name, ptr + 8, nameLength);
        vfsChild->name[nameLength] = '\0';

        // move the size of the directory entry
        ptr += size;

        if(isPseudoDirectory(vfsChild->name)) {
            free(vfsChild);
            continue;
        }

        vfsChild->device = ext2Inode;

        addChild(parentVfsNode, vfsChild);

        if(isExt2Directory(ext2Inode)) {
            vfsChild->flags |= FS_FLAG_DIRECTORY;
        }
    }

    free(block);
}

void addChildNodesRecursive(IdeDevice* device, Ext2Fs* fs, VfsNode* parentVfsNode, Ext2Inode* parentExtNode) {
    addChildNodes(device, fs, parentVfsNode, parentExtNode);

    for(int i = 0; i < parentVfsNode->numChildren; i++) {
        VfsNode* childVfsNode = (VfsNode*) parentVfsNode->children[i];
        Ext2Inode* ext2Inode = childVfsNode->device;

        if(!ext2Inode || !isExt2Directory(ext2Inode)) continue;

        addChildNodesRecursive(device, fs, childVfsNode, ext2Inode);
    }
}

void mountExt2(IdeDevice* device, Ext2Fs* fs, VfsNode* vfsNode) {
    if(!isDirectory(vfsNode)) {
        printf("Can't mountExt2 fs on a non-directory");
        return;
    }

    readSuperblock(device, fs);
    readBlockGroupDescriptor(device, fs);

    Ext2Inode rootNode;
    readInode(device, fs, 2, &rootNode);

    addChildNodesRecursive(device, fs, vfsNode, &rootNode);
}
