#ifndef OS_PAGING_H
#define OS_PAGING_H

#include <stdint.h>

typedef struct {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t reserved   : 2;
    uint32_t accessed   : 1;
    uint32_t dirty      : 1;
    uint32_t reserved2  : 2;
    uint32_t available  : 3;
    uint32_t frame      : 20;
} PageTableEntry;

typedef struct {
    uint32_t present    : 1;
    uint32_t writable         : 1;
    uint32_t user       : 1;
    uint32_t w_through  : 1;
    uint32_t cache      : 1;
    uint32_t access     : 1;
    uint32_t reserved   : 1;
    uint32_t largePage  : 1;
    uint32_t global     : 1;
    uint32_t available  : 3;
    uint32_t frame      : 20;
} PageDirectoryEntry;

typedef struct {
    PageTableEntry pages[1024];
} PageTable;

typedef struct {
    PageDirectoryEntry entries[1024];
    PageTable* tables[1024];
} PageDirectory;

extern PageDirectory* boot_page_directory;

/**
 * @param virtualAddress
 * @return the index of the page table in a page directory
 */
static inline uint32_t getPageDirectoryIndex(uint32_t virtualAddress) {
    return virtualAddress >> 22;
}

/**
 * @param virtualAddress
 * @return the index of the page in a page table
 */
static inline uint32_t getTableIndex(uint32_t virtualAddress) {
    return (virtualAddress >> 12) & 0x3FF;
}

/**
 * @param virtualAddress
 * @return number of bytes into a page
 */
static inline uint32_t getPageIndex(uint32_t virtualAddress) {
    return virtualAddress & 0xFFF;
}

void initPaging();
extern void setPageDirectoryLowLevel(uint32_t dir);

void* mmapPhysical(void* address, size_t length);
void* mmap(void* address, size_t length);
void* munmap(void* address, size_t length);

#endif //OS_PAGING_H
