
#include "eeprom.h"
#include "ee24.h"

#if DEBUG_EEPROM
#include <stdio.h>
#endif

#define EEPROM_VAR_MAX_SIZE 4

size_t _getVarSize(eepromVar_t varName)
{
    switch (varName)
    {
    case counterDate:
    case counterMonth:
    case counter:
        return 1;

    case nextId:
        return 2;

    case pageAddress:
    case offsetInBytes:
    case lastPageAddress:
    case lastOffsetInBytes:
        return 4;

    default:
        return 1;
    }
}

void eeprom_writeVariable(eepromVar_t varName, uint8_t* varData)
{
    ee24_write((uint16_t) varName, varData, _getVarSize(varName), __UINT32_MAX__);
}

void eeprom_readVariable(eepromVar_t varName, uint8_t* varData)
{
    ee24_read((uint16_t) varName, varData, _getVarSize(varName), __UINT32_MAX__);
}

void eeprom_eraseVariables()
{
    ee24_eraseChip();
    uint8_t temp[EEPROM_VAR_MAX_SIZE] = {0,0,0,0};
    eeprom_writeVariable(counterDate, temp);
    eeprom_writeVariable(counterMonth, temp);
    eeprom_writeVariable(counter, temp);
    eeprom_writeVariable(nextId, temp);
    eeprom_writeVariable(pageAddress, temp);
    eeprom_writeVariable(offsetInBytes, temp);
    eeprom_writeVariable(lastPageAddress, temp);
    eeprom_writeVariable(lastOffsetInBytes, temp);
}

bool eeprom_compare(eepromVar_t varName, uint8_t* varData)
{
    //read variable in question
    uint8_t temp[EEPROM_VAR_MAX_SIZE] = {0,0,0,0};
    eeprom_readVariable(varName, temp);
    //compare read variable with acquired data
    for(size_t i = 0; i < _getVarSize(varName); i++){
        if(temp[i] != varData[i])
            return false;
    }
    return true;
}

#if DEBUG_EEPROM
void eeprom_printVar(eepromVar_t varName)
{
    uint8_t temp[EEPROM_VAR_MAX_SIZE] = {0,0,0,0};
    eeprom_readVariable(varName, temp);
    printf("eeprom var:\nDDR = %u\n", (uint16_t)varName);
    for(size_t i = 0; i < _getVarSize(varName); i++){
        printf("varData[%u] = %u\n", i, temp[i]);
    }
    printf("\n\n");
}

void eeprom_printAllVariables()
{
    printf("-------\nAll variables:\n");
    eeprom_printVar(nextId);
    eeprom_printVar(pageAddress);
    eeprom_printVar(offsetInBytes);
    eeprom_printVar(lastPageAddress);
    eeprom_printVar(lastOffsetInBytes);
    eeprom_printVar(counterDate);
    eeprom_printVar(counterMonth);
    eeprom_printVar(counter);
    printf("---------\n");
}
#endif

