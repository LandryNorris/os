#ifndef KERNEL_CPU_STRUCTURE_H
#define KERNEL_CPU_STRUCTURE_H

#define MAX_CPUS 64

typedef struct {
    int processorApicId;
    int processorId;
} CPU;

typedef struct {
    int numCpus;
    CPU cpus[MAX_CPUS];
} CPUTopology;

void loadCpuList();

#endif //KERNEL_CPU_STRUCTURE_H
