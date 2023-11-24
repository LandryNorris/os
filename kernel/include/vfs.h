#ifndef OS_VFS_H
#define OS_VFS_H

#include <stdint.h>

#define FS_FLAG_DIRECTORY 1

struct VfsNode;

typedef void (*openVfs) (struct VfsNode* node, uint32_t flags);
typedef int (*readVfs) (struct VfsNode* node, uint8_t* buffer, uint32_t length);
typedef int (*writeVfs) (struct VfsNode* node, uint8_t* data, uint32_t length);
typedef void (*closeVfs) (struct VfsNode* node);

typedef struct {
    char name[256];
    uint8_t flags;
    void* device;
    int numChildren;
    int numChildrenReserved;
    struct VfsNode** children;

    openVfs open;
    readVfs read;
    writeVfs write;
    closeVfs close;
} VfsNode;

extern VfsNode vfsRoot;

VfsNode* getVfsNode(VfsNode* directory, char* name);
void addChild(VfsNode* directory, VfsNode* child);

inline int isDirectory(VfsNode* node) {
    return node->flags | FS_FLAG_DIRECTORY;
}

#endif //OS_VFS_H
