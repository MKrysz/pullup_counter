
#include "flash.h"

#include <stdio.h>
#include <stdlib.h>

#include "w25qxx.h"

#define _FLASH_PAGE_SIZE (256) //pageSize in bytes
#define FLASH_SECTOR_SIZE (4096) //sectorSize in bytes
#define FLASH_PAGES_IN_SECTOR (FLASH_SECTOR_SIZE/_FLASH_PAGE_SIZE)

#define EEPROMVARS_Sector (0)
#define EEPROMVARS_offset (EEPROMVARS_Sector*FLASH_SECTOR_SIZE)
#define SETTINGS_Sector (1)
#define SETTINGS_offset (SETTINGS_Sector*FLASH_SECTOR_SIZE)
#define RECORS_Sector (2)
#define RECORS_offset (RECORS_Sector*FLASH_SECTOR_SIZE)
#define BUFFER_Sector (3)
#define BUFFER_offset (BUFFER_Sector*FLASH_SECTOR_SIZE)
#define Entry_DB_offset (4*FLASH_SECTOR_SIZE)

#define EntryDdrToDdr(entryDdr) ((entryDdr) * sizeof(entry_t) + Entry_DB_offset)

// TODO: comment everything
// TODO: test me!

void FLASH_EEPROM_Write(uint8_t * pBuffer, uint32_t Sector, uint32_t len)
{
    bool isBufEmpty;
    // uint8_t buf[sizeof(settings_t)];
    uint8_t *buf = malloc(len);
    for (uint32_t i = 0; i+len <= FLASH_SECTOR_SIZE; i+=len){
        FLASH_Read(buf, (Sector*FLASH_SECTOR_SIZE + i), len);
        isBufEmpty = true;
        for(size_t j = 0; j < len; i++){
            if (buf[i] != 0xFF) {
                isBufEmpty = false;
                break;
            }
        }
        if(isBufEmpty){
            FLASH_Write(pBuffer, (Sector*FLASH_SECTOR_SIZE + i), len);
            return;
        }
    }
    W25qxx_EraseSector(Sector);
    FLASH_Write(pBuffer, Sector*FLASH_SECTOR_SIZE, len);
}

void FLASH_EEPROM_Read(uint8_t * pBuffer, uint32_t Sector, uint32_t len)
{
    bool isEmpty = false;
    for (uint32_t i = 0; i+len <= FLASH_SECTOR_SIZE; i+=len){
        FLASH_Read(pBuffer, ((Sector*FLASH_SECTOR_SIZE) +  i), len);
        for(size_t j = 0; j < len; i++){
            isEmpty = false;
            if (pBuffer[i] != 0xFF) {
                isEmpty = false;
                break;
            }
        }
        if(isEmpty){
            if(i==0)
                return;
            else{
                i -= len;
                FLASH_Read(pBuffer, ((Sector*FLASH_SECTOR_SIZE) +  i), len);
                return;
            }
        }
    }
}

void FLASH_Write(uint8_t *pBuffer, uint32_t ddr, uint32_t len)
{
    //calculate page and offset
    uint32_t page = ddr/w25qxx.PageSize;
    uint32_t offset = ddr%w25qxx.PageSize;

    W25qxx_WritePage(pBuffer, page, offset, len);
}

void FLASH_Read(uint8_t *pBuffer, uint32_t ddr, uint32_t len)
{
    W25qxx_ReadBytes(pBuffer, ddr, len);
}

/**
 * @brief function used for erasing specified bytes while retaining rest of data !- currently works only whithin one sector
 * 
 * @param ddr 
 * @param len 
 */
void FLASH_Erase(uint32_t ddr, uint32_t len)
{
    /*
        This function uses another sector of the flash as a temporary buffer
        in which data wished to be retained is stored, while other is cleared
        before written. Then main sector is erased and data stored in buffer
        sector is copied back.
    */
   // TODO: add multisector operation
    uint8_t buffer[_FLASH_PAGE_SIZE];
    uint32_t sectorIdx = ddr/FLASH_SECTOR_SIZE;
    // uint32_t sectorOffset = ddr%FLASH_SECTOR_SIZE;
    uint32_t currentDdr;
    uint32_t endDdr = ddr + len;

    W25qxx_EraseSector(BUFFER_Sector);
    for (size_t i = 0; i < FLASH_SECTOR_SIZE; i+=_FLASH_PAGE_SIZE)
    {
        currentDdr = sectorIdx + i;
        FLASH_Read(buffer, currentDdr, _FLASH_PAGE_SIZE);
        if(ddr>=currentDdr && ddr<currentDdr*_FLASH_PAGE_SIZE){
            if(endDdr<=currentDdr*_FLASH_PAGE_SIZE){ // less than a page worth of deleting
                for (size_t j = ddr%_FLASH_PAGE_SIZE; j < len; j++){
                    buffer[j] = 0xff;
                }
            }
            else{ // delete at max one page size in bytes
                for (size_t j = ddr%_FLASH_PAGE_SIZE; j<_FLASH_PAGE_SIZE; j++){
                    buffer[j] = 0xff;
                    len--;
                    ddr++;
                }
            }
        }
        FLASH_Write(buffer, BUFFER_offset + i, _FLASH_PAGE_SIZE);
    }

    W25qxx_EraseSector(sectorIdx);
    for (size_t i = 0; i < FLASH_SECTOR_SIZE; i+=_FLASH_PAGE_SIZE)
    {
        FLASH_Read(buffer, BUFFER_offset + i, _FLASH_PAGE_SIZE);
        FLASH_Write(buffer, sectorIdx + i, _FLASH_PAGE_SIZE);
    }
}

