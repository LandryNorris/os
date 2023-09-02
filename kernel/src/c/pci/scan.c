
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
                uint8_t headerType = pciReadHeaderType(bus, device);

                pciDevice->deviceId = deviceId;
                pciDevice->vendorId = vendor;
                pciDevice->classCode = pciReadClassCode(bus, device);
                pciDevice->subClass = pciReadSubClassCode(bus, device);
                pciDevice->multiFunction = headerType != 0;
                pciDevice->headerType = headerType & 0x7F;

                pciBus->devices[bus][device] = pciDevice;

                printf("Found PCI device %x:%x %d/%d (%d:%d) (%d)\n", bus, device,
                       vendor, deviceId, pciDevice->classCode, pciDevice->subClass,
                       pciDevice->headerType);
            }
        }
    }
    printf("Done scanning PCI bus\n");
}
