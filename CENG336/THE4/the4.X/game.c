/* 
 * File:   game.c
 * Author: ceng33620232
 *
 * Created on June 22, 2023, 11:42 AM
 */
#include "common.h"
#include "rcv.h"
#include "string.h"

char started = 0;

char val = 0;
char flag = 0;

char responseString[21];
char responseHead = 0;
unsigned char isResponse = 0;

extern char string_pool[2][16];

void sendMessage(char *message, unsigned char length)
{
    unsigned char i = 0; 
    for (i = 0; i < length; i++)
    {
        buf_push(message[i], OUTBUF);
    }
}

void clearResponse()
{
    int i;
    for (i = 0; i < 21; i++)
    {
        responseString[i] = 0;
    }
    responseHead = 0;
}

void go(void)
{
    started = 1;
    
}

void endGame(void)
{
    
}

void getStatus(void)
{
    
}

void getHash(void)
{
    
}

void playGame()
{
    if (isResponse)
    {   
        char goString[] = "$GO:";
        char endString[] = "$END:";
        if (responseString[1] == 'G' && responseString[2] == 'O')
        {
            go();
            //clearResponse();
            return;
        }
        if (strcmp(responseString, endString) == 0)
        {
            endGame();
            clearResponse();
            return;
        }
        if (responseString[1] == 'R')
        {
            getStatus();
            clearResponse();
            return;
        }
        if (responseString[1] == 'H')
        {
            getHash();
            clearResponse();
            return;
        }
       
    }
    else if(!buf_isempty(INBUF)) // if there is char in buf
    {
        while(!buf_isempty(INBUF))
        {
            val = buf_pop(INBUF);
            if(val == '$') flag = 1;
            if(flag)
            {
                responseString[responseHead] = val;
                responseHead++;
            }
            if(val == ':')
            {
                flag = 0;
                responseHead = 0;
                isResponse=1;
            }
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
