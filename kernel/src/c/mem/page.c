
#include <stdio.h>
#include <string.h>
#include "pmm.h"
#include "../../../include/paging.h"
#include "../../../../libk/include/system.h"
#include "../../../include/pmm.h"
#include "../../../include/isr.h"

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
    uint32_t dirIndex = getPageDirectoryIndex(virtualAddress);
    uint32_t tableIndex = getTableIndex(virtualAddress);
    uint32_t pageIndex = getPageIndex(virtualAddress);

    PageTable* table = dir->tables[dirIndex];
    if(!table) {
        printf("Table at %d not set", dirIndex);
    }

    PageTableEntry entry = table->pages[tableIndex];
    if(!entry.present) {
        printf("Page table entry at %d does not exist", dirIndex);
    }
    uint32_t frame = entry.frame;

    frame = (frame << 12) + pageIndex;

    return (void*) frame;
}

void allocatePage(PageDirectory* dir, uint32_t virtualAddress) {
    uint32_t dirIndex = getPageDirectoryIndex(virtualAddress);
    uint32_t tableIndex = getTableIndex(virtualAddress);

    PageTable* table = dir->tables[dirIndex];

    //Allocate the table if it doesn't exist yet.
    if(!table) {
        // ToDo: use vmm if it's initialized.
        table = dumbMalloc(sizeof(PageTable));
        memset(table, 0, sizeof(PageTable));

        //we need to convert the address to physical. Just subtract the base address.
        uint32_t address = (uint32_t) (table - LOAD_MEMORY_ADDRESS);
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

    if(!tableEntry->present) {
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

    uint32_t errorCode = registers->err_code;
    uint32_t present = errorCode & 0x01;
    uint32_t rw = errorCode & 0x02;
    uint32_t reserved = errorCode & 0x08;
    uint32_t instructionFetch = errorCode & 0x10;

    printf("Possible causes:\n");
    if(!present) printf("Page is not present\n");
    if(rw) printf("Page is read-only\n");
    if(reserved) printf("Overwrote reserved bit\n");
    if(instructionFetch) printf("Error on instruction fetch\n");
    printf("\n");
}

void enablePaging() {
    uint32_t cr0, cr4;

    __asm__ volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 &= ~0x10;
    __asm__ volatile("mov %0, %%cr4" :: "r"(cr4));

    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0));

    isPagingEnabled = 1;
}

void setPageDirectory(PageDirectory* dir) {
    uint32_t address = (uint32_t) (dir - LOAD_MEMORY_ADDRESS);

    printf("Address: %x\n", address);
    setPageDirectoryLowLevel(address);
}

extern uint8_t* bitmap;
extern uint32_t bitmapSize;

void initPaging() {
    tableAllocatorPtr = bitmap + bitmapSize;

    pageDirectory = dumbMalloc(sizeof(PageDirectory));

    memset(pageDirectory, 0, sizeof(PageDirectory));

    registerInterruptHandler(14, pageFaultHandler);

    // Map the 4MiB from 0xC0000000 to 0xC0400000
    uint32_t end = LOAD_MEMORY_ADDRESS + 4*1024*1024;
    for(uint32_t i = LOAD_MEMORY_ADDRESS; i < end; i += PAGE_SIZE) {
        allocatePage(pageDirectory, i);
    }

    setPageDirectory(pageDirectory);

    enablePaging();
}
