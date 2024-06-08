#ifndef KERNEL_RSDT_H
#define KERNEL_RSDT_H

#include "acpi/acpiPreload.h"
#include "acpi/rsdp.h"
#include <stdint.h>

#define OEM_ID_LENGTH 6
#define SIGNATURE_LENGTH 4
#define OEM_TABLE_ID_LENGTH 8

int loadRsdt(RSDP* rsdp, RSDT** rsdtPtrPtr);
int validateRsdt(RSDT* rsdt);
uint32_t numTablesInRsdt(RSDT* rsdt);

#endif //KERNEL_RSDT_H
