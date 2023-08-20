
#include <stdio.h>
#include "pci.h"

void pciScan() {
    printf("Scanning PCI bus...\n");

    for(int bus = 0; bus < 256; bus++) {
        for(int device = 0; device < 32; device++) {
            uint16_t vendor = pciReadVendor(bus, device);
            if(vendor != 0xFFFF) {
                uint16_t deviceId = pciReadDevice(bus, device);
                printf("Found PCI device %x:%x %d/%d\n", bus, device, vendor, deviceId);
            }
        }
    }
    printf("Done scanning PCI bus\n");
}
