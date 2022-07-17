/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
uint32_t ambientDistance = 0;
/* USER CODE END 0 */

ADC_HandleTypeDef hadc;

/* ADC init function */
void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC GPIO Configuration
    PA0     ------> ADC_IN0
    PB1     ------> ADC_IN9
    */
    GPIO_InitStruct.Pin = DISTANCE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DISTANCE_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BAT_SENSE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BAT_SENSE_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC GPIO Configuration
    PA0     ------> ADC_IN0
    PB1     ------> ADC_IN9
    */
    HAL_GPIO_DeInit(DISTANCE_GPIO_Port, DISTANCE_Pin);

    HAL_GPIO_DeInit(BAT_SENSE_GPIO_Port, BAT_SENSE_Pin);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief returns raw abolute reading from proximity sensor
 * 
 * @return uint32_t 
 */
uint32_t _ADC_DistanceRaw()
{
  HAL_GPIO_WritePin(DISTANCE_EN_GPIO_Port, DISTANCE_EN_Pin, 1);
  HAL_Delay(10);
  HAL_ADC_Start(&hadc);
  while(HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY) != HAL_OK);
  uint32_t raw = HAL_ADC_GetValue(&hadc);
  HAL_GPIO_WritePin(DISTANCE_EN_GPIO_Port, DISTANCE_EN_Pin, 0);
  return raw;
}

/**
 * @brief returns relative reading from proximity sensor
 * 
 * @return uint32_t differnce between ambient and current voltage
 */
int32_t ADC_MeasureDistance()
{
  uint32_t raw = _ADC_DistanceRaw();
  return (int32_t) (raw - ambientDistance);
}

/**
 * @brief recalibrates ambient value
 * 
 */
void ADC_Distance_Calibrate()
{
  const size_t bufferSize = 10;
  uint32_t result = 0;
  for (size_t i = 0; i < bufferSize; i++){
    result += _ADC_DistanceRaw();
  }
  ambientDistance = result/bufferSize;
}

/**
 * @brief 
 * 
 * @return uint32_t battery voltage in V
 */
uint32_t ADC_MeasureBattery()
{
  // TODO implement multichannel ADC measurements 
  // //TODO: calculate multiplier
  // const float multiplier = 0.1F;
  // HAL_GPIO_WritePin(BAT_SENSE_EN_GPIO_Port, BAT_SENSE_EN_Pin, 1);
  // HAL_ADC_Start(&hadc);
  // HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
  // uint16_t raw = HAL_ADC_GetValue(&hadc);
  // return (float)(raw) * multiplier;
  return 3000;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
