
#include "canvas.h"
#include "psf.h"

void initializeCanvas(Canvas* canvas, uint32_t w, uint32_t h, uint32_t pitch,
                      uint32_t bpp, uint8_t* buffer, ColorSpace colorSpace) {
    canvas->width = w;
    canvas->height = h;
    canvas->pitch = pitch;
    canvas->bpp = bpp;
    canvas->buffer = buffer;
    canvas->colorSpace = colorSpace;
    canvas->textColor.r = 255;
    canvas->textColor.g = 255;
    canvas->textColor.b = 255;
}

void drawRect(Canvas* canvas, int x, int y, int w, int h, RgbColor color) {
    for(int row = y; row < y+h; row++) {
        for(int col = x; col < x+w; col++) {
            uint32_t index = canvas->bpp/8*col + row*canvas->pitch;
            canvas->buffer[index+2] = color.r;
            canvas->buffer[index+1] = color.g;
            canvas->buffer[index] = color.b;
        }
    }
}

void drawCharacter(Canvas* canvas, char c, int x, int y) {
    if(c < 32 || c > 127) return; //Todo: render non-printable character symbol

    uint32_t fontWidth = canvas->fontWidth;
    uint32_t fontHeight = canvas->fontHeight;
    PsfFont font = defaultFont; //Todo: store font in canvas
    PsfCharacter character = font.characterTable[c-32];
    int bytesPerPixel = canvas->bpp/8;

    for(uint32_t drawY = 0; drawY < fontHeight; drawY++) {
        for(uint32_t drawX = 0; drawX < fontWidth; drawX++) {
            uint32_t fontX = drawX*8/fontWidth;
            uint32_t fontY = drawY*16/fontHeight;

            if((drawX + x) >= canvas->width || (drawY + y) >= canvas->height) {
                //don't draw out of bounds
                continue;
            }

            uint8_t isColored = character.bitmap[fontY] & (1 << (7-fontX));
            if(isColored) {
                int index = bytesPerPixel*(drawX + x) + (drawY + y)*canvas->pitch;
                RgbColor color = canvas->textColor;
                canvas->buffer[index+2] = color.r;
                canvas->buffer[index+1] = color.g;
                canvas->buffer[index] = color.b;
            }
        }
    }
}

void drawString(Canvas* canvas, const char* s, int x, int y) {
    const char* sPtr = s;
    int characterX = x;
    int fontWidth = canvas->fontWidth;
    int characterSpacing = 2;
    for(char c = *sPtr; c; c = *++sPtr) {
        drawCharacter(canvas, c, characterX, y);
        characterX += fontWidth + characterSpacing;
    }
}
