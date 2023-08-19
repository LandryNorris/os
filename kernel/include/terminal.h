#ifndef OS_TERMINAL_H
#define OS_TERMINAL_H

#include <stdint.h>
#include <stddef.h>
#include "canvas.h"

extern const size_t VGA_WIDTH;
extern const size_t VGA_HEIGHT;

extern size_t terminalRow;
extern size_t terminalColumn;
extern uint8_t terminalColor;

extern Canvas* terminalCanvas;

void terminalScroll(int numLines);
void initializeTerminal(Canvas* canvas);
int terminalPutChar(char c);
int terminalPutString(const char* data);

#endif //OS_TERMINAL_H
