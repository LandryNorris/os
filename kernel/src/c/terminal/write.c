#include "../../../include/terminal.h"
#include "../../../include/color.h"

RgbColor terminalColors[] = {
        { //black
            .r = 0,
            .g = 0,
            .b = 0
        },
        { //blue
            .r = 0,
            .g = 0,
            .b = 255
        },
        { //green
                .r = 0,
                .g = 255,
                .b = 0
        },
        { //cyan
                .r = 0,
                .g = 255,
                .b = 255
        },
        { //red
                .r = 255,
                .g = 0,
                .b = 0
        },
        { //magenta
                .r = 255,
                .g = 0,
                .b = 255
        },
        { //brown
                .r = 0x96,
                .g = 0x4b,
                .b = 0
        },
        { //light gray
                .r = 200,
                .g = 200,
                .b = 200
        }
};

size_t kernel_strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void terminalSetColor(uint8_t color) {
    terminalColor = color;
}

void terminalPutEntryAt(char c, uint8_t color, size_t x, size_t y) {
    terminalCanvas->textColor = terminalColors[color];
    int drawX = (int)(x * terminalCanvas->fontWidth);
    int drawY = (int)(y * terminalCanvas->fontHeight);
    drawCharacter(terminalCanvas, c, drawX, drawY);
}

void terminalScroll(int numLines) {
    //Todo: scroll
    terminalRow -= numLines;
}

int terminalPutChar(char c) {
    if (c == '\n') {
        terminalColumn = 0;
        terminalRow++;
    } else {
        terminalPutEntryAt(c, terminalColor, terminalColumn, terminalRow);
        if (++terminalColumn == VGA_WIDTH) {
            terminalColumn = 0;
            terminalRow++;
        }
    }
    if (terminalRow >= VGA_HEIGHT) {
        //terminalScroll(1);
    }
    return c;
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminalPutChar(data[i]);
}

int terminalPutString(const char* data) {
    int len = (int) kernel_strlen(data);
    terminal_write(data, len);

    return len;
}
