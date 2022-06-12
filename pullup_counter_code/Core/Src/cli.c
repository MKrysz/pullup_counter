
#include "cli.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "usart.h"
#include "gpio.h"
#include "adc.h"
#include "eeprom.h"
#include "entry.h"
#include "rtc.h"

#define NR_OF_STR 2
#define BATTERY 0
#define DISTANCE 1

#define xstr(s) str(s)
#define str(s) #s

const char welcomeMsg[] = 
    "Pull-up counter V02\n\r"
    "Type help for list of commands\n\r";

const char helpMsg[] = 
    "for specific help type help X, where X can be:\n\r"
    "'flash', 'eeprom', 'adc', 'rtc'\n\r";

const char helpFlashMsg[] = 
    "flash read X - reads entry X written in flash\n\r"
    "flash last - reads last written entry\n\r"
    "flash dump - reads all entries from flash in chronological order\n\r"
    "flash raw - reads all entries from flash in machine-friendly format:\n\r"
    "\t id hour min month date weekday, separated by tabs\n\r";

const char helpEepromMsg[] = 
    "Address map:\n\r" 
    str(EEPROM_VAR_LAST_DDR) " - " xstr(EEPROM_VAR_LAST_DDR)"\n\r"
    str(EEPROM_VAR_PULLUP_COUNTER) " - " xstr(EEPROM_VAR_PULLUP_COUNTER) "\n\r"
    "eeprom read X - reads data from EEPROM in address X\n\r"
    "eeprom write X Y - writes Y to EEPROM in address X\n\r";

const char defaultMsg[] = 
    "Incorrect command \n\rType 'help' for list of available functions\n\r";

const char helpAdcMsg[] = 
    "adc battery - reads voltage value of the battery\n\r"
    "adc read X - reads distance value for X ms, type 0 to measure once\n\r";

const char helpRtcMsg[] = 
    "pass -1 for 'no changes'"
    "rtc read - reads value from the RTC\n\r"
    "rtc date X Y - writes X day of the Y month to the RTC\n\r"
    "rtc time X Y - writes X hour Y min to the RTC\n\r"
    "rtc weekday X - writes X weekday, where Monday is 1 and Sunday is 7, to the RTC\n\r";

void flashRead(uint32_t ddr);
void flashLast();
void flashDump();
void flashRaw();

void eepromRead(uint32_t ddr);
void eepromWrite(uint32_t ddr, uint32_t data);

void adcBattery();
void adcRead(uint32_t repeat);

void rtcRead();
void rtcDate(int64_t date, int64_t month);
void rtcTime(int64_t hour, int64_t minutes);
void rtcWeekday(int64_t weekday);




typedef struct cli_args_struct
{
    char str[NR_OF_STR][16];
    long long int integer;
}cli_args_t;

void _CLI_ClearArgs(cli_args_t * cli_args)
{
    for (size_t i = 0; i < NR_OF_STR; i++)
    {
        cli_args->str[i][0] = '\0';
    }
    cli_args->integer = 0;
    
}

void _CLI_ReadArgs(cli_args_t * cli_args, char* userInput)
{
    sscanf(userInput, "%s %s %lli", cli_args->str[0], cli_args->str[1], &(cli_args->integer));
}

/**
 * @brief takes instructions from user and executes them as long as USB is connected
 * 
 */
