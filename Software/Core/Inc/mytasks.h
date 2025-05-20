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

extern const osThreadAttr_t createMyTasksInitTask_attributes;
extern const osThreadAttr_t toggleLedOnButtonPushTask_attributes;

void ToggleLedOnButtonPushTask(void *argument);
void CreateMyTasksInitTask(void *argument);

#endif /* INC_MYTASKS_H_ */
