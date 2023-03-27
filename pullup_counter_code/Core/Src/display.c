
#include "display.h"

#include <stdio.h>

#include "ssd1306.h"
#include "images.h"
#include "settings.h"
#include "rtc.h"
#include "tim.h"
#include "adc.h"

#define ABS(X) ((X)>=0?(X):(-X))
#define ABS_DIFF(X, Y) (((X)>(Y))?((X)-(Y)):((Y)-(X)))

volatile display_mode_t _display_mode = Disp_Off;
volatile uint32_t frame = 0;

// % from 0 to 100
volatile uint8_t SD_Done = 0;

void _DrawFilledRectangle(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd);
void _DrawXLine(uint8_t xStart, uint8_t y, uint8_t xEnd);

void _LowVoltage_Callback();
void _LowVoltage_Draw();
void _Counting_Callback();
void _Counting_Draw();
void _USB_Draw();
void _USB_Callback();
void _SD_Draw();
void _SD_Callback();
void _Logout_Draw();
void _Logout_Callback();

int DISPLAY_Init()
{
    HAL_TIM_Base_Start_IT(&DISPLAY_HTIM);
    return ssd1306_Init();
}

void Display_SetMode(display_mode_t display_mode)
{ // TODO: finish me
    _display_mode = display_mode;
    ssd1306_Fill(Black);
    switch (display_mode)
    {
        case Disp_Off:
            break;
        case DispCounting:
            _Counting_Draw();
            break;
        case DispLowVoltage:
            _LowVoltage_Draw();
            break;
        case DispUSB:
            _USB_Draw();
            break;
        case DispSD:
            _SD_Draw();
            break;
        case DispLogout:
            _Logout_Draw();
            break;
        default:
            break;
    }
    frame = 0;
}

void DISPLAY_TIM_Callback()
{ //TODO: finish me
    if(!ssd1306_IsUpdateFinished())
        return;
    switch (_display_mode)
    {
    case DispLowVoltage:
        _LowVoltage_Callback();
        break;
    case DispCounting:
        _Counting_Callback();
        break;
    case DispUSB:
        _USB_Callback();
        break;
    case DispSD:
        _SD_Callback();
        break;
    case DispError:
        break;
    case DispLogout:
        _Logout_Callback();
        break;
    default:
        break;
    }
    ssd1306_UpdateScreen();
    frame++;
}


void _LowVoltage_Draw()
{
    _DrawFilledRectangle(10, 3, 92, 9);
    _DrawFilledRectangle(86, 9, 92, 20);
    _DrawFilledRectangle(92, 14, 104, 20);
    _DrawFilledRectangle(98, 20, 104, 42);
    _DrawFilledRectangle(86, 36, 98, 42);
    _DrawFilledRectangle(10, 9, 17, 56);
    _DrawFilledRectangle(17, 50, 92, 56);
    _DrawFilledRectangle(86, 42, 92, 50);
    
    ssd1306_SetCursor(20,20);
    ssd1306_WriteString("Low Bat", Font_11x18, White);

}

void _LowVoltage_Callback()
{ //TODO: finish me
}

void _Counting_Draw()
{
    Image_Draw(34, 6, img_meter);
    Image_Draw(78, 1, img_clk);
    Image_Draw(84, 52, img_smallBattery);
    Image_Draw(90, 14, img_sd);
    Image_Draw(103, 15, img_autoUpdate); 
    Image_Draw(90, 28, img_proxOn);
    ssd1306_SetCursor(0, 2);
    ssd1306_WriteString("23", Font_11x18, White);
    ssd1306_SetCursor(0, 22);
    ssd1306_WriteString("65", Font_11x18, White);
    ssd1306_SetCursor(0, 42);
    ssd1306_WriteString("10", Font_11x18, White);
}

