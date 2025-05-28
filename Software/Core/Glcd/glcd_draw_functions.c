#include "glcd.h"
#include <stdio.h>
#include "glcd_buffers.h"

void GlcdDrawPixel(uint8_t *buf, uint8_t y, uint8_t x, uint8_t color)
{
    if (y >= GLCD_ROWS || x >= GLCD_COLS)
    	return;

    uint16_t idx = (x / 8) * GLCD_ROWS + y;
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

void GlcdDrawLine(uint8_t *buf, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    if (x0 == x1)
    {
        // Vertical line
        uint8_t y_start = (y0 < y1) ? y0 : y1;
        uint8_t y_end   = (y0 > y1) ? y0 : y1;
        for (uint8_t y = y_start; y <= y_end; y++)
            GlcdDrawPixel(buf, x0, y, color);
    }
    else
    	if (y0 == y1)
    	{
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
    // Clamp position to screen boundaries so block doesn't fall off horizontally (X-axis)
    if (position > GLCD_COLS - width)
        position = GLCD_COLS - width;

    // Calculate block's vertical (Y) position (bottom-aligned, with a gap)
    uint16_t gapFromBottom = glcdCurrentLevel * BLOCK_HEIGHT - 1;
    uint8_t blockBottomRow = (gapFromBottom >= GLCD_ROWS) ? 0 : (GLCD_ROWS - gapFromBottom - 1);
    uint8_t blockTopRow = (blockBottomRow >= (BLOCK_HEIGHT - 1)) ? (blockBottomRow - (BLOCK_HEIGHT - 1)) : 0;

    // Horizontal (X): position from function argument, with variable width
    uint8_t blockRightCol = position;
    uint8_t blockLeftCol = blockRightCol + width - 1;
    if (blockLeftCol >= GLCD_COLS) blockLeftCol = GLCD_COLS - 1;

    // Draw block outline (not filled)
    GlcdDrawBox(glcdImageBuffer, blockTopRow, blockRightCol, blockBottomRow, blockLeftCol, 1);
}

// posX: starting column (0..127), posY: starting row (0..63)
void GlcdDrawDigit(uint8_t *buffer, uint8_t digit, uint8_t posY, uint8_t posX, uint8_t color)
{
    if (digit > GLCD_DIGIT_LENGTH) return; // Only digits 0..9, or 10 for 'x'
    for (uint8_t row = 0; row < GLCD_DIGIT_HEIGHT; ++row) // 10 rows, left to right
    {
        uint8_t rowBits = numFont[digit][row];
        for (uint8_t col = 0; col < GLCD_DIGIT_WIDTH; ++col) // 8 columns, top to bottom
        {
            uint8_t notColor = (color == GLCD_EMPTY_BIT) ? GLCD_FULL_BIT : GLCD_EMPTY_BIT;
            GlcdDrawPixel(buffer, posY + row, posX + col, (rowBits & (1 << col)) ? notColor : color);
        }
    }
}

void GlcdDrawNumber(uint8_t *buffer, uint16_t number, uint8_t posY, uint8_t posX, uint8_t color)
{
    uint8_t digits[3];

    // Out of bounds: show 3 times X
	if (number > 999)
	{
		// 10 means 'X' character
		digits[0] = digits[1] = digits[2] = 10;
	}
	else
	{
		// Split into digits, most significant first (hundreds, tens, ones)
		digits[0] = number / 100;         // Hundreds
		digits[1] = (number / 10) % 10;   // Tens
		digits[2] = number % 10;          // Ones
	}

    for (uint8_t i = 0; i < 3; ++i)
    {
        GlcdDrawDigit(buffer, digits[i], posY, posX + (2-i)*7, color);
    }
}

void GlcdDrawScoreText(uint8_t *buffer, uint8_t posY, uint8_t posX)
{
    uint8_t height = 9;
    uint8_t scoreBlockWidth = GLCD_SCORE_WIDTH / GLCD_DATA_WIDTH;
    uint8_t color;
    uint8_t bits;

    // Goes through the GLCD_SCORE_WIDTH from up to down
    for (uint8_t row = 0; row < GLCD_SCORE_HEIGHT; ++row)
    {
    	// then goes through the scoreBlockWidth times from right to left
        for (uint8_t block = 0; block < scoreBlockWidth; ++block)
        {
            bits = scoreTxt[row * scoreBlockWidth + block];

            // draws the GLCD_DATA_WIDTH bits of the score 1 by 1
            for (uint8_t col = 0; col < GLCD_DATA_WIDTH; ++col)
            {
                color = (bits & (1 << col)) ? GLCD_EMPTY_BIT : GLCD_FULL_BIT; // calculates the color of the specific bit of the bits of the row
                GlcdDrawPixel(buffer, posY - row + height, posX + block * GLCD_DATA_WIDTH + col, color);
            }
        }
    }
}

void GlcdDrawGameOver(uint16_t score)
{
	uint8_t posY = 64, posX = 0;

	glcdImageBuffer = gameOverBG;
	GlcdDrawNumber(glcdImageBuffer, score, posY, posX, GLCD_EMPTY_BIT);
	GlcdPrintFromImageBuffer(GLCD_DEFAULT_REFRESH_SPEED);
}

