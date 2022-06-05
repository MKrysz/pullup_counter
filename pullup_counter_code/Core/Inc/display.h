#ifndef B26D581D_34C2_473E_8428_F33A65AC4FB3
#define B26D581D_34C2_473E_8428_F33A65AC4FB3

#include "main.h"

#define DISPLAY_DP_USED 0
#define NR_OF_DISPLAY 2
#define DISPLAY_HTIM htim6

// a,b,c,d,e,f,g,dp
// typedef uint8_t displayCode_t;

typedef enum displayPin{
    dis0 = 0,
    dis1,
    dis2,
    dis3,
    dis4,
    dis5,
    dis6,
    dis7,
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    dp
}displayPin_t;

void Display_turnOnTimed(uint32_t ONtime);
void Display_setCharToDigit(uint8_t displayNr, char data);
void Display_setIntToDigit(uint8_t displayNr, uint8_t digit);
void Display_timerCallback();
void Display_setInt(unsigned int integer);
void Display_enable();
void Display_disable();



#endif /* B26D581D_34C2_473E_8428_F33A65AC4FB3 */
