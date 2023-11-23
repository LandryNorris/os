#include <stdio.h>
#include <terminal.h>
#include <gdt.h>
#include <string.h>
#include "idt.h"
#include "system.h"
#include "isr.h"
#include "interrupts.h"
#include "paging.h"
#include "pmm.h"
#include "serial.h"
#include "mem.h"
#include "multiboot2.h"
#include "privatestdbuf.h"
#include "createbuffer.h"
#include "pci.h"
#include "ide.h"

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

    FileBuffer* in = allocFileBuffer(4096);
    FileBuffer* out = allocFileBuffer(4096);

    initializeBuffers(in, out);

    registerInterruptHandler(0x20 + 1, handleKeyboard);

    PciBus* pciBus = malloc(sizeof(PciBus));

    pciScan(pciBus);

    initializeIdeCompatibility();

    for(int i = 0; i < 4; i++) {
        IdeDevice* device = &ide.devices[i];
        if(!device->exists) continue;
        printf("Found drive %s with size %d GB\n", device->model,
               device->size/1024/1024/2);
    }

    uint8_t data[1024];
    ataReadSectors(&ide.devices[0], 2, data, 1024);

    printf("Hello, World\n");

    //We don't want kernel_main to exit, so we need to loop.
    for (;;) {
        if(bufferAvailable(out)) {
            char c;
            int result = readChar(out, &c);

            if(result) {
                terminalPutChar(c);
            }
        }
    }
}
