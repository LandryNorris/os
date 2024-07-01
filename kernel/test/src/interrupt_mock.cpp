extern "C" {
#include "isr.h"
}

isr interrupts[256];

void registerInterruptHandler(int num, isr handler) {
    if (num < 256)
        interrupts[num] = handler;
}
