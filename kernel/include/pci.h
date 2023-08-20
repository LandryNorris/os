#ifndef OS_PCI_H
#define OS_PCI_H

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

void pciScan();
uint16_t pciReadVendor(uint8_t bus, uint8_t device);
uint16_t pciReadDevice(uint8_t bus, uint8_t device);
uint16_t pciReadWord(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

#endif //OS_PCI_H
