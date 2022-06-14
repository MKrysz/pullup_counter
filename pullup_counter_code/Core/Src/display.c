#include "display.h"
#include <stdbool.h>
#include "tim.h"

uint8_t data[NR_OF_DISPLAY];
bool isDisplayON = false;
extern TIM_HandleTypeDef DISPLAY_HTIM;

void _writeDisplayPin(displayPin_t pin, uint8_t value);
void _writeDisplayPort(uint8_t value);
void _MLX_clear();
void _MLX_set(uint8_t dis);
uint8_t _decodeChar(char digit);
uint8_t _decodeInt(uint8_t digit);

/**
    @brief interface between displayPin_t and HAL
    @param pin pin nr
    @param value 1 or 0
*/
void _writeDisplayPin(displayPin_t pin, uint8_t value)
{
    switch (pin)
    {

    case a:
        HAL_GPIO_WritePin(dis_a_GPIO_Port, dis_a_Pin, value);
        break;

    case b:
        HAL_GPIO_WritePin(dis_b_GPIO_Port, dis_b_Pin, value);
        break;

    case c:
        HAL_GPIO_WritePin(dis_c_GPIO_Port, dis_c_Pin, value);
        break;

    case d:
        HAL_GPIO_WritePin(dis_d_GPIO_Port, dis_d_Pin, value);
        break;

    case e:
        HAL_GPIO_WritePin(dis_e_GPIO_Port, dis_e_Pin, value);
        break;
        
    case f:
        HAL_GPIO_WritePin(dis_f_GPIO_Port, dis_f_Pin, value);
        break;

    case g:
        HAL_GPIO_WritePin(dis_g_GPIO_Port, dis_g_Pin, value);
        break;

    case dp:
        #if DISPLAY_DP_USED
            HAL_GPIO_WritePin(dis_dp_GPIO_Port, dis_dp_Pin, value);
        #endif
        break;

    case dis0:
        HAL_GPIO_WritePin(dis_sel_0_GPIO_Port, dis_sel_0_Pin, value);
        break;

    case dis1:
        HAL_GPIO_WritePin(dis_sel_1_GPIO_Port, dis_sel_1_Pin, value);
        break;

    default:
        break;
    }
}

/**
    @brief decodes char to display code for displaying
    @param digit character to be decoded
    @returns display code for particular character (abcdefg[dp])
*/
uint8_t _decodeChar(char digit)
{
    switch (digit)
    {
    case '0':
        return 0xfc;
    
    case '1':
    case '|':
        return 0x60;

    case '2':
        return 0xda;
    
    case '3':
        return 0xf2;
    
    case '4':
        return 0x66;
    
    case '5':
    case 'S':
    case 's':
        return 0xb6;
    
    case '6':
        return 0xbe;
    
    case '7':
        return 0xe0;
    
    case '8':
        return 0xfe;
    
    case '9':
        return 0xf6;
    
    case 'a':
    case 'A':
        return 0xee;
    
    case 'b':
    case 'B':
        return 0x3e;
    
    case 'c':
    case 'C':
        return 0x9c;
    
    case 'd':
    case 'D':
        return 0x7a;
    
    case 'e':
    case 'E':
        return 0x9e;
    
    case 'f':
    case 'F':
        return 0x8e;

    case '-':
        return 0x02;

    case '_':
        return 0x10;

    case 'r':
    case 'R':
        return 0x0A;

    case 't':
    case 'T':
        return 0x1e;

    case 'l':
    case 'L':
        return 0x1C;
    
    case 'o':
    case 'O':
        return 0x3A;

    case 'y':
    case 'Y':
        return 0x76;

    case 'w':
    case 'W':
        return 0x54;

    default:
    return 0;
        break;
    }
}

/**
 * @brief encodes intiger to displayCode
 * 
 * @param digit integer to encode
 * @return uint8_t 
 */
uint8_t _decodeInt(uint8_t digit)
{
    uint8_t truthTable[16] = {
        0xfc, 0x60, 0xda, 0xf2,
        0x66, 0xb6, 0xbe, 0xe0,
        0xfe, 0xf6, 0xee, 0x3e,
        0x9c, 0x7a, 0x9e, 0x8e
    }; 
    return truthTable[digit];
}

