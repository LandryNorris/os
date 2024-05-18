#include "isr.h"
#include "paging.h"
#include "pmm.h"
#include "system.h"

#include <mem.h>
#include <stdio.h>
#include <string.h>

/*
 * Page Directory structure:
 *
 * Directory:
 * [1024 page directory entries]
 * These entries each manage 4MiB of memory space. Which
 * 4MiB an entry manages is based on its index: the first
 * manages the first 4MiB, the second manages the next 4MiB,
 * and so on. Each entry contains a 4kiB-aligned pointer to
 * a Page Table
 *
 * Page Table:
 * [1024 ]
 */

void* tableAllocatorPtr;
int isPagingEnabled = 0;

PageDirectory* pageDirectory;

/**
 * Very simple rolling pointer allocator. This is
 * used before we have our actual allocator set up.
 * It should never be called after our virtual
 * memory allocator is initialized.
 *
 * This allocator just starts storing data after the
 * pmm bitmap.
 *
 * @param size
 */
void* dumbMalloc(uint32_t size) {
    void* result = tableAllocatorPtr;
    tableAllocatorPtr += size;
    return result;
}

void* convertVirtualToPhysical(PageDirectory* dir, uint32_t virtualAddress) {
    // if we haven't set up the page directory, we just need to subtract the load address.
    if(!isPagingEnabled) return (void*) (virtualAddress - LOAD_MEMORY_ADDRESS);

    uint32_t dirIndex = getPageDirectoryIndex(virtualAddress);
    uint32_t tableIndex = getTableIndex(virtualAddress);
    uint32_t pageIndex = getPageIndex(virtualAddress);

    PageTable* table = dir->tables[dirIndex];
    if (!table) {
        printf("Table at %d not set", dirIndex);
    }

    PageTableEntry entry = table->pages[tableIndex];
    if (!entry.present) {
        printf("Page table entry at %d does not exist", dirIndex);
    }
    uint32_t frame = entry.frame;

    frame = (frame << 12) + pageIndex;

    return (void*) frame;
}

/**
 * Determine if the page containing a given address is allocated yet.
 * @param dir
 * @param virtualAddress
 * @return
 */
int isPageAllocated(PageDirectory* dir, uint32_t virtualAddress) {
    uint32_t dirIndex = getPageDirectoryIndex(virtualAddress);
    uint32_t tableIndex = getTableIndex(virtualAddress);
    //printf("Checking if page is allocated: %d %d\n", dirIndex, tableIndex);

    PageTable* table = dir->tables[dirIndex];
    if (!table) return 0;

    PageTableEntry entry = table->pages[tableIndex];
    return entry.present;
}

void allocatePhysicalPage(PageDirectory* dir, uint32_t virtualAddress, uint32_t physicalAddress) {
    uint32_t dirIndex = getPageDirectoryIndex(virtualAddress);
    uint32_t tableIndex = getTableIndex(virtualAddress);

    PageTable* table = dir->tables[dirIndex];

    //Allocate the table if it doesn't exist yet.
    if (!table) {
        table = malloc(sizeof(PageTable));
        memset(table, 0, sizeof(PageTable));

        //We need a physical address.
        uint32_t address = (uint32_t)convertVirtualToPhysical(dir, (uint32_t)table);

        PageDirectoryEntry* entry = &dir->entries[dirIndex];
        entry->frame = address >> 12;
        entry->present = 1;
        entry->writable = 1;
        entry->user = 1;
        entry->largePage = 0;

        //Save a reference for later usage.
        dir->tables[dirIndex] = table;
    }

    PageTableEntry* tableEntry = &table->pages[tableIndex];

    if (!tableEntry->present) {
        uint32_t address = physicalAddress/PAGE_SIZE;

        tableEntry->present = 1;
        tableEntry->frame = address;
        tableEntry->rw = 1;
        tableEntry->user = 1;
    }
}

void allocatePage(PageDirectory* dir, uint32_t virtualAddress) {
    uint32_t dirIndex = getPageDirectoryIndex(virtualAddress);
    uint32_t tableIndex = getTableIndex(virtualAddress);

    PageTable* table = dir->tables[dirIndex];

    //Allocate the table if it doesn't exist yet.
    if (!table) {
        table = malloc(sizeof(PageTable));
        memset(table, 0, sizeof(PageTable));

        //We need a physical address.
        uint32_t address = (uint32_t)convertVirtualToPhysical(dir, (uint32_t)table);

        PageDirectoryEntry* entry = &dir->entries[dirIndex];
        entry->frame = address >> 12;
        entry->present = 1;
        entry->writable = 1;
        entry->user = 1;
        entry->largePage = 0;

        //Save a reference for later usage.
        dir->tables[dirIndex] = table;
    }

    PageTableEntry* tableEntry = &table->pages[tableIndex];

    if (!tableEntry->present) {
        uint32_t address = allocateNextPage();

        tableEntry->present = 1;
        tableEntry->frame = address;
        tableEntry->rw = 1;
        tableEntry->user = 1;
    }
}

