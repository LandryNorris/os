
#include <stdio.h>
#include <mem.h>
#include "pci.h"

void pciScan(PciBus* pciBus) {
    printf("Scanning PCI pciBus...\n");

    for(int bus = 0; bus < 256; bus++) {
        for(int device = 0; device < 32; device++) {
            uint16_t vendor = pciReadVendor(bus, device);
            if(vendor != 0xFFFF) {
                uint16_t deviceId = pciReadDevice(bus, device);
                PciDevice* pciDevice = malloc(sizeof(PciDevice));

                pciDevice->deviceId = deviceId;
                pciDevice->vendorId = vendor;
                pciDevice->classCode = pciReadClassCode(bus, device);
                pciDevice->headerType = pciReadHeaderType(bus, device);

                pciBus->devices[bus][device] = pciDevice;

                printf("Found PCI device %x:%x %d/%d (%d)\n", bus, device,
                       vendor, deviceId, pciDevice->classCode);
            }
        }
    }
    printf("Done scanning PCI pciBus\n");
}
