#ifndef OS_PCI_H
#define OS_PCI_H

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

typedef struct {
    uint16_t deviceId;
    uint16_t vendorId;
    uint8_t classCode;
    uint8_t headerType;
} PciDevice;

typedef struct {
    PciDevice* devices[256][32];
} PciBus;

void pciScan(PciBus* pciBus);

uint16_t pciReadVendor(uint8_t bus, uint8_t device);
uint16_t pciReadDevice(uint8_t bus, uint8_t device);
uint16_t pciReadWord(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

uint8_t pciReadClassCode(uint8_t bus, uint8_t device);
uint8_t pciReadHeaderType(uint8_t bus, uint8_t device);

#endif //OS_PCI_H
