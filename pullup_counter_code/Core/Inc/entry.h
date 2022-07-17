#ifndef A7F47A81_E286_4B5E_AAFF_FFE1E1D6999C
#define A7F47A81_E286_4B5E_AAFF_FFE1E1D6999C

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
    uint8_t year_;
} entry_t;




void ENTRY_Write(entry_t* entry, uint32_t ddr);
void ENTRY_Read(entry_t* entry, uint32_t ddr);
void ENTRY_SetTimestamp(entry_t* entry);
bool ENTRY_IsEqual(entry_t* left, entry_t* right);
void ENTRY_Print(entry_t* entry);
bool ENTRY_WriteWithCheck(entry_t *entry);

#endif /* A7F47A81_E286_4B5E_AAFF_FFE1E1D6999C */
