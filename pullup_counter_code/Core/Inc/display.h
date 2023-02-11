#ifndef _DISPLAY_H
#define _DISPLAY_H

typedef enum {
    Disp_Off,
    DispLowVoltage,
    DispCounting,
    DispUSB,
    DispSD,
    DispError,
} display_mode_t;

void DISPLAY_Init();
void Display_SetMode(display_mode_t display_mode);
void DISPLAY_TIM_Callback();

#endif // _DISPLAY_H