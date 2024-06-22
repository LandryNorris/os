#ifndef KERNEL_HPET_H
#define KERNEL_HPET_H

#include "acpiPreload.h"
#include <stdbool.h>

typedef struct {
    uint8_t revId;
    uint8_t numTimers;
    bool canBe64Bit;
    bool canUseLegacyMapping;
} Capabilities;

typedef struct {
    uint32_t registerAddress;
    bool enabled;
    bool legacyReplacementEnabled;
    Capabilities capabilities;

    uint8_t numComparators;


    uint16_t vendorId;
    uint32_t periodFemptoseconds;
} HPET;

int isHPETSignature(char* s);
void parseHPET(HPETLiteral* table);

#endif //KERNEL_HPET_H
