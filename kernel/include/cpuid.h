//
// Created by landry on 2/17/24.
//

#ifndef KERNEL_CPUID_H
#define KERNEL_CPUID_H

#include <stdint.h>

int hasCpuId(); // defined in cpuid.asm
void callCpuId(uint32_t value, uint32_t* ebx, uint32_t* ecx, uint32_t* edx); // defined in cpuid.asm

void getVendorIdString(char* string, int length);

#endif //KERNEL_CPUID_H
