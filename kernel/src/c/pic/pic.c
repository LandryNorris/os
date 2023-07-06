
#include <system.h>
#include <pic.h>
#include <stdint.h>

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */


void initPic(int offset1, int offset2) {
    //save current state
    uint8_t a1 = inportByte(PIC1_DATA);
    uint8_t a2 = inportByte(PIC2_DATA);

    // ICW1
    outportByte(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outportByte(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // ICW2, irq 0 to 7 is mapped to offset1 to offset1+7, irq 8 to F is mapped to offset2 to offset2 + (F-8)
    outportByte(PIC1_DATA, offset1);
    io_wait();
    outportByte(PIC2_DATA, offset2);
    io_wait();

    // ICW3, connect master pic with slave pic
    outportByte(PIC1_DATA, 0x4);
    io_wait();
    outportByte(PIC2_DATA, 0x2);
    io_wait();

    // ICW4, set x86 mode
    outportByte(PIC1_DATA, ICW4_8086);
    outportByte(PIC2_DATA, ICW4_8086);

    // restore masks
    outportByte(PIC1_DATA, a1);
    outportByte(PIC2_DATA, a2);
}

void sendEoi(uint8_t irq) {
    if(irq >= 8) {
        outportByte(PIC2_COMMAND, PIC_EOI);
    }
    outportByte(PIC1_COMMAND, PIC_EOI);
}
