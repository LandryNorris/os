//
// Created by landry on 7/5/23.
//

#ifndef OS_IDT_H
#define OS_IDT_H

#include <stdint.h>

#define NUM_IDT_ENTRIES 256

typedef struct {
    uint16_t baseLow;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t baseHigh;
} __attribute__((packed)) IdtEntry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) IdtPtr;

extern void setIdt(uint32_t ptr);

void initializeIdt();
void setIdtEntry(int index, uint32_t base, uint16_t sel, uint8_t ring);

#endif //OS_IDT_H
