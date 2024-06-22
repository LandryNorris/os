#include "acpi/acpiPreload.h"
#include "acpi/hpet.h"
#include "paging.h"
#include "pmm.h"

HPET hpet;

int isHPETSignature(char* s) {
    return s[0] == 'H' && s[1] == 'P' && s[2] == 'E' && s[3] == 'T';
}

// NOLINTBEGIN(readability-magic-numbers)
void parseHPETConfiguration() {
    uint64_t generalRegister = *(uint64_t*) hpet.registerAddress;
    hpet.capabilities.revId = generalRegister & 0xFF;
    hpet.capabilities.numTimers = ((generalRegister >> 8) & 0x1FU) + 1;
    hpet.capabilities.canBe64Bit = generalRegister & (1U << 13U);
    hpet.capabilities.canUseLegacyMapping = generalRegister & (1U << 15U);
    hpet.vendorId = (generalRegister >> 16U) & 0xFFFF;
    hpet.periodFemptoseconds = generalRegister >> 32U;
}
// NOLINTEND(readability-magic-numbers)

void parseHPET(HPETLiteral* hpetTable) {
    if(!isHPETSignature(hpetTable->header.signature)) {
        return;
    }

    mmapPhysical((void*) hpetTable->addressInfo.address, PAGE_SIZE);

    hpet.registerAddress = hpetTable->addressInfo.address;

    parseHPETConfiguration();
}
