
#include "entry.h"
#include "rtc.h"
#include "w25qxx.h"
#include <stdio.h>

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

    entry->hour_ = stime.Hours;
    entry->minutes_ = stime.Minutes;
    
    entry->weekday_ = sdate.WeekDay;
    entry->date_ = sdate.Date;
    entry->month_ = sdate.Month;
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
    return (left->id_ == right->id_)
        && (left->hour_ == right->hour_)
        && (left->minutes_ == right->minutes_)
        && (left->month_ == right->month_)
        && (left->date_ == right->date_)
        && (left->weekday_ == right->weekday_)
        && (left->UNUSED == right->UNUSED);
}

/**
 * @brief prints entry to the console
 * 
 * @param entry 
 */
void ENTRY_Print(entry_t* entry)
{
    printf("Entry nr:%u\r\n", entry->id_);
    printf("Time of creation %u:%u\n", entry->hour_, entry->minutes_);
    printf("Date of creation: %u/%u\nweekday:%u\r\n", entry->date_, entry->month_, entry->weekday_);
    // printf("UNUSED = %u\n\n\n", entry->UNUSED);
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
