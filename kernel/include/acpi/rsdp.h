#ifndef OS_ACPI_H
#define OS_ACPI_H

#include <stdint.h>

typedef struct __attribute__ ((packed)) {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdtAddress;
} RSDP;

int validateRSDP(RSDP* rsdp);
int getAcpiVersion(RSDP* rsdp);

#endif //OS_ACPI_H