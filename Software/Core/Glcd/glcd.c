#include "lcd.h"

//uint8_t glcdImageBuffer[(128 * 64)/8];
uint8_t* glcdImageBuffer;
uint16_t glcdCurrentLevel = 0;

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
                Delay_us(25);
            }
        }
    }
}

//TODO: flip vertical naming to horizontal and vice verse
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

// to have enough space for the new block move the blocks down
void GlcdMoveAllBlockDown(uint8_t levels)
{
	 if (levels == 0)
	        return;

	uint8_t cols_to_shift = levels * GLCD_BLOCK_HEIGHT;
	if (cols_to_shift >= (GAME_AREA_END_COL - GAME_AREA_START_COL))
	{
		// If shifting more than fits, just clear the area
		for (int row = 0; row < (GLCD_ROWS / 8); ++row)
		{
			for (int col = GAME_AREA_START_COL; col < GAME_AREA_END_COL; ++col)
			{
				glcdImageBuffer[row * GLCD_COLS + col] = GLCD_EMPTY_BYTE;
			}
		}
	}
	else
	{
		// Shift each "game" column's data right
		for (int row = 0; row < (GLCD_ROWS / 8); ++row)
		{
			for (int col = GAME_AREA_END_COL - 1; col >= GAME_AREA_START_COL; --col)
			{
				int src_col = col - cols_to_shift;
				uint8_t val = 0x00;
				if (src_col >= GAME_AREA_START_COL)
				{
					val = glcdImageBuffer[row * GLCD_COLS + src_col];
				}

				glcdImageBuffer[row * GLCD_COLS + col] = val;
			}

			// Clear the leftmost new columns that just opened up
			for (int col = GAME_AREA_START_COL; col < GAME_AREA_START_COL + cols_to_shift; ++col)
			{
				glcdImageBuffer[row * GLCD_COLS + col] = GLCD_EMPTY_BYTE;
			}
		}
	}
}
