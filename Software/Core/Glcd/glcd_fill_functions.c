#include "glcd.h"

void GlcdFillBox(uint8_t *buf, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t color)
{
    for (uint8_t x = top; x <= bottom; ++x)
        for (uint8_t y = left; y <= right; ++y)
            GlcdDrawPixel(buf, y, x, color);
}

void GlcdDrawNewFilledBlock(uint8_t position, uint8_t width)
{
    // Clamp position to screen boundaries so block doesn't fall off vertically (Y-axis)
    if (position > GLCD_COLS - width)
        position = GLCD_COLS - width;

    // Verical (Y): (bottom-aligned, with a gap)
    uint16_t gapFromBottom = glcdCurrentLevel * BLOCK_HEIGHT;
    uint8_t blockBottomRow = (gapFromBottom >= GLCD_ROWS) ? 0 : (GLCD_ROWS - gapFromBottom);
    uint8_t blockTopRow = (blockBottomRow >= (BLOCK_HEIGHT - 1)) ? (blockBottomRow - (BLOCK_HEIGHT - 1)) : 0;

    // Horizontal (X): based on position and width
    uint8_t blockRightCol = position;
    uint8_t blockLeftCol = blockRightCol + width - 1;
    if (blockLeftCol >= GLCD_COLS) blockLeftCol = GLCD_COLS - 1;

    // Fill block (rectangle)
    GlcdFillBox(glcdImageBuffer, blockTopRow, blockRightCol, blockBottomRow, blockLeftCol, GLCD_EMPTY_BIT);
}

