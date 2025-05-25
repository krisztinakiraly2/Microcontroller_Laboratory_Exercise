/*
 * freertos.h
 *
 *  Created on: May 20, 2025
 *      Author: krisz
 */

#ifndef INC_MYTASKS_H_
#define INC_MYTASKS_H_

#include "cmsis_os.h"
#include "lcd.h"

#define GLCD_BACKLIGHT_MIN 100
#define GLCD_BACKLIGHT_MAX 1000
#define GLCD_BACKLIGHT_START 800
#define GLCD_BACKLIGHT_STEP 50   // How much each press increases/decreases
#define PWM_TIMER htim3
#define PWM_CHANNEL TIM_CHANNEL_1

extern volatile uint16_t glcdBacklightLevel;
extern volatile uint16_t score;
extern TIM_HandleTypeDef PWM_TIMER;
extern UART_HandleTypeDef huart2;

extern osThreadId_t createMyTasksInitTaskHandle;
extern osThreadId_t uartCommunicationTaskHandle;
extern osThreadId_t gameLogicTaskHandle;
extern osThreadId_t glcdUpdateTaskHandle;
extern osThreadId_t glcdBacklightControlTaskHandle;
extern osThreadId_t glcdBacklightPlusTaskHandle;
extern osThreadId_t glcdBacklightMinusTaskHandle;
extern osThreadId_t resetTaskHandle;

extern const osThreadAttr_t createMyTasksInitTask_attributes;
extern const osThreadAttr_t toggleLedOnButtonPushTask_attributes;
extern const osThreadAttr_t uartCommunicationTask_attributes;
extern const osThreadAttr_t gameLogicTask_attributes;
extern const osThreadAttr_t glcdUpdateTask_attributes;
extern const osThreadAttr_t glcdBacklightControlTask_attributes;
extern const osThreadAttr_t glcdBacklightPlusTask_attributes;
extern const osThreadAttr_t glcdBacklightMinusTask_attributes;
extern const osThreadAttr_t resetTask_attributes;

void CreateMyTasksInitTask(void *argument);
void UartCommunicationTask(void *argument);
void GameLogicTask(void *argument);
void GlcdUpdateTask(void *argument);
void GlcdBacklightControlTask(void *argument);
void GlcdBacklightPlusTask(void *argument);
void GlcdBacklightMinusTask(void *argument);
void ResetTask(void *argument);

#endif /* INC_MYTASKS_H_ */
