#include "vfs.h"

VfsNode* getVfsNode(VfsNode* directory, char* name) {
    return (VfsNode*) directory->get((struct VfsNode*) directory, name);
}
