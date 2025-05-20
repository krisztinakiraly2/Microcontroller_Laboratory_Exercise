#include "main.h"  // make sure the defines are included

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
    	case Place_Pin: HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (!HAL_GPIO_ReadPin(Place_GPIO_Port, Place_Pin)) ? GPIO_PIN_SET : GPIO_PIN_RESET); break;
    	case LightPlus_Pin: HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (!HAL_GPIO_ReadPin(LightPlus_GPIO_Port, LightPlus_Pin)) ? GPIO_PIN_SET : GPIO_PIN_RESET); break;
    	case LightMinus_Pin: HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (!HAL_GPIO_ReadPin(LightMinus_GPIO_Port, LightMinus_Pin)) ? GPIO_PIN_SET : GPIO_PIN_RESET); break;
    	case Reset_Pin: HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (!HAL_GPIO_ReadPin(Reset_GPIO_Port, Reset_Pin)) ? GPIO_PIN_SET : GPIO_PIN_RESET); break;
    }
}
