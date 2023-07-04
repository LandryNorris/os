#include <stddef.h>
#include <stdint.h>

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color = 0;
__attribute__((unused)) uint16_t* terminal_buffer = (uint16_t*) 0xB8000;
