#include "game_logic.h"

void ClearBlocksFromLevel(uint8_t level)
{
    uint16_t gapFromBottom = level * BLOCK_HEIGHT;
    uint8_t blockTopY = (gapFromBottom >= GLCD_ROWS) ? 0 : (GLCD_ROWS - gapFromBottom);
    uint8_t blockBottomY = (blockTopY >= (BLOCK_HEIGHT - 1)) ? (blockTopY - (BLOCK_HEIGHT - 1)) : 0;

    // For each row the block covers
    for (uint8_t row = blockBottomY; row <= blockTopY; row++)
    {
        // For each column, set the byte for this row to 0
        for (uint8_t col = 0; col < (GLCD_COLS / 8); col++)
        {
            uint16_t idx = col * GLCD_ROWS + row;
            glcdImageBuffer[idx] = GLCD_EMPTY_BYTE;
        }
    }
}

// to have enough space for the new block move the blocks down
void MoveAllBlockDown(uint8_t levels)
{
    if (levels == 0)
        return;

    uint8_t rows_to_shift = levels * BLOCK_HEIGHT;
    if (rows_to_shift >= (GAME_AREA_END_ROW - GAME_AREA_START_ROW))
    {
        // If shifting more than fits, just clear the area
        for (int row = 0; row < (GLCD_COLS / 8); ++row)
        {
            for (int col = GAME_AREA_START_ROW; col < GAME_AREA_END_ROW; ++col)
            {
                glcdImageBuffer[row * GLCD_ROWS + col] = GLCD_EMPTY_BYTE;
            }
        }
    }
    else
    {
        // Shift each "game" row's data down
        for (int row = 0; row < (GLCD_COLS / 8); ++row)
        {
            for (int col = GAME_AREA_END_ROW - 1; col >= GAME_AREA_START_ROW; --col)
            {
                int src_col = col - rows_to_shift;
                uint8_t val = 0x00;
                if (src_col >= GAME_AREA_START_ROW)
                {
                    val = glcdImageBuffer[row * GLCD_ROWS + src_col];
                }

                glcdImageBuffer[row * GLCD_ROWS + col] = val;
            }

            // Clear the leftmost new rows that just opened up
            for (int col = GAME_AREA_START_ROW; col < GAME_AREA_START_ROW + rows_to_shift; ++col)
            {
                glcdImageBuffer[row * GLCD_ROWS + col] = GLCD_EMPTY_BYTE;
            }
        }
    }
}

// Returns 1 if there is overlap, 0 if game over (no overlap)
// Updates currentBlock's leftX and rightX to the overlapped area
int AlignAndTrimBlock(BlockRect* prevBlock, BlockRect* currBlock)
{
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