/**
 * @brief sends displayCode to display
 * 
 * @param value 
 */
void _writeDisplayPort(uint8_t value)
{
    // value = ~value;
    _writeDisplayPin(a, value&0x80);
    _writeDisplayPin(b, value&0x40);
    _writeDisplayPin(c, value&0x20);
    _writeDisplayPin(d, value&0x10);
    _writeDisplayPin(e, value&0x08);
    _writeDisplayPin(f, value&0x04);
    _writeDisplayPin(g, value&0x02);
    _writeDisplayPin(dp, value&0x01);
    
}

/**
 * @brief turns the display for specified amount of time !function obsolete - do not use!
 * !funcion obsolete
 * 
 * @param ONtime time for display to be on, !will wait for the next multiple of 33!
 */
void Display_turnOnTimed(uint32_t ONtime)
{
    const uint32_t mlxTime = 20; //for about 30fps
    uint32_t startingTime = HAL_GetTick();
    const uint8_t dis_max = 2;
    uint8_t dis = 0;
    while(HAL_GetTick() <= startingTime + ONtime){
        _MLX_set(dis);
        _writeDisplayPort(data[dis]);
        if(dis >= dis_max)
            dis = 0;
        else
            dis++;
        HAL_Delay(mlxTime/(dis_max+1));
    }
    _MLX_clear();
}

/**
 * @brief Function for handling display in background
 * 
 */
void Display_timerCallback()
{
    const uint8_t disMax = NR_OF_DISPLAY-1;

    static uint8_t dis = 0;
    
    if(dis == disMax)
        dis = 0;
    else
        dis++;

    _MLX_set(dis);
    _writeDisplayPort(data[dis]);
}

/**
 * @brief set the display to char
 * 
 * @param displayNr number of the display, counting from 0
 * @param data character to be displayed
 */
void Display_setCharToDigit(uint8_t displayNr, char character)
{
    data[displayNr] = _decodeChar(character);
}

/**
 * @brief set the display to int
 * 
 * @param displayNr number of the display, counting from 0
 * @param digit integer to be displayed
 */
void Display_setIntToDigit(uint8_t displayNr, uint8_t digit)
{
    data[displayNr] = _decodeInt(digit);
}

/**
 * @brief clear all currently chosen displayes
 * 
 */
void _MLX_clear()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        _writeDisplayPin(i, 0);
    }
    
}

/**
 * @brief Choose a dsiplay for multiplexing
 * 
 * @param dis display to be shown
 */
void _MLX_set(uint8_t dis)
{
    _MLX_clear();
    _writeDisplayPin(dis, 1);
}

/**
 * @brief turns display ON 
 * 
 */
void Display_enable()
{
    if(isDisplayON == false){
        HAL_TIM_Base_Start_IT(&DISPLAY_HTIM);
        isDisplayON = true;
    }
}

/**
 * @brief turns display OFF
 * 
 */
void Display_disable()
{
    if(isDisplayON){
        _MLX_clear();
        _writeDisplayPort(0x00);
        HAL_TIM_Base_Stop_IT(&DISPLAY_HTIM);
        isDisplayON = false;
    }
}

/**
 * @brief non-blocking function used to show unsigned int on the display 
 * 
 * @param integer 
 */
void Display_setInt(unsigned int integer)
{
    for(int dis = 0; dis<NR_OF_DISPLAY; dis++){
        Display_setIntToDigit(NR_OF_DISPLAY-dis-1, integer%10);
        integer /= 10;
    }
}

/**
 * @brief blocking function used to show scrolling text on the display; stops scrolling after one rotation
 * ! only works for 2 digits
 * @param msg pointer to a string
 * @param delayTime time in ms between between each scroll
 */
void Display_ShowString(char *msg, uint32_t delayTime)
{
    Display_setCharToDigit(0, ' ');
    Display_setCharToDigit(1, msg[0]);
    HAL_Delay(delayTime);

    for(size_t i = 1; msg[i] != '\0'; i++){
        Display_setCharToDigit(0, msg[i-1]);
        Display_setCharToDigit(1, msg[i]);
        HAL_Delay(delayTime);
    }
}
