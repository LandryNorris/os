#ifndef OS_FONT_H
#define OS_FONT_H

#include <stdint.h>

#define PCF_CHAR(...)   \
    {                   \
        .bitmap = {  \
             __VA_ARGS__\
        }               \
    }

typedef uint64_t Font[];

typedef struct {
    uint8_t bitmap[16];
} PsfCharacter;

typedef struct {
    PsfCharacter characterTable[127 - 32];
} PsfFont;

extern PsfFont defaultFont;

#endif //OS_FONT_H
