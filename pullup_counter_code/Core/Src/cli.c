
#include "cli.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "usart.h"
#include "gpio.h"

#define NR_OF_STR 2

typedef struct cli_args_struct
{
    char str[NR_OF_STR][16];
    int integer;
}cli_args_t;

void _CLI_ClearArgs(cli_args_t * cli_args)
{
    for (size_t i = 0; i < NR_OF_STR; i++)
    {
        cli_args->str[i][0] = '\0';
    }
    cli_args->integer = 0;
    
}

void _CLI_ReadArgs(cli_args_t * cli_args, char* userInput)
{
    sscanf(userInput, "%s %s %i", cli_args->str[0], cli_args->str[1], &(cli_args->integer));
}

/**
 * @brief takes instructions from user and executes them as long as USB is connected
 * 
 */
void CLI_UserInterface()
{
  const size_t bufferSize = 32;
  char instructionBuffer[bufferSize];
  while(GPIO_GetUsbFlag()){
    size_t i = 0;

    // get instruction from user
    while(i < bufferSize-1){
      char temp;
      HAL_UART_Receive(&huart2, &temp, 1, HAL_MAX_DELAY);
      temp = tolower(temp);
      instructionBuffer[i] = temp;
      i++;
      if(temp == '\0')
        break;
    }

    // parse user input
    cli_args_t args;
    _CLI_ClearArgs(&args);
    _CLI_ReadArgs(&args, instructionBuffer);
    
    if(strcmp(args.str[0], "help") == 0){
        //TODO help and help-based function
    }
    else if(strcmp(args.str[0], "flash") == 0){
        //TODO flash-based instructions
    }
    else if(strcmp(args.str[0], 'eeprom') == 0){
        //TODO eeprom-based functions
    }
  }
}