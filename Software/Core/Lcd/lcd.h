/*
 * lcd2.h
 *
 *  Created on: May 23, 2025
 *      Author: krisz
 */

#ifndef LCD_LCD2_H_
#define LCD_LCD2_H_

#include "stdbool.h"

#define GLCD_DI_DATA_MODE GPIO_PIN_SET
#define GLCD_DI_COMMAND_MODE GPIO_PIN_RESET

#define GLCD_EMPTY_BYTE 0xFF

#define GLCD_TURN_ON 0x3F
#define GLCD_Y_ADDR_ZERO 0x40
#define GLCD_X_ADDR_ZERO 0xB8
#define GLCD_Z_ADDR_ZERO 0xC0

#define GLCD_ROWS 64
#define GLCD_COLS 128
extern uint8_t glcdImageBuffer[];

void PulseEnable();
void GlcdWriteData(uint8_t data, GPIO_PinState);
void GlcdInit();
void GlcdClearMemory();

#endif /* LCD_LCD2_H_ */
