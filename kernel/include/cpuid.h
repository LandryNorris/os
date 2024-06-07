//
// Created by landry on 2/17/24.
//

#ifndef KERNEL_CPUID_H
#define KERNEL_CPUID_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    union {
        uint32_t raw1;
        struct {
            uint32_t sse3: 1;
            uint32_t pclmulqdq: 1;
            uint32_t dtes64: 1;
            uint32_t monitor: 1;
            uint32_t ds_cpl: 1;
            uint32_t vmx: 1;
            uint32_t smx: 1;
            uint32_t est: 1;
            uint32_t tm2: 1;
            uint32_t ssse3: 1;
            uint32_t cnxt_id: 1;
            uint32_t sdbg: 1;
            uint32_t fma: 1;
            uint32_t cx16: 1;
            uint32_t xtpr: 1;
            uint32_t pdcm: 1;
            uint32_t rsvd: 1;
            uint32_t pcid: 1;
            uint32_t dca: 1;
            uint32_t sse4_1: 1;
            uint32_t sse4_2: 1;
            uint32_t x2apic: 1;
            uint32_t movbe: 1;
            uint32_t popcnt: 1;
            uint32_t tsc_deadline: 1;
            uint32_t aes_ni: 1;
            uint32_t xsave: 1;
            uint32_t osxsave: 1;
            uint32_t avx: 1;
            uint32_t f16c: 1;
            uint32_t rdrnd: 1;
            uint32_t hypervisor: 1;
        };
    };
    union {
        uint32_t raw2;
        struct {
            uint32_t fpu: 1;
            uint32_t vme: 1;
            uint32_t de: 1;
            uint32_t pse: 1;
            uint32_t tsc: 1;
            uint32_t msr: 1;
            uint32_t pae: 1;
            uint32_t mce: 1;
            uint32_t cx8: 1;
            uint32_t apic: 1;
            uint32_t mtrr_rsvd: 1;
            uint32_t sep: 1;
            uint32_t mtrr: 1;
            uint32_t pge: 1;
            uint32_t mca: 1;
            uint32_t cmov: 1;
            uint32_t pat: 1;
            uint32_t pse36: 1;
            uint32_t psn: 1;
            uint32_t clfsh: 1;
            uint32_t nx: 1;
            uint32_t ds: 1;
            uint32_t acpi: 1;
            uint32_t mmx: 1;
            uint32_t fxsr: 1;
            uint32_t sse: 1;
            uint32_t sse2: 1;
            uint32_t ss: 1;
            uint32_t htt: 1;
            uint32_t tm: 1;
            uint32_t ia64: 1;
            uint32_t pbe: 1;
        };
    };
} CpuFeatures;

enum CpuIdRequests {
    CPUID_GETVENDORSTRING,
    CPUID_GETFEATURES,
    CPUID_GETTLB,
    CPUID_GETSERIAL,

    CPUID_INTELEXTENDED = 0x80000000,
    CPUID_INTELFEATURES,
    CPUID_INTELBRANDSTRING,
    CPUID_INTELBRANDSTRINGMORE,
    CPUID_INTELBRANDSTRINGEND,
};

int hasCpuId(); // defined in cpuid.asm
void callCpuId(uint32_t value, uint32_t* ebx, uint32_t* ecx, uint32_t* edx); // defined in cpuid.asm

void getVendorIdString(char* string, int length);
void getCpuFeatures(CpuFeatures* features);

#ifdef __cplusplus
}
#endif

#endif //KERNEL_CPUID_H
