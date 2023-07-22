#include <pic.h>
#include <idt.h>
#include <string.h>
#include "isr.h"

__attribute__((aligned(0x10)))
IdtEntry idt[NUM_IDT_ENTRIES];
IdtPtr idtPtr;

void initializeIdt() {
    initPic(0x20, 0x28);

    memset(idt, 0, sizeof(idt));
    idtPtr.base = (uint32_t) idt;
    idtPtr.limit = sizeof(idt) - 1;

    setIdtEntry(0, (uint32_t) exception0, 0x08, 0x8E);
    setIdtEntry(1, (uint32_t) exception1, 0x08, 0x8E);
    setIdtEntry(2, (uint32_t) exception2, 0x08, 0x8E);
    setIdtEntry(3, (uint32_t) exception3, 0x08, 0x8E);
    setIdtEntry(4, (uint32_t) exception4, 0x08, 0x8E);
    setIdtEntry(5, (uint32_t) exception5, 0x08, 0x8E);
    setIdtEntry(6, (uint32_t) exception6, 0x08, 0x8E);
    setIdtEntry(7, (uint32_t) exception7, 0x08, 0x8E);
    setIdtEntry(8, (uint32_t) exception8, 0x08, 0x8E);
    setIdtEntry(9, (uint32_t) exception9, 0x08, 0x8E);
    setIdtEntry(10, (uint32_t) exception10, 0x08, 0x8E);
    setIdtEntry(11, (uint32_t) exception11, 0x08, 0x8E);
    setIdtEntry(12, (uint32_t) exception12, 0x08, 0x8E);
    setIdtEntry(13, (uint32_t) exception13, 0x08, 0x8E);
    setIdtEntry(14, (uint32_t) exception14, 0x08, 0x8E);
    setIdtEntry(15, (uint32_t) exception15, 0x08, 0x8E);
    setIdtEntry(16, (uint32_t) exception16, 0x08, 0x8E);
    setIdtEntry(17, (uint32_t) exception17, 0x08, 0x8E);
    setIdtEntry(18, (uint32_t) exception18, 0x08, 0x8E);
    setIdtEntry(19, (uint32_t) exception19, 0x08, 0x8E);
    setIdtEntry(20, (uint32_t) exception20, 0x08, 0x8E);
    setIdtEntry(21, (uint32_t) exception21, 0x08, 0x8E);
    setIdtEntry(22, (uint32_t) exception22, 0x08, 0x8E);
    setIdtEntry(23, (uint32_t) exception23, 0x08, 0x8E);
    setIdtEntry(24, (uint32_t) exception24, 0x08, 0x8E);
    setIdtEntry(25, (uint32_t) exception25, 0x08, 0x8E);
    setIdtEntry(26, (uint32_t) exception26, 0x08, 0x8E);
    setIdtEntry(27, (uint32_t) exception27, 0x08, 0x8E);
    setIdtEntry(28, (uint32_t) exception28, 0x08, 0x8E);
    setIdtEntry(29, (uint32_t) exception29, 0x08, 0x8E);
    setIdtEntry(30, (uint32_t) exception30, 0x08, 0x8E);
    setIdtEntry(31, (uint32_t) exception31, 0x08, 0x8E);
    setIdtEntry(32, (uint32_t) irq0, 0x08, 0x8E);
    setIdtEntry(33, (uint32_t) irq1, 0x08, 0x8E);
    setIdtEntry(34, (uint32_t) irq2, 0x08, 0x8E);
    setIdtEntry(35, (uint32_t) irq3, 0x08, 0x8E);
    setIdtEntry(36, (uint32_t) irq4, 0x08, 0x8E);
    setIdtEntry(37, (uint32_t) irq5, 0x08, 0x8E);
    setIdtEntry(38, (uint32_t) irq6, 0x08, 0x8E);
    setIdtEntry(39, (uint32_t) irq7, 0x08, 0x8E);
    setIdtEntry(40, (uint32_t) irq8, 0x08, 0x8E);
    setIdtEntry(41, (uint32_t) irq9, 0x08, 0x8E);
    setIdtEntry(42, (uint32_t) irq10, 0x08, 0x8E);
    setIdtEntry(43, (uint32_t) irq11, 0x08, 0x8E);
    setIdtEntry(44, (uint32_t) irq12, 0x08, 0x8E);
    setIdtEntry(45, (uint32_t) irq13, 0x08, 0x8E);
    setIdtEntry(46, (uint32_t) irq14, 0x08, 0x8E);
    setIdtEntry(47, (uint32_t) irq15, 0x08, 0x8E);
    setIdtEntry(128, (uint32_t) exception128, 0x08, 0x8E);

    setIdt((uint32_t) &(idtPtr));
    __asm__ volatile("sti");
}

void setIdtEntry(int index, uint32_t base, uint16_t sel, uint8_t flags) {
    IdtEntry* entry = &idt[index];
    entry->baseLow = base & 0xFFFF;
    entry->baseHigh = (base >> 16) & 0xFFFF;
    entry->always0 = 0;
    entry->sel = sel;
    entry->flags = flags | 0x60;
}
