#include "acpi/acpiPreload.h"
#include "acpi/rsdp.h"
#include "acpi/rsdt.h"
#include "paging.h"
#include "pmm.h"

int isRsdtSignature(char* s) {
    return s[0] == 'R' && s[1] == 'S' && s[2] == 'D' && s[3] == 'T';
}

int validateRsdt(RSDT* rsdt) {
    if(!isRsdtSignature(rsdt->header.signature)) {
        return 0;
    }

    uint8_t sum = 0;
    uint8_t* bytePointer = (uint8_t*) rsdt;

    for(int i = 0; i < rsdt->header.length; i++) {
        sum += bytePointer[i];
    }

    return sum == 0;
}

uint32_t numTablesInRsdt(RSDT* rsdt) {
    return (rsdt->header.length - sizeof(ACPISdtHeader))/4;
}

int loadRsdt(RSDP* rsdp, RSDT** rsdtPtrPtr) {
    // We can ignore the lint that casting int -> ptr pessimizes
    // optimization
    // NOLINTNEXTLINE
    void* address = (void*)rsdp->rsdtAddress;

    mmapPhysical(address, PAGE_SIZE);

    RSDT* rsdt = address;

    *rsdtPtrPtr = rsdt;

    for(int i = 0; i < numTablesInRsdt(rsdt); i++) {
        // NOLINTNEXTLINE
        mmapPhysical((void*) rsdt->otherSdtPointers[i], PAGE_SIZE);
    }

    return validateRsdt(*rsdtPtrPtr);
}
