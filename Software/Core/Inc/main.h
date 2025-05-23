/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GLCD_DB1_Pin GPIO_PIN_0
#define GLCD_DB1_GPIO_Port GPIOA
#define GLCD_DB3_Pin GPIO_PIN_1
#define GLCD_DB3_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define GLCD_DB5_Pin GPIO_PIN_4
#define GLCD_DB5_GPIO_Port GPIOA
#define GLCD_CS1_Pin GPIO_PIN_5
#define GLCD_CS1_GPIO_Port GPIOA
#define GLCD_CS2_Pin GPIO_PIN_6
#define GLCD_CS2_GPIO_Port GPIOA
#define GLCD_RESET_Pin GPIO_PIN_7
#define GLCD_RESET_GPIO_Port GPIOA
#define GLCD_DB7_Pin GPIO_PIN_0
#define GLCD_DB7_GPIO_Port GPIOB
#define GLCDEN_Pin GPIO_PIN_1
#define GLCDEN_GPIO_Port GPIOB
#define GLCD_DI_Pin GPIO_PIN_2
#define GLCD_DI_GPIO_Port GPIOB
#define GLCD_DB2_Pin GPIO_PIN_13
#define GLCD_DB2_GPIO_Port GPIOB
#define GLCD_DB6_Pin GPIO_PIN_14
#define GLCD_DB6_GPIO_Port GPIOB
#define GLCD_DB4_Pin GPIO_PIN_15
#define GLCD_DB4_GPIO_Port GPIOB
#define Place_Pin GPIO_PIN_8
#define Place_GPIO_Port GPIOA
#define Place_EXTI_IRQn EXTI9_5_IRQn
#define LightPlus_Pin GPIO_PIN_9
#define LightPlus_GPIO_Port GPIOA
#define LightPlus_EXTI_IRQn EXTI9_5_IRQn
#define LightMinus_Pin GPIO_PIN_10
#define LightMinus_GPIO_Port GPIOA
#define LightMinus_EXTI_IRQn EXTI15_10_IRQn
#define GLCD_E_Pin GPIO_PIN_11
#define GLCD_E_GPIO_Port GPIOA
#define GLCD_DB0_Pin GPIO_PIN_12
#define GLCD_DB0_GPIO_Port GPIOA
#define GLCD_RW_Pin GPIO_PIN_15
#define GLCD_RW_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define Reset_Pin GPIO_PIN_5
#define Reset_GPIO_Port GPIOB
#define Reset_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
