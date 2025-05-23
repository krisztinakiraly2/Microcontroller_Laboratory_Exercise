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

extern osThreadId_t createMyTasksInitTaskHandle;
extern osThreadId_t uartCommunicationTaskHandle;
extern osThreadId_t gameLogicTaskHandle;
extern osThreadId_t glcdUpdateTaskHandle;
extern osThreadId_t backlightControlTaskHandle;
extern osThreadId_t resetTaskHandle;

extern const osThreadAttr_t createMyTasksInitTask_attributes;
extern const osThreadAttr_t toggleLedOnButtonPushTask_attributes;
extern const osThreadAttr_t uartCommunicationTask_attributes;
extern const osThreadAttr_t gameLogicTask_attributes;
extern const osThreadAttr_t glcdUpdateTask_attributes;
extern const osThreadAttr_t backlightControlTask_attributes;
extern const osThreadAttr_t resetTask_attributes;

void CreateMyTasksInitTask(void *argument);
void UartCommunicationTask(void *argument);
void GameLogicTask(void *argument);
void GlcdUpdateTask(void *argument);
void BacklightControlTask(void *argument);
void ResetTask(void *argument);

#endif /* INC_MYTASKS_H_ */
