#include "acpi/acpiPreload.h"
#include "acpi/madt.h"

static ApicInfo apicInfoList[MAX_APICS];
static int numAcpiInfo = 0;

int isMadtSignature(char* s) {
    return s[0] == 'A' && s[1] == 'P' && s[2] == 'I' && s[3] == 'C';
}

void parseApicInfo(const MADTRecordHeader* record, ApicInfo* info) {
    switch(record->type) {
        case MADT_TYPE_PROCESSOR_LOCAL_APIC: {
            info->processorLocalApic.processorId = record->data[0];
            info->processorLocalApic.apicId = record->data[1];
            info->processorLocalApic.flags = *((uint32_t*)(record->data + 3));
            break;
        }
        case MADT_TYPE_IO_APIC: {
            info->ioApic.id = record->data[0];
            info->ioApic.apicAddress = *((uint32_t*)(record->data + 2));
            info->ioApic.globalInterruptBase = *((uint32_t*)(record->data + 6));
            break;
        }
        case MADT_TYPE_IO_APIC_INTERRUPT_SOURCE_OVERRIDE: {
            info->ioApicInterruptSourceOverride.busSource = record->data[0];
            info->ioApicInterruptSourceOverride.irqSource = record->data[1];
            info->ioApicInterruptSourceOverride.globalSystemInterrupt =
                    *((uint32_t*) (record->data + 2));
            info->ioApicInterruptSourceOverride.flags =
                    *((uint16_t*) (record->data+6));
            break;
        }
        case MADT_TYPE_IO_NMI: {
            info->ioApicNmiSource.nmiSource = record->data[0];
            info->ioApicNmiSource.flags = *((uint16_t*) record->data+2);
            info->ioApicNmiSource.globalSystemInterrupt =
                    *((uint32_t*) record->data+4);
            break;
        }
        case MADT_TYPE_LOCAL_APIC_NMI: {
            info->localApicNmi.processorId = record->data[0];
            info->localApicNmi.flags = *((uint16_t*) record->data+1);
            info->localApicNmi.lintNumber = record->data[3];
            break;
        }
        case MADT_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE: {
            info->localApicAddressOverride.localApicAddress =
                    *((uint64_t*) record->data);
            break;
        }
        case MADT_TYPE_PROCESSOR_LOCAL_X2_APIC: {
            info->localX2Apic.acpiId = *((uint32_t*) record->data + 2);
            info->localX2Apic.flags = *((uint32_t*) record->data + 6);
            info->localX2Apic.id = *((uint32_t*) record->data + 10);
            break;
        }
        default: break;
    }
}

void parseMadt(MADTLiteral* madt) {
    if(!isMadtSignature(madt->header.signature)) {
        return;
    }

    // the MADT has a header and 8 bytes before its list
    int lengthParsedSoFar = sizeof(ACPISdtHeader) + 8;

    int i = 0;
    while(lengthParsedSoFar < madt->header.length) {
        const MADTRecordHeader* record =
                (MADTRecordHeader*) ((uint8_t*)madt + lengthParsedSoFar);
        if(i >= MAX_APICS) {
            continue;
        }
        parseApicInfo(record, &apicInfoList[i++]);
        numAcpiInfo++;
        lengthParsedSoFar += record->length;
    }
}
