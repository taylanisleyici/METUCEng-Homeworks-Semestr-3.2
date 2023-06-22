#ifndef COMMON_H
#define COMMON_H

#include "device.h"

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _10MHZ	63320
#define _16MHZ	61768
#define _20MHZ	60768
#define _32MHZ	57768
#define _40MHZ 	55768

/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define ALARM_EVENT       0x80
#define LCD_EVENT         0x40
#define LCD1_EVENT        0x01

/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * TASK0: USART
 * TASK1: USART
 */
/*ALARMS*/
#define ALARM_TSK0              0
#define LCD_ALARM_ID            1


#define RCV_ID                  1
#define RCV_SENT_ID             2
#define LCD0_ID                 3
#define LCD1_ID                 4
#define GAME_ID                 5

/* Priorities of the tasks */
#define TASK0_PRIO           8
#define TASK1_PRIO           7
#define LCD0_PRIO            10
#define LCD1_PRIO            10
#define GAME_PRIO            9

#define ALARM_TSK0           0
#define ALARM_TSK1           1
#define ALARM_LCD            2
#define GAME_ALARM           3


#endif

/* End of File : common.h */
