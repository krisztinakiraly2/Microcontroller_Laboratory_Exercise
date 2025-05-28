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
volatile uint16_t glcdBacklightLevel = GLCD_BACKLIGHT_START;
volatile uint16_t score = 0;
osMessageQueueId_t uartQueueHandle;

osThreadId_t createMyTasksInitTaskHandle;
osThreadId_t uartCommunicationTaskHandle;
osThreadId_t glcdUpdateTaskHandle;
osThreadId_t glcdBacklightControlTaskHandle;
osThreadId_t glcdBacklightPlusTaskHandle;
osThreadId_t glcdBacklightMinusTaskHandle;
osThreadId_t resetTaskHandle;

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

const osThreadAttr_t glcdUpdateTask_attributes =
{
  .name = "LcdUpdateTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t glcdBacklightControlTask_attributes =
{
  .name = "BacklightControlTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t glcdBacklightPlusTask_attributes =
{
  .name = "BacklightControlTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t glcdBacklightMinusTask_attributes =
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
	// Initialize the lcd
	GlcdInit();

	// create the queue for the uart task
	uartQueueHandle = osMessageQueueNew(UART_QUEUE_LENGTH, sizeof(UartTaskParams_t), NULL);

	// Create tasks
	uartCommunicationTaskHandle = osThreadNew(UartCommunicationTask, NULL, &uartCommunicationTask_attributes);
	glcdUpdateTaskHandle = osThreadNew(GlcdUpdateTask, NULL, &glcdUpdateTask_attributes);
	glcdBacklightControlTaskHandle = osThreadNew(GlcdBacklightControlTask, NULL, &glcdBacklightControlTask_attributes);
	glcdBacklightPlusTaskHandle = osThreadNew(GlcdBacklightPlusTask, NULL, &glcdBacklightPlusTask_attributes);
	glcdBacklightMinusTaskHandle = osThreadNew(GlcdBacklightMinusTask, NULL, &glcdBacklightMinusTask_attributes);
	resetTaskHandle = osThreadNew(ResetTask, NULL, &resetTask_attributes);

	// Delete self
	vTaskDelete(createMyTasksInitTaskHandle);
	while(1);
}

void UartCommunicationTask(void *argument)
{
	UartTaskParams_t params;
	while (1)
	{
		if (osMessageQueueGet(uartQueueHandle, &params, NULL, osWaitForever) == osOK)
		{
			HAL_UART_Transmit(&huart2, params.pData, params.length, params.Timeout);
		}
	}
}

void GlcdUpdateTask(void *argument)
{
	int8_t direction = -1; // Start moving "up" (to 0)
	uint8_t position = BLOCK_RIGHT_MARGIN;
	uint16_t gameSpeed = GLCD_DEFAULT_REFRESH_SPEED;
	const int gameSpeedMin = 5;
	bool moveBlocksEnabled = false;
	int currentWidth = BLOCK_WIDTH;

	static char scoreMsgStatic[8];
	UartTaskParams_t uartMsg;
	uartMsg.Timeout = 100;

	BlockRect currentBlock;
	BlockRect prevBlock;
	prevBlock.rightX = BLOCK_RIGHT_MARGIN;
	prevBlock.leftX = BLOCK_RIGHT_MARGIN + BLOCK_WIDTH - 1;

	GlcdDrawStartBG(position,currentWidth);
	GlcdPrintFromImageBuffer(GLCD_DEFAULT_REFRESH_SPEED);

	while (1)
	{
		// Check for notification (non-blocking)
		if (ulTaskNotifyTake(pdTRUE, 0))
		{
			if (!moveBlocksEnabled && glcdCurrentLevel >= 3)
				moveBlocksEnabled = true;

			if (!AlignAndTrimBlock(&prevBlock, &currentBlock))
			{
				// No overlap = game over
				GlcdDrawGameOver(score);

				// Prepare the score as a string (e.g., "34\r\n")
				snprintf(scoreMsgStatic, sizeof(scoreMsgStatic), "%d\r\n", score);

				// Prepare the struct
				uartMsg.pData = (uint8_t*)scoreMsgStatic;
				uartMsg.length = strlen(scoreMsgStatic);

				// Send to UART queue (wait max 10ms if queue is full)
				osMessageQueuePut(uartQueueHandle, &uartMsg, 0, 10);

				if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
				{
					score = 0;
					direction = -1; // Start moving "up" (to 0)
					position = BLOCK_RIGHT_MARGIN;
					gameSpeed = GLCD_DEFAULT_REFRESH_SPEED;
					moveBlocksEnabled = false;
					currentWidth = BLOCK_WIDTH;

					prevBlock.rightX = BLOCK_RIGHT_MARGIN;
					prevBlock.leftX = BLOCK_RIGHT_MARGIN + BLOCK_WIDTH - 1;

					GlcdDrawStartBG(position,currentWidth);
					GlcdPrintFromImageBuffer(GLCD_DEFAULT_REFRESH_SPEED);
					continue;
				}
			}
			else
			{
				// Block survived, update for next round
				prevBlock = currentBlock;
				currentWidth = currentBlock.leftX - currentBlock.rightX + 1;
				position=currentBlock.rightX;
				score++;
				if ((score % 5 == 0) && (gameSpeed > gameSpeedMin))
				{
				    gameSpeed -= 4;
				    if (gameSpeed < gameSpeedMin)
				        gameSpeed = gameSpeedMin;
				}

			}

			ClearBlocksFromLevel(glcdCurrentLevel);
			GlcdDrawNewFilledBlock(currentBlock.rightX,currentWidth);

			if (moveBlocksEnabled)
			{
				MoveAllBlockDown(1);
				// Do not increment glcdCurrentLevel, so moving block stays at the same spot
			}
			else
			{
				glcdCurrentLevel++; // Regular increment for first 3 rounds
			}
		}

		// Do animation as usual
		ClearBlocksFromLevel(glcdCurrentLevel);
	    GlcdDrawNewBlock(position,currentWidth);
	    UpdateScoreDisplay(score);
	    GlcdPrintFromImageBuffer(gameSpeed);

	    currentBlock.rightX = position;
	    currentBlock.leftX = position + currentWidth - 1;

	    // Bounce logic
	    if (direction < 0) // Going "up" (to 0)
	    {
	        if (position > 0)
	            position--;
	        else
	            direction = 1; // Hit top, reverse
	    }
	    else // Going "down" (to 64 - BLOCK_WIDTH)
	    {
	    	if (position < (64 - currentWidth))
	            position++;
	        else
	            direction = -1; // Hit bottom, reverse
	    }

	}
}

void GlcdBacklightControlTask (void *argument)
{
	uint8_t prevLevel = 0xFF; // Force first update
	while(1)
	{
		if (glcdBacklightLevel != prevLevel)
		{
			// Update PWM compare (duty cycle)
			__HAL_TIM_SET_COMPARE(&PWM_TIMER, PWM_CHANNEL, glcdBacklightLevel); // 100-1000 for 10%-100%
			prevLevel = glcdBacklightLevel;
		}
		osDelay(10); // Polling period (adjust as needed)
	}
}

void GlcdBacklightPlusTask (void *argument)
{
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Wait for notification (blocks until given)
		vTaskDelay(pdMS_TO_TICKS(20)); // crude debounce
		// Do the light+
		if (glcdBacklightLevel+GLCD_BACKLIGHT_STEP < GLCD_BACKLIGHT_MAX)
		{
			glcdBacklightLevel += GLCD_BACKLIGHT_STEP;
		}
		else
		{
			glcdBacklightLevel = GLCD_BACKLIGHT_MAX;
		}
	}
}

void GlcdBacklightMinusTask (void *argument)
{
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Wait for notification (blocks until given)
		vTaskDelay(pdMS_TO_TICKS(20)); // crude debounce
		// Do the light-
		if (glcdBacklightLevel-GLCD_BACKLIGHT_STEP > GLCD_BACKLIGHT_MIN)
		{
			glcdBacklightLevel -= GLCD_BACKLIGHT_STEP;
		}
		else
		{
			glcdBacklightLevel = GLCD_BACKLIGHT_MIN;
		}
	}
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

