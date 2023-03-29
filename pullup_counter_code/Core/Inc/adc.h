/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
// Vcc in mV
#define ADC_VCC 3300

// 
#define ADC_RES 12

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
void ADC_Init();
uint32_t ADC_MeasureBattery();
uint32_t ADC_MeasureBackupBattery();
int32_t ADC_MeasureDistance();
void ADC_DistanceCalibrate();
void ADC_PROX_ON();
void ADC_PROX_OFF();

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

