#include <system.h>
#include "pci.h"

uint16_t pciReadDword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t id = (bus << 16) | (device << 11) | (function << 8);
    uint32_t address = id | (0x80000000) | (offset & 0xFC);

    outportInt(PCI_CONFIG_ADDRESS, address);
    return inportInt(PCI_CONFIG_DATA);
}

uint16_t pciReadWord(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t result = pciReadDword(bus, device, function, offset);
    return (result >> ((offset & 2)*8)) & 0xFFFF;
}

uint8_t pciReadByte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t result = pciReadDword(bus, device, function, offset);
    return (result >> (offset & 3)*8) & 0xFF;
}

uint16_t pciReadVendor(uint8_t bus, uint8_t device, uint8_t function) {
    return pciReadWord(bus, device, function, 0);
}

uint16_t pciReadDevice(uint8_t bus, uint8_t device, uint8_t function) {
    return pciReadWord(bus, device, function, 2);
}

uint8_t pciReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    return pciReadByte(bus, device, function, 0xB);
}

uint8_t pciReadSubClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    return pciReadByte(bus, device, function, 0xA);
}

uint8_t pciReadProgrammingInterface(uint8_t bus, uint8_t device, uint8_t function) {
    return pciReadByte(bus, device, function, 0x9);
}

uint8_t pciReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
    return pciReadByte(bus, device, function, 0xE);
}
