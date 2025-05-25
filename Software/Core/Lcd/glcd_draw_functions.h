#ifndef LCD_GLCD_DRAW_FUNCTIONS_H_
#define LCD_GLCD_DRAW_FUNCTIONS_H_

void GlcdDrawPixel(uint8_t*, uint8_t, uint8_t, uint8_t);
void GlcdDrawStartBG();
void GlcdDrawLine(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void GlcdDrawBox(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void GlcdDrawWindow(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void GlcdDrawTwoWindows(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void GlcdDrawNewBlock(uint8_t position);

#endif /* LCD_GLCD_DRAW_FUNCTIONS_H_ */
