#ifndef OS_MULTIBOOT_H
#define OS_MULTIBOOT_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint32_t architecture;
    uint32_t length;
    uint32_t checksum;
} MultibootHeader;

typedef struct __attribute__((packed)) {
    uint32_t size;
    uint32_t reserved;
} Multiboot2TagHeader;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
} Multiboot2Tag;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint32_t lowerMemory;
    uint32_t upperMemory;
} MemInfo;

typedef struct __attribute__((packed)) {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bitsPerPixel;
    uint8_t framebufferType;
    uint8_t reserved;
    uint8_t colorInfo[];
} FramebufferInfo;

typedef struct __attribute__((packed)) {
    /**
     * Memory available for use in kB
     */
    uint32_t memoryAvailable;
    /**
     * Whether we are in line graphics mode or character mode
     */
    uint8_t isGraphics;
    /**
     * Location of the framebuffer if isGraphics, NULL otherwise.
     */
    uint32_t framebuffer;
    /**
     * Width of the screen. Pixels if isGraphics, characters otherwise
     */
    uint32_t width;
    /**
     * Height of the screen. Pixels if isGraphics, characters otherwise
     */
    uint32_t height;
} BootInfo;

int parseBootInfo(BootInfo* bootInfo, void* address);

#endif //OS_MULTIBOOT_H
