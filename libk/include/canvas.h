#ifndef OS_CANVAS_H
#define OS_CANVAS_H

#include <stdint.h>

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
    uint8_t* buffer;
    ColorSpace colorSpace;
} Canvas;

void initializeCanvas(Canvas* canvas, uint32_t w, uint32_t h, uint32_t pitch,
                      uint32_t bpp, uint8_t* buffer, ColorSpace colorSpace);

void drawRect(Canvas* canvas, int x, int y, int w, int h, RgbColor color);

#endif //OS_CANVAS_H
