#include "eeprom.h"

#define EEPROM_BASE_ADDR	0x08080000
//TODO TEST ME

/**
 * @brief writes one word of data into internal EEPROM 
 * 
 * @param ddr address of the variable
 * @param data data to be written
 */
void EEPROM_WriteUINT32(uint32_t ddr, uint32_t data)
{
	HAL_FLASHEx_DATAEEPROM_Unlock();
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAM_WORD, EEPROM_BASE_ADDR+ddr, data);
	HAL_FLASHEx_DATAEEPROM_Lock();
}

/**
 * @brief reads one word of data from internal EEPROM
 * 
 * @param ddr address of the variable
 * @return uint32_t read data
 */
uint32_t EEPROM_ReadUINT32(uint32_t ddr)
{
    return *((uint32_t *)ddr+EEPROM_BASE_ADDR);
}
