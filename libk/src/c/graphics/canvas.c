
#include "canvas.h"

void initializeCanvas(Canvas* canvas, uint32_t w, uint32_t h, uint32_t pitch,
                      uint32_t bpp, uint8_t* buffer, ColorSpace colorSpace) {
    canvas->width = w;
    canvas->height = h;
    canvas->pitch = pitch;
    canvas->bpp = bpp;
    canvas->buffer = buffer;
    canvas->colorSpace = colorSpace;
}

void drawRect(Canvas* canvas, int x, int y, int w, int h, RgbColor color) {
    for(int row = y; row < y+h; row++) {
        for(int col = x; col < x+w; col++) {
            int index = 4*col + row*canvas->pitch;
            canvas->buffer[index+2] = color.r;
            canvas->buffer[index+1] = color.g;
            canvas->buffer[index] = color.b;
        }
    }
}
