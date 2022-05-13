/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_hal.h"

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
void sleepRoutine();
void wakeUpRoutine();
void programmingRoutine();

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BAT_SENSE_Pin GPIO_PIN_0
#define BAT_SENSE_GPIO_Port GPIOA
#define BAT_SENSE_EN_Pin GPIO_PIN_1
#define BAT_SENSE_EN_GPIO_Port GPIOA
#define NCE_Pin GPIO_PIN_4
#define NCE_GPIO_Port GPIOA
#define dis_sel_1_Pin GPIO_PIN_12
#define dis_sel_1_GPIO_Port GPIOB
#define dis_sel_0_Pin GPIO_PIN_13
#define dis_sel_0_GPIO_Port GPIOB
#define dis_f_Pin GPIO_PIN_14
#define dis_f_GPIO_Port GPIOB
#define dis_a_Pin GPIO_PIN_15
#define dis_a_GPIO_Port GPIOB
#define dis_b_Pin GPIO_PIN_8
#define dis_b_GPIO_Port GPIOA
#define dis_c_Pin GPIO_PIN_9
#define dis_c_GPIO_Port GPIOA
#define dis_e_Pin GPIO_PIN_10
#define dis_e_GPIO_Port GPIOA
#define dis_d_Pin GPIO_PIN_11
#define dis_d_GPIO_Port GPIOA
#define dis_g_Pin GPIO_PIN_12
#define dis_g_GPIO_Port GPIOA
#define DIST_OUTD_Pin GPIO_PIN_15
#define DIST_OUTD_GPIO_Port GPIOA
#define DIST_OUTD_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */
#define DEBUG_MAIN 0
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
