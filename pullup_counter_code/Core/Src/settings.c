#include "settings.h"
#include <stdio.h>

/**
 * @brief 
 * 
 * @param settings 
 */ 
void Settings_Print(settings_t *settings)
{
    printf("Distance Threshold = %ld\n", settings->distanceThreshold);
    printf("pullup time max = %lu\n", settings->pullupTimeMax);
    printf("pullup time min = %lu\n", settings->pullupTimeMin);
    printf("time till shutdown = %lu\n", settings->timeTillShutdown);
    printf("battery voltage threshold = %lu\n", settings->batteryVoltageThreshold);
    printf("start of next day = %lu\n", settings->startOfNextDay);
}