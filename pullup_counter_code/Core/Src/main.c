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
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>

#include "display.h"
#include "eeprom.h"
#include "cli.h"
#include "entry.h"
#include "w25qxx.h"
#include "settings.h"
#include "config.h"
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
settings_t settings = {0};

extern volatile bool GPIO_StartFlag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void programmingRoutine();
bool isTheNextDay(uint8_t day, uint8_t month, uint8_t nextDay, uint8_t nextDaysMonth);
uint32_t measurePullupTime();
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
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_ADC_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  W25qxx_Init();
  // programmingRoutine();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  Settings_Read(settings);

  // user interface
  if(GPIO_GetUsbFlag())
    CLI_UserInterface();

  // Check battery voltage
  // if low voltage detected inform user via 7-seg display
  if(ADC_MeasureBattery() < settings.batteryVoltageThreshold){
    Display_enable();
    for (size_t i = 0; i < 3; i++)
    {
      Display_ShowString("LOW BATTERY", 250);
      HAL_Delay(1000);
    }
    Display_disable();
  }


  ADC_Distance_Calibrate();

  // initialize variables
  uint32_t pullupCounter = EEPROM_ReadUINT32(EEPROM_VAR_PULLUP_COUNTER);
  Display_enable();
  uint8_t pullupCount = 0;
  RTC_DateTypeDef sDate = {0};
  RTC_TimeTypeDef sTime = {0};
  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
  entry_t lastEntry;
  ENTRY_Read(&lastEntry, EEPROM_ReadUINT32(EEPROM_VAR_LAST_DDR));
  if(lastEntry.day != sDate.Date || lastEntry.month != sDate.Month || lastEntry.year != sDate.Year){
      pullupCounter = 0;
  }
  Display_setInt(pullupCounter);

  // count pull-ups loop
  uint32_t lastDetectedPullup = HAL_GetTick(); 
  while(HAL_GetTick()-lastDetectedPullup < settings.timeTillShutdown){
    uint32_t timeDelta = measurePullupTime(); 
    if(settings.pullupTimeMin < timeDelta && timeDelta < settings.pullupTimeMax){
      lastDetectedPullup = HAL_GetTick();
      pullupCount++;
      Display_setInt(pullupCounter+pullupCount);
    } 
  }

  // save entry
  EEPROM_WriteUINT32(EEPROM_VAR_PULLUP_COUNTER, pullupCounter+pullupCount);
  if(pullupCount != 0){

    uint32_t id = EEPROM_ReadUINT32(EEPROM_VAR_NEXT_ID);
    entry_t entry;
    entry.id = id;
    entry.count = pullupCount;

    entry.year = sDate.Year;
    entry.month = sDate.Month;
    entry.day = sDate.Date;
    
    entry.hour = sTime.Hours;
    entry.minute = sTime.Minutes;
    EEPROM_WriteUINT32(EEPROM_VAR_NEXT_ID, ++id);

    if(!ENTRY_WriteWithCheck(&entry)){
      while(1){
        Display_ShowString("FLASH ERROR", 250);
        HAL_Delay(1000);
      }
    }
  }

  //shutdown routine
  //going into STOP mode with RTC
  Display_disable();
  #if SLEEP_ENABLE
  HAL_SuspendTick();
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  #endif

  
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  #if TOUCHPAD_ENABLE
  SystemClock_Config();
  HAL_ResumeTick();
  HAL_NVIC_SystemReset();
  #endif
}

void programmingRoutine()
{
  // // RTC time&date setting
  // {
  //   RTC_DateTypeDef sDate;
  //   sDate.Date = 23;
  //   sDate.Month = 6;
  //   sDate.Year = 22;
  //   sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  //   HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BIN);

  //   RTC_TimeTypeDef sTime;
  //   sTime.Hours = 21;
  //   sTime.Minutes = 22;
  //   sTime.Seconds = 0;
  //   HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);
  // }

  // // erase the entire flash memory
  // W25qxx_EraseChip();

  // // erase the entire eeprom 
  // EEPROM_Erase();

  // set the default settings
  settings.distanceThreshold = -250;
  settings.pullupTimeMax = 2000;
  settings.pullupTimeMin = 300;
  settings.timeTillShutdown = 7000;
  settings.batteryVoltageThreshold = 2650;
  Settings_Save(settings);

}

bool isTheNextDay(uint8_t day, uint8_t month, uint8_t nextDay, uint8_t nextDaysMonth)
{
    uint8_t lenOfMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(month == nextDaysMonth && nextDay == day+1)
        return true;
    else if(nextDaysMonth == month+1 && nextDay == 1 && day == lenOfMonths[month-1])
        return true;
    return false;
}

uint32_t measurePullupTime()
{
  uint32_t pullupStart = HAL_GetTick();
  while(ADC_MeasureDistance() < settings.distanceThreshold);
  uint32_t pullupEnd = HAL_GetTick();
  uint32_t timeDelta = pullupEnd - pullupStart;
  return timeDelta;
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