void pageFaultHandler(Register* registers) {
    __asm__ volatile("sti");
    printf("Page Fault!\n");

    uint32_t errorCode = registers->errno;
    uint32_t present = errorCode & 0x01;
    uint32_t rw = errorCode & 0x02;
    uint32_t reserved = errorCode & 0x08;
    uint32_t instructionFetch = errorCode & 0x10;

    printf("Possible causes:\n");
    if (!present) printf("Page is not present\n");
    if (rw) printf("Page is read-only\n");
    if (reserved) printf("Overwrote reserved bit\n");
    if (instructionFetch) printf("Error on instruction fetch\n");
    printf("\n");
}

void enablePaging() {
    uint32_t cr0, cr4;

    __asm__ volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 &= ~0x10;
    __asm__ volatile("mov %0, %%cr4"::"r"(cr4));

    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0"::"r"(cr0));

    isPagingEnabled = 1;
}

void setPageDirectory(PageDirectory* dir) {
    uint32_t address = (uint32_t) convertVirtualToPhysical(dir, (uint32_t) dir);
    setPageDirectoryLowLevel(address);
}

/**
 * Align a pointer to the next 4K alignment, or return it if it is aligned
 * @param ptr
 * @return
 */
static inline uint32_t alignToPage(uint32_t ptr) {
    if ((ptr & 0x0000FFF) == 0) return ptr;
    return (ptr & 0xFFFFF000) + 0x1000;
}

void* mmapPhysical(void* address, size_t length) {
    size_t numPages = length / PAGE_SIZE;
    uint32_t alignedAddress = alignToPage((uint32_t) address);

    for (size_t i = 0; i < numPages; i++) {
        uint32_t pageAddress = alignedAddress + i * PAGE_SIZE;
        //ToDo: check if page is not already allocated
        if (isPageAllocated(pageDirectory, pageAddress)) {
            printf("Page %x is already allocated!\n", pageAddress);
        }
        allocatePhysicalPage(pageDirectory, pageAddress, pageAddress);
    }

    return (void*) alignedAddress;
}

void* mmap(void* address, size_t length) {
    size_t numPages = length / PAGE_SIZE;
    uint32_t alignedAddress;
    if (address == NULL) {
        //find some unallocated pages after our kernel.
        uint32_t addressToCheck = LOAD_MEMORY_ADDRESS + 4 * 1024 * 1024;
        while (isPageAllocated(pageDirectory, addressToCheck)) {
            addressToCheck += 4 * 1024;
        }
        alignedAddress = alignToPage(addressToCheck);
    } else {
        alignedAddress = alignToPage((uint32_t) address);
    }
    for (size_t i = 0; i < numPages; i++) {
        uint32_t pageAddress = alignedAddress + i * PAGE_SIZE;
        //ToDo: check if page is not already allocated
        if (isPageAllocated(pageDirectory, pageAddress)) {
            printf("Page %x is already allocated!\n", pageAddress);
        }
        allocatePage(pageDirectory, pageAddress);
    }

    return (void*) alignedAddress;
}

void* munmap(void* address, size_t length) {
    //Todo: Implement memory unmapping
}

extern uint8_t* bitmap;
extern uint32_t bitmapSize;

void initPaging() {
    tableAllocatorPtr = bitmap + bitmapSize;

    pageDirectory = malloc(sizeof(PageDirectory));

    memset(pageDirectory, 0, sizeof(PageDirectory));

    registerInterruptHandler(14, pageFaultHandler);

    // Map the 4MiB from 0xC0000000 to 0xC0400000
    uint32_t end = LOAD_MEMORY_ADDRESS + 4 * 1024 * 1024;
    for (uint32_t i = LOAD_MEMORY_ADDRESS; i < end; i += PAGE_SIZE) {
        allocatePage(pageDirectory, i);
    }

    setPageDirectory(pageDirectory);

    enablePaging();
}
