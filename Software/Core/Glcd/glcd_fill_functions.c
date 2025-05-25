#include "lcd.h"

void GlcdFillBox(uint8_t *buf, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t color)
{
    for (uint8_t y = top; y <= bottom; ++y)
        for (uint8_t x = left; x <= right; ++x)
            GlcdDrawPixel(buf, x, y, color);
}

void GlcdFillWindow(
    uint8_t *buf,
    uint8_t left, uint8_t top, uint8_t right, uint8_t bottom,
    uint8_t win_width, uint8_t win_height,
    uint8_t vert_line_height, uint8_t horz_line_width,
    uint8_t color)
{
    // Fill window background
    GlcdFillBox(buf, left, top, right, bottom, (color==0)?1:0);

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

void GlcdFillTwoWindows(
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

    GlcdFillWindow(buf, win_leftX, win1_topY, win_rightX, win1_bottomY, win_width, win_height, vert_line_height, horz_line_width, color);

    // Second window
    uint8_t win2_topY    = win1_bottomY + 1 + win_spacing;
    uint8_t win2_bottomY = win2_topY + win_width - 1;
    if (win2_bottomY > block_bottomY) win2_bottomY = block_bottomY;

    GlcdFillWindow(buf, win_leftX, win2_topY, win_rightX, win2_bottomY, win_width, win_height, vert_line_height, horz_line_width, color);
}

void GlcdDrawNewFilledBlock(uint8_t position)
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

    // Fill block (rectangle)
    GlcdFillBox(glcdImageBuffer, blockLeftX, blockTopY, blockRightX, blockBottomY, 1);

    // Draw two filled windows inside the block
    GlcdFillTwoWindows(
        glcdImageBuffer,
        blockLeftX, blockTopY, blockRightX, blockBottomY,
        GLCD_LEFT_WINDOW_MARGIN_FROM_BORDER, GLCD_SPACING_BETWEEN_WINDOWS,
        GLCD_WINDOW_HEIGHT, GLCD_WINDOW_WIDTH,
        GLCD_WINDOW_VERTICAL_LINE_HEIGHT, GLCD_WINDOW_HORIZONTAL_LINE_WIDTH,
        0
    );
}
