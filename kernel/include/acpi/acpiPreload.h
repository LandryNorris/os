#ifndef KERNEL_ACPIPRELOAD_H
#define KERNEL_ACPIPRELOAD_H

#include "rsdp.h"
#include <stdint.h>

#define OEM_ID_LENGTH 6
#define SIGNATURE_LENGTH 4
#define OEM_TABLE_ID_LENGTH 8

typedef struct __attribute__ ((packed)) {
    char signature[SIGNATURE_LENGTH];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemId[OEM_ID_LENGTH];
    char oemTableId[OEM_TABLE_ID_LENGTH];
    uint32_t oemRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
} ACPISdtHeader;

typedef struct __attribute__ ((packed)) {
    ACPISdtHeader header;
    uint32_t otherSdtPointers[];
} RSDTLiteral;

typedef struct __attribute__ ((packed)) {
    ACPISdtHeader header;
    uint32_t localApicAddress;
    uint32_t flags;
} MADTLiteral;

typedef struct __attribute__ ((packed)) {
    uint8_t type;
    uint8_t length;
    uint8_t data[];
} MADTRecordHeader;

typedef struct __attribute__ ((packed)) {
    ACPISdtHeader header;
    uint32_t otherSdtPointers[];
} RSDT;

typedef struct __attribute__ ((packed)) {
    uint8_t type;
    uint8_t length;
    uint8_t data[];
} SRATRecordHeader;

typedef struct __attribute__ ((packed)) {
    ACPISdtHeader header;
    uint8_t reserved[12];
} SRATLiteral;



void loadAcpiTables(RSDP* rsdp);

#endif //KERNEL_ACPIPRELOAD_H
