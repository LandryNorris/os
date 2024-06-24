#include "acpi/acpiPreload.h"
#include "acpi/hpet.h"
#include "paging.h"
#include "pmm.h"

HPET hpet;

int isHPETSignature(char* s) {
    return s[0] == 'H' && s[1] == 'P' && s[2] == 'E' && s[3] == 'T';
}

uint64_t* timerConfigRegister(int timer) {
    // NOLINTNEXTLINE
    uint32_t offset = 0x100 + 0x20*timer;
    return (uint64_t*) (hpet.registerAddress + offset);
}

uint64_t* timerComparatorRegister(int timer) {
    // NOLINTNEXTLINE
    uint32_t offset = 0x108 + 0x20*timer;
    return (uint64_t*) (hpet.registerAddress + offset);
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

void parseTimers() {
    for(int i = 0; i < hpet.capabilities.numTimers; i++) {
        Timer* timer = &hpet.timers[i];
        timer->configurationRegister = timerConfigRegister(i);
        timer->comparatorRegister = timerComparatorRegister(i);

        uint64_t configValue = *timer->configurationRegister;

        timer->enabled = (configValue & TIMER_ENABLED_MASK) != 0;
        timer->isPeriodic = (configValue & TIMER_IS_PERIODIC_MASK) != 0;
        timer->supports64Bit = (configValue & TIMER_CONFIG_SUPPORTS_64_BIT_MASK) != 0;
        timer->supportsPeriodic = (configValue & TIMER_CONFIG_SUPPORTS_PERIODIC_MASK) != 0;
    }
}

// NOLINTEND(readability-magic-numbers)

void parseHPET(HPETLiteral* hpetTable) {
    if(!isHPETSignature(hpetTable->header.signature)) {
        return;
    }

    mmapPhysical((void*) hpetTable->addressInfo.address, PAGE_SIZE);

    hpet.registerAddress = hpetTable->addressInfo.address;

    parseHPETConfiguration();
    parseTimers();

    hpet.counterRegister = (uint64_t*) (hpet.registerAddress + 0x20U);
}

void setHPETEnabled(bool isEnabled) {
    hpet.enabled = isEnabled;
    uint64_t* generalConfigRegister = (uint64_t*) (hpet.registerAddress + 0x10U);

    if(isEnabled) {
        *generalConfigRegister |= HPET_CONFIG_ENABLED_MASK;
    } else {
        *generalConfigRegister &= ~HPET_CONFIG_ENABLED_MASK;
    }
}

uint64_t getMainCounterValue() {
    // TODO(Landry): Check if upper 32 bits changes between reads to check
    //  for overflow
    return *hpet.counterRegister;
}

void setTimer64BitMode(int timer, bool is64BitEnabled) {
    uint64_t* configRegister = timerConfigRegister(timer);

    if(!is64BitEnabled) {
        *configRegister |= TIMER_CONFIG_SET_32_BIT_MODE_MASK;
    } else {
        *configRegister &= ~TIMER_CONFIG_SET_32_BIT_MODE_MASK;
    }
}

void setTimerComparator(int timer, uint64_t comparatorValue) {
    *hpet.timers[timer].comparatorRegister = comparatorValue;
}

void startPeriodicTimer(int timer, int frequency) {
    int periodFemptoSeconds = (int) (1e15 / frequency);

    int numTicks = periodFemptoSeconds / hpet.periodFemptoseconds;



    // The timer is still going. We want to minimize time between
    // reading counter register and setting time to trigger.
    int timeToTrigger = getMainCounterValue() + numTicks;
    setTimerComparator(timer, timeToTrigger);
}
