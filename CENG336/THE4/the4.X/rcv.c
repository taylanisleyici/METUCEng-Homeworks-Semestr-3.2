#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char rcv_value;
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
        TXREG = rcv_value;
	}
	TerminateTask();
}

TASK(RCV)
{
	SetRelAlarm(ALARM_TSK0, 100, 5000);
	while(1) {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        TXREG = '@';

	}
	TerminateTask();
}

/* End of File : tsk_task1.c */