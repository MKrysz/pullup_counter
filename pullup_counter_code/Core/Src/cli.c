
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
#include "w25qxx.h"
#include "rtc.h"
#include "settings.h"

#define CLI_DEBUG 0

// nr of strings in command
#define NR_OF_STR 2
// strings size
#define STR_SIZE 32
//nr of int arguments in command
#define NR_OF_INT 2

//macros for making strings out of variables

// returns string with variables value
#define xstr(s) str(s)
// returns string with varaibles name
#define str(s) #s

#define CMD_EQ(ARG_NR, X) (strcmp(args.str[(ARG_NR)], (X)) == 0)

const char welcomeMsg[] = 
    "/******************** Pull-up counter V02 *********************\\\n"
    "/********************      By MKrysz      *********************\\\n"
    "\n"
    "Type help for list of commands\n";

const char helpMsg[] = 
    "for specific help type help X, where X can be:\n"
    "'flash', 'eeprom', 'adc', 'rtc', 'settings'\n";

const char helpFlashMsg[] = 
    "flash readSince X - reads all entries starting from address X\n"
    "flash clear - clears the entire flash memory\n"
    "flash readOne X - reads an entry from address X"
    "flash last - reads last written entry\n"
    "flash readAll - reads all entries from flash in machine-friendly format:\n"
    "flash write: starts writing sequence where you put flash data in machine-friendly format\n"
    "machine-friendly format:\n"
    "\t id hour min month date weekday year, separated by spaces\n";

const char helpEepromMsg[] = 
    "Address map:\n" 
    str(EEPROM_VAR_LAST_DDR) " - " xstr(EEPROM_VAR_LAST_DDR)"\n"
    str(EEPROM_VAR_PULLUP_COUNTER) " - " xstr(EEPROM_VAR_PULLUP_COUNTER) "\n"
    str(EEPROM_VAR_NEXT_ID) " - " xstr(EEPROM_VAR_NEXT_ID) "\n"
    "eeprom read X - reads data from EEPROM in address X\n"
    "eeprom write X Y - writes Y to EEPROM in address X\n";

const char defaultMsg[] = 
    "Incorrect command \nType 'help' for list of available functions\n";

const char helpAdcMsg[] = 
    "adc battery - reads voltage value of the battery\n"
    "adc read X - reads distance value for X ms, type 0 to measure once\n"
    "adc calibrate - calibrates ambient brightness of distance detector\n";

const char helpRtcMsg[] = 
    "pass -1 for \"no changes\"\n"
    "rtc read - reads value from the RTC\n"
    "rtc date X Y - writes X day of the Y month to the RTC\n"
    "rtc time X Y - writes X hour Y min to the RTC\n"
    "rtc weekday X - writes X weekday, where Monday is 1 and Sunday is 7, to the RTC\n"
    "rtc year X - writes X year to the rtc (0 is 2000, 1 is 2001 and so on)\n";

const char helpSettingsMsg[] = 
    "settings shutdownTime X - sets shutdown time to X\n"
    "settings pullupTimeMax - sets maximum time of the pullup to be detected\n"
    "setting pullupTimeMin - sets minimum time of the pullup to be detected\n"
    "settings distanceThreshold - sets threshold of the distance sensor\n"
    "settings batteryVoltageThreshold - sets low voltage warning threshold\n"
    "settings startOfNextDay - sets hour in wich pullup counter is reset\n"
    "settings read - prints current settings\n";

extern settings_t settings;

void flashRead(uint32_t ddr);
void flashLast();
void flashDump();
void flashReadAll();
void flashClear();
void flashWrite();
void flashReadSince(uint32_t ddr);

void eepromRead(uint32_t ddr);
void eepromWrite(uint32_t ddr, uint32_t data);

void adcBattery();
void adcRead(uint32_t repeat);

