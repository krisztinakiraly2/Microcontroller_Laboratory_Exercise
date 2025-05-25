#include "lcd.h"

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

void GlcdDrawStartBG()
{
	glcdImageBuffer = startBG;
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


void GlcdDrawWindow(
    uint8_t *buf,
    uint8_t left, uint8_t top, uint8_t right, uint8_t bottom,
    uint8_t win_width, uint8_t win_height,
    uint8_t vert_line_height, uint8_t horz_line_width,
    uint8_t color)
{
    // Draw window outline
    GlcdDrawBox(buf, left, top, right, bottom, color);

    // Vertical line (centered)
    uint8_t vert_x = left + win_height / 2 - 1;
    uint8_t vert_ytop = top + (win_width - vert_line_height) / 2;
    uint8_t vert_ybot = vert_ytop + vert_line_height - 1;
    if (vert_ytop < top) vert_ytop = top;
    if (vert_ybot > bottom) vert_ybot = bottom;
    GlcdDrawLine(buf, vert_x, vert_ytop, vert_x, vert_ybot, color);

    // Horizontal line (centered)
    uint8_t horz_y = top + win_width / 2;
    uint8_t horz_xleft = left + (win_height - horz_line_width) / 2;
    uint8_t horz_xright = horz_xleft + horz_line_width - 1;
    if (horz_xleft < left) horz_xleft = left;
    if (horz_xright > right) horz_xright = right;
    GlcdDrawLine(buf, horz_xleft, horz_y, horz_xright, horz_y, color);
}

void GlcdDrawTwoWindows(
    uint8_t *buf,
    uint8_t block_leftX, uint8_t block_topY, uint8_t block_rightX, uint8_t block_bottomY,
    uint8_t win_margin, uint8_t win_spacing,
    uint8_t win_height, uint8_t win_width,
    uint8_t vert_line_height, uint8_t horz_line_width,
    uint8_t color)
{
    // Window X (horizontal): centered in block
    uint8_t win_leftX  = block_leftX + (GLCD_BLOCK_HEIGHT - win_height) / 2;
    uint8_t win_rightX = win_leftX + win_height - 1;
    if (win_rightX > block_rightX) win_rightX = block_rightX;

    // First window
    uint8_t win1_topY    = block_topY + win_margin;
    uint8_t win1_bottomY = win1_topY + win_width - 1;
    if (win1_bottomY > block_bottomY) win1_bottomY = block_bottomY;

    GlcdDrawWindow(buf, win_leftX, win1_topY, win_rightX, win1_bottomY, win_width, win_height, vert_line_height, horz_line_width, color);

    // Second window
    uint8_t win2_topY    = win1_bottomY + 1 + win_spacing;
    uint8_t win2_bottomY = win2_topY + win_width - 1;
    if (win2_bottomY > block_bottomY) win2_bottomY = block_bottomY;

    GlcdDrawWindow(buf, win_leftX, win2_topY, win_rightX, win2_bottomY, win_width, win_height, vert_line_height, horz_line_width, color);
}

void GlcdDrawNewBlock(uint8_t position)
{
    // Clamp position to screen boundaries so block doesn't fall off
    if (position > GLCD_ROWS - GLCD_BLOCK_WIDTH)
        position = GLCD_ROWS - GLCD_BLOCK_WIDTH;

    // Calculate block's horizontal (X) position (right-aligned, with a gap)
    uint16_t gapFromRight = glcdCurrentLevel * GLCD_BLOCK_HEIGHT - 1;
    uint8_t blockRightX = (gapFromRight >= GLCD_COLS) ? 0 : (GLCD_COLS - gapFromRight - 1);
    uint8_t blockLeftX = (blockRightX >= (GLCD_BLOCK_HEIGHT - 1)) ? (blockRightX - (GLCD_BLOCK_HEIGHT - 1)) : 0;

    // Vertical (Y): position from function argument
    uint8_t blockTopY = position;
    uint8_t blockBottomY = blockTopY + GLCD_BLOCK_WIDTH - 1;
    if (blockBottomY >= GLCD_ROWS) blockBottomY = GLCD_ROWS - 1;

    // Clear buffer if needed here (not shown in your original code)

    // Draw block outline (not filled)
    GlcdDrawBox(glcdImageBuffer, blockLeftX, blockTopY, blockRightX, blockBottomY, 1);

    // Draw two windows inside the block
    GlcdDrawTwoWindows(
        glcdImageBuffer,
        blockLeftX, blockTopY, blockRightX, blockBottomY,
        GLCD_LEFT_WINDOW_MARGIN_FROM_BORDER, GLCD_SPACING_BETWEEN_WINDOWS,
        GLCD_WINDOW_HEIGHT, GLCD_WINDOW_WIDTH,
        GLCD_WINDOW_VERTICAL_LINE_HEIGHT, GLCD_WINDOW_HORIZONTAL_LINE_WIDTH,
        1
    );
}
