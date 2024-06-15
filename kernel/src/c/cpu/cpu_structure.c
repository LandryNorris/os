#include "acpi/madt.h"
#include "cpu_structure.h"

static CPUTopology cpuTopology;

void addCpuInfo(CPU cpu) {
    cpuTopology.cpus[cpuTopology.numCpus++] = cpu;
}

void loadLocalApic(ProcessorLocalApic* apic) {
    if(!isProcessorEnabled(apic)) {
        return;
    }
    
    CPU cpu = {
            .processorApicId = apic->apicId,
            .processorId = apic->processorId
    };
    addCpuInfo(cpu);
}

void loadCpuList() {
    ApicInfo info;
    int index = 0;
    while(readApicInfo(index++, &info)) {
        if(info.type == MADT_TYPE_PROCESSOR_LOCAL_APIC) {
            loadLocalApic(&info.processorLocalApic);
        }
    }
}
