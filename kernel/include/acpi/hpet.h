#ifndef KERNEL_HPET_H
#define KERNEL_HPET_H

#include "acpiPreload.h"
#include "isr.h"
#include <stdbool.h>

#define HPET_CONFIG_ENABLED_MASK 0x1U
#define HPET_CONFIG_USE_LEGACY_MAPPING (1U << 1U)
#define TIMER_CONFIG_SET_32_BIT_MODE_MASK (1U << 8U)
#define TIMER_IS_PERIODIC_MASK (1U << 3U)
#define TIMER_ENABLED_MASK (1U << 2U)
#define TIMER_CONFIG_SUPPORTS_64_BIT_MASK (1U << 5U)
#define TIMER_CONFIG_SUPPORTS_PERIODIC_MASK (1U << 4U)
#define TIMER_CONFIG_WITHOUT_INT_ROUTE_MASK (0xFFFFFFFF0000C1FF)
#define TIMER_CONFIG_AVAILABLE_INT_ROUTE_MASK (0xFFFFFFFF00000000)

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

void setHPETEnabled(bool isEnabled);
void setHPETLegacyMappingEnabled(bool isEnabled);
bool setTimerCallback(int timer, int irq, isr irqHandler);
void setPeriodicTimerFrequency(int timer, int frequency);
void startTimeCounter(int timer);

#endif //KERNEL_HPET_H
