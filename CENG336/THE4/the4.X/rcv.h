/* 
 * File:   rcv.h
 * Author: ceng33620232
 *
 * Created on June 22, 2023, 11:09 AM
 */

#ifndef RCV_H
#define	RCV_H

typedef enum {INBUF = 0, OUTBUF = 1} buf_t;



void buf_push(char v, buf_t buf);
char buf_pop(buf_t buf);
char buf_isempty(buf_t buf);



#endif	/* RCV_H */

