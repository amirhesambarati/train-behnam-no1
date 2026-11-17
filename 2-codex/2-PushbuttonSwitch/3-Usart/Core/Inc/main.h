/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define digital_out_9_Pin GPIO_PIN_9
#define digital_out_9_GPIO_Port GPIOD
#define digital_out_10_Pin GPIO_PIN_10
#define digital_out_10_GPIO_Port GPIOD
#define digital_out_11_Pin GPIO_PIN_11
#define digital_out_11_GPIO_Port GPIOD
#define digital_out_12_Pin GPIO_PIN_12
#define digital_out_12_GPIO_Port GPIOD
#define digital_out_8_Pin GPIO_PIN_13
#define digital_out_8_GPIO_Port GPIOD
#define digital_out_7_Pin GPIO_PIN_14
#define digital_out_7_GPIO_Port GPIOD
#define digital_out_6_Pin GPIO_PIN_15
#define digital_out_6_GPIO_Port GPIOD
#define digital_out_5_Pin GPIO_PIN_6
#define digital_out_5_GPIO_Port GPIOC
#define digital_out_4_Pin GPIO_PIN_7
#define digital_out_4_GPIO_Port GPIOC
#define digital_out_3_Pin GPIO_PIN_8
#define digital_out_3_GPIO_Port GPIOC
#define digital_out_2_Pin GPIO_PIN_9
#define digital_out_2_GPIO_Port GPIOC
#define digital_out_1_Pin GPIO_PIN_8
#define digital_out_1_GPIO_Port GPIOA
#define mode_Pin GPIO_PIN_7
#define mode_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
