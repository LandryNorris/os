#ifndef KERNEL_MADT_H
#define KERNEL_MADT_H

#include "acpiPreload.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_APICS 15

#define MADT_TYPE_PROCESSOR_LOCAL_APIC 0
#define MADT_TYPE_IO_APIC 1
#define MADT_TYPE_IO_APIC_INTERRUPT_SOURCE_OVERRIDE 2
#define MADT_TYPE_IO_NMI 3
#define MADT_TYPE_LOCAL_APIC_NMI 4
#define MADT_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE 5
#define MADT_TYPE_PROCESSOR_LOCAL_X2_APIC 9

typedef struct {
    uint8_t processorId;
    uint8_t apicId;
    uint32_t flags;
} ProcessorLocalApic;

typedef struct {
    uint8_t id;
    uint32_t apicAddress;
    uint32_t globalInterruptBase;
} IOApic;

typedef struct {
    uint8_t busSource;
    uint8_t irqSource;
    uint32_t globalSystemInterrupt;
    uint16_t flags;
} IOApicInterruptSourceOverride;

typedef struct {
    uint8_t nmiSource;
    uint16_t flags;
    uint32_t globalSystemInterrupt;
} IOApicNMISource;

typedef struct {
    uint8_t processorId;
    uint8_t lintNumber;
    uint16_t flags;
} LocalApicNMI;

typedef struct {
    uint64_t localApicAddress;
} LocalApicAddressOverride;

typedef struct {
    uint32_t id;
    uint32_t flags;
    uint32_t acpiId;
} LocalX2Apic;

typedef struct {
    int type;
    union {
        ProcessorLocalApic processorLocalApic;
        IOApic ioApic;
        IOApicInterruptSourceOverride ioApicInterruptSourceOverride;
        IOApicNMISource ioApicNmiSource;
        LocalApicNMI localApicNmi;
        LocalApicAddressOverride localApicAddressOverride;
        LocalX2Apic localX2Apic;
    };
} ApicInfo;

int isMadtSignature(char* s);
void parseMadt(MADTLiteral* madt);
bool readApicInfo(int index, ApicInfo* info);

bool isProcessorEnabled(ProcessorLocalApic* apic);
bool canProcessorBeEnabled(ProcessorLocalApic* apic);

#endif //KERNEL_MADT_H
