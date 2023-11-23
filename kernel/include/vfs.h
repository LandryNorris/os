#ifndef OS_VFS_H
#define OS_VFS_H

#include <stdint.h>

#define FS_FLAG_DIRECTORY 1

struct VfsNode;

typedef void (*openVfs) (struct VfsNode* node, uint32_t flags);
typedef int (*readVfs) (struct VfsNode* node, uint8_t* buffer, uint32_t length);
typedef int (*writeVfs) (struct VfsNode* node, uint8_t* data, uint32_t length);
typedef struct VfsNode* (*getVfs) (struct VfsNode* node, char* name);
typedef void (*closeVfs) (struct VfsNode* node);

typedef struct {
    char name[256];
    uint8_t flags;
    void* device;
    int numChildren;
    struct VfsNode* children;

    openVfs open;
    readVfs read;
    writeVfs write;
    closeVfs close;
    getVfs get;
} VfsNode;

VfsNode* getVfsNode(VfsNode* directory, char* name);

#endif //OS_VFS_H