void rtcRead();
void rtcDate(int32_t date, int32_t month);
void rtcTime(int32_t hour, int32_t minutes);
void rtcWeekday(int32_t weekday);
void rtcYear(int year);





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

    HAL_UART_Receive(&huart2, (uint8_t *)instructionBuffer, bufferSize, 500);
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
    
    if(CMD_EQ(0, "help")){
        if(CMD_EQ(1, "adc")){
            printf(helpAdcMsg);
        }
        else if(CMD_EQ(1, "rtc")){
            printf(helpRtcMsg);
        }
        else if(CMD_EQ(1, "flash")){
            printf(helpFlashMsg);
        }
        else if(CMD_EQ(1, "eeprom")){
            printf(helpEepromMsg);
        }
        else if(CMD_EQ(1, "settings")){
            printf(helpSettingsMsg);
        }
        
        else{
            printf(helpMsg);
        }
    }
    else if(CMD_EQ(0, "flash")){
        if(CMD_EQ(1, "readSince")){
            flashReadSince(args.integer[0]);
        }
        else if(CMD_EQ(1, "clear")){
            flashClear();
        }
        else if(CMD_EQ(1, "readOne")){
            flashRead(args.integer[0]);
        }
        else if(CMD_EQ(1, "last")){
            flashLast();
        }
        else if(CMD_EQ(1, "readAll")){
            flashReadAll();
        }
        else if(CMD_EQ(1, "write")){
            flashWrite();
        }
    }
    else if(CMD_EQ(0, "eeprom")){
        if(CMD_EQ(1, "read")){
            eepromRead(args.integer[0]);
        }
        else if(CMD_EQ(1, "write")){
            eepromWrite(args.integer[0], args.integer[1]);
        }
    }
    else if(CMD_EQ(0, "adc")){
        if(CMD_EQ(1, "battery")){
            adcBattery();
        }
        else if(CMD_EQ(1, "read")){
            adcRead(args.integer[0]);
        }
        else if(CMD_EQ(1, "calibrate")){
            ADC_Distance_Calibrate();
        }
    }
    
    else if(CMD_EQ(0, "rtc")){
        if(CMD_EQ(1, "date")){
            rtcDate(args.integer[0], args.integer[1]);
        }
        else if(CMD_EQ(1, "time")){
            rtcTime(args.integer[0], args.integer[1]);
        }
        else if(CMD_EQ(1, "read")){
            rtcRead();
        }
        else if(CMD_EQ(1, "weekday")){
            rtcWeekday(args.integer[0]);
        }
        else if(CMD_EQ(1, "year")){
            rtcYear(args.integer[0]);
        }
    }
    else if(CMD_EQ(0, "settings")){
        if(CMD_EQ(1, "shutdownTime")){
            settings.timeTillShutdown = args.integer[0];
            Settings_Save(settings);
        }
        else if(CMD_EQ(1, "pullupTimeMax")){
            settings.pullupTimeMax = args.integer[0];
            Settings_Save(settings);
        }
        else if(CMD_EQ(1, "pullupTimeMin")){
            settings.pullupTimeMin = args.integer[0];
            Settings_Save(settings);
        }
        else if(CMD_EQ(1, "distanceThreshold")){
            settings.distanceThreshold = args.integer[0];
            Settings_Save(settings);
        }
        else if(CMD_EQ(1, "batteryVoltageThreshold")){
            settings.batteryVoltageThreshold = args.integer[0];
            Settings_Save(settings);
        }
        else if(CMD_EQ(1, "startOfNextDay")){
            settings.startOfNextDay = args.integer[0];
            Settings_Save(settings);
        }
        else if(CMD_EQ(1, "read")){
            Settings_Print(&settings);
        }
    }
    //if didn't detect correct command
    else{
        printf(defaultMsg);
    }
    printf("OK\n");
  }
}

void flashReadSince(uint32_t ddr)
{
    ENTRY_PrintRawFormat();
    uint32_t lastDdr = EEPROM_ReadUINT32(EEPROM_VAR_LAST_DDR);
    for(uint32_t i = ddr+1; i <= lastDdr; i++){
        entry_t entry;
        ENTRY_Read(&entry, i);
        ENTRY_PrintRaw(&entry);
    }
}

void flashRead(uint32_t ddr)
{
    entry_t entry;
    ENTRY_Read(&entry, ddr+1);
    ENTRY_PrintRawFormat();
    ENTRY_PrintRaw(&entry);
    printf("\n");
}

void flashLast()
{
    uint32_t lastDdr = EEPROM_ReadUINT32(EEPROM_VAR_LAST_DDR);
    flashRead(lastDdr);
}

void flashReadAll()
{
    flashReadSince(0);
}

void flashClear()
{
    printf("Erasing started...\n");
    W25qxx_EraseChip();
    EEPROM_WriteUINT32(EEPROM_VAR_LAST_DDR, 0);
    EEPROM_WriteUINT32(EEPROM_VAR_NEXT_ID, 0);
    printf("Erasing completed\n");
}


// "\t id hour min month date weekday year, separated by tabs\n";
void flashWrite()
{
    const size_t bufferSize = 32;
    char instructionBuffer[bufferSize];
    printf("To end writing sequence just press enter\n");


    while(1){
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
        entry_t entry = {0};

        //! couldn't get  sscanf to work with uint8_t so I found a work-around
        uint32_t id;
        uint32_t hour;
        uint32_t min;
        uint32_t date;
        uint32_t month;
        uint32_t year;
        uint32_t weekday;
        sscanf(instructionBuffer, "%u %u %u %u %u %u %u", &id, &hour, &min,
            &month, &date, &weekday, &year);
        entry.id_ = id;
        entry.hour_ = hour;
        entry.minutes_ = min;
        entry.date_ = date;
        entry.year_ = year;
        entry.weekday_ = weekday;
        entry.month_ = month;

        entry_t zeroEntry = {0};
        if(ENTRY_IsEqual(&entry, &zeroEntry)){
            break;
        }
        if(!ENTRY_WriteWithCheck(&entry)){
            printf("Error occurred while saving the entry\n Restart required\n");
            while(1);
        }
        printf("writing success\n");
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

extern uint32_t ambientDistance;
void adcRead(uint32_t repeat){
    uint32_t start = HAL_GetTick();
    do{
        int32_t data = ADC_MeasureDistance();
        // printf("%lu.%lu mm\n", data/10, (data%10));
        printf("%li\n", data);
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

void rtcYear(int year)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, FORMAT_BIN);

    if(year >= 0)
        sDate.Year = year;

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
