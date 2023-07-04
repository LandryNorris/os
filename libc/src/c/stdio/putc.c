
#include <terminal.h>

__attribute__((unused)) int putchar(char c) {
    return terminal_putchar(c);
}
