#include "gtest/gtest.h"
extern "C" {
#include "idt.h"

extern IdtEntry idt[NUM_IDT_ENTRIES];
extern IdtPtr idtPtr;

uint32_t idtPointer = 0;

// mock setIdt
void setIdt(uint32_t ptr) {
    idtPointer = ptr;
}

// mock initPic
void initPic(int offset1, int offset2) {

}

int32_t exception0;
int32_t exception1;
int32_t exception2;
int32_t exception3;
int32_t exception4;
int32_t exception5;
int32_t exception6;
int32_t exception7;
int32_t exception8;
int32_t exception9;
int32_t exception10;
int32_t exception11;
int32_t exception12;
int32_t exception13;
int32_t exception14;
int32_t exception15;
int32_t exception16;
int32_t exception17;
int32_t exception18;
int32_t exception19;
int32_t exception20;
int32_t exception21;
int32_t exception22;
int32_t exception23;
int32_t exception24;
int32_t exception25;
int32_t exception26;
int32_t exception27;
int32_t exception28;
int32_t exception29;
int32_t exception30;
int32_t exception31;
int32_t exception128;

int32_t irq0;
int32_t irq1;
int32_t irq2;
int32_t irq3;
int32_t irq4;
int32_t irq5;
int32_t irq6;
int32_t irq7;
int32_t irq8;
int32_t irq9;
int32_t irq10;
int32_t irq11;
int32_t irq12;
int32_t irq13;
int32_t irq14;
int32_t irq15;
}

void assertEntry(int index, void* rawAddress) {
    uint32_t address = (uint64_t) rawAddress;
    ASSERT_EQ((uint16_t)(address & 0xFFFF), idt[index].baseLow);
    ASSERT_EQ((uint16_t)((address >> 16) & 0xFFFF), idt[index].baseHigh);
}

TEST(IDTTest, initializeIdt) {
    initializeIdt();

    ASSERT_EQ(idtPtr.base, (uint32_t) (uint64_t) &idt);
    ASSERT_EQ(idtPointer, (uint32_t) (uint64_t) &idtPtr);

    assertEntry(0, &exception0);
    assertEntry(1, &exception1);
    assertEntry(31, &exception31);

    assertEntry(32, &irq0);
    assertEntry(47, &irq15);
    assertEntry(128, &exception128);
}
