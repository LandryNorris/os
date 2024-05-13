#include "rsdp.h"

int getAcpiVersion(RSDP* rsdp) {
    if(rsdp->revision == 0 || rsdp->revision == 1) return 1;
    else if(rsdp->revision == 2) return 2;
    else return -1;
}

int validateRSDP(RSDP* rsdp) {
    int signatureCheck =
            rsdp->signature[0] == 'R' &&
            rsdp->signature[1] == 'S' &&
            rsdp->signature[2] == 'D' &&
            rsdp->signature[3] == ' ' &&
            rsdp->signature[4] == 'P' &&
            rsdp->signature[5] == 'T' &&
            rsdp->signature[6] == 'R' &&
            rsdp->signature[7] == ' ';

    int versionCheck = rsdp->revision == 0 || rsdp->revision == 2;

    uint8_t checksum = 0;

    uint8_t* rsdpBytes = (uint8_t*) rsdp;

    for(int i = 0; i < sizeof(RSDP); i++) {
        checksum += rsdpBytes[i];
    }

    return checksum == 0 && versionCheck && signatureCheck;
}
