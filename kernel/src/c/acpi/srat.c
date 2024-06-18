//
// Created by landry on 6/15/24.
//

#include "acpi/acpiPreload.h"
#include "acpi/srat.h"

static SRATInfo sratInfoList[MAX_NUMA];
int numSRATInfo = 0;

int isSRATSignature(char* s) {
    return s[0] == 'S' && s[1] == 'R' && s[2] == 'A' && s[3] == 'T';
}

// NOLINTBEGIN(readability-magic-numbers)
void parseSRATInfo(const SRATRecordHeader* record, SRATInfo* info) {
    info->type = record->type;
    switch(record->type) {
        case SRAT_PROCESSOR_LOCAL_AFFINITY_TYPE: {
            info->processorLocalApicAffinity.apicId = record->data[1];
            info->processorLocalApicAffinity.proximityDomain =
                    record->data[0] | record->data[7] << 8U |
                    record->data[8] << 16U | record->data[9] << 24U;
            info->processorLocalApicAffinity.flags = *((uint32_t*) record->data + 2);
            info->processorLocalApicAffinity.clockDomain = *((uint32_t*) record->data + 10);
            break;
        }
        case SRAT_MEMORY_AFFINITY_TYPE: {
            uint8_t flags = record->data[27]; // we can ignore the rest of the bits in flags
            uint32_t baseLow = *((uint32_t*) record->data + 6);
            uint32_t baseHigh = *((uint32_t*) record->data + 10);
            uint32_t lengthLow = *((uint32_t*) record->data + 14);
            uint32_t lengthHigh = *((uint32_t*) record->data + 18);

            if((flags & 0x1) == 0) {
                break;
            }

            info->memoryAffinity.domain = *((uint32_t*) record->data);
            info->memoryAffinity.baseAddress = baseLow | ((uint64_t)baseHigh << 32U);
            info->memoryAffinity.length = lengthLow | ((uint64_t)lengthHigh << 32U);
            info->memoryAffinity.hotPluggable = flags & 0x2;
            info->memoryAffinity.nonVolatile = flags & 0x4;
            break;
        }
        case SRAT_PROCESSOR_LOCAL_X2_AFFINITY_TYPE: {
            // TODO(Landry): parse this when I care about X2Apic
            break;
        }
        default: break;
    }
}
// NOLINTEND(readability-magic-numbers)

void parseSRAT(SRATLiteral* srat) {
    if(!isSRATSignature(srat->header.signature)) {
        return;
    }

    // the SRAT has a header and 12 bytes before its list
    int lengthParsedSoFar = sizeof(ACPISdtHeader) + 12;

    int i = 0;
    while(lengthParsedSoFar < srat->header.length) {
        const SRATRecordHeader* record =
                (SRATRecordHeader*) ((uint8_t*)srat + lengthParsedSoFar);
        if(i >= MAX_NUMA) {
            continue;
        }
        parseSRATInfo(record, &sratInfoList[i++]);
        numSRATInfo++;
        lengthParsedSoFar += record->length;
    }
}
