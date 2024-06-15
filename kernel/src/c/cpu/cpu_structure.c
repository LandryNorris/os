#include "acpi/madt.h"
#include "cpu_structure.h"

static CPUTopology cpuTopology;

void addCpuInfo(CPU cpu) {
    cpuTopology.cpus[cpuTopology.numCpus++] = cpu;
}

void loadCpuList() {
    ApicInfo info;
    int index = 0;
    while(readApicInfo(index++, &info)) {
        if(info.type == MADT_TYPE_PROCESSOR_LOCAL_APIC) {
            if(isProcessorEnabled(&info.processorLocalApic)) {
                CPU cpu = {
                        .processorApicId = info.processorLocalApic.apicId,
                        .processorId = info.processorLocalApic.processorId
                };
                addCpuInfo(cpu);
            }
        }
    }
}
