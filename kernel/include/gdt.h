//
// Created by landry on 7/4/23.
//

#ifndef OS_GDT_H
#define OS_GDT_H

#include <stdint.h>

#define DESCRIPTOR_COUNT 8

typedef struct {
    uint16_t limitLowByte;
    uint16_t baseAddressLowByte;
    uint8_t  baseAddressMidByte;
    uint8_t  access;
    uint8_t  flags;
    uint8_t  baseAddressHighByte;
} __attribute__((packed)) GdtEntry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) GdtPtr;

void initializeGdt();
void setGdtEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
extern void setGdt(uint32_t gdtPtr);

#endif //OS_GDT_H
