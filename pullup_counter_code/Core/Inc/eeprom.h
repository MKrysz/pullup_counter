#ifndef DCA791D8_90E2_4460_A4F4_F2BFCD20A055
#define DCA791D8_90E2_4460_A4F4_F2BFCD20A055

#include "stdint.h"
#include "main.h"

#define EEPROM_VAR_LAST_DDR 0
#define EEPROM_VAR_PULLUP_COUNTER 4

void EEPROM_WriteUINT32(uint32_t ddr, uint32_t data);
uint32_t EEPROM_ReadUINT32(uint32_t ddr);

#endif /* DCA791D8_90E2_4460_A4F4_F2BFCD20A055 */