/*
 * File: main.c
 */


#include "pragmas.h"
#include "adc.h"
#include "lcd.h"
#include "the3.h"
#include <string.h>
#include <stdio.h>


volatile char CONVERT=0;
void setLCD();
void startTimer0();
void set7Display();
char isAllocated(unsigned short x, unsigned short y);


unsigned char character[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
};

char locations[5][2] = {{3,2},{3,3},{14,2},{14,3},{9,2}};
unsigned short selectedChar = 0;
unsigned short teamAScore = 0;
unsigned short teamBScore = 0;
char displayCount = 0;


void __interrupt(high_priority) FNC()
{
    if(INTCONbits.INT0IF)
    {
        //LCDStr("INT");
        CONVERT = 1;
        INTCONbits.INT0IF = 0;
    }
    if(INTCONbits.RBIF) // RB interrupt
    {
        startTimer0();
        PORTA = 0;

        if (PORTBbits.RB4 == 0)
        {
            if(locations[selectedChar][1] != 1 && isAllocated(locations[selectedChar][0], locations[selectedChar][1] - 1)) locations[selectedChar][1]--;
        } 
        else if (PORTBbits.RB5 == 0)
        {
            if(locations[selectedChar][0] != 16 && isAllocated(locations[selectedChar][0] + 1, locations[selectedChar][1])) locations[selectedChar][0]++;
        }
        else if (PORTBbits.RB6 == 0)
        {
            if(locations[selectedChar][1] != 4 && isAllocated(locations[selectedChar][0], locations[selectedChar][1] + 1)) locations[selectedChar][1]++;
        }
        else if (PORTBbits.RB7 == 0)
        {
            if(locations[selectedChar][0] != 1 && isAllocated(locations[selectedChar][0] - 1, locations[selectedChar][1])) locations[selectedChar][0]--;
        }
            LCDCmd(LCD_CLEAR);
            setLCD();
            
            INTCONbits.RBIF = 0;
    }
    
    
}

char isAllocated(unsigned short x, unsigned short y)
{
    for(unsigned short i = 0; i < 4; i++)
    {
        if(locations[i][0] == x && locations[i][1] == y) return 0;
    }
    return 1;
}

void setLCD()
{
    for(short i = 4; i >= 0 ; i--)
    {
        if(selectedChar == i)
        {
            if(i < 2)
            {
                LCDGoto(locations[i][0],locations[i][1]);
                if(locations[i][0] == locations[4][0] && locations[i][1] == locations[4][1]) LCDDat(2);
                else LCDDat(1);
            }
            else
            {
                LCDGoto(locations[i][0],locations[i][1]);
                if(locations[i][0] == locations[4][0] && locations[i][1] == locations[4][1]) LCDDat(5);
                else LCDDat(4);
            }
        }
        else
        {
            LCDGoto(locations[i][0],locations[i][1]);
            switch(i)
            {
                case 0:
                    LCDDat(0);
                    break;
                case 1:
                    LCDDat(0);
                    break;
                case 2:
                    LCDDat(3);
                    break;
                case 3:
                    LCDDat(3);
                    break;
                case 4:
                    LCDDat(6);
                    break;                        
            }
        }  
    }
}

void set7Display() // set the 7 segment display
{
    if(displayCount == 0)
    {
        PORTA = 8;
        switch(teamAScore)
        {
            case 0:
                PORTD = 0b00111111;
                break;
            case 1:
                PORTD = 0b00000110;
                break;
            case 2:
                PORTD = 0b01011011;
                break;
            case 3:
                PORTD = 0b01001111;
                break;
            case 4:
                PORTD = 0b01100110;
                break;
            case 5:
                PORTD = 0b01101101;
                break;
        }
        displayCount++;      
    }
    else if(displayCount == 1)
    {
        PORTA = 16;
        PORTD = 64;
        displayCount++;
    }
    else
    {
        PORTA = 32;
        switch(teamBScore)
        {
            case 0:
                PORTD = 0b00111111;
                break;
            case 1:
                PORTD = 0b00000110;
                break;
            case 2:
                PORTD = 0b01011011;
                break;
            case 3:
                PORTD = 0b01001111;
                break;
            case 4:
                PORTD = 0b01100110;
                break;
            case 5:
                PORTD = 0b01101101;
                break;
        }
        displayCount = 0;
    }
    
    
}

void startTimer0() // it will used for 7 segment display
{
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 0;
    T0CONbits.T08BIT = 1; // 8 bit
    T0CONbits.T0CS = 0; // instruction clock
    T0CONbits.PSA = 1; // 0 enables prescaler
    T0CONbits.T0PS = 0b000; // prescaler
    TMR0 = 100; //
    T0CONbits.TMR0ON = 1; // enable
}

void main(void) { 
    
    initADC();
    
    InitLCD();
    
    LCDAddSpecialCharacter(0, teamA_player);
    LCDAddSpecialCharacter(1, selected_teamA_player);
    LCDAddSpecialCharacter(2, selected_teamA_player_with_frisbee);
    LCDAddSpecialCharacter(3, teamB_player);
    LCDAddSpecialCharacter(4, selected_teamB_player);
    LCDAddSpecialCharacter(5, selected_teamB_player_with_frisbee);
    LCDAddSpecialCharacter(6, frisbee);
    LCDAddSpecialCharacter(7, frisbee_target);
    
    char values[10] = {0};
    
    
    
    unsigned short convertion = 0;
    
    TRISBbits.RB0 = 1;
    INTCONbits.INT0IE = 1; //Enable INT0 pin interrupt
    INTCONbits.INT0IF = 0;  
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    // to control the up and down
    PORTBbits.RB4 = 0; 
    PORTBbits.RB5 = 0;
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 0;
    TRISBbits.RB4 = 1; 
    TRISBbits.RB5 = 1;
    TRISBbits.RB6 = 1;
    TRISBbits.RB7 = 1;
    TRISA = 0;
    
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
    INTCON2bits.RBIP = 1;
    INTCON2bits.RBPU = 0;
    
    startTimer0();
    
    
    /*TRISBbits.RB1 = 1;
    TRISBbits.TRISB1 = 1;
    INTCON2bits.RBIP = 0; 
    INTCONbits.RBIE = 1;*/
    
    setLCD();
    
    
    while(1)
    {
        if (INTCONbits.TMR0IF == 1)
        {
            set7Display();
            INTCONbits.TMR0IF = 0;
            startTimer0();
        }
        
        if(CONVERT == 1)
        {
            LCDCmd(LCD_CLEAR);
            setLCD();
            
            CONVERT = 0;
        }
        
    }

    return;
}
