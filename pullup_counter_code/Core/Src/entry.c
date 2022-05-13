
#include "entry.h"
#include "rtc.h"
#include "w25qxx.h"

#if DEBUG_ENTRY
#include <stdio.h>
#endif


/**
 * @brief Set timestamp to an entry
 * 
 * @param entry
 */
void entry_setTimestamp(entry_t* entry)
{
    RTC_DateTypeDef date = rtc_getDate();
    RTC_TimeTypeDef time = rtc_getTime();

    entry->hour_ = time.Hours;
    entry->minutes_ = time.Minutes;
    
    entry->weekday_ = date.WeekDay;
    entry->date_ = date.Date;
    entry->month_ = date.Month;
}


/**
 * @brief Read last saved entry and compares it
 * 
 * @param entry entry for a read entry to be compared to
 * @return comparison result
 */
bool entry_verifyLast(entry_t* entry)
{
    uint32_t tempLastPageAddress, tempLastOffsetInBytes;

    // eeprom_readVariable(lastPageAddress, (uint8_t*)(&tempLastPageAddress));
    // eeprom_readVariable(lastOffsetInBytes, (uint8_t*)(&tempLastOffsetInBytes));

    entry_t lastEntry;
    entry_read(&lastEntry, tempLastPageAddress, tempLastOffsetInBytes);
    
    #if DEBUG_ENTRY
    printf("Last written entry:\n\r\n");
    entry_print(&lastEntry);
    #endif
    
    return entry_isEqual(entry, &lastEntry);
}

/**
 * @brief compares two entries
 * 
 * @param left first entry
 * @param right second entry
 * @return true if all of both entries' attributes hold the same value
 */
bool entry_isEqual(entry_t* left, entry_t* right)
{
    return (left->id_ == right->id_)
        && (left->hour_ == right->hour_)
        && (left->minutes_ == right->minutes_)
        && (left->month_ == right->month_)
        && (left->date_ == right->date_)
        && (left->weekday_ == right->weekday_)
        && (left->counter == right->counter);
}

/**
 * @brief Reads entry from specified position in flash
 * 
 * @param entry will hold read entry
 * @param page flash address page
 * @param offset flash address offset in a page
 */
void entry_read(entry_t* entry, uint32_t page, uint32_t offset)
{
    W25qxx_ReadBytes((uint8_t *)entry, page*w25qxx.PageSize + offset, sizeof(entry_t));
}

#if DEBUG_ENTRY
/**
 * @brief prints entry to the console
 * 
 * @param entry 
 */
void entry_print(entry_t* entry)
{
    printf("Entry nr:%u\r\n", entry->id_);
    printf("Time of creation %u:%u\n", entry->hour_, entry->minutes_);
    printf("Date of creation:\nmonth:%u\ndate:%u\nweekday:%u\r\n", entry->month_, entry->date_, entry->weekday_);
    printf("placeHolder = %u\n\n\n", entry->placeHolder_);
}
#endif


void entry_Write(entry_t* entry, uint32_t ddr)
{
    //convert from entries to bytes
    uint32_t ddr_inBytes = ddr * sizeof(entry_t);

    //calculate page and offset
    uint32_t page = ddr_inBytes/w25qxx.PageSize;
    uint32_t offset = ddr_inBytes%w25qxx.PageSize;

    W25qxx_WritePage((uint8_t*)entry, page, offset, sizeof(entry_t));
}

void entry_Read(entry_t* entry, uint32_t ddr)
{
    //convert from entries to bytes
    uint32_t ddr_inBytes = ddr * sizeof(entry_t);

    W25qxx_ReadBytes((uint8_t *)entry, ddr_inBytes, sizeof(entry_t));
}

bool entry_IsEmpty(entry_t* entry)
{
    const entry_t blankEntry = {0xffffffffffffffff};
    return entry_isEqual(entry, &blankEntry);
}

uint32_t entry_GetLastEntry(entry_t entry)
{
    //binary search

    uint32_t ddr_right = w25qxx.PageSize * w25qxx.PageCount / sizeof(entry_t) - 1;
    uint32_t ddr_left = 0;

    while(ddr_left <= ddr_right){
        
    }

}