void FLASH_EntryWrite(entry_t *entry, uint32_t entryDdr)
{
    //convert from entry_ddr to byte_ddr
    uint32_t ddr = EntryDdrToDdr(entryDdr);
    FLASH_Write((uint8_t*) entry, ddr, sizeof(entry_t));
}

/**
 * @brief Reads entry from specified position in flash
 * 
 * @param entry will hold read entry
 * @param entryDdr address
 */
void FLASH_EntryRead(entry_t *entry, uint32_t entryDdr)
{
    //convert from entries to bytes
    uint32_t ddr = EntryDdrToDdr(entryDdr);
    FLASH_Read((uint8_t *)entry, ddr, sizeof(entry_t));
}

void FLASH_EntryDelete(uint32_t entryDdr)
{
    uint32_t ddr = EntryDdrToDdr(entryDdr);
    FLASH_Erase(ddr, sizeof(entry_t));
}


void FLASH_SettingsRead(settings_t *settings)
{
    for (uint32_t i = 0; i < FLASH_SECTOR_SIZE; i+=sizeof(settings_t)){
        FLASH_Read((uint8_t*)settings, (SETTINGS_offset + FLASH_SECTOR_SIZE - i), sizeof(settings_t));
        for(size_t j = 0; j < sizeof(settings_t); i++){
            if (((uint8_t*)(settings))[i] != 0xFF) {
                return;
            }
        }
    }
}

void FLASH_SettingsWrite(settings_t *settings)
{
    bool isBufEmpty;
    uint8_t buf[sizeof(settings_t)];
    for (uint32_t i = 0; i < FLASH_SECTOR_SIZE; i+=sizeof(settings_t)){
        FLASH_Read(buf, (SETTINGS_offset + i), sizeof(settings_t));
        isBufEmpty = true;
        for(size_t j = 0; j < sizeof(settings_t); i++){
            if (buf[i] != 0xFF) {
                isBufEmpty = false;
                break;
            }
        }
        if(isBufEmpty){
            FLASH_Write((uint8_t*)settings, (SETTINGS_offset + i), sizeof(settings_t));
            return;
        }
    }
    W25qxx_EraseSector(SETTINGS_Sector);
    FLASH_Write((uint8_t*)settings, SETTINGS_offset, sizeof(settings_t));

}

void FLASH_VarsRead(eepromVars_t *eepromVars)
{
    for (uint32_t i = 0; i < FLASH_SECTOR_SIZE; i+=sizeof(eepromVars_t)){
        FLASH_Read((uint8_t*)eepromVars, (EEPROMVARS_offset + FLASH_SECTOR_SIZE - i), sizeof(eepromVars_t));
        for(size_t j = 0; j < sizeof(eepromVars_t); i++){
            if (((uint8_t*)(eepromVars))[i] != 0xFF) {
                return;
            }
        }
    }
}

void FLASH_VarsWrite(eepromVars_t *eepromVars)
{
    bool isBufEmpty;
    uint8_t buf[sizeof(eepromVars_t)];
    for (uint32_t i = 0; i < FLASH_SECTOR_SIZE; i+=sizeof(eepromVars_t)){
        FLASH_Read(buf, (EEPROMVARS_offset + i), sizeof(eepromVars_t));
        isBufEmpty = true;
        for(size_t j = 0; j < sizeof(eepromVars_t); i++){
            if (buf[i] != 0xFF) {
                isBufEmpty = false;
                break;
            }
        }
        if(isBufEmpty){
            FLASH_Write((uint8_t*)eepromVars, (EEPROMVARS_offset + i), sizeof(eepromVars_t));
            return;
        }
    }
    W25qxx_EraseSector(EEPROMVARS_Sector);
    FLASH_Write((uint8_t*)eepromVars, EEPROMVARS_offset, sizeof(eepromVars_t));
}

void FLASH_Init()
{
    W25qxx_Init();
}
