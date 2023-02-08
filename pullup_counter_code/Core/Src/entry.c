
#include "entry.h"
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
    printf("Id,Hour,Minute,Day,Month,Year,Count\n");
}

/**
 * @brief prints entry to the console in machine-friendly format
 * 
 * @param entry 
 */
void ENTRY_PrintRaw(entry_t *entry)
{
    printf("%lu,%u,%u,%u,%u,%u,%u\n", 
    entry->id, entry->hour, entry->minute, entry->day,
    entry->month, entry->year, entry->count);
}
