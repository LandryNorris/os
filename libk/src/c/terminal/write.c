#include "terminal.h"
#include "color.h"

size_t k_strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminalScroll(int numLines) {
    for(size_t i = 0; i < VGA_HEIGHT; i++) {
        for(size_t j = 0; j < VGA_WIDTH; j++) {
            //grab the character <numLines> below.
            size_t indexToRead = (i+numLines)*VGA_WIDTH + j;
            size_t indexToWrite = i*VGA_WIDTH+j;
            uint16_t c;
            if(i+numLines < VGA_HEIGHT) {
                c = terminal_buffer[indexToRead];
            } else {
                c = vga_entry(' ', terminal_color);
            }
            terminal_buffer[indexToWrite] = c;
        }
    }
    terminal_row -= numLines;
}

int terminal_putchar(char c) {
    if(c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }
    if (terminal_row >= VGA_HEIGHT) {
        terminalScroll(1);
    }
    return c;
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

int terminal_puts(const char* data) {
    int len = (int) k_strlen(data);
    terminal_write(data, len);

    return len;
}