void CLI_UserInterface()
{
  const size_t bufferSize = 32;
  char instructionBuffer[bufferSize];

  printf(welcomeMsg);

  while(GPIO_GetUsbFlag()){
    
    for (size_t i = 0; i < bufferSize; i++){
        instructionBuffer[i] = 0;
    }

    HAL_UART_Receive(&huart2, instructionBuffer, bufferSize, 1000);
    if(instructionBuffer[0] == 0)
        continue;
    

    printf("echo %s\n\r", instructionBuffer);

    // parse user input
    cli_args_t args;
    _CLI_ClearArgs(&args);
    _CLI_ReadArgs(&args, instructionBuffer);
    printf("arg.str[0] = %s\n\r", args.str[0]);
    printf("arg.str[1] = %s\n\r", args.str[1]);
    printf("arg.integer = %lli\n\r", args.integer);
    
    if(strcmp(args.str[0], "help") == 0){
        if(strcmp(args.str[1], "adc") == 0){
            printf(helpAdcMsg);
        }
        else if(strcmp(args.str[1], "rtc") == 0){
            printf(helpRtcMsg);
        }
        else if(strcmp(args.str[1], "flash") == 0){
            printf(helpFlashMsg);
        }
        else if(strcmp(args.str[1], "eeprom") == 0){
            printf(helpEepromMsg);
        }
        else{
            printf(helpMsg);
        }
    }
    else if(strcmp(args.str[0], "flash") == 0){
        //TODO flash-based instructions
    }
    else if(strcmp(args.str[0], "eeprom") == 0){
        //TODO eeprom-based functions
    }
    //if didn't detect correct command
    else{
        printf(defaultMsg);
    }
    printf("OK\n\r");
  }
}



void flashRead(uint32_t ddr)
{
    entry_t entry;
    ENTRY_Read(&entry, ddr);
    ENTRY_Print(&entry);
    printf("\n\r");
}

void flashLast()
{
    uint32_t lastDdr = EEPROM_ReadUINT32(EEPROM_VAR_LAST_DDR);
    flashRead(lastDdr);
}

void flashDump()
{
    uint32_t lastDdr = EEPROM_ReadUINT32(EEPROM_VAR_LAST_DDR);
    for (uint32_t ddr = 0; ddr <= lastDdr; ddr++)
    {
        flashRead(ddr);
    }
    
}

void flashRaw()
{
    printf("Id\tHour\tMin\tMonth\tDate\tWeekday\n\r");
    uint32_t lastDdr = EEPROM_ReadUINT32(EEPROM_VAR_LAST_DDR);
    for (uint32_t ddr = 0; ddr <= lastDdr; ddr++){
        entry_t entry;
        ENTRY_Read(&entry, ddr);
        printf("%hu\t%hhu\t%hhu\t%hhu\t%hhu\t%hhu\n\r", 
        entry.id_, entry.hour_, entry.minutes_, entry.month_,
        entry.date_, entry.weekday_);
    }
}


void eepromRead(uint32_t ddr)
{
    uint32_t data = EEPROM_ReadUINT32(ddr);
    printf("%lu\r\n", data);
}

void eepromWrite(uint32_t ddr, uint32_t data)
{
    EEPROM_WriteUINT32(ddr, data);
    printf("Written %lu to an address %lu\n\r", data, ddr);
}

void adcBattery()
{
    float batVol = ADC_MeasureBattery()*100;
    printf("%i,%i%iV\r\n", (int)batVol/100, (int)batVol%100);
}

void adcRead(uint32_t repeat){
    uint32_t start = HAL_GetTick();
    do{
        uint32_t data = ADC_MeasureDistance();
        printf("%lu mm\r\n", data);
    }
    while(HAL_GetTick()-repeat < start);
}

void rtcRead()
{
    RTC_Print();
}

void rtcDate(int64_t date, int64_t month)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, FORMAT_BIN);

    if(date >= 0)
        sDate.Date = date;
    if(month >= 0)
        sDate.Month = month;

    HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BIN);
}

void rtcTime(int64_t hour, int64_t minutes)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, FORMAT_BIN);

    if(hour >= 0)
        sTime.Hours = hour;
    if(minutes >= 0)
        sTime.Minutes = minutes;

    HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BIN);

}

void rtcWeekday(int64_t weekday)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, FORMAT_BIN);
    
    if(weekday >= 0)
        sDate.WeekDay = weekday;

    HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BIN);
}