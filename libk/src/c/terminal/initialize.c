#include <terminal.h>
#include "color.h"

void initializeTerminal(Canvas* canvas) {
    terminalCanvas = canvas;
    terminalRow = 0;
    terminalColumn = 0;
    terminalColor = VGA_COLOR_LIGHT_GREY;
}
