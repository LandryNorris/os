#include <stddef.h>
#include "canvas.h"

const size_t VGA_WIDTH = 50;
const size_t VGA_HEIGHT = 20;

size_t terminalRow = 0;
size_t terminalColumn = 0;
uint8_t terminalColor = 0;
Canvas* terminalCanvas = NULL;
