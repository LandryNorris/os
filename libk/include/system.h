//
// Created by landry on 7/4/23.
//

#ifndef OS_SYSTEM_H
#define OS_SYSTEM_H

#include <stdint.h>

// We've set up our kernel to load at 0xC0000000, so any absolute addresses need to
// subtract this value.
#define LOAD_MEMORY_ADDRESS 0xC0000000
#define VGA_ADDRESS 0xC03FF000

uint8_t inportByte(uint16_t port);
void outportByte(uint16_t port, uint8_t data);
uint16_t inportShort(uint16_t port);
void outportShort(uint16_t port, uint16_t data);
uint32_t inportInt(uint16_t port);
void outportInt(uint16_t port, uint32_t data);

typedef struct registers
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
}Register;

#endif //OS_SYSTEM_H
