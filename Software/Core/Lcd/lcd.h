#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#include "main.h"
#include "stdbool.h"

#define DATA true
#define COMMAND false

void WriteToDisplay(uint8_t,bool);

#endif /* LCD_LCD_H_ */
