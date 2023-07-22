
#include <terminal.h>

int puts(const char* s) {
    return terminalPutString(s);
}
