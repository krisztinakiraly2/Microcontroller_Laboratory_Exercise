#ifndef LCD_GLCD_FILL_FUNCTIONS_H_
#define LCD_GLCD_FILL_FUNCTIONS_H_

void GlcdFillBox(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void GlcdFillWindow(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void GlcdFillTwoWindows(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,uint8_t, uint8_t);
void GlcdDrawNewFilledBlock(uint8_t position);

#endif /* LCD_GLCD_FILL_FUNCTIONS_H_ */
