/*
 * glcd_defines.h
 *
 *  Created on: May 25, 2025
 *      Author: krisz
 */

#ifndef LCD_GLCD_DEFINES_H_
#define LCD_GLCD_DEFINES_H_

#define GLCD_DI_DATA_MODE GPIO_PIN_SET
#define GLCD_DI_COMMAND_MODE GPIO_PIN_RESET

#define GLCD_EMPTY_BYTE 0xFF
#define GLCD_FULL_BYTE 0x00

#define GLCD_TURN_ON 0x3F
#define GLCD_Y_ADDR_ZERO 0x40
#define GLCD_X_ADDR_ZERO 0xB8
#define GLCD_Z_ADDR_ZERO 0xC0

#define GLCD_ROWS 64
#define GLCD_COLS 128

#define GLCD_DEFAULT_REFRESH_SPEED 25

typedef struct
{
    uint8_t leftX;   // left (min X)
    uint8_t rightX;  // right (max X)
} BlockRect;


#endif /* LCD_GLCD_DEFINES_H_ */
