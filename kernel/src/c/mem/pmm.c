#include "pmm.h"

#include <stdio.h>

/**
 * This bitmap is used to determine which pages are currently in use.
 */
uint8_t* bitmap;
uint32_t pageCount;
uint32_t bitmapSize;

static inline void setBit(uint32_t i) {
    uint32_t index = i / PAGES_PER_BUCKET;
    uint32_t bitIndex = i % PAGES_PER_BUCKET;
    bitmap[index] |= (1 << bitIndex);
}

static inline void clearBit(int i) {
    int index = i / PAGES_PER_BUCKET;
    int bitIndex = i % PAGES_PER_BUCKET;
    bitmap[index] &= ~(1 << bitIndex);
}

void initializePmm(uint32_t size) {
    bitmap = (uint8_t*) &_kernel_end;
    pageCount = size / PAGE_SIZE;
    bitmapSize = pageCount / PAGES_PER_BUCKET;

    // We can speed up our writing by a factor of 4
    // by writing 32 bits at a time instead of 8.
    // Assumes bitmapSize is a multiple of 4.
    uint32_t* efficientBitmap = (uint32_t*) bitmap;

    for (uint32_t i = 0; i < bitmapSize / 4; i++) {
        efficientBitmap[i] = 0; //clear the bitmap
    }
}

int isSet(uint32_t i) {
    return ((bitmap[i / PAGES_PER_BUCKET] >> (i % PAGES_PER_BUCKET)) & 0x1);
}

uint32_t firstFreePage() {
    for (uint32_t i = 0; i < pageCount; i++) {
        if (!isSet(i)) return i;
    }

    printf("Out of free blocks\n");
    return -1;
}

uint32_t allocateNextPage() {
    uint32_t index = firstFreePage();
    setBit(index);
    return index;
}
