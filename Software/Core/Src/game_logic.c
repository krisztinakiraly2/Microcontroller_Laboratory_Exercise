#include "game_logic.h"

//TODO: flip vertical naming to horizontal and vice verse
void ClearBlocksFromLevel(uint8_t level)
{
    uint16_t gapFromRight = level * BLOCK_HEIGHT;
    uint8_t blockRightX = (gapFromRight >= GLCD_COLS) ? 0 : (GLCD_COLS - gapFromRight);
    uint8_t blockLeftX = (blockRightX >= (BLOCK_HEIGHT - 1)) ? (blockRightX - (BLOCK_HEIGHT - 1)) : 0;

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
void MoveAllBlockDown(uint8_t levels)
{
	 if (levels == 0)
	        return;

	uint8_t cols_to_shift = levels * BLOCK_HEIGHT;
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


// Returns 1 if there is overlap, 0 if game over (no overlap)
// Updates currentBlock's leftX and rightX to the overlapped area
int AlignAndTrimBlock(BlockRect* prevBlock, BlockRect* currBlock)
{
    // On flipped axes: leftX > rightX
    uint8_t overlapLeft = (currBlock->leftX < prevBlock->leftX) ? currBlock->leftX : prevBlock->leftX;
    uint8_t overlapRight = (currBlock->rightX > prevBlock->rightX) ? currBlock->rightX : prevBlock->rightX;

    if (overlapLeft < overlapRight)
    {
        // No overlap
        return 0; // Game over!
    }

    // Trim the current block to overlap area
    currBlock->leftX = overlapLeft;
    currBlock->rightX = overlapRight;
    return 1; // Still in game
}

void UpdateScoreDisplay(uint16_t score)
{
	GlcdDrawNumber(glcdImageBuffer, score, 0, 0, 0);
}
