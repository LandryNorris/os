#ifndef KERNEL_CPUID_MOCK_H
#define KERNEL_CPUID_MOCK_H

#include "stdint.h"

extern "C" {
#include "cpuid.h"
};

void setCpuVendor(char name[12]);
void setFeatures(CpuFeatures features);

#endif //KERNEL_CPUID_MOCK_H
