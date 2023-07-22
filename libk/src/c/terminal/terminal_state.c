#include <stddef.h>
#include <stdint.h>
#include <system.h>

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

size_t terminalRow = 0;
size_t terminalColumn = 0;
uint8_t terminalColor = 0;
__attribute__((unused)) uint16_t* terminalBuffer = (uint16_t*) (VGA_ADDRESS);
