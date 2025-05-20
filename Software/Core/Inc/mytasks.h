/*
 * freertos.h
 *
 *  Created on: May 20, 2025
 *      Author: krisz
 */

#ifndef INC_MYTASKS_H_
#define INC_MYTASKS_H_
#pragma once

#include "cmsis_os.h"

extern osThreadId_t createMyTasksInitTaskHandle;
extern osThreadId_t toggleLedOnButtonPushTaskHandle;
extern osThreadId_t uartCommunicationTaskHandle;

extern const osThreadAttr_t createMyTasksInitTask_attributes;
extern const osThreadAttr_t toggleLedOnButtonPushTask_attributes;
extern const osThreadAttr_t uartCommunicationTask_attributes;

void ToggleLedOnButtonPushTask(void *argument);
void CreateMyTasksInitTask(void *argument);
void UartCommunicationTask(void *argument);

#endif /* INC_MYTASKS_H_ */
