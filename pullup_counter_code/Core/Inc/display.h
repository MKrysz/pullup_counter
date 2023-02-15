#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <stdint.h>

typedef enum {
    Disp_Off,
    DispLowVoltage,
    DispCounting,
    DispUSB,
    DispSD,
    DispError,
    DispLogout,
} display_mode_t;

extern volatile uint8_t SD_Done;

int DISPLAY_Init();
void Display_SetMode(display_mode_t display_mode);
void DISPLAY_TIM_Callback();


#endif // _DISPLAY_H