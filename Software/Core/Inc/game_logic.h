/*
 * game_logic.h
 *
 *  Created on: May 26, 2025
 *      Author: krisz
 */

#ifndef INC_GAME_LOGIC_H_
#define INC_GAME_LOGIC_H_

#include "glcd.h"

#define GAME_AREA_START_COL 10
#define GAME_AREA_END_COL GLCD_COLS

#define BLOCK_HEIGHT 20
#define BLOCK_WIDTH 38
#define BLOCK_RIGHT_MARGIN 13
#define OUTLINE_WIDTH 1

void ClearBlocksFromLevel(uint8_t);
void MoveAllBlockDown(uint8_t);
int AlignAndTrimBlock(BlockRect*, BlockRect*);
void UpdateScoreDisplay(uint16_t);

#endif /* INC_GAME_LOGIC_H_ */
