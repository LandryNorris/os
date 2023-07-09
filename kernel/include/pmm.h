#ifndef OS_PMM_H
#define OS_PMM_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGES_PER_BUCKET 8

extern uint32_t _kernel_end;

void initializePmm(uint32_t size);
uint32_t firstFreePage();

#endif //OS_PMM_H
