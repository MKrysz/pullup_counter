#include "eeprom.h"

#define EEPROM_BASE_ADDR	0x08080000

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
    uint32_t* temp = ddr+EEPROM_BASE_ADDR;
    return *temp;
}

/**
 * @brief writes multiple 32-bit words to the internal EEPROM
 * 
 * @param ddr address to write to
 * @param data array of words to be written
 * @param len size of data array, in words ie. sizeof(X)/4
 */
void EEPROM_WriteMultipleWords(uint32_t ddr, const uint32_t *data, size_t len)
{
    HAL_FLASHEx_DATAEEPROM_Unlock();
    for(size_t i = 0; i < len; i++){
        HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAM_WORD, EEPROM_BASE_ADDR+ddr+4*i, data[i]);
    }
    HAL_FLASHEx_DATAEEPROM_Lock();

}

/**
 * @brief reads multiple 32-bit words from the internal EEPROM
 * 
 * @param ddr address of the variable
 * @param data array for read data
 * @param len nr of words to be read
 */
void EEPROM_ReadMultipleWords(uint32_t ddr, uint32_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        data[i] = EEPROM_ReadUINT32(ddr+4*i);
    }
    
}