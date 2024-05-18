#include "vfs.h"

#include <mem.h>
#include <string.h>

#define DEFAULT_NUM_CHILDREN 10

VfsNode vfsRoot;

void initializeVfsRoot() {
    strcpy(vfsRoot.name, "/");

    initializeVfsNode(&vfsRoot);
    vfsRoot.flags |= FS_FLAG_DIRECTORY;
}

void initializeVfsNode(VfsNode* node) {
    node->children = 0;
    node->numChildren = 0;
    node->numChildrenReserved = 0;
    node->flags = 0;
    node->device = 0;
    node->name[0] = '\0';
}

struct VfsNode** createVfsNodeList(int size) {
    VfsNode** result = (VfsNode**) malloc(size*sizeof(VfsNode*));
    return (struct VfsNode**) result;
}

VfsNode* getVfsNode(VfsNode* directory, char* name) {
    if(!isDirectory(directory) || !directory->children) return 0;

    for(VfsNode* node = (VfsNode*) directory->children[0]; node; node++) {
        if(strcmp(name, node->name) == 0) {
            return node;
        }
    }

    return 0;
}

void addChild(VfsNode* directory, VfsNode* child) {
    if(!isDirectory(directory)) return;

    if(!directory->children) {
        directory->children = createVfsNodeList(DEFAULT_NUM_CHILDREN);
        directory->numChildrenReserved = DEFAULT_NUM_CHILDREN;
    }

    //check if we need to grow the list
    if(directory->numChildren+1 > directory->numChildrenReserved) {
        VfsNode** previous = (VfsNode**) directory->children;
        int newSize = (int)(directory->numChildrenReserved*1.5);
        directory->children = createVfsNodeList(newSize);

        memcpy((void*) directory->children, (void*) previous, directory->numChildren*sizeof(VfsNode*));
        free((void*) previous);

        directory->numChildrenReserved = newSize;
    }

    directory->children[directory->numChildren++] = (struct VfsNode*) child;
}
