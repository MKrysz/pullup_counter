#ifndef A7F47A81_E286_4B5E_AAFF_FFE1E1D6999C
#define A7F47A81_E286_4B5E_AAFF_FFE1E1D6999C

#include "main.h"
#include <stdbool.h>

#define DEBUG_ENTRY 0

/**
 * @brief data type representing one entry as is it saved in flash
 * 
 */
typedef struct _entry_struct
{
    uint32_t id;
    unsigned int count:6;
    unsigned int minute:6;
    unsigned int hour:5;
    unsigned int day:5;
    unsigned int month:4;
    unsigned int year:6;
}entry_t;

void ENTRY_CreateFromString(entry_t* entry, char* str);
void ENTRY_Write(entry_t* entry, uint32_t ddr);
void ENTRY_Read(entry_t* entry, uint32_t ddr);
void ENTRY_SetTimestamp(entry_t* entry);
bool ENTRY_IsEqual(entry_t* left, entry_t* right);
void ENTRY_Print(entry_t* entry);
void ENTRY_PrintRawFormat();
void ENTRY_PrintRaw(entry_t *entry);
bool ENTRY_WriteWithCheck(entry_t *entry);

#endif /* A7F47A81_E286_4B5E_AAFF_FFE1E1D6999C */
