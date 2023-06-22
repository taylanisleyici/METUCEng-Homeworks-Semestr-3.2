/* 
 * File:   game.c
 * Author: ceng33620232
 *
 * Created on June 22, 2023, 11:42 AM
 */
#include "common.h"
#include "rcv.h"

char started = 0;

char val = 0;
char flag = 0;
void playGame()
{
    if(!started) // the game is not started yet
    {
        if(!buf_isempty(INBUF)) // if there is char in buf
        {
            while(!buf_isempty(INBUF))
            {
                val = buf_pop(INBUF);
                if(val == '$') flag = 1;
                if(flag)            
                    if(val == 'G') started = 1;
                if(val == ':') flag = 0;
            }
            
        }
        flag = 0;
    }
    else // take go command
    {
        if(!flag)
        {
            buf_push('$', OUTBUF);
            buf_push('W', OUTBUF);
            buf_push(':', OUTBUF);
            flag = 1;
        }
    }
}

TASK(GAME)
{
    
    SetRelAlarm(GAME_ALARM, 1, 50);
    while(1)
    {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        playGame();
    }
    TerminateTask();
}
