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
extern char string_pool_started[2][16];
extern char started;



TASK(LCD0)
{
    SetRelAlarm(ALARM_TSK1, 100, 350);
    while (1)
    {
        WaitEvent(LCD_EVENT);
        ClearEvent(LCD_EVENT);
        
        if (started)
        {
            refreshLCD();
            LcdPrintString(string_pool_started[0], 0, 0);
            LcdPrintString(string_pool_started[1], 0, 1);

        }
        else
        {
            LcdPrintString(string_pool[0], 0, 0);
            LcdPrintString(string_pool[1], 0, 1);
        }
        //ClearLCDScreen();

        
    }
    TerminateTask();
}
