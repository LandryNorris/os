#include "cpuid.h"
#include <stdint.h>

void getVendorIdString(char* string, int length) {
    uint32_t ebx, ecx, edx;

    callCpuId(0, &ebx, &ecx, &edx);

    if(length >= 5) {
        *((uint32_t*) string) = ebx;
    }
    if(length >= 9) {
        *((uint32_t*) (string+4)) = edx;
    }
    if(length >= 13) {
        *((uint32_t*) (string+8)) = ecx;
    }
    if(length > 13) {
        string[12] = '\0';
    } else {
        string[length-1] = '\0';
    }
}

void getCpuFeatures(CpuFeatures* features) {
    uint32_t ecx, edx;
    uint32_t dummy = 0;
    callCpuId(1, &dummy, &ecx, &edx);

    features->raw1 = ecx;
    features->raw2 = edx;
}
