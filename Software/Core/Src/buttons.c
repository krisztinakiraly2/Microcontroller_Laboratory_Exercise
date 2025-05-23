#include "main.h"  // make sure the defines are included
#include "mytasks.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
    	case Place_Pin: break;
    	case LightPlus_Pin: break;
    	case LightMinus_Pin: break;
    	case Reset_Pin:
    		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    		vTaskNotifyGiveFromISR(resetTaskHandle, &xHigherPriorityTaskWoken); // Notify the ResetTask
    		portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // Request a context switch if needed
    		break;
    }
}
