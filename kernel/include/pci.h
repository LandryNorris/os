#ifndef OS_PCI_H
#define OS_PCI_H

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

#define HEADER_TYPE_GENERAL 0
#define HEADER_TYPE_PCI_BRIDGE 1
#define HEADER_TYPE_PCI_CARDBUS_BRIDGE 2

typedef struct {
    uint16_t deviceId;
    uint16_t vendorId;
    uint8_t classCode;
    uint8_t programmingInterface;
    uint8_t subClass;
    uint8_t headerType;
    uint8_t multiFunction;
    uint8_t bus;
    uint8_t device;
    uint8_t function;
} PciDevice;

typedef struct {
    PciDevice* devices[20];
} PciBus;

void pciScan(PciBus* pciBus);
PciDevice* scanForDeviceClass(PciBus* pciBus, uint8_t class, uint8_t subclass);

uint16_t pciReadVendor(uint8_t bus, uint8_t device, uint8_t function);
uint16_t pciReadDevice(uint8_t bus, uint8_t device, uint8_t function);
uint32_t pciReadDword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
uint16_t pciReadWord(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
uint8_t pciReadByte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

        uint8_t pciReadClassCode(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pciReadSubClassCode(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pciReadProgrammingInterface(uint8_t bus, uint8_t device, uint8_t function);
uint8_t pciReadHeaderType(uint8_t bus, uint8_t device, uint8_t function);

#endif //OS_PCI_H
