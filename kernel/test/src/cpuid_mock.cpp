#include "cpuid_mock.h"

#include <cstdio>
#include <cstring>
#include <gtest/gtest.h>

typedef struct {
    char name[12];
    CpuFeatures features;
} CpuIdInfo;

CpuIdInfo cpuInfo;

void setCpuVendor(char* name) {
    int length = strlen(name);
    ASSERT_EQ(length, 12) << "The length of the CPU Vendor name must be exactly 12";
    memcpy(cpuInfo.name, name, 12);
}

void setFeatures(CpuFeatures features) {
    cpuInfo.features = features;
}

void callCpuId(uint32_t value, uint32_t* ebx, uint32_t* ecx, uint32_t* edx) {
    switch(value) {
        case 0: {
            char* name = cpuInfo.name;
            *ebx = *((uint32_t*)name);
            *edx = *((uint32_t*) (name+4));
            *ecx = *((uint32_t*) (name+8));
            break;
        }
        case 1: {
            *ecx = cpuInfo.features.raw1;
            *edx = cpuInfo.features.raw2;
            break;
        }
        default:
            printf("No implementation for CPUID %d mocked!\n", value);
            break;
    }
}
