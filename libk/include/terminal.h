
#ifndef OS_TERMINAL_H
#define OS_TERMINAL_H

#include <stdint.h>
#include <stddef.h>

extern const size_t VGA_WIDTH;
extern const size_t VGA_HEIGHT;

extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;

void terminal_initialize(void);
int terminal_putchar(char c);
int terminal_puts(const char* data);

#endif //OS_TERMINAL_H
