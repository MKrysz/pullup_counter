/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w25qxx.h"
#include <stdbool.h>
#include "display.h"
#include "entry.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern volatile bool distanceFlag;
extern volatile bool shutdownFlag;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void myErrorHandler();
void updateCounter(uint8_t date, uint8_t month);
void batteryCheck();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_ADC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint32_t timeStart, timeDelta;
  timeStart = HAL_GetTick();
  while(!W25qxx_Init());
  // programmingRoutine(); 
  wakeUpRoutine();
  Display_setInt(0); 

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  const uint32_t pullupTimeMin = 250;
  const uint32_t pullupTimeMax = 1000;
  printf("start\n");
  while (1)
  {
    //TODO: add battery check
    //TODO: add usb interface

    if (distanceFlag)
    {
      timeStart = HAL_GetTick();
      while ((!HAL_GPIO_ReadPin(DIST_OUTD_GPIO_Port, DIST_OUTD_Pin)) && ((HAL_GetTick() - timeStart) < pullupTimeMax));
      distanceFlag = false;
      timeDelta = HAL_GetTick() - timeStart;
      if (timeDelta < pullupTimeMin)
      {
      }
      else if (timeDelta < pullupTimeMax)
      {
        entry_t entry;
        entry_create(&entry);
        entry_save(&entry);
        if (!entry_verifyLast(&entry))
          myErrorHandler();
        updateCounter(entry.date_, entry.month_);
      }
    }

    if (shutdownFlag)
    {
      shutdownFlag = false;
      sleepRoutine();
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
 * @brief this function should only be used when programming !call W25qxx_Init() first!
 * 
 */
void programmingRoutine()
{
  // rtc_setTime();
  W25qxx_EraseChip();
  // eeprom_eraseVariables();
}


/**
 * @brief routine for putting MCU to low power mode
 * 
 */
void sleepRoutine()
{
  Display_disable();
  // printf("entering stop mode\n");

  HAL_SuspendTick();
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}


/**
 * @brief routine for waking MCU up
 * 
 */
void wakeUpRoutine()
{
  SystemClock_Config();
  HAL_ResumeTick();
  // printf("wake up\n");

  Display_enable();
  tim_shutdownCountdown();
}

/**
 * @brief updates counter in eeprom and displays current value
 * 
 * @param date current date
 * @param month current month
 */
void updateCounter(uint8_t date, uint8_t month)
{
}

/**
 * @brief function informing that an error has happened
 * 
 */
void myErrorHandler()
{
  Display_setCharToDigit(dis0, '-');
  Display_setCharToDigit(dis1, '-');
  Display_enable();
  while(1);
}

int _write(int fd, char* ptr, int len) {
  HAL_StatusTypeDef hstatus;

  hstatus = HAL_UART_Transmit(&huart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
  if (hstatus == HAL_OK)
    return len;
  return -1;
}

void batteryCheck()
{
  //TODO: calculate thr
  const float thr = 3.4F;
  const char message[] = "BATTERY LOW";
  const size_t msgSize = 12;

  float voltage = ADC_MeasureBatteryVoltage();
  if(voltage < thr){
    for(size_t j = 0; j<3; j++)
    {
      for (size_t i = 1; i < msgSize; i++)
      {
        Display_setCharToDigit(0, message[i]);
        Display_setCharToDigit(1, message[i-1]);
        HAL_Delay(500);
      }
    }
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
