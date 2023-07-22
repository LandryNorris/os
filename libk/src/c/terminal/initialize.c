#include <terminal.h>
#include <color.h>
#include "system.h"

void initializeTerminal() {
    terminalRow = 0;
    terminalColumn = 0;
    terminalColor = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminalBuffer = (uint16_t*) (VGA_ADDRESS);
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminalBuffer[index] = vga_entry(' ', terminalColor);
        }
    }
}
