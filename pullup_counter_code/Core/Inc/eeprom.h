#ifndef CD624D06_35DD_4396_88AF_7ADB513745A3
#define CD624D06_35DD_4396_88AF_7ADB513745A3

#include <stdint.h>
#include <stdbool.h>

#define DEBUG_EEPROM 0

typedef enum _variablesDDR_struct{
    nextId = 0,
    pageAddress = nextId + 2,
    offsetInBytes = pageAddress + 4,
    lastPageAddress = offsetInBytes + 4,
    lastOffsetInBytes = lastPageAddress + 4,
    counterDate = lastOffsetInBytes + 4,
    counterMonth = counterDate + 1,
    counter = counterMonth + 1
}eepromVar_t;

void eeprom_writeVariable(eepromVar_t varName, uint8_t* varData);
void eeprom_readVariable(eepromVar_t varName, uint8_t* varData);
void eeprom_eraseVariables();
bool eeprom_compare(eepromVar_t varName, uint8_t* varData);

#if DEBUG_EEPROM
void eeprom_printVar(eepromVar_t varName);
void eeprom_printAllVariables();
#endif

#endif /* CD624D06_35DD_4396_88AF_7ADB513745A3 */
