#ifndef KERNEL_HPET_H
#define KERNEL_HPET_H

#include "acpiPreload.h"
#include <stdbool.h>

#define HPET_CONFIG_ENABLED_MASK 0x1U
#define TIMER_CONFIG_SET_32_BIT_MODE_MASK (1U << 8U)
#define TIMER_IS_PERIODIC_MASK (1U << 3U)
#define TIMER_ENABLED_MASK (1U << 2U)
#define TIMER_CONFIG_SUPPORTS_64_BIT_MASK (1U << 5U)
#define TIMER_CONFIG_SUPPORTS_PERIODIC_MASK (1U << 4U)

typedef struct {
    uint8_t revId;
    uint8_t numTimers;
    bool canBe64Bit;
    bool canUseLegacyMapping;
} Capabilities;

typedef struct {
    bool enabled;
    bool isPeriodic;
    bool supports64Bit;
    bool supportsPeriodic;
    uint64_t* configurationRegister;
    uint64_t* comparatorRegister;
} Timer;

typedef struct {
    uint32_t registerAddress;
    bool enabled;
    bool legacyReplacementEnabled;
    Capabilities capabilities;

    uint16_t vendorId;
    uint32_t periodFemptoseconds;

    uint64_t* counterRegister;

    Timer timers[32];
} HPET;

int isHPETSignature(char* s);
void parseHPET(HPETLiteral* table);

#endif //KERNEL_HPET_H
