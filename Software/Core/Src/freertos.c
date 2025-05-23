/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mytasks.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
    uint8_t *pData;
    uint16_t length;
    uint32_t Timeout;
} UartTaskParams_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
bool init = true;

osThreadId_t createMyTasksInitTaskHandle;
osThreadId_t uartCommunicationTaskHandle;
osThreadId_t gameLogicTaskHandle;
osThreadId_t glcdUpdateTaskHandle;
osThreadId_t backlightControlTaskHandle;
osThreadId_t resetTaskHandle;

extern UART_HandleTypeDef huart2;

const osThreadAttr_t createMyTasksInitTask_attributes =
{
  .name = "CreateMyTasksInitTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t uartCommunicationTask_attributes =
{
  .name = "UartCommunicationTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t gameLogicTask_attributes =
{
  .name = "GameLogicTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t glcdUpdateTask_attributes =
{
  .name = "LcdUpdateTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t backlightControlTask_attributes =
{
  .name = "BacklightControlTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t resetTask_attributes =
{
  .name = "ResetTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal+1,
};
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void CreateMyTasksInitTask(void *argument)
{
	// Create paramaters
	UartTaskParams_t uartParams =
	{
		.pData = (uint8_t *)"Hello world!",
		.length = 12,
		.Timeout = 2000
	};

	// Initialize the lcd
	GlcdInit();

	// Create tasks
	uartCommunicationTaskHandle = osThreadNew(UartCommunicationTask, &uartParams, &uartCommunicationTask_attributes);
	glcdUpdateTaskHandle = osThreadNew(GlcdUpdateTask, NULL, &glcdUpdateTask_attributes);
	resetTaskHandle = osThreadNew(ResetTask, NULL, &resetTask_attributes);

	// Delete self
	vTaskDelete(createMyTasksInitTaskHandle);
	while(1);
}

void UartCommunicationTask(void *argument)
{
	UartTaskParams_t *params;
	while(1)
	{
		params = (UartTaskParams_t *)argument; // Get the params
		HAL_UART_Transmit(&huart2, params->pData, params->length, params->Timeout); // Send the message
		osDelay(1000); // Delay so it isn't spaming the data
	}
}

void GlcdUpdateTask(void *argument)
{
	// write the letter R to page 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // CS1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); // CS2
	GlcdWriteData(GLCD_X_ADDR_ZERO, GLCD_DI_COMMAND_MODE);
	uint8_t rLetter[] = {0x00, 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00};
	for (uint8_t i = 0; i < sizeof(rLetter); ++i)
	{
		GlcdWriteData(rLetter[i],GLCD_DI_DATA_MODE);
	}

	osDelay(100);
}

void ResetTask(void *argument)
{
    while(1)
    {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Wait for notification (blocks until given)
		vTaskDelay(pdMS_TO_TICKS(20)); // crude debounce
		NVIC_SystemReset(); // Do the reset
    }
}
/* USER CODE END Application */

