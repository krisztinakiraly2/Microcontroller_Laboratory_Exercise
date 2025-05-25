#include "lcd.h"

//uint8_t glcdImageBuffer[(128 * 64)/8];
uint8_t* glcdImageBuffer;
uint16_t glcdCurrentLevel = 1;

// os_delay is too slow for refreshing the display
void Delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000);
    while ((DWT->CYCCNT - start) < ticks);
}

// The enable's falling edge triggers the reading of the command or data in the lcd
void PulseEnable()
{
	HAL_GPIO_WritePin(GLCD_E_GPIO_Port, GLCD_E_Pin, GPIO_PIN_SET); // 1 active
	Delay_us(1);
	HAL_GPIO_WritePin(GLCD_E_GPIO_Port, GLCD_E_Pin, GPIO_PIN_RESET);
	Delay_us(1);
}

// write the data to the pins based on the bits in data and triggers write to lcd
void GlcdWriteData(uint8_t data, GPIO_PinState mode)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, mode); // Set DI based on mode var

	HAL_GPIO_WritePin(GLCD_DB0_GPIO_Port, GLCD_DB0_Pin, (data & (1 << 0)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB1_GPIO_Port, GLCD_DB1_Pin, (data & (1 << 1)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB2_GPIO_Port, GLCD_DB2_Pin, (data & (1 << 2)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB3_GPIO_Port, GLCD_DB3_Pin, (data & (1 << 3)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB4_GPIO_Port, GLCD_DB4_Pin, (data & (1 << 4)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB5_GPIO_Port, GLCD_DB5_Pin, (data & (1 << 5)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB6_GPIO_Port, GLCD_DB6_Pin, (data & (1 << 6)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB7_GPIO_Port, GLCD_DB7_Pin, (data & (1 << 7)) ? GPIO_PIN_SET : GPIO_PIN_RESET);

	PulseEnable(); // trigger write
}

// Does the required resets based on the datasheet
void GlcdInit()
{
	HAL_GPIO_WritePin(GLCDEN_GPIO_Port, GLCDEN_Pin, GPIO_PIN_RESET); // enable buffers (0 active)
	HAL_GPIO_WritePin(GLCD_RW_GPIO_Port, GLCD_RW_Pin, GPIO_PIN_RESET); // Set the direction to writing (0: write, 1: read)
	HAL_GPIO_WritePin(GLCD_RESET_GPIO_Port, GLCD_RESET_Pin, GPIO_PIN_RESET); // reset the lcd panel (0 active)
	osDelay(1);
	HAL_GPIO_WritePin(GLCD_RESET_GPIO_Port, GLCD_RESET_Pin, GPIO_PIN_SET); // ends the reset

	HAL_GPIO_WritePin(GLCD_CS1_GPIO_Port, GLCD_CS1_Pin, GPIO_PIN_RESET);  // enable CS1 (0 active)
	HAL_GPIO_WritePin(GLCD_CS2_GPIO_Port, GLCD_CS2_Pin, GPIO_PIN_RESET);  // enable CS2 (0 active)

	GlcdWriteData(GLCD_TURN_ON, GLCD_DI_COMMAND_MODE); // turn on the lcd
	osDelay(1);
    GlcdWriteData(GLCD_Y_ADDR_ZERO, GLCD_DI_COMMAND_MODE); // set y adrr to 0
	GlcdWriteData(GLCD_X_ADDR_ZERO, GLCD_DI_COMMAND_MODE); // set x addr to 0
	GlcdWriteData(GLCD_Y_ADDR_ZERO, GLCD_DI_COMMAND_MODE); // set z adrr to 0 (for ram memory)
	osDelay(1);

	GlcdClearMemory();
}

void GlcdClearMemory()
{
	HAL_GPIO_WritePin(GLCD_CS1_GPIO_Port, GLCD_CS1_Pin, GPIO_PIN_RESET);  // enable CS1 (0 active)
	HAL_GPIO_WritePin(GLCD_CS2_GPIO_Port, GLCD_CS2_Pin, GPIO_PIN_RESET);  // enable CS2 (0 active)

	for (int i=0; i<= GLCD_ROWS/8; ++i)
	{
		for (uint8_t j = 0; j < GLCD_COLS/2; ++j)
		{
			GlcdWriteData(GLCD_EMPTY_BYTE, GLCD_DI_DATA_MODE);
		}

		GlcdWriteData(GLCD_X_ADDR_ZERO+i, GLCD_DI_COMMAND_MODE);
	}
}

#define DELAY_US 25

void GlcdPrintFromImageBuffer(void)
{
    for (uint8_t half = 0; half < 2; ++half)
    {
        // Set the correct chip select lines
        if (half == 0)
        {
            // Left half (CS1 ON, CS2 OFF)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // CS1 ON
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   // CS2 OFF
        }
        else
        {
            // Right half (CS1 OFF, CS2 ON)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);   // CS1 OFF
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // CS2 ON
        }

        uint8_t col_start = half * (GLCD_COLS / 2);
        uint8_t col_end = col_start + (GLCD_COLS / 2);

        for (uint8_t row = 0; row < GLCD_ROWS / 8; ++row) // Each page is 8 rows
        {
            // Set row (page) address
            GlcdWriteData(GLCD_X_ADDR_ZERO + row, GLCD_DI_COMMAND_MODE);

            for (uint8_t col = col_start; col < col_end; ++col)
            {
                uint16_t idx = row * GLCD_COLS + col;
                GlcdWriteData(glcdImageBuffer[idx], GLCD_DI_DATA_MODE);
                Delay_us(DELAY_US); // Remove or move as needed!
            }
        }
    }
}


void GlcdDrawStartBG()
{
	glcdImageBuffer = startBG;
}

// Set pixel in GLCD buffer
static inline void GlcdSetPixel(uint8_t *buf, uint8_t x, uint8_t y, uint8_t color)
{
    if (x >= GLCD_COLS || y >= GLCD_ROWS) return;
    uint16_t idx = (y / 8) * GLCD_COLS + x;
    uint8_t bit = 1 << (y % 8);
    if (color)
    	buf[idx] &= ~bit;
    else
    	buf[idx] |= bit;
}

// Clear buffer and draw one horizontal block centered from the top margin
void GlcdDrawNewBlock(uint8_t position)
{
    // Clamp position to screen boundaries so block doesn't fall off
    if (position > GLCD_ROWS - GLCD_BLOCK_WIDTH)
        position = GLCD_ROWS - GLCD_BLOCK_WIDTH;

    // Calculate block's horizontal (X) position (right-aligned, with a gap)
    uint16_t gapFromRight = glcdCurrentLevel * GLCD_BLOCK_HEIGHT-1;
    uint8_t blockRightX = (gapFromRight >= GLCD_COLS) ? 0 : (GLCD_COLS - gapFromRight - 1);
    uint8_t blockLeftX = (blockRightX >= (GLCD_BLOCK_HEIGHT - 1)) ? (blockRightX - (GLCD_BLOCK_HEIGHT - 1)) : 0;

    // Vertical (Y): position from function argument
    uint8_t blockTopY = position;
    uint8_t blockBottomY = blockTopY + GLCD_BLOCK_WIDTH - 1;
    if (blockBottomY >= GLCD_ROWS) blockBottomY = GLCD_ROWS - 1;

    // --- Draw block outline (not filled) ---
    for (uint8_t x = blockLeftX; x <= blockRightX; x++) {
        GlcdSetPixel(glcdImageBuffer, x, blockTopY, 1);        // Top edge
        GlcdSetPixel(glcdImageBuffer, x, blockBottomY, 1);     // Bottom edge
    }
    for (uint8_t y = blockTopY; y <= blockBottomY; y++) {
        GlcdSetPixel(glcdImageBuffer, blockLeftX, y, 1);       // Left edge
        GlcdSetPixel(glcdImageBuffer, blockRightX, y, 1);      // Right edge
    }

    // First window: top margin from block top
    uint8_t win1_topY    = blockTopY + GLCD_LEFT_WINDOW_MARGIN_FROM_BORDER;
    uint8_t win1_bottomY = win1_topY + GLCD_WINDOW_WIDTH - 1;

    // Second window: below first, with spacing
    uint8_t win2_topY    = win1_bottomY + 1 + GLCD_SPACING_BETWEEN_WINDOWS;
    uint8_t win2_bottomY = win2_topY + GLCD_WINDOW_WIDTH - 1;

    // Windows always drawn, but clipped if outside block
    if (win1_bottomY > blockBottomY) win1_bottomY = blockBottomY;
    if (win2_bottomY > blockBottomY) win2_bottomY = blockBottomY;

    // Window X (horizontal): centered in block
    uint8_t win_leftX  = blockLeftX + (GLCD_BLOCK_HEIGHT - GLCD_WINDOW_HEIGHT) / 2;
    uint8_t win_rightX = win_leftX + GLCD_WINDOW_HEIGHT - 1;
    if (win_rightX > blockRightX) win_rightX = blockRightX;

    // Draw Window 1 Outline
    for (uint8_t x = win_leftX; x <= win_rightX; x++) {
        GlcdSetPixel(glcdImageBuffer, x, win1_topY, 1);        // Top edge
        GlcdSetPixel(glcdImageBuffer, x, win1_bottomY, 1);     // Bottom edge
    }
    for (uint8_t y = win1_topY; y <= win1_bottomY; y++) {
        GlcdSetPixel(glcdImageBuffer, win_leftX, y, 1);        // Left edge
        GlcdSetPixel(glcdImageBuffer, win_rightX, y, 1);       // Right edge
    }

    // Draw Window 2 Outline
    for (uint8_t x = win_leftX; x <= win_rightX; x++) {
        GlcdSetPixel(glcdImageBuffer, x, win2_topY, 1);        // Top edge
        GlcdSetPixel(glcdImageBuffer, x, win2_bottomY, 1);     // Bottom edge
    }
    for (uint8_t y = win2_topY; y <= win2_bottomY; y++) {
        GlcdSetPixel(glcdImageBuffer, win_leftX, y, 1);        // Left edge
        GlcdSetPixel(glcdImageBuffer, win_rightX, y, 1);       // Right edge
    }

    // --- Draw lines inside each window (vertical and horizontal) ---

    // Window 1 vertical line (centered)
    uint8_t win1_vert_x = win_leftX + GLCD_WINDOW_HEIGHT / 2 - 1;
    uint8_t win1_vert_ytop = win1_topY + (GLCD_WINDOW_WIDTH - GLCD_WINDOW_VERTICAL_LINE_HEIGHT) / 2;
    uint8_t win1_vert_ybot = win1_vert_ytop + GLCD_WINDOW_VERTICAL_LINE_HEIGHT - 1;
    if (win1_vert_ytop < win1_topY) win1_vert_ytop = win1_topY;
    if (win1_vert_ybot > win1_bottomY) win1_vert_ybot = win1_bottomY;
    for (uint8_t y = win1_vert_ytop; y <= win1_vert_ybot; y++) {
        GlcdSetPixel(glcdImageBuffer, win1_vert_x, y, 1);
    }
    // Window 1 horizontal line (centered)
    uint8_t win1_horz_y = win1_topY + GLCD_WINDOW_WIDTH / 2;
    uint8_t win1_horz_xleft = win_leftX + (GLCD_WINDOW_HEIGHT - GLCD_WINDOW_HORIZONTAL_LINE_WIDTH) / 2;
    uint8_t win1_horz_xright = win1_horz_xleft + GLCD_WINDOW_HORIZONTAL_LINE_WIDTH - 1;
    if (win1_horz_xleft < win_leftX) win1_horz_xleft = win_leftX;
    if (win1_horz_xright > win_rightX) win1_horz_xright = win_rightX;
    for (uint8_t x = win1_horz_xleft; x <= win1_horz_xright; x++) {
        GlcdSetPixel(glcdImageBuffer, x, win1_horz_y, 1);
    }

    // Window 2 vertical line (centered)
    uint8_t win2_vert_x = win_leftX + GLCD_WINDOW_HEIGHT / 2 - 1;
    uint8_t win2_vert_ytop = win2_topY + (GLCD_WINDOW_WIDTH - GLCD_WINDOW_VERTICAL_LINE_HEIGHT) / 2;
    uint8_t win2_vert_ybot = win2_vert_ytop + GLCD_WINDOW_VERTICAL_LINE_HEIGHT - 1;
    if (win2_vert_ytop < win2_topY) win2_vert_ytop = win2_topY;
    if (win2_vert_ybot > win2_bottomY) win2_vert_ybot = win2_bottomY;
    for (uint8_t y = win2_vert_ytop; y <= win2_vert_ybot; y++) {
        GlcdSetPixel(glcdImageBuffer, win2_vert_x, y, 1);
    }
    // Window 2 horizontal line (centered)
    uint8_t win2_horz_y = win2_topY + GLCD_WINDOW_WIDTH / 2;
    uint8_t win2_horz_xleft = win_leftX + (GLCD_WINDOW_HEIGHT - GLCD_WINDOW_HORIZONTAL_LINE_WIDTH) / 2;
    uint8_t win2_horz_xright = win2_horz_xleft + GLCD_WINDOW_HORIZONTAL_LINE_WIDTH - 1;
    if (win2_horz_xleft < win_leftX) win2_horz_xleft = win_leftX;
    if (win2_horz_xright > win_rightX) win2_horz_xright = win_rightX;
    for (uint8_t x = win2_horz_xleft; x <= win2_horz_xright; x++) {
        GlcdSetPixel(glcdImageBuffer, x, win2_horz_y, 1);
    }
}

void GlcdClearBlockColumns()
{
    uint16_t gapFromRight = glcdCurrentLevel * GLCD_BLOCK_HEIGHT - 1;
    uint8_t blockRightX = (gapFromRight >= GLCD_COLS) ? 0 : (GLCD_COLS - gapFromRight - 1);
    uint8_t blockLeftX = (blockRightX >= (GLCD_BLOCK_HEIGHT - 1)) ? (blockRightX - (GLCD_BLOCK_HEIGHT - 1)) : 0;

    // For each column the block covers
    for (uint8_t x = blockLeftX; x <= blockRightX; x++)
    {
        // For each page (8 rows), set the byte for this column to 0
        for (uint8_t page = 0; page < (GLCD_ROWS / 8); page++)
        {
            uint16_t idx = page * GLCD_COLS + x;
            glcdImageBuffer[idx] = GLCD_EMPTY_BYTE;
        }
    }
}

