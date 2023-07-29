
#include "canvas.h"
#include "system.h"

void initializeCanvas(Canvas* canvas, uint32_t width, uint32_t height, void* buffer,
                      uint32_t pitch, uint8_t bpp, ColorSpace colorSpace) {
    canvas->width = width;
    canvas->height = height;
    canvas->buffer = buffer + LOAD_MEMORY_ADDRESS;
    canvas->pitch = pitch;
    canvas->bpp = bpp;
    canvas->colorSpace = colorSpace;
    canvas->fontWidth = (width / 80) - 2;
    canvas->fontHeight = canvas->fontWidth * 2;
    canvas->textColor.r = 255;
    canvas->textColor.g = 255;
    canvas->textColor.b = 255;
    canvas->font = defaultFont;
}

void drawPixel(Canvas* canvas, int x, int y, RgbColor color) {
    //Todo: currently assumes RGB24 with 32 bits per pixel
    uint32_t index = 4*(x + y*canvas->pitch);
    uint32_t value = (color.r << 16) | (color.g << 8) | color.b;
    canvas->buffer[index] = value;
}

void drawCharacter(Canvas* canvas, char c, int x, int y) {
    uint32_t fontWidth = canvas->fontWidth;
    uint32_t fontHeight = canvas->fontHeight;
    PsfFont font = canvas->font;
    PsfCharacter character = font.characterTable[c-32];

    for(uint32_t drawY = 0; drawY < fontHeight; drawY++) {
        for(uint32_t drawX = 0; drawX < fontWidth; drawX++) {
            uint32_t fontX = drawX*8/fontWidth;
            uint32_t fontY = drawY*16/fontHeight;

            uint8_t isColored = character.bitmap[fontY] & (1 << (8-fontX));
            if(isColored) {
                drawPixel(canvas, drawX + x, drawY + y, canvas->textColor);
            }
        }
    }
}

void drawString(Canvas* canvas, const char* s, int x, int y) {
    const char* sPtr = s;
    int characterX = x;
    int fontWidth = canvas->fontWidth;
    int characterSpacing = 2;
    for(char c = *sPtr; c; c = *sPtr++) {
        drawCharacter(canvas, c, characterX, y);
        characterX += fontWidth + characterSpacing;
    }
}
