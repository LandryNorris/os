//
// Created by landry on 7/5/23.
//

#ifndef OS_PIC_H
#define OS_PIC_H

#include <system.h>
#include <stdint.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */

#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		0x20		/* End-of-interrupt command code */

void initPic(int offset1, int offset2);
void sendEoi(uint8_t irq);

static inline void io_wait(void) {
    outportByte(0x80, 0);
}

#endif //OS_PIC_H
