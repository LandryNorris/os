#include "acpi/acpiPreload.h"
#include "acpi/madt.h"
#include "acpi/rsdp.h"
#include "acpi/rsdt.h"
#include "acpi/srat.h"

/*
 * Everything in this file will run before we've set up
 * our paging. We cannot call malloc or print to the screen
 * anywhere in this file. Care should be taken such that
 * we never use any advanced features, since almost nothing
 * is initialized at this point
 */

void loadAcpiTables(RSDP* rsdp) {
    if(!validateRSDP(rsdp)) {
        return;
    }

    RSDT* rsdt = 0;

    if(!loadRsdt(rsdp, &rsdt)) {
        return;
    }

    uint32_t numTables = numTablesInRsdt(rsdt);

    for(int i = 0; i < numTables; i++) {
        ACPISdtHeader * table = (ACPISdtHeader*) rsdt->otherSdtPointers[i];
        if(isMadtSignature(table->signature)) {
            parseMadt((MADTLiteral*) table);
        } else if(isSRATSignature(table->signature)) {
            parseSRAT((SRATLiteral*) table);
        }
    }
}
