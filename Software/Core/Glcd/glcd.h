/*
 * lcd2.h
 *
 *  Created on: May 23, 2025
 *      Author: krisz
 */

#ifndef LCD_LCD2_H_
#define LCD_LCD2_H_

#include "stdbool.h"
#include "cmsis_os.h"
#include "main.h"
#include "string.h"
#include "glcd_defines.h"
#include "glcd_buffers.h"
#include "glcd_draw_functions.h"
#include "glcd_fill_functions.h"
#include "mytasks.h"

extern uint8_t* glcdImageBuffer;
extern uint16_t glcdCurrentLevel;

void PulseEnable();
void GlcdWriteData(uint8_t data, GPIO_PinState);
void GlcdInit();
void GlcdClearMemory();
void GlcdPrintFromImageBuffer();
void GlcdClearBlockColumns();
void GlcdMoveAllBlockDown(uint8_t levels);

#endif /* LCD_LCD2_H_ */
