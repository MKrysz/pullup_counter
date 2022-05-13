#ifndef A4EB8C9D_260A_4F93_872D_1CB5062E31F3
#define A4EB8C9D_260A_4F93_872D_1CB5062E31F3

#include "main.h"
#include <stdbool.h>

#define DEBUG_ENTRY 0

/**
 * @brief data type representing one pullup, used to be saved in flash
 * 
 */
typedef struct _entry_struct
{
    uint16_t id_;

    uint8_t hour_;
    uint8_t minutes_;

    uint8_t month_;
    uint8_t date_;
    uint8_t weekday_;

    //nr of pullups that have already been done that day
    uint8_t counter;
} entry_t;




void entry_Write(entry_t* entry, uint32_t ddr);
void entry_Read(entry_t* entry, uint32_t ddr);
uint32_t entry_GetLastEntry(entry_t entry);
void entry_setTimestamp(entry_t* entry);
bool entry_verifyLast(entry_t* entry);
bool entry_isEqual(entry_t* left, entry_t* right);
bool entry_IsEmpty(entry_t* entry);

#if DEBUG_ENTRY
void entry_print(entry_t* entry);
#endif

#endif /* A4EB8C9D_260A_4F93_872D_1CB5062E31F3 */
