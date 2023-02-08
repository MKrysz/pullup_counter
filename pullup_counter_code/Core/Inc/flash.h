#ifndef _FLASH_H
#define _FLASH_H

#include <stdbool.h>

#include "entry.h"
#include "settings.h"

typedef enum {
    LastDdr,
    pullupCounter
} flash_VarDdr_t;

void FLASH_EntrySave(entry_t *entry);
void FLASH_EntryRead(entry_t *entry, uint32_t entryDdr);
void Flash_EntryDelete(uint32_t entryDdr);

uint32_t FLASH_VarRead_u32(flash_VarDdr_t ddr);
bool FLASH_VarSave_u32(flash_VarDdr_t ddr, uint32_t value);

void FLASH_SettingsRead(settings_t *settings);
bool FLASH_SettingsWrite(settings_t *settings);

#endif // _FLASH_H