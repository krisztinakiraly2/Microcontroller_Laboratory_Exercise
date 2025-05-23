#include "main.h"
#include "lcd.h"
#include "cmsis_os.h"

uint8_t glcdImageBuffer[(128 * 64)/8];

// The enable's falling edge triggers the reading of the command or data in the lcd
void PulseEnable()
{
	HAL_GPIO_WritePin(GLCD_E_GPIO_Port, GLCD_E_Pin, GPIO_PIN_SET); // 1 active
	osDelay(1);
	HAL_GPIO_WritePin(GLCD_E_GPIO_Port, GLCD_E_Pin, GPIO_PIN_RESET);
	osDelay(1);
}

// write the data to the pins based on the bits in data and triggers write to lcd
void GlcdWriteData(uint8_t data, GPIO_PinState mode)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, mode); // Set DI based on mode var

	HAL_GPIO_WritePin(GLCD_DB0_GPIO_Port, GLCD_DB0_Pin, (data & (1 << 0)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB1_GPIO_Port, GLCD_DB1_Pin, (data & (1 << 1)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB2_GPIO_Port, GLCD_DB2_Pin, (data & (1 << 2)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB3_GPIO_Port, GLCD_DB3_Pin, (data & (1 << 3)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB4_GPIO_Port, GLCD_DB4_Pin, (data & (1 << 4)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB5_GPIO_Port, GLCD_DB5_Pin, (data & (1 << 5)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB6_GPIO_Port, GLCD_DB6_Pin, (data & (1 << 6)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB7_GPIO_Port, GLCD_DB7_Pin, (data & (1 << 7)) ? GPIO_PIN_SET : GPIO_PIN_RESET);

	PulseEnable(); // trigger write
}

// Does the required resets based on the datasheet
void GlcdInit()
{
	HAL_GPIO_WritePin(GLCDEN_GPIO_Port, GLCDEN_Pin, GPIO_PIN_RESET); // enable buffers (0 active)
	HAL_GPIO_WritePin(GLCD_RW_GPIO_Port, GLCD_RW_Pin, GPIO_PIN_RESET); // Set the direction to writing (0: write, 1: read)
	HAL_GPIO_WritePin(GLCD_RESET_GPIO_Port, GLCD_RESET_Pin, GPIO_PIN_RESET); // reset the lcd panel (0 active)
	osDelay(1);
	HAL_GPIO_WritePin(GLCD_RESET_GPIO_Port, GLCD_RESET_Pin, GPIO_PIN_SET); // ends the reset

	HAL_GPIO_WritePin(GLCD_CS1_GPIO_Port, GLCD_CS1_Pin, GPIO_PIN_RESET);  // enable CS1 (0 active)
	HAL_GPIO_WritePin(GLCD_CS2_GPIO_Port, GLCD_CS2_Pin, GPIO_PIN_RESET);  // enable CS2 (0 active)

	GlcdWriteData(GLCD_TURN_ON, GLCD_DI_COMMAND_MODE); // turn on the lcd
	osDelay(1);
    GlcdWriteData(GLCD_Y_ADDR_ZERO, GLCD_DI_COMMAND_MODE); // set y adrr to 0
	GlcdWriteData(GLCD_X_ADDR_ZERO, GLCD_DI_COMMAND_MODE); // set x addr to 0
	GlcdWriteData(GLCD_Y_ADDR_ZERO, GLCD_DI_COMMAND_MODE); // set z adrr to 0 (for ram memory)
	osDelay(1);

	GlcdClearMemory();
}

void GlcdClearMemory()
{
	HAL_GPIO_WritePin(GLCD_CS1_GPIO_Port, GLCD_CS1_Pin, GPIO_PIN_RESET);  // enable CS1 (0 active)
	HAL_GPIO_WritePin(GLCD_CS2_GPIO_Port, GLCD_CS2_Pin, GPIO_PIN_RESET);  // enable CS2 (0 active)

	for (int i=0; i<= GLCD_ROWS/8; ++i)
	{
		for (uint8_t j = 0; j < GLCD_COLS/2; ++j)
		{
			GlcdWriteData(GLCD_EMPTY_BYTE, GLCD_DI_DATA_MODE);
		}

		GlcdWriteData(GLCD_X_ADDR_ZERO+i, GLCD_DI_COMMAND_MODE);
	}
}
