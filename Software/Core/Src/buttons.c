#include "main.h"
#include "mytasks.h"
#include "cmsis_os.h"

uint32_t lastPlusTime = 0;
uint32_t lastMinusTime = 0;
uint32_t lastResetTime = 0;
uint32_t debounceDelay = 50; // milliseconds

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	uint32_t now = xTaskGetTickCountFromISR();

	switch(GPIO_Pin)
	{
		case Place_Pin: break;
		case LightPlus_Pin:
			if ((now - lastPlusTime) > debounceDelay)
			{
				vTaskNotifyGiveFromISR(glcdBacklightPlusTaskHandle, &xHigherPriorityTaskWoken);
				lastPlusTime = now;
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			}
			break;
		case LightMinus_Pin:
			if ((now - lastMinusTime) > debounceDelay)
			{
				vTaskNotifyGiveFromISR(glcdBacklightMinusTaskHandle, &xHigherPriorityTaskWoken);
				lastMinusTime = now;
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			}
			break;
		case Reset_Pin:
			if ((now - lastResetTime) > debounceDelay)
			{
				vTaskNotifyGiveFromISR(resetTaskHandle, &xHigherPriorityTaskWoken);
				lastResetTime = now;
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			}
			break;
	}
}
