/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "app_fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "display.h"
#include "cli.h"
#include "entry.h"
#include "settings.h"
#include "config.h"
#include "flash.h"
#include "w25qxx.h"
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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void programmingRoutine();
uint32_t measurePullupTime();
void SD_Update();
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART3_UART_Init();
  if (MX_FATFS_Init() != APP_OK) {
    Error_Handler();
  }
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  ADC_Init();
  FLASH_Init();
  DISPLAY_Init();
  HAL_TIM_Base_Start(&DISPLAY_HTIM);

  if(GetUsbFlag()){
    Display_SetMode(DispUSB);
    CLI_StartUserInterface();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  FLASH_SettingsRead(&settings);
  FLASH_VarsRead(&eepromVars);
  
  #ifdef PROGRAM_ROUTINE
  programmingRoutine();
  #endif

  // check battery voltage and warn of low voltage
  if(ADC_MeasureBattery() < settings.batteryVoltageThreshold){
    Display_SetMode(DispLowVoltage);
    HAL_Delay(3000);
  }


  ADC_DistanceCalibrate();

  uint8_t currentCount = 0;
  RTC_ReadTime();
  entry_t lastEntry;
  FLASH_EntryRead(&lastEntry, eepromVars.lastDdr);
  if(lastEntry.day != sDate.Date || lastEntry.month != sDate.Month || lastEntry.year != sDate.Year){
      eepromVars.pullup_counter = 0;
  }
  Display_SetMode(DispCounting);

  // count pull-ups loop
  uint32_t lastDetectedPullup = HAL_GetTick(); 
  while(HAL_GetTick()-lastDetectedPullup < settings.timeTillShutdown){
    if(flags.SD_Update)
      break;
    uint32_t timeDelta = measurePullupTime(); 
    if(settings.pullupTimeMin < timeDelta && timeDelta < settings.pullupTimeMax){
      lastDetectedPullup = HAL_GetTick();
      currentCount++;
      eepromVars.pullup_counter++;
    } 
  }
  
  if(currentCount != 0){

    uint32_t id = eepromVars.lastDdr;
    entry_t entry;
    entry.id = id;
    entry.count = currentCount;

    entry.year = sDate.Year;
    entry.month = sDate.Month;
    entry.day = sDate.Date;
    
    entry.hour = sTime.Hours;
    entry.minute = sTime.Minutes;
    eepromVars.lastDdr++;

    FLASH_EntryWrite(&entry, eepromVars.lastDdr);
    entry_t entryRead;
    FLASH_EntryRead(&entryRead, eepromVars.lastDdr);
    if(!ENTRY_IsEqual(&entry, &entryRead)){
      Display_SetMode(DispError);
    }
  }

  FLASH_VarsWrite(&eepromVars);
  FLASH_SettingsWrite(&settings);

  if(flags.SD_Update){
    Display_SetMode(DispSD);
    SD_Update();
  }

  #ifdef SLEEP_ENABLE
  HAL_SuspendTick();
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  #endif

  #ifdef SHUTDOWN_ENABLE
  HAL_GPIO_WritePin(PWR_EN_GPIO_Port, PWR_EN_Pin, 0);
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void programmingRoutine()
{
  #ifdef SET_RTC
    RTC_SetCurrentTime();
  #endif

  #ifdef ERASE_FLASH
    W25qxx_EraseChip();
  #endif

  #ifdef RESET_SETTINGS
    settings.distanceThreshold = -250;
    settings.pullupTimeMax = 2000;
    settings.pullupTimeMin = 300;
    settings.timeTillShutdown = 7000;
    settings.batteryVoltageThreshold = 2650;
  #endif

  #ifdef RESET_EEPROM
    eepromVars.lastDdr = 0;
    eepromVars.pullup_counter = 0;
  #endif

}

uint32_t measurePullupTime()
{
  uint32_t pullupStart = HAL_GetTick();
  while(ADC_MeasureDistance() < settings.distanceThreshold);
  uint32_t pullupEnd = HAL_GetTick();
  uint32_t timeDelta = pullupEnd - pullupStart;
  return timeDelta;
}

void SD_Update()
{
  //TODO: finish me
  FATFS FatFs;  //Fatfs handle
  FIL fil;  //File handle
  FRESULT fres; //Result after operations  
  uint32_t lastSavedId;
  const char fileName[] = "pullup_data.csv";
  char buffer[128];
  unsigned int br; // number of bytes actualy read from f_read()

  fres = f_mount(&FatFs, "", 1); //1=mount now

  fres = f_open(&fil, fileName, FA_READ);
  switch (fres)
  {
  case FR_OK:
    while(1){ //seek last entry
      f_gets(buffer, 128, &fil);
      if(f_eof(&fil)){
        break;
      }
      entry_t entry;
      ENTRY_CreateFromString(&entry, buffer);
      lastSavedId = entry.id;
    }
    break;
  case FR_NO_FILE:
    lastSavedId = 0;
    break;
  default:
    break;
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
