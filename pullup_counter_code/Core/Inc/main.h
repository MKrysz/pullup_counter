/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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
#define OLED_RST_Pin GPIO_PIN_1
#define OLED_RST_GPIO_Port GPIOC
#define PWR_EN_Pin GPIO_PIN_2
#define PWR_EN_GPIO_Port GPIOC
#define PROX_EN_Pin GPIO_PIN_3
#define PROX_EN_GPIO_Port GPIOC
#define PROX_OUT_Pin GPIO_PIN_0
#define PROX_OUT_GPIO_Port GPIOA
#define BAT_RAW_Pin GPIO_PIN_1
#define BAT_RAW_GPIO_Port GPIOA
#define UsrBtn3_Pin GPIO_PIN_1
#define UsrBtn3_GPIO_Port GPIOD
#define UsrBtn2_Pin GPIO_PIN_2
#define UsrBtn2_GPIO_Port GPIOD
#define UsrBtn1_Pin GPIO_PIN_3
#define UsrBtn1_GPIO_Port GPIOD
#define UsrBtn0_Pin GPIO_PIN_4
#define UsrBtn0_GPIO_Port GPIOD
#define SD_CS_Pin GPIO_PIN_5
#define SD_CS_GPIO_Port GPIOD
#define MEM_CE_Pin GPIO_PIN_5
#define MEM_CE_GPIO_Port GPIOB
#define USB_FLAG_Pin GPIO_PIN_9
#define USB_FLAG_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
