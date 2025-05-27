#include "glcd.h"
#include <stdio.h>

void GlcdDrawPixel(uint8_t *buf, uint8_t y, uint8_t x, uint8_t color)
{
    if (y >= GLCD_COLS || x >= GLCD_ROWS) return;
    uint16_t idx = (x / 8) * GLCD_COLS + y;
    uint8_t bit = 1 << (x % 8);
    if (color)
    	buf[idx] &= ~bit;
    else
    	buf[idx] |= bit;
}

void GlcdDrawStartBG(uint8_t pos, uint8_t width)
{
	glcdCurrentLevel = 0;
	glcdImageBuffer = Buffer;

	// clear blocks for after reset
	for(int i=1; i<4; ++i)
	{
		ClearBlocksFromLevel(i);
	}

	GlcdDrawScoreText(glcdImageBuffer, 0, 24);
	GlcdDrawNewFilledBlock(pos,width);
	glcdCurrentLevel++;
}

//TODO: flip every vertical naming to horizontal and vice verse
void GlcdDrawLine(uint8_t *buf, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    if (x0 == x1) {
        // Vertical line
        uint8_t y_start = (y0 < y1) ? y0 : y1;
        uint8_t y_end   = (y0 > y1) ? y0 : y1;
        for (uint8_t y = y_start; y <= y_end; y++)
            GlcdDrawPixel(buf, x0, y, color);
    } else if (y0 == y1) {
        // Horizontal line
        uint8_t x_start = (x0 < x1) ? x0 : x1;
        uint8_t x_end   = (x0 > x1) ? x0 : x1;
        for (uint8_t x = x_start; x <= x_end; x++)
            GlcdDrawPixel(buf, x, y0, color);
    }
}

void GlcdDrawBox(uint8_t *buf, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t color)
{
    GlcdDrawLine(buf, left,  top,    right, top,    color); // Top edge
    GlcdDrawLine(buf, left,  bottom, right, bottom, color); // Bottom edge
    GlcdDrawLine(buf, left,  top,    left,  bottom, color); // Left edge
    GlcdDrawLine(buf, right, top,    right, bottom, color); // Right edge
}

void GlcdDrawNewBlock(uint8_t position, uint8_t width)
{
    // Clamp position to screen boundaries so block doesn't fall off vertically (Y-axis)
    if (position > GLCD_ROWS - width)
        position = GLCD_ROWS - width;

    // Calculate block's horizontal (X) position (right-aligned, with a gap)
    uint16_t gapFromRight = glcdCurrentLevel * BLOCK_HEIGHT - 1;
    uint8_t blockRightX = (gapFromRight >= GLCD_COLS) ? 0 : (GLCD_COLS - gapFromRight - 1);
    uint8_t blockLeftX = (blockRightX >= (BLOCK_HEIGHT - 1)) ? (blockRightX - (BLOCK_HEIGHT - 1)) : 0;

    // Vertical (Y): position from function argument, with variable width
    uint8_t blockTopY = position;
    uint8_t blockBottomY = blockTopY + width - 1;
    if (blockBottomY >= GLCD_ROWS) blockBottomY = GLCD_ROWS - 1;

    // Draw block outline (not filled)
    GlcdDrawBox(glcdImageBuffer, blockLeftX, blockTopY, blockRightX, blockBottomY, 1);
}

// posX: starting column (0..127), posY: starting row (0..63)
void GlcdDrawDigit(uint8_t *buffer, uint8_t digit, uint8_t posX, uint8_t posY, uint8_t color)
{
	if (digit > 10) return; // Only digits 0..9, or 10 for 'x'
	for (uint8_t col = 0; col < 10; ++col) // 10 columns, left to right
	{
		uint8_t colBits = numFont[digit][col];
		for (uint8_t row = 0; row < 8; ++row) // 8 rows, top to bottom
		{
			uint8_t notColor = (color==1) ? 0 : 1;
			GlcdDrawPixel(buffer, posX + col, posY + row, (colBits & (1 << row)) ? notColor : color);
		}
	}
}

void GlcdDrawNumber(uint8_t *buffer, uint16_t number, uint8_t posX, uint8_t posY, uint8_t color)
{
    // Out of bounds: show 'XXX'
    if (number > 999)
    {
        GlcdDrawDigit(buffer, 10, posX, posY, color);        // X (left)
        GlcdDrawDigit(buffer, 10, posX, posY+7, color);   // X (center)
        GlcdDrawDigit(buffer, 10, posX, posY+2*7, color);   // X (right)
        return;
    }

    // Split into digits, most significant first (hundreds, tens, ones)
    uint8_t digits[3];
    digits[0] = number / 100;         // Hundreds
    digits[1] = (number / 10) % 10;   // Tens
    digits[2] = number % 10;          // Ones

    for (uint8_t i = 0; i < 3; ++i)
    {
        GlcdDrawDigit(buffer, digits[i], posX, posY + (2-i)*7, color);
    }
}

void GlcdDrawScoreText(uint8_t *buffer, uint8_t posX, uint8_t posY)
{
	uint8_t height = 9;
    for (uint8_t col = 0; col < 10; ++col) // 10 columns, right to left
    {
        for (uint8_t block = 0; block < 5; ++block) // 5 blocks of 8 pixels (40 total)
        {
            uint8_t bits = scoreTxt[col * 5 + block];
            for (uint8_t row = 0; row < 8; ++row)
            {
                uint8_t color = (bits & (1 << row)) ? 1 : 0;
                // X: horizontal, Y: vertical. Highest X = posX, then decrease left.
                GlcdDrawPixel(buffer, posX - col + height, posY + block * 8 + row, color);
            }
        }
    }

}

void GlcdDrawGameOver(uint16_t score)
{
	glcdImageBuffer = gameOverBG;
	GlcdDrawNumber(glcdImageBuffer, score, 64, 0, 1);
	GlcdPrintFromImageBuffer(GLCD_DEFAULT_REFRESH_SPEED);
}

