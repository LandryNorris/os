#ifndef OS_CANVAS_H
#define OS_CANVAS_H

#include <stdint.h>
#include "font.h"

typedef enum {
    RGB24
} ColorSpace;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RgbColor;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t bpp; //bits per pixel
    uint32_t fontWidth; //in pixels
    uint32_t fontHeight; //in pixels
    uint8_t* buffer;
    PsfFont font;
    ColorSpace colorSpace;
    RgbColor textColor;
} Canvas;

void initializeCanvas(Canvas* canvas, uint32_t width, uint32_t height, void* buffer,
                      uint32_t pitch, uint8_t bpp, ColorSpace colorSpace);
void drawPixel(Canvas* canvas, int x, int y, RgbColor color);
void drawCharacter(Canvas* canvas, char c, int x, int y);
void drawString(Canvas* canvas, const char* s, int x, int y);

#endif //OS_CANVAS_H
