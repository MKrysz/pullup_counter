#ifndef C5E5DABD_4473_4C88_9B94_2889FE42CA41
#define C5E5DABD_4473_4C88_9B94_2889FE42CA41

#include <stdbool.h>
#include <stdint.h>
#include "eeprom.h"

typedef struct _settings_struct{
    uint32_t timeTillShutdown;
    uint32_t pullupTimeMin;
    uint32_t pullupTimeMax;
     int32_t distanceThreshold;
    uint32_t batteryVoltageThreshold;
    uint32_t startOfNextDay; 
}settings_t;

#define Settings_Save(settings) EEPROM_WriteMultipleWords(EEPROM_VAR_SETTINGS, (uint32_t*)(&(settings)), (sizeof(settings_t)/4))
#define Settings_Read(settings) EEPROM_ReadMultipleWords(EEPROM_VAR_SETTINGS, (uint32_t*)(&(settings)), (sizeof(settings_t)/4))

void Settings_Print(settings_t *settings);

#endif /* C5E5DABD_4473_4C88_9B94_2889FE42CA41 */
