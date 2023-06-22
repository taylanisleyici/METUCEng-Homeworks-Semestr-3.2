#include "common.h"
#include "rcv.h"
/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char rcv_value;

#define BUFSIZE 128

char inbuf[BUFSIZE];
char outbuf[BUFSIZE];
char head[2] = {0,0};
char tail[2] = {0,0};

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


/**********************************************************************
 * ------------------------------ TASK1 -------------------------------
 *
 * Basic echo function test. Typing a single char should reply with the same char, 
 * then some chars that alphabetically precede this char. You will need buffers to echo strings.
 *
 **********************************************************************/
char i = 0;
TASK(RCV_SENT) 
{
	while(1) {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        //TXREG = rcv_value;
	}
	TerminateTask();
}

TASK(RCV)
{
	SetRelAlarm(ALARM_TSK0, 1, 50); // second variable?
	while(1) {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        if(!buf_isempty(OUTBUF))
        {
            TXREG = buf_pop(OUTBUF);
        }
   
	}
	TerminateTask();
}

char buf_isempty(buf_t buf)
{
    return (head[buf] == tail[buf])?1:0;
}

void buf_push(char v, buf_t buf)
{
    if(buf == INBUF) inbuf[head[buf]] = v;
    else outbuf[head[buf]] = v;
    head[buf]++;
    if(head[buf] == BUFSIZE) head[buf] = 0;
}

char buf_pop(buf_t buf)
{
    char v;
    if(buf_isempty(buf))
    {
        return 0;
    }
    else
    {
        if(buf == INBUF) v = inbuf[tail[buf]];
        else v = outbuf[tail[buf]];
        tail[buf]++;
        if(tail[buf] == BUFSIZE) tail[buf] = 0;
        return v;
    }
}

/* End of File : tsk_task1.c */