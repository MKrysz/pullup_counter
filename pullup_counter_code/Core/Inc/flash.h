#ifndef _FLASH_H
#define _FLASH_H

#include <stdbool.h>

#include "entry.h"
#include "settings.h"

typedef enum {
    LastDdr=1,
    pullupCounter=2,
} flash_VarDdr_t;

void FLASH_Init();

void FLASH_Write(uint8_t *pBuffer, uint32_t ddr, uint32_t len);
void FLASH_Read(uint8_t *pBuffer, uint32_t ddr, uint32_t len);
void FLASH_Erase(uint32_t ddr, uint32_t len);

void FLASH_EntryWrite(entry_t *entry, uint32_t ddr);
void FLASH_EntryRead(entry_t *entry, uint32_t entryDdr);
void Flash_EntryDelete(uint32_t entryDdr);

void FLASH_VarsRead(eepromVals_t *eepromVals);
void FLASH_VarsWrite(eepromVals_t *eepromVals);

void FLASH_SettingsRead(settings_t *settings);
void FLASH_SettingsWrite(settings_t *settings);

#endif // _FLASH_H