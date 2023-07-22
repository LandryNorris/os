#include <system.h>

uint8_t inportByte(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outportByte(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(data));
}

uint16_t inportShort(uint16_t port) {
    uint16_t result;
    __asm__ volatile ("inw %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

void outportShort(uint16_t port, uint16_t data) {
    __asm__ volatile ("outw %1, %0" : : "dN" (port), "a" (data));
}

uint32_t inportInt(uint16_t port) {
    uint32_t result;
    __asm__ volatile ("inl %%dx, %%eax" : "=a" (result) : "dN" (port));
    return result;
}

void outportInt(uint16_t port, uint32_t data) {
    __asm__ volatile ("outl %%eax, %%dx" : : "dN" (port), "a" (data));
}
