
#include <stdio.h>
#include <mem.h>
#include "pci.h"

void pciScan(PciBus* pciBus) {
    printf("Scanning PCI pciBus...\n");

    for(int bus = 0; bus < 256; bus++) {
        for(int device = 0; device < 32; device++) {
            uint16_t vendor = pciReadVendor(bus, device, 0);
            if(vendor != 0xFFFF) {
                PciDevice* pciDevice = malloc(sizeof(PciDevice));
                uint8_t headerType = pciReadHeaderType(bus, device, 0);
                uint8_t multifunction = (headerType & 0x80) != 0;

                for(int function = 0; function < 8; function++) {
                    if(pciReadVendor(bus, device, function) == 0xFFFF) continue; //check vendor
                    if(function > 0 && !multifunction) continue; //only one function
                    uint16_t deviceId = pciReadDevice(bus, device, function);
                    pciDevice->deviceId = deviceId;
                    pciDevice->vendorId = vendor;
                    pciDevice->classCode = pciReadClassCode(bus, device, function);
                    pciDevice->subClass = pciReadSubClassCode(bus, device, function);
                    pciDevice->programmingInterface = pciReadProgrammingInterface(bus, device, function);
                    pciDevice->multiFunction = multifunction;
                    pciDevice->headerType = headerType & 0x7F;

                    pciBus->devices[bus][device][function] = pciDevice;

                    printf("Found PCI device %x:%x(%d) %x/%x (%d:%d:%d) (%d)\n", bus, device, function,
                           vendor, deviceId, pciDevice->classCode, pciDevice->subClass,
                           pciDevice->programmingInterface, pciDevice->headerType);
                }
            }
        }
    }
    printf("Done scanning PCI bus\n");
}
