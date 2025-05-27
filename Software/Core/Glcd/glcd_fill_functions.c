#include "glcd.h"

void GlcdFillBox(uint8_t *buf, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t color)
{
    for (uint8_t y = top; y <= bottom; ++y)
        for (uint8_t x = left; x <= right; ++x)
            GlcdDrawPixel(buf, x, y, color);
}

void GlcdDrawNewFilledBlock(uint8_t position, uint8_t width)
{
    // Clamp position to screen boundaries so block doesn't fall off vertically (Y-axis)
    if (position > GLCD_ROWS - width)
        position = GLCD_ROWS - width;

    // Horizontal (X): same as before (right-aligned, with a gap)
    uint16_t gapFromRight = glcdCurrentLevel * BLOCK_HEIGHT;
    uint8_t blockRightX = (gapFromRight >= GLCD_COLS) ? 0 : (GLCD_COLS - gapFromRight);
    uint8_t blockLeftX = (blockRightX >= (BLOCK_HEIGHT - 1)) ? (blockRightX - (BLOCK_HEIGHT - 1)) : 0;

    // Vertical (Y): based on position and width
    uint8_t blockTopY = position;
    uint8_t blockBottomY = blockTopY + width - 1;
    if (blockBottomY >= GLCD_ROWS) blockBottomY = GLCD_ROWS - 1;

    // Fill block (rectangle)
    GlcdFillBox(glcdImageBuffer, blockLeftX, blockTopY, blockRightX, blockBottomY, 1);
}
