#include <stdio.h>
#include <terminal.h>
#include <gdt.h>
#include "idt.h"
#include "system.h"
#include "isr.h"
#include "interrupts.h"
#include "paging.h"
#include "pmm.h"
#include "serial.h"
#include "mem.h"
#include "multiboot2.h"
#include "buffer.h"

__attribute__((unused)) void kernel_main(uint32_t magic, uint32_t rawAddress) {
    uint32_t address = rawAddress + LOAD_MEMORY_ADDRESS;

    BootInfo bootInfo;
    parseBootInfo(&bootInfo, (void*)address);

    initializeGdt();
    initializeIdt();

    Serial com1;
    initializeSerial(&com1, COM1_PORT);
    serialPrint(&com1, "Printing to COM1\n");

    serialPrint(&com1, "Initializing PMM\n");
    initializePmm(1024 * 1024 * 1024);

    serialPrint(&com1, "Initializing Paging\n");
    initPaging();

    mmapPhysical(bootInfo.framebuffer, bootInfo.pitch*bootInfo.height);

    Canvas canvas;
    initializeCanvas(&canvas, bootInfo.width, bootInfo.height,
                     bootInfo.pitch, bootInfo.bpp, bootInfo.framebuffer, RGB24);

    initializeTerminal(&canvas);

    initializeMalloc(1024 * 1024 * 1024);

    registerInterruptHandler(0x20 + 1, handleKeyboard);

    FileBuffer* buffer = allocFileBuffer(35);

    for(int j = 0; j < 28; j++) {
        for(int i = 0; i < 29; i++) {
            writeByte(buffer, i);
        }

        for(int i = 0; i < 29; i++) {
            uint8_t value;
            readByte(buffer, &value);
            printf("%d ", value);
        }
        printf("\n");
    }

    printf("Hello, World\n");

    //We don't want kernel_main to exit, so we need to loop.
    for (;;);
}
