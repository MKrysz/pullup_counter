#include "settings.h"
#include <stdio.h>


settings_t settings = {0};
eepromVars_t eepromVars = {0};
volatile flags_t flags = {0}; 
volatile task_t task = task_normalOperation;

/**
 * @brief prints settings via the console
 * 
 * @param settings 
 */ 
void Settings_Print(settings_t *settings)
{
    printf("Distance Threshold = %ld\n", settings->distanceThreshold);
    printf("Pullup time max = %lu\n", settings->pullupTimeMax);
    printf("Pullup time min = %lu\n", settings->pullupTimeMin);
    printf("Time till shutdown = %lu\n", settings->timeTillShutdown);
    printf("Battery voltage threshold = %lu\n", settings->batteryVoltageThreshold);
    // printf("Start of next day = %lu\n", settings->startOfNextDay);
}