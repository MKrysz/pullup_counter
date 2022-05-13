
#include "entry.h"
#include "rtc.h"
#include "w25qxx.h"
#include "eeprom.h"

#if DEBUG_ENTRY
#include <stdio.h>
#endif

/**
 * @brief Create a Entry object with unique ID and created timestamp
 * 
 * @param entry
 */
void entry_create(entry_t* entry)
{
    uint16_t tempNextId;
    eeprom_readVariable(nextId, (uint8_t*)(&tempNextId));
    entry->id_ = tempNextId;
    tempNextId++;
    eeprom_writeVariable(nextId, (uint8_t*)(&tempNextId));
    RTC_DateTypeDef date = rtc_getDate();
    RTC_TimeTypeDef time = rtc_getTime();

    entry->hour_ = time.Hours;
    entry->minutes_ = time.Minutes;
    
    entry->weekday_ = date.WeekDay;
    entry->date_ = date.Date;
    entry->month_ = date.Month;
}

/**
 * @brief Saves entry to an external FLASH memory
 * 
 * @param entry 
 */
void entry_save(entry_t* entry)
{
    uint32_t tempPageAddress, tempOffsetInBytes;

    eeprom_readVariable(pageAddress, (uint8_t*)(&tempPageAddress));
    eeprom_readVariable(offsetInBytes, (uint8_t*)(&tempOffsetInBytes));

    W25qxx_WritePage((uint8_t*)entry, tempPageAddress, tempOffsetInBytes, sizeof(entry_t));

    eeprom_writeVariable(lastPageAddress, (uint8_t*)(&tempPageAddress));
    eeprom_writeVariable(lastOffsetInBytes, (uint8_t*)(&tempOffsetInBytes));

    tempOffsetInBytes += sizeof(entry_t);
    if(tempOffsetInBytes == w25qxx.PageSize){
        tempOffsetInBytes = 0;
        tempPageAddress++;
    }

    eeprom_writeVariable(pageAddress, (uint8_t*)(&tempPageAddress));
    eeprom_writeVariable(offsetInBytes, (uint8_t*)(&tempOffsetInBytes));

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

    eeprom_readVariable(lastPageAddress, (uint8_t*)(&tempLastPageAddress));
    eeprom_readVariable(lastOffsetInBytes, (uint8_t*)(&tempLastOffsetInBytes));

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
        && (left->placeHolder_ == right->placeHolder_);
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
