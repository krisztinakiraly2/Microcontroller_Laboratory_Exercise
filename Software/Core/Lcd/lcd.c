#include "lcd.h"

void WriteToDisplay(uint8_t num, bool isData)
{
	// Set DI (Instruction mode)
    HAL_GPIO_WritePin(GLCD_DI_GPIO_Port, GLCD_DI_Pin, (isData==DATA) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Set RW to write mode (0: write, 1: read)
    HAL_GPIO_WritePin(GLCD_RW_GPIO_Port, GLCD_RW_Pin, GPIO_PIN_RESET);

    // Set data pins according to 'num'
    HAL_GPIO_WritePin(GLCD_DB0_GPIO_Port, GLCD_DB0_Pin, (num & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET); // D0
    HAL_GPIO_WritePin(GLCD_DB1_GPIO_Port, GLCD_DB1_Pin, (num & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET); // D1
    HAL_GPIO_WritePin(GLCD_DB2_GPIO_Port, GLCD_DB2_Pin, (num & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET); // D2
    HAL_GPIO_WritePin(GLCD_DB3_GPIO_Port, GLCD_DB3_Pin, (num & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET); // D3
    HAL_GPIO_WritePin(GLCD_DB4_GPIO_Port, GLCD_DB4_Pin, (num & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET); // D4
    HAL_GPIO_WritePin(GLCD_DB5_GPIO_Port, GLCD_DB5_Pin, (num & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET); // D5
    HAL_GPIO_WritePin(GLCD_DB6_GPIO_Port, GLCD_DB6_Pin, (num & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET); // D6
    HAL_GPIO_WritePin(GLCD_DB7_GPIO_Port, GLCD_DB7_Pin, (num & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET); // D7

    // Set E (Enable) high
    HAL_GPIO_WritePin(GLCD_E_GPIO_Port, GLCD_E_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GLCDEN_GPIO_Port, GLCDEN_Pin, GPIO_PIN_SET);

    // Small delay to let data latch (adjust if needed)
    for(volatile int i=0; i<50; i++); // 1-2 us waiting

    // Set E (Enable) low (falling edge triggers write)
    HAL_GPIO_WritePin(GLCD_E_GPIO_Port, GLCD_E_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCDEN_GPIO_Port, GLCDEN_Pin, GPIO_PIN_RESET);
}

