#ifndef A4EB8C9D_260A_4F93_872D_1CB5062E31F3
#define A4EB8C9D_260A_4F93_872D_1CB5062E31F3

#include "main.h"
#include <stdbool.h>

#define DEBUG_ENTRY 0

typedef struct _entry_struct
{
    uint16_t id_;
    uint8_t hour_;
    uint8_t minutes_;

    uint8_t month_;
    uint8_t date_;
    uint8_t weekday_;
    uint8_t placeHolder_;//for making a struct 8-bytes wide in order to divide page evenly
} entry_t;

void entry_create(entry_t* entry);
void entry_save(entry_t* entry);
void entry_read(entry_t* entry, uint32_t page, uint32_t offset);
bool entry_verifyLast(entry_t* entry);
bool entry_isEqual(entry_t* left, entry_t* right);

#if DEBUG_ENTRY
void entry_print(entry_t* entry);
#endif

#endif /* A4EB8C9D_260A_4F93_872D_1CB5062E31F3 */
