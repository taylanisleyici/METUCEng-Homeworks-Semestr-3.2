/* 
 * File:   lcdtask.c
 * Author: ceng33620232
 *
 * Created on June 22, 2023, 12:30 AM
 */

#include "common.h"
#include "LCD.h"


/*
 * 
 */
extern char string_pool[2][16];

TASK(LCD0)
{
    SetRelAlarm(ALARM_TSK1, 100, 350);

    WaitEvent(LCD_EVENT);
    ClearEvent(LCD_EVENT);

    ClearLCDScreen();
    
    LcdPrintString(string_pool[0], 0, 0);
    LcdPrintString(string_pool[1], 0, 1);
    TerminateTask();
}
