
#include "display.h"

#include <stdio.h>

#include "ssd1306.h"
#include "images.h"
#include "settings.h"
#include "rtc.h"
#include "adc.h"

volatile display_mode_t _display_mode = Disp_Off;
volatile uint32_t frame = 0;

void _LowVoltage_Callback();
void _Counting_Callback();

void DISPLAY_Init()
{
    ssd1306_Init();
}

void Display_SetMode(display_mode_t display_mode)
{ // TODO: finish me
    frame = 0;
    _display_mode = display_mode;
    switch (display_mode)
    {
    case Disp_Off:
        ssd1306_Fill(Black);
        break;
    case DispCounting:
        ssd1306_Fill(Black);
        Image_Draw(28, 32, img_meter);
        Image_Draw(78, 13, img_clk);
        Image_Draw(90, 25, img_sd);
        Image_Draw(103, 24, img_autoUpdate);
        Image_Draw(90, 37, img_proxOn);
        Image_Draw(84, 62, img_smallBattery);
        break;
    
    default:
        break;
    }
}

void DISPLAY_TIM_Callback()
{ //TODO: finish me
    switch (_display_mode)
    {
    case DispLowVoltage:
        _LowVoltage_Callback();
        break;
    case DispCounting:
        _Counting_Callback();
        break;
    default:
        break;
    }
    frame++;
}

void _LowVoltage_Callback()
{ //TODO: finish me
    
}

void _Counting_Callback()
{
    char strBuf[16];

    // timer
    ssd1306_SetCursor(92,12);
    sprintf(strBuf, "%02d:%02d", sTime.Hours, sTime.Minutes);
    ssd1306_WriteString(strBuf, Font_7x10, White);

    // voltage
    uint32_t Vbat = ADC_MeasureBattery();
    ssd1306_SetCursor(92, 62);
    sprintf(strBuf, "%lu.%lu%luV", Vbat/1000, (Vbat/100)%10, (Vbat/10)%10);
    ssd1306_WriteString(strBuf, Font_16x26, White);


    // pullup counter
    ssd1306_SetCursor(29, 61);
    sprintf(strBuf, "%03lu", eepromVars.pullup_counter);
    ssd1306_WriteString(strBuf, Font_16x26, White);
}