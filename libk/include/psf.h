#ifndef OS_PSF_H
#define OS_PSF_H

#include <stdint.h>

#define PCF_CHAR(...)   \
    {                   \
        .bitmap = {  \
             __VA_ARGS__\
        }               \
    }

typedef struct {
    uint8_t bitmap[16];
} PsfCharacter;

typedef struct {
    PsfCharacter characterTable[127 - 32];
} PsfFont;

extern PsfFont defaultFont;

#endif //OS_PSF_H
