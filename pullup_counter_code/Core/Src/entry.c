
#include "entry.h"
#include "rtc.h"
#include "w25qxx.h"
#include "eeprom.h"
#include <stdio.h>


/**
 * @brief Creates an entry from a given string
 * 
 * @param entry 
 * @param str 
 */
void ENTRY_CreateFromString(entry_t* entry, char* str)
{
    //! couldn't get  sscanf to work with uint8_t so I found a work-around
    uint32_t id;
    uint32_t hour;
    uint32_t min;
    uint32_t day;
    uint32_t month;
    uint32_t year;
    uint32_t count;
    sscanf(str, "%lu %u %u %u %u %u %u", &id, &hour, &min,
        &day, &month, &year, &count);
    entry->id = id;
    entry->hour = hour;
    entry->minute = min;
    entry->day = day;
    entry->year = year;
    entry->month = month;
    entry->count = count;
}

/**
 * @brief Set timestamp to an entry
 * 
 * @param entry
 */
void ENTRY_SetTimestamp(entry_t* entry)
{
    RTC_TimeTypeDef stime;
    RTC_DateTypeDef sdate;

    HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sdate, RTC_FORMAT_BIN);

    entry->hour = stime.Hours;
    entry->minute = stime.Minutes;
    
    entry->day = sdate.Date;
    entry->month = sdate.Month;
    entry->year = sdate.Year;
}

/**
 * @brief compares two entries
 * 
 * @param left first entry
 * @param right second entry
 * @return true if all of both entries' attributes hold the same value
 */
bool ENTRY_IsEqual(entry_t* left, entry_t* right)
{
    return (left->id == right->id)
        && (left->hour == right->hour)
        && (left->minute == right->minute)
        && (left->month == right->month)
        && (left->day == right->day)
        && (left->year == right->year);
    // for (size_t i = 0; i < sizeof(entry_t); i++)
    // {
    //     if(((uint8_t*)left)[i] != ((uint8_t*)right)[i])
    //         return false;
    // }
    // return true;
}

/**
 * @brief prints entry to the console in human-friendly format
 * 
 * @param entry 
 */
void ENTRY_Print(entry_t* entry)
{
    printf("Entry nr:%lu\r\n", entry->id);
    printf("Time of creation %u:%u\n", entry->hour, entry->minute);
    printf("Date of creation: %u/%u\n", entry->day, entry->month);
    // printf("UNUSED = %u\n\n\n", entry->UNUSED);
}

void ENTRY_PrintRawFormat()
{
    printf("Id Hour Minute Day Month Year Count\n");
}

/**
 * @brief prints entry to the console in machine-friendly format
 * 
 * @param entry 
 */
void ENTRY_PrintRaw(entry_t *entry)
{
    printf("%lu %u %u %u %u %u %u\n", 
    entry->id, entry->hour, entry->minute, entry->day,
    entry->month, entry->year, entry->count);
}

void ENTRY_Write(entry_t* entry, uint32_t ddr)
{
    //convert from entries to bytes
    uint32_t ddr_inBytes = ddr * sizeof(entry_t);

    //calculate page and offset
    uint32_t page = ddr_inBytes/w25qxx.PageSize;
    uint32_t offset = ddr_inBytes%w25qxx.PageSize;

    W25qxx_WritePage((uint8_t*)entry, page, offset, sizeof(entry_t));
}

/**
 * @brief Reads entry from specified position in flash
 * 
 * @param entry will hold read entry
 * @param ddr address
 */
void ENTRY_Read(entry_t* entry, uint32_t ddr)
{
    //convert from entries to bytes
    uint32_t ddr_inBytes = ddr * sizeof(entry_t);

    W25qxx_ReadBytes((uint8_t *)entry, ddr_inBytes, sizeof(entry_t));
}

/**
 * @brief writes entry to flash and checks if operation was successful; also increments EEPROM_LAST_DDR variable
 * 
 * @param entry entry to be written
 * @return true if operation was a success
 */
bool ENTRY_WriteWithCheck(entry_t *entry)
{
      uint32_t currentDDR = EEPROM_ReadUINT32(EEPROM_VAR_LAST_DDR)+1;
      ENTRY_Write(entry, currentDDR);
      entry_t tempEntry;
      ENTRY_Read(&tempEntry, currentDDR);
      EEPROM_WriteUINT32(EEPROM_VAR_LAST_DDR, currentDDR);
      return ENTRY_IsEqual(entry, &tempEntry);
}

