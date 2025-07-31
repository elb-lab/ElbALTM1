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
#include "stm32c0xx_hal.h"

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
#define SCL1_Pin GPIO_PIN_7
#define SCL1_GPIO_Port GPIOB
#define SDA1_Pin GPIO_PIN_14
#define SDA1_GPIO_Port GPIOC
#define BEEP_Pin GPIO_PIN_15
#define BEEP_GPIO_Port GPIOC
#define BUTT2_Pin GPIO_PIN_0
#define BUTT2_GPIO_Port GPIOA
#define SCLK_Pin GPIO_PIN_1
#define SCLK_GPIO_Port GPIOA
#define MOSI_Pin GPIO_PIN_2
#define MOSI_GPIO_Port GPIOA
#define STBATT_Pin GPIO_PIN_3
#define STBATT_GPIO_Port GPIOA
#define BUTT3_Pin GPIO_PIN_4
#define BUTT3_GPIO_Port GPIOA
#define BUTT1_Pin GPIO_PIN_5
#define BUTT1_GPIO_Port GPIOA
#define MISO_Pin GPIO_PIN_6
#define MISO_GPIO_Port GPIOA
#define SCL2_Pin GPIO_PIN_7
#define SCL2_GPIO_Port GPIOA
#define CSEL_Pin GPIO_PIN_8
#define CSEL_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SDA2_Pin GPIO_PIN_4
#define SDA2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
