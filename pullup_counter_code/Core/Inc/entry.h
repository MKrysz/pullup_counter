#ifndef _ENTRY_H
#define _ENTRY_H

#include <stdint.h>
#include <stdbool.h>


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

bool ENTRY_IsEqual(entry_t* left, entry_t* right);

void ENTRY_CreateFromString(entry_t* entry, char* str);

void ENTRY_ToStrings(entry_t *entry, char* str);
void ENTRY_Print(entry_t* entry);
void ENTRY_PrintRawFormat();
void ENTRY_PrintRaw(entry_t *entry);

#endif // _ENTRY_h