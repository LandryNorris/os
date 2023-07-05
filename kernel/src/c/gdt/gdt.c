#include <gdt.h>

GdtEntry gdt[DESCRIPTOR_COUNT];
GdtPtr gdtPtr;

void initializeGdt() {
    gdtPtr.limit = sizeof(gdt)-1;
    gdtPtr.base = (uint32_t) gdt;

    //kernel code and data
    //NULL entry
    setGdtEntry(0, 0, 0, 0, 0);

    /*
     * Access: (0x9A = 1 00 1 1 0 1 0)
     * 1 present
     * 00 ring 0
     * 1 code/data segment
     * 1 code segment
     * 0 segment can only be executed from ring 0
     * 1 read access allowed
     * 0 access bit, always 0, cpu set this to 1 when accessing this sector
     */
    setGdtEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /*
     * Access: (0x92 = 1 00 1 0 0 1 0)
     * 1 present
     * 00 ring 0
     * 1 code/data segment
     * 0 data segment
     * 0 segment grows up
     * 1 write access allowed
     * 0 access bit, always 0, cpu set this to 1 when accessing this sector
     */
    setGdtEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    //User code and data
    /*
     * Access: (0xFA = 1 11 1 1 0 1 0)
     * 1 present
     * 11 ring 3
     * 1 code/data segment
     * 1 code segment
     * 0 segment can be executed from ring 3 or lower
     * 1 read access allowed
     * 0 access bit, always 0, cpu set this to 1 when accessing this sector
     */
    setGdtEntry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    /*
     * Access: (0x92 = 1 11 1 0 0 1 0)
     * 1 present
     * 11 ring 3
     * 1 code/data segment
     * 0 data segment
     * 0 segment grows up
     * 1 write access allowed
     * 0 access bit, always 0, cpu set this to 1 when accessing this sector
     */
    setGdtEntry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
}

void setGdtEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    GdtEntry * entry = &gdt[index];

    // Low 16 bits, middle 8 bits and high 8 bits of base
    entry->base_low = base & 0xFFFF;
    entry->base_mid = (base >> 16) & 0xFF;
    entry->base_high = (base >> 24 & 0xFF);

    /*
     * The limit is set in a strange way for compatibility with the 286.
     * We use the lower 2 bytes of limit_low and the low 4 bits of flags
     * to store the limit. The low 4 bits of flags stores the high 4 bits
     * of limit.
     */
    entry->limit_low = limit & 0xFFFF;
    entry->flags = (limit >> 16) & 0x0F;

    entry->access = access;

    // Only need the high 4 bits of flags
    entry->flags = entry->flags | (flags & 0xF0);
}
