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
#include "main.h"
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
osThreadId_t createMyTasksInitTaskHandle;
osThreadId_t toggleLedOnButtonPushTaskHandle;
osThreadId_t uartCommunicationTaskHandle;
osThreadId_t gameLogicTaskHandle;
osThreadId_t lcdUpdateTaskHandle;
osThreadId_t BacklightControlTaskHandle;

extern UART_HandleTypeDef huart2;

const osThreadAttr_t createMyTasksInitTask_attributes =
{
  .name = "CreateMyTasksInitTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t toggleLedOnButtonPushTask_attributes =
{
  .name = "ToggleLedOnButtonPushTask",
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

const osThreadAttr_t lcdUpdateTask_attributes =
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
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void ToggleLedOnButtonPushTask(void *argument)
{
	int buttonState;
    for(;;)
    {
        buttonState = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, buttonState ? GPIO_PIN_SET : GPIO_PIN_RESET);
        osDelay(10);
    }
}

void CreateMyTasksInitTask(void *argument)
{
	UartTaskParams_t uartParams =
	{
		.pData = (uint8_t *)"Hello world!",
		.length = 12,
		.Timeout = 2000
	};

	// Create tasks
	//toggleLedOnButtonPushTaskHandle = osThreadNew(ToggleLedOnButtonPushTask, NULL, &toggleLedOnButtonPushTask_attributes);
	uartCommunicationTaskHandle = osThreadNew(UartCommunicationTask, &uartParams, &toggleLedOnButtonPushTask_attributes);

	// Delete self
	vTaskDelete(createMyTasksInitTaskHandle);

	while(1);
}

void UartCommunicationTask(void *argument)
{
	UartTaskParams_t *params;
	while(1)
	{
		params = (UartTaskParams_t *)argument;
		HAL_UART_Transmit(&huart2, params->pData, params->length, params->Timeout);
		osDelay(100);
	}
}
/* USER CODE END Application */

