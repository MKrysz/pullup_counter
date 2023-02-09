#include "settings.h"
#include <stdio.h>

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
    printf("Start of next day = %lu\n", settings->startOfNextDay);
}