
#include <stdio.h>
#include "multiboot.h"

void* align(const void* address) {
    return (void*)((uint32_t)((uint8_t*)address + 7) & ~7);
}

int parseBootInfo(BootInfo* bootInfo, void* address) {
    Multiboot2TagHeader* header = (Multiboot2TagHeader*) address;

    uint32_t totalSize = header->size;
    printf("Total size is %d\n", totalSize);

    for(Multiboot2Tag* tagAddress = (Multiboot2Tag*)((uint8_t*)address + sizeof(Multiboot2TagHeader));
        (uint8_t*)tagAddress < (uint8_t*) address + totalSize;
        tagAddress = align((uint8_t*)tagAddress + tagAddress->size)) {
        printf("Got type: %d\n", tagAddress->type);

        switch (tagAddress->type) {
            case 4: {
                MemInfo* memInfo = (MemInfo*) tagAddress;
                bootInfo->memoryAvailable = memInfo->upperMemory;
                break;
            }
            case 8: {
                FramebufferInfo* framebufferInfo = (FramebufferInfo*) tagAddress;
                bootInfo->isGraphics = 1;
                bootInfo->framebuffer = (void*)framebufferInfo->framebuffer;
                bootInfo->height = framebufferInfo->height;
                bootInfo->width = framebufferInfo->width;
                bootInfo->bpp = framebufferInfo->bitsPerPixel;
                bootInfo->pitch = framebufferInfo->pitch;
                break;
            }
        }
    }

    return 0;
}
