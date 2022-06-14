
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

#define CLI_DEBUG 1

// nr of strings in command
#define NR_OF_STR 2
// strings size
#define STR_SIZE 16
//nr of int arguments in command
#define NR_OF_INT 2

//macros for making strings out of variables

// returns string with variables value
#define xstr(s) str(s)
// returns string with varaibles name
#define str(s) #s

const char welcomeMsg[] = 
    "Pull-up counter V02\n"
    "Type help for list of commands\n";

const char helpMsg[] = 
    "for specific help type help X, where X can be:\n"
    "'flash', 'eeprom', 'adc', 'rtc', 'display'\n";

const char helpFlashMsg[] = 
    "flash read X - reads entry X written in flash\n"
    "flash last - reads last written entry\n"
    "flash dump - reads all entries from flash in chronological order\n"
    "flash raw - reads all entries from flash in machine-friendly format:\n"
    "\t id hour min month date weekday, separated by tabs\n";

const char helpEepromMsg[] = 
    "Address map:\n" 
    str(EEPROM_VAR_LAST_DDR) " - " xstr(EEPROM_VAR_LAST_DDR)"\n"
    str(EEPROM_VAR_PULLUP_COUNTER) " - " xstr(EEPROM_VAR_PULLUP_COUNTER) "\n"
    "eeprom read X - reads data from EEPROM in address X\n"
    "eeprom write X Y - writes Y to EEPROM in address X\n";

const char defaultMsg[] = 
    "Incorrect command \nType 'help' for list of available functions\n";

const char helpAdcMsg[] = 
    "adc battery - reads voltage value of the battery\n"
    "adc read X - reads distance value for X ms, type 0 to measure once\n";

const char helpRtcMsg[] = 
    "pass -1 for 'no changes\n'"
    "rtc read - reads value from the RTC\n"
    "rtc date X Y - writes X day of the Y month to the RTC\n"
    "rtc time X Y - writes X hour Y min to the RTC\n"
    "rtc weekday X - writes X weekday, where Monday is 1 and Sunday is 7, to the RTC\n";

void flashRead(uint32_t ddr);
void flashLast();
void flashDump();
void flashRaw();

void eepromRead(uint32_t ddr);
void eepromWrite(uint32_t ddr, uint32_t data);

void adcBattery();
void adcRead(uint32_t repeat);

void rtcRead();
void rtcDate(int32_t date, int32_t month);
void rtcTime(int32_t hour, int32_t minutes);
void rtcWeekday(int32_t weekday);





typedef struct cli_args_struct
{
    char str[NR_OF_STR][STR_SIZE];
    long int integer[NR_OF_INT];
}cli_args_t;

void _CLI_ClearArgs(cli_args_t * cli_args)
{
    for (size_t i = 0; i < NR_OF_STR; i++)
    {
        for (size_t j = 0; j < STR_SIZE; j++)
        {
            cli_args->str[i][j] = '\0';
        }
    }
    for (size_t i = 0; i < 2; i++)
    {
        cli_args->integer[i] = 0;
    }
    
}

void _CLI_ReadArgs(cli_args_t* cli_args, char* userInput)
{
    sscanf(userInput, "%s %s %li %li", cli_args->str[0], cli_args->str[1], 
        &(cli_args->integer[0]), &(cli_args->integer[1]));
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

    HAL_UART_Receive(&huart2, instructionBuffer, bufferSize, 500);
    // if nothing received contine
    if(instructionBuffer[0] == 0)
        continue;
    
    #if CLI_DEBUG
    printf("echo %s\n", instructionBuffer);
    #endif

    // parse user input
    cli_args_t args;
    _CLI_ClearArgs(&args);
    _CLI_ReadArgs(&args, instructionBuffer);
    #if CLI_DEBUG
    printf("arg.str[0] = %s\n", args.str[0]);
    printf("arg.str[1] = %s\n", args.str[1]);
    printf("arg.integer[0] = %li\n", args.integer[0]);
    printf("arg.integer[1] = %li\n", args.integer[1]);
    #endif
    
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
        if(strcmp(args.str[1], "read") == 0){
            flashRead(args.integer[0]);
        }
        else if(strcmp(args.str[1], "last") == 0){
            flashLast();
        }
        else if(strcmp(args.str[1], "dump") == 0){
            flashDump();
        }
        else if(strcmp(args.str[1], "raw") == 0){
            flashRaw();
        }
    }
    else if(strcmp(args.str[0], "eeprom") == 0){
        if(strcmp(args.str[1], "read") == 0){
            eepromRead(args.integer[0]);
        }
        else if(strcmp(args.str[1], "write") == 0){
            eepromWrite(args.integer[0], args.integer[1]);
        }
    }
    else if(strcmp(args.str[0], "adc") == 0){
        if(strcmp(args.str[1], "battery") == 0){
            adcBattery();
        }
        else if(strcmp(args.str[1], "read") == 0){
            adcRead(args.integer[0]);
        }
    }
    
    else if(strcmp(args.str[0], "rtc") == 0){
        if(strcmp(args.str[1], "date") == 0){
            rtcDate(args.integer[0], args.integer[1]);
        }
        else if(strcmp(args.str[1], "time") == 0){
            rtcTime(args.integer[0], args.integer[1]);
        }
        else if(strcmp(args.str[1], "read") == 0){
            rtcRead();
        }
        else if(strcmp(args.str[1], "weekday") == 0){
            rtcWeekday(args.integer[0]);
        }
    }
    //if didn't detect correct command
    else{
        printf(defaultMsg);
    }
    printf("OK\n");
  }
}



void flashRead(uint32_t ddr)
{
    entry_t entry;
    ENTRY_Read(&entry, ddr);
    ENTRY_Print(&entry);
    printf("\n");
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
    printf("Id\tHour\tMin\tMonth\tDate\tWeekday\n");
    uint32_t lastDdr = EEPROM_ReadUINT32(EEPROM_VAR_LAST_DDR);
    for (uint32_t ddr = 0; ddr <= lastDdr; ddr++){
        entry_t entry;
        ENTRY_Read(&entry, ddr);
        printf("%u\t%u\t%u\t%u\t%u\t%u\n", 
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
    printf("Written %lu to an address %lu\n", data, ddr);
}

void adcBattery()
{
    uint32_t batVol = ADC_MeasureBattery();
    printf("%lu mV\n", batVol);
}

void adcRead(uint32_t repeat){
    uint32_t start = HAL_GetTick();
    do{
        uint32_t data = ADC_MeasureDistance();
        // printf("%lu.%lu mm\n", data/10, (data%10));
        printf("%lu\n", data);
        //delay as to not flood terminal
        HAL_Delay(100);
    }
    while(HAL_GetTick()-repeat < start);
}

void rtcRead()
{
    RTC_Print();
}

void rtcDate(int32_t date, int32_t month)
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

void rtcTime(int32_t hour, int32_t minutes)
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

void rtcWeekday(int32_t weekday)
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
