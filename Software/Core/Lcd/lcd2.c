#include "main.h"
#include "lcd2.h"
#include "cmsis_os.h"

#define LCD_CS1_PIN        GLCD_CS1_Pin
#define LCD_CS1_PORT       GLCD_CS1_GPIO_Port

#define LCD_CS2_PIN        GLCD_CS2_Pin
#define LCD_CS2_PORT       GLCD_CS2_GPIO_Port

#define LCD_RST_PIN        GLCD_RESET_Pin
#define LCD_RST_PORT       GLCD_RESET_GPIO_Port

#define LCD_E_PIN          GLCD_E_Pin
#define LCD_E_PORT         GLCD_E_GPIO_Port

#define LCD_RS_PIN         GLCD_DI_Pin
#define LCD_RS_PORT        GLCD_DI_GPIO_Port

//#define LCD_RW_PIN         GLCD_RW_Pin
//#define LCD_RW_PORT        GLCD_RW_GPIO_Port

// Example pulse enable (you may need to tune timing with delay or NOPs)
void pulse_enable(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	osDelay(5);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); //e
	osDelay(5);
}

void glcd_write_data_bus(uint8_t data)
{
    HAL_GPIO_WritePin(GLCD_DB0_GPIO_Port, GLCD_DB0_Pin, (data & (1 << 0)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB1_GPIO_Port, GLCD_DB1_Pin, (data & (1 << 1)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB2_GPIO_Port, GLCD_DB2_Pin, (data & (1 << 2)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB3_GPIO_Port, GLCD_DB3_Pin, (data & (1 << 3)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB4_GPIO_Port, GLCD_DB4_Pin, (data & (1 << 4)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB5_GPIO_Port, GLCD_DB5_Pin, (data & (1 << 5)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB6_GPIO_Port, GLCD_DB6_Pin, (data & (1 << 6)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GLCD_DB7_GPIO_Port, GLCD_DB7_Pin, (data & (1 << 7)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/*void ks_write_cmd(uint8_t cmd, bool chip_left) {
    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET); // RS = 0 (command)
    HAL_GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_RESET); // R/W = 0 (write)

    HAL_GPIO_WritePin(LCD_CS1_PORT, LCD_CS1_PIN, chip_left ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_CS2_PORT, LCD_CS2_PIN, chip_left ? GPIO_PIN_RESET : GPIO_PIN_SET);

    glcd_write_data_bus(cmd);
    pulse_enable();
}

void ks_write_data(uint8_t data, bool chip_left)
{
    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET);   // RS = 1 (data)
    HAL_GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_RESET); // R/W = 0 (write)

    HAL_GPIO_WritePin(LCD_CS1_PORT, LCD_CS1_PIN, chip_left ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_CS2_PORT, LCD_CS2_PIN, chip_left ? GPIO_PIN_RESET : GPIO_PIN_SET);

    glcd_write_data_bus(data);
    pulse_enable();
}

void putPixel(uint8_t x, uint8_t y)
{
    bool left = x < 64;
    uint8_t page = y >> 3;
    uint8_t bit = y & 7;
    uint8_t mask = 1 << bit;

    ks_write_cmd(0xB8 | page, left);
    ks_write_cmd(0x40 | (x & 0x3F), left);

    uint8_t cur = 0; // If not reading, just assume 0
    uint8_t nw = cur | mask;

    ks_write_cmd(0x40 | (x & 0x3F), left);
    ks_write_data(nw, left);
}

void glcd_init(void)
{
	HAL_GPIO_WritePin(GLCDEN_GPIO_Port, GLCDEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS1_PORT, LCD_CS1_PIN, GPIO_PIN_SET);

    // Reset GLCD
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
	osDelay(1000);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
	osDelay(1000);

    // Turn display ON, clear each page
    for (int chip = 0; chip < 2; chip++)
    {
        ks_write_cmd(0x3F, chip);  // Display ON
        for (uint8_t page = 0; page < 8; page++)
        {
            ks_write_cmd(0xB8 | page, chip);
            ks_write_cmd(0x40, chip);
            for (uint8_t col = 0; col < 64; col++)
            {
                ks_write_data(0x00, chip);  // Clear screen
            }
        }
    }
}

void glcd_test_data_bus(void)
{
    // Pull R/W LOW (write mode), CS1 LOW, CS2 LOW
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET); // R/W
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);  // CS1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);  // CS2

    // Reset display
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    osDelay(10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    osDelay(10);

    // Set to instruction mode
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);  // RS = 0

    // Send Display ON command (0x3F)
    glcd_write_data_bus(0x3F); pulse_enable();

    // Send page 0
    glcd_write_data_bus(0xB8); pulse_enable();

    // Column = 0
    glcd_write_data_bus(0x40); pulse_enable();

    // Data mode
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);    // RS = 1

    // Output a known byte
    glcd_write_data_bus(0xFF); pulse_enable();  // Should light up alternating pixels (10101010)
}*/