void _Counting_Callback()
{
    uint32_t goal = 70;
    char strBuf[16];

    // timer
    ssd1306_SetCursor(92,3);
    // sprintf(strBuf, "%02d:%02d", 9, 34);
    sprintf(strBuf, "%02d:%02d", sTime.Hours, sTime.Minutes);
    ssd1306_WriteString(strBuf, Font_7x10, White);

    // voltage
    uint32_t Vbat = ADC_MeasureBattery();
    // uint32_t Vbat = 2784;
    ssd1306_SetCursor(92, 53);
    sprintf(strBuf, "%lu.%lu%luV", (Vbat/1000)%10, (Vbat/100)%10, (Vbat/10)%10);
    ssd1306_WriteString(strBuf, Font_7x10, White);


    // pullup counter
    ssd1306_SetCursor(29+6, 36);
    // sprintf(strBuf, "%03lu", pullupCnt%1000);
    sprintf(strBuf, "%03lu", eepromVars.pullup_counter);
    ssd1306_WriteString(strBuf, Font_16x26, White);

    Image_DrawNeedle(34, 6, eepromVars.pullup_counter*67/goal);

}

/**
 * @brief Draws USB lgoo
 * 
 */
void _USB_Draw()
{
    _DrawFilledRectangle(4, 26, 25, 35);
    _DrawFilledRectangle(25, 28, 122, 33);
    _DrawFilledRectangle(88, 44, 101, 57);
    _DrawFilledRectangle(73, 48, 88, 53);
    _DrawFilledRectangle(57, 8, 84, 13);
    // _DrawFilledRectangle(10, 20, 19, 41);
    _DrawFilledRectangle(111, 24, 114, 28);

    ssd1306_DrawPixel(111, 23, White);
    ssd1306_DrawPixel(123, 30, White);
    ssd1306_DrawPixel(122, 29, White);
    ssd1306_DrawPixel(122, 30, White);
    ssd1306_DrawPixel(122, 31, White);
    ssd1306_DrawPixel(111, 37, White);
    
    _DrawXLine(111, 36, 114);
    _DrawXLine(111, 35, 116);
    _DrawXLine(111, 34, 118);
    _DrawXLine(111, 33, 120);
    _DrawXLine(111, 32, 120);

    _DrawXLine(111, 24, 114);
    _DrawXLine(111, 25, 116);
    _DrawXLine(111, 26, 118);
    _DrawXLine(111, 27, 120);
    _DrawXLine(111, 28, 120);

    _DrawXLine(75, 4, 80);
    _DrawXLine(73, 5, 82);
    _DrawXLine(72, 6, 83);
    _DrawXLine(72, 7, 83);

    _DrawXLine(75, 16, 80);
    _DrawXLine(73, 15, 82);
    _DrawXLine(72, 14, 83);
    _DrawXLine(72, 13, 83);

    _DrawXLine(10, 20, 19);
    _DrawXLine(8, 21, 21);
    _DrawXLine(7, 22, 22);
    _DrawXLine(6, 23, 23);
    _DrawXLine(5, 24, 24);
    _DrawXLine(5, 25, 24);
    _DrawXLine(4, 26, 25);
    _DrawXLine(4, 27, 25);

    _DrawXLine(10, 40, 19);
    _DrawXLine(8, 39, 21);
    _DrawXLine(7, 38, 22);
    _DrawXLine(6, 37, 23);
    _DrawXLine(5, 36, 24);
    _DrawXLine(5, 35, 24);
    _DrawXLine(4, 34, 25);
    _DrawXLine(4, 33, 25);

    uint8_t i = 0;
    for(uint8_t y = 33; y<52; y++){
        i++;
        for(uint8_t x = 53+i; x<60+i; x++){
            ssd1306_DrawPixel(x, y, White);
        }
    }
    i = 0;
    for(uint8_t y = 8; y<28; y++){
        i++;
        for(uint8_t x = 55-i; x<62-i; x++){
            ssd1306_DrawPixel(x, y, White);
        }
    }
}

void _USB_Callback()
{
    // ssd1306_UpdateScreen();
}

/**
 * @brief Draws SD logo and empty progress bar
 * 
 */
