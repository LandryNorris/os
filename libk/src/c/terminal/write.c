#include "terminal.h"
#include "color.h"

size_t k_strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void terminal_setcolor(uint8_t color) {
    terminalColor = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminalBuffer[index] = vga_entry(c, color);
}

void terminalScroll(int numLines) {
    for (size_t i = 0; i < VGA_HEIGHT; i++) {
        for (size_t j = 0; j < VGA_WIDTH; j++) {
            //grab the character <numLines> below.
            size_t indexToRead = (i + numLines) * VGA_WIDTH + j;
            size_t indexToWrite = i * VGA_WIDTH + j;
            uint16_t c;
            if (i + numLines < VGA_HEIGHT) {
                c = terminalBuffer[indexToRead];
            } else {
                c = vga_entry(' ', terminalColor);
            }
            terminalBuffer[indexToWrite] = c;
        }
    }
    terminalRow -= numLines;
}

int terminalPutChar(char c) {
    if (c == '\n') {
        terminalColumn = 0;
        terminalRow++;
    } else {
        terminal_putentryat(c, terminalColor, terminalColumn, terminalRow);
        if (++terminalColumn == VGA_WIDTH) {
            terminalColumn = 0;
            terminalRow++;
        }
    }
    if (terminalRow >= VGA_HEIGHT) {
        terminalScroll(1);
    }
    return c;
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminalPutChar(data[i]);
}

int terminalPutString(const char* data) {
    int len = (int) k_strlen(data);
    terminal_write(data, len);

    return len;
}
