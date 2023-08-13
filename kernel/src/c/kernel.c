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
#include "canvas.h"

__attribute__((unused)) void kernel_main(uint32_t magic, uint32_t rawAddress) {
    uint32_t address = rawAddress + LOAD_MEMORY_ADDRESS;
    initializeTerminal();

    BootInfo bootInfo;
    parseBootInfo(&bootInfo, (void*)address);

    initializeGdt();
    initializeIdt();

    Serial com1;
    initializeSerial(&com1, COM1_PORT);
    serialPrint(&com1, "Printing to COM1\n");

    Canvas canvas;
    initializeCanvas(&canvas, bootInfo.width, bootInfo.height,
                     bootInfo.pitch, bootInfo.bpp, bootInfo.framebuffer, RGB24);

    serialPrint(&com1, "Initializing PMM\n");
    initializePmm(1024 * 1024 * 1024);

    serialPrint(&com1, "Initializing Paging\n");
    initPaging();

    mmapPhysical(bootInfo.framebuffer, bootInfo.pitch*bootInfo.height);

    drawString(&canvas, "Hello, World!", 20, 20);

//    RgbColor color;
//    color.r = 255;
//    color.g = 255;
//    color.b = 255;
//    drawRect(&canvas, 0, 0, canvas.width/2, canvas.height/2, color);
//    color.g = 0;
//    color.b = 0;
//    drawRect(&canvas, canvas.width/2, 0, canvas.width/2, canvas.height/2, color);
//    color.g = 255;
//    color.r = 0;
//    drawRect(&canvas, 0, canvas.height/2, canvas.width/2, canvas.height/2, color);

    initializeMalloc(1024 * 1024 * 1024);

    registerInterruptHandler(0x20 + 1, handleKeyboard);

    printf("Hello, World\n");

    //We don't want kernel_main to exit, so we need to loop.
    for (;;);
}