void _SD_Draw()
{
    _DrawFilledRectangle(10, 38, 117, 40);
    _DrawFilledRectangle(10, 60, 117, 62);
    _DrawFilledRectangle(115, 40, 117, 60);
    _DrawFilledRectangle(10, 40, 12, 60);

    _DrawXLine(37, 5, 62);
    _DrawXLine(31, 6, 62);
    _DrawXLine(28, 7, 61);
    _DrawXLine(25, 8, 61);
    _DrawXLine(24, 9, 60);
    _DrawXLine(24, 10, 36);
    _DrawXLine(24, 11, 34);
    _DrawXLine(24, 12, 34);
    _DrawXLine(24, 13, 35);
    _DrawXLine(24, 14, 37);
    _DrawXLine(25, 15, 42);
    _DrawXLine(25, 16, 45);
    _DrawXLine(27, 17, 48);
    _DrawXLine(29, 18, 50);
    _DrawXLine(30, 19, 52);
    _DrawXLine(32, 20, 53);
    _DrawXLine(37, 21, 54);
    _DrawXLine(39, 22, 54);
    _DrawXLine(40, 23, 55);
    _DrawXLine(40, 24, 55);
    _DrawXLine(17, 25, 55);
    _DrawXLine(16, 26, 55);
    _DrawXLine(16, 27, 54);
    _DrawXLine(16, 28, 54);
    _DrawXLine(15, 29, 53);
    _DrawXLine(15, 30, 52);
    _DrawXLine(15, 31, 51);
    _DrawXLine(14, 32, 49);

    _DrawXLine(68, 5, 82);
    _DrawXLine(67, 6, 81);
    _DrawXLine(66, 7, 80);
    _DrawXLine(66, 8, 78);
    _DrawXLine(66, 9, 76);
    _DrawXLine(72, 10, 75);

    _DrawXLine(88, 6, 91);
    _DrawXLine(86, 7, 92);
    _DrawXLine(85, 8, 92);
    _DrawXLine(83, 9, 89);
    _DrawXLine(81, 10, 86);
    _DrawXLine(78, 11, 83);
    _DrawXLine(78, 12, 80);

    _DrawXLine(95, 9, 99);
    _DrawXLine(93, 10, 100);
    _DrawXLine(90, 11, 101);
    _DrawXLine(86, 12, 102);
    _DrawXLine(81, 13, 103);
    _DrawXLine(80, 14, 104);
    _DrawXLine(80, 15, 105);
    _DrawXLine(79, 16, 105);
    _DrawXLine(79, 17, 105);
    _DrawXLine(76, 18, 105);
    _DrawXLine(75, 19, 105);
    _DrawXLine(73, 20, 104);
    _DrawXLine(69, 21, 103);
    _DrawXLine(61, 22, 102);
    _DrawXLine(61, 23, 101);
    _DrawXLine(61, 24, 100);
    _DrawXLine(60, 25, 99);
    _DrawXLine(60, 26, 97);
    _DrawXLine(60, 27, 95);
    _DrawXLine(59, 28, 93);
    _DrawXLine(59, 29, 90);
    _DrawXLine(59, 30, 86);
    _DrawXLine(58, 31, 82);
    _DrawXLine(58, 32, 78);
    _DrawXLine(58, 33, 67);
}

void _SD_Callback()
{
    _DrawFilledRectangle(13, 41, 13+SD_Done, 59);
    // ssd1306_UpdateScreen();
}

void _Logout_Draw()
{
    const uint8_t x = 10;
    const uint8_t y = 2;
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString("Pullup", Font_11x18, White);
    ssd1306_SetCursor(x, y+20);
    ssd1306_WriteString("Counter", Font_11x18, White);
    ssd1306_SetCursor(x, y+40);
    ssd1306_WriteString("by mkrysz", Font_11x18, White);
}

void _Logout_Callback()
{
    // ssd1306_UpdateScreen();
}

void _DrawFilledRectangle(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd)
{
    for(uint8_t x = xStart; x<xEnd; x++){
        for(uint8_t y = yStart; y<yEnd; y++){
            ssd1306_DrawPixel(x, y, White);
        }
    }
}

void _DrawXLine(uint8_t xStart, uint8_t y, uint8_t xEnd)
{
    for(uint8_t x = xStart; x < xEnd; x++){
        ssd1306_DrawPixel(x, y, White);
    }
}