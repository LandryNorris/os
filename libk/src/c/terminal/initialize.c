#include <terminal.h>
#include "color.h"

void initializeTerminal(Canvas* canvas) {
    terminalCanvas = canvas;
    terminalRow = 0;
    terminalColumn = 0;
    terminalCanvas->fontWidth = terminalCanvas->width/VGA_WIDTH;
    terminalCanvas->fontHeight = terminalCanvas->fontWidth*2;

    terminalColor = VGA_COLOR_LIGHT_GREY;
}
