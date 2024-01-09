#include <string.h>
#include <mem.h>
#include "vfs.h"

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
        directory->children = malloc(DEFAULT_NUM_CHILDREN*sizeof(VfsNode));
        directory->numChildrenReserved = DEFAULT_NUM_CHILDREN;
    }

    //check if we need to grow the list
    if(directory->numChildren+1 > directory->numChildrenReserved) {
        VfsNode** previous = (VfsNode**) directory->children;
        int newSize = (int)(directory->numChildrenReserved*1.5);
        directory->children = malloc(newSize*sizeof(VfsNode));

        memcpy(directory->children, previous, directory->numChildren*sizeof(VfsNode));
        free(previous);

        directory->numChildrenReserved = newSize;
    }

    directory->children[directory->numChildren++] = (struct VfsNode*) child;
}
