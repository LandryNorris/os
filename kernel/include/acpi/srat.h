#ifndef KERNEL_SRAT_H
#define KERNEL_SRAT_H

#include "acpi/acpiPreload.h"

#include <stdbool.h>
#include <stdint.h>

#define MAX_NUMA 64

#define SRAT_PROCESSOR_LOCAL_AFFINITY_TYPE 0
#define SRAT_MEMORY_AFFINITY_TYPE 1
#define SRAT_PROCESSOR_LOCAL_X2_AFFINITY_TYPE 2

typedef struct {
    uint32_t proximityDomain;
    uint8_t apicId;
    uint32_t flags;
    uint8_t sapicEid;
    uint32_t clockDomain;
} SRATProcessorLocalApicAffinity;

typedef struct {
    uint32_t domain;
    uint64_t baseAddress;
    uint64_t length;
    bool hotPluggable;
    bool nonVolatile;
} SRATMemoryAffinity;

typedef struct {
    uint32_t proximityDomain;
    uint32_t x2ApicId;
    uint32_t flags;
    uint32_t clockDomain;
} SRATProcessorLocalX2ApicAffinity;

typedef struct {
    uint8_t type;
    union {
        SRATProcessorLocalApicAffinity processorLocalApicAffinity;
        SRATMemoryAffinity memoryAffinity;
        SRATProcessorLocalX2ApicAffinity processorLocalX2ApicAffinity;
    };
} SRATInfo;

void parseSRAT(SRATLiteral* srat);
int isSRATSignature(char* s);

#endif //KERNEL_SRAT_H
