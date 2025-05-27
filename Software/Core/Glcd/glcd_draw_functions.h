#ifndef LCD_GLCD_DRAW_FUNCTIONS_H_
#define LCD_GLCD_DRAW_FUNCTIONS_H_

void GlcdDrawPixel(uint8_t*, uint8_t, uint8_t, uint8_t);
void GlcdDrawStartBG(uint8_t, uint8_t);
void GlcdDrawLine(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void GlcdDrawBox(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void GlcdDrawNewBlock(uint8_t, uint8_t);
void GlcdDrawDigit(uint8_t*, uint8_t, uint8_t, uint8_t, uint8_t);
void GlcdDrawNumber(uint8_t*, uint16_t, uint8_t, uint8_t, uint8_t);
void GlcdDrawScoreText(uint8_t*, uint8_t, uint8_t);
void GlcdDrawGameOver(uint16_t);

#endif /* LCD_GLCD_DRAW_FUNCTIONS_H_ */
