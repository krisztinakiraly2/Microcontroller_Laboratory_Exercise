/*
 * lcd2.h
 *
 *  Created on: May 23, 2025
 *      Author: krisz
 */

#ifndef LCD_LCD2_H_
#define LCD_LCD2_H_

#include "stdbool.h"

void ks_write_cmd(uint8_t cmd, bool chip_left);
void ks_write_data(uint8_t data, bool chip_left);
void glcd_write_data_bus(uint8_t data);
void pulse_enable(void);
void putPixel(uint8_t x, uint8_t y);
void glcd_init(void);
void glcd_test_data_bus(void);

#endif /* LCD_LCD2_H_ */
