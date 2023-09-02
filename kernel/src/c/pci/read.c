#include <system.h>
#include "pci.h"

uint16_t pciReadWord(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t id = (bus << 16) | (device << 11) | (function << 8);
    uint32_t address = id | (0x80000000) | (offset & 0xFC);

    outportInt(PCI_CONFIG_ADDRESS, address);
    uint32_t result = inportInt(PCI_CONFIG_DATA);

    return (result >> ((offset & 2)*8)) & 0xFFFF;
}

uint16_t pciReadVendor(uint8_t bus, uint8_t device) {
    return pciReadWord(bus, device, 0, 0);
}

uint16_t pciReadDevice(uint8_t bus, uint8_t device) {
    return pciReadWord(bus, device, 0, 2);
}

uint8_t pciReadClassCode(uint8_t bus, uint8_t device) {
    uint16_t word = pciReadWord(bus, device, 0, 0xA);
    return word >> 8;
}

uint8_t pciReadSubClassCode(uint8_t bus, uint8_t device) {
    return pciReadWord(bus, device, 0, 0xA) & 0xFF;
}

uint8_t pciReadProgrammingInterface(uint8_t bus, uint8_t device) {
    uint16_t word = pciReadWord(bus, device, 0, 0x8);
    return word >> 8;
}

uint8_t pciReadHeaderType(uint8_t bus, uint8_t device) {
    return pciReadWord(bus, device, 0, 0xE) & 0xFF;
}
