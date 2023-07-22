//
// Created by landry on 7/22/23.
//

#ifndef OS_MEM_H
#define OS_MEM_H

#include <stddef.h>

void initializeMalloc(size_t size);
void* malloc(size_t size);
void free(void* ptr);

#endif //OS_MEM_H
