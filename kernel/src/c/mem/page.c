
#include <stdio.h>
#include <string.h>
#include "pmm.h"
#include "../../../include/paging.h"
#include "../../../../libk/include/system.h"
#include "../../../include/pmm.h"

void* tableAllocatorPtr;

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
    tableAllocatorPtr += size;
    return tableAllocatorPtr;
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
    uint32_t pageIndex = getPageIndex(virtualAddress);

    PageTable* table = dir->tables[dirIndex];

    //Allocate the table if it doesn't exist yet.
    if(!table) {
        // ToDo: use vmm if it's initialized.
        table = dumbMalloc(sizeof(PageTable));

        memset(table, 0, sizeof(PageTable));

        //we need to convert the address to physical. Just subtract the base address.
        uint32_t address = table - LOAD_MEMORY_ADDRESS;
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
        uint32_t address = firstFreePage();

        tableEntry->present = 1;
        tableEntry->frame = address;
        tableEntry->rw = 1;
        tableEntry->user = 1;
    }
}

extern uint8_t* bitmap;
extern uint32_t bitmapSize;

void initPaging() {
    tableAllocatorPtr = bitmap + bitmapSize;
}
