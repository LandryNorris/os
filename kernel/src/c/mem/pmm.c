
#include <stdint.h>
#include <stdio.h>
#include "pmm.h"

/**
 * This bitmap
 */
uint8_t* bitmap = (uint8_t*) &_kernel_end;
uint8_t* memStart;
uint32_t pageCount;
uint32_t bitmapSize;

/**
 * Align a pointer to the next 4K alignment, or return it if it is aligned
 * @param ptr
 * @return
 */
inline uint32_t alignToPage(uint32_t ptr) {
    if((ptr & 0x0000FFF) == 0) return ptr;
    return (ptr & 0xFFFFF000) + 0x1000;
}

inline void setBit(int i) {
    int index = i/PAGES_PER_BUCKET;
    int bitIndex = i % PAGES_PER_BUCKET;
    bitmap[index] |= (1 << bitIndex);
}

inline void clearBit(int i) {
    int index = i/PAGES_PER_BUCKET;
    int bitIndex = i % PAGES_PER_BUCKET;
    bitmap[index] &= ~(1 << bitIndex);
}

void initializePmm(uint32_t size) {
    pageCount = size / PAGE_SIZE;
    bitmapSize = pageCount / PAGES_PER_BUCKET;

    printf("Page count is %d. Bitmap size is %d\n", pageCount, bitmapSize);

    // We can speed up our writing by a factor of 4
    // by writing 32 bits at a time instead of 8.
    // Assumes bitmapSize is a multiple of 4.
    uint32_t* efficientBitmap = (uint32_t*) bitmap;

    for(int i = 0; i < bitmapSize/4; i++) {
        efficientBitmap[i] = 0; //clear the bitmap
    }
}

uint32_t firstFreePage() {
    //32-bit aligned index
    int i = 0;

    //first, we scan 4 bytes at a time for any bits being 0.
    uint32_t* efficientBitmap = (uint32_t*) bitmap;
    for(; i < bitmapSize/4; i++) {
        //check 4 bytes at a time. Break if any bits are not set.
        if(efficientBitmap[i] != 0xFFFFFFFF) break;
    }

    //8-bit aligned index of the start of the word
    int index = (i - 1) * 4;
    for(; index < 4; index++) {
        if(bitmap[index] != 0) return index;
    }

    printf("Out of free blocks");
    return -1;
}
