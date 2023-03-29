#ifndef C5E5DABD_4473_4C88_9B94_2889FE42CA41
#define C5E5DABD_4473_4C88_9B94_2889FE42CA41

#include <stdbool.h>
#include <stdint.h>


typedef struct _settings_struct{
    uint32_t timeTillShutdown;
    uint32_t pullupTimeMin;
    uint32_t pullupTimeMax;
     int32_t distanceThreshold;
    uint32_t batteryVoltageThreshold;
    // uint32_t startOfNextDay; 
}settings_t;

typedef struct {
    uint32_t lastDdr;
    uint32_t pullup_counter;
} eepromVars_t;

typedef struct {
    bool SD_Update;
    bool IsProxOn;
} flags_t;

typedef struct {
    uint16_t wholeTraining;
    uint8_t oneSeries;
} records_t;

typedef enum {
    task_normalOperation,
    task_SD_Update,
    task_USB_handler,
} task_t;


extern settings_t settings;
extern eepromVars_t eepromVars;
extern volatile flags_t flags; 
extern volatile task_t task; 
extern records_t records;

void Settings_Print(settings_t *settings);

#endif /* C5E5DABD_4473_4C88_9B94_2889FE42CA41 */
