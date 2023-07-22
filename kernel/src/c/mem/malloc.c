#include <stdio.h>
#include "bucket/bucket.h"

void testMalloc();
void* dumbMalloc(uint32_t size); //in page.c

int isMallocInitialized = 0;

void initializeMalloc(size_t size) {
    kernel_heap = initializeKernelHeap(size);
    isMallocInitialized = 1;

    testMalloc();
}

void* malloc(size_t size) {
    if (isMallocInitialized) {
        return kmalloc(size, 1, 0, "malloc'ed");
    } else {
        return dumbMalloc(size);
    }
}

void free(void* ptr) {
    mfree(ptr);
}

void testMalloc() {
    void* first = malloc(100);
    free(first);
    void* second = malloc(100);
    free(second);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer" //we intentionally free the memory. We're comparing pointers.
    if (second != first) {
        printf("Reallocated memory does not match location\n");
    }
#pragma clang diagnostic pop

    void* large = malloc(100000);
    void* next = malloc(10000);

    if (next - large < 100000) {
        printf("Allocated next memory too close to existing");
    }

    free(large);
    free(next);

    void* addresses[20];
    for (int i = 0; i < 20; i++) {
        addresses[i] = malloc(10000);
    }

    for (int i = 0; i < 20; i++) {
        free(addresses[i]);
    }
}
