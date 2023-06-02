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
void startTimer2(void);
void startTimer0(void);
void set7Display();
char isAvailable(unsigned short x, unsigned short y);
void changeSpeedLevel(void);
void iterateGame(void);
char isActive = 0;
char seeTarget = 1;
char isThrownByA = 1;
unsigned char speedLevel = 1;

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
char targetPosition[2] = {0,0};
unsigned short selectedChar = 0;
unsigned short teamAScore = 0;
unsigned short teamBScore = 0;
char displayCount = 0;
char rb4last, rb5last, rb6last, rb7last;
unsigned short currentStep = 0;
unsigned short totalStep = 0;

void timer1Interrupt()
{
    changeSpeedLevel();
    IPR1bits.TMR1IP = 0;
    //PIE1bits.TMR1IE = 0;
    PIR1bits.TMR1IF = 0;
    INTCONbits.RBIE = 1;
    INTCON3bits.INT1E = 1;
    INTCONbits.RBIF = 0;
}

void startTimer1() // it will be used for debouncing 100 ms
{
    IPR1bits.TMR1IP = 1;
    PIE1bits.TMR1IE = 1;
    TMR1L = 0;
    TMR1H = 0b10000000;
    T1CONbits.TMR1ON = 1;
    T1CONbits.T1CKPS0 = 1; 
    T1CONbits.T1CKPS1 = 1;
    INTCONbits.RBIE = 0;
    INTCONbits.RBIF = 0;
    INTCON3bits.INT1E = 0; 
}

void movePlayer(unsigned char i, unsigned short move)
{
    unsigned short nextX = locations[i][0], nextY = locations[i][1];
    switch(move)
    {
        case 0:
            nextX--;
            nextY--;
            break;
        case 1:
            nextY--;
            break;
        case 2:
            nextX++;
            nextY--;
            break;
        case 3:
            nextX--;
            break;
        case 4:
            break;
        case 5:
            nextX++;
            break;
        case 6:
            nextX--;
            nextY++;
            break;
        case 7:
            nextY++;
            break;
        case 8:
            nextX++;
            nextY++;
            break;
    }
    if (isAvailable(nextX, nextY))
    {
        locations[i][0] = nextX;
        locations[i][1] = nextY;
    }
    
}

char isAvailable(unsigned short x, unsigned short y)
{
    if (x < 1 || x > 16 || y < 1 || y> 4)
    {
        return 0;
    }
    for(unsigned char i = 0; i < 4; i++)
    {
        if(locations[i][0] == x && locations[i][1] == y) return 0;
    }
    return 1;
}

short counter = 0;
void movement()
{
    if(counter == 1) return;
    if (PORTBbits.RB4 == 0)
    {
        
        if(locations[selectedChar][1] != 1 && isAvailable(locations[selectedChar][0], locations[selectedChar][1] - 1)) locations[selectedChar][1]--;
        counter = 1;
        INTCONbits.RBIF = 0;
        return;
    } 
    if (PORTBbits.RB5 == 0)
    {  
        if(locations[selectedChar][0] != 16 && isAvailable(locations[selectedChar][0] + 1, locations[selectedChar][1])) locations[selectedChar][0]++;
        counter = 1;
        INTCONbits.RBIF = 0;
        return;
    }
    if (PORTBbits.RB6 == 0)
    {
        if(locations[selectedChar][1] != 4 && isAvailable(locations[selectedChar][0], locations[selectedChar][1] + 1)) locations[selectedChar][1]++;
        counter = 1;
        INTCONbits.RBIF = 0;
        
        return;
    }
    if (PORTBbits.RB7 == 0)
    {
        if(locations[selectedChar][0] != 1 && isAvailable(locations[selectedChar][0] - 1, locations[selectedChar][1])) locations[selectedChar][0]--;
        counter = 1;
        INTCONbits.RBIF = 0;
        return;
    }
    
        
}

void throwFrisbee()
{
    if (locations[selectedChar][0] != locations[4][0] || locations[selectedChar][0] != locations[4][0] || isActive)
    {
        return;
    }
    if (selectedChar < 2)
    {
        isThrownByA = 1;
    }
    else
    {
        isThrownByA = 0;
    }
    //LCDGoto(frisbee_steps[totalStep - 1][0], frisbee_steps[totalStep - 1][1]);
    currentStep = 0;
    totalStep = compute_frisbee_target_and_route(locations[4][0], locations[4][1]);
    isActive = 1;
    //unsigned short currentStep = 0;
    //unsigned short totalStep = 0;
}

void rb1Interrupt()
{
    if (counter == 1) return;
    if (locations[selectedChar][0] == locations[4][0] && locations[selectedChar][1] == locations[4][1]) return;
    if(PORTBbits.RB1 == 0)
    {
        selectedChar = (selectedChar+1)%4;
        INTCON3bits.INT1E = 0;
    }
    INTCON3bits.INT1F = 0;
    counter = 1;
}

void changeSpeedLevel(void)
{
    //unsigned short readADCChannel(unsigned char channel)
    unsigned short value = readADCChannel(0);
    if (value < 256)
    {
        speedLevel = 1;
    }
    else if (value < 512)
    {
        speedLevel = 2;
    }
    else if (value < 768)
    {
        speedLevel = 3;
    }
    else
    {
        speedLevel = 4;
    }

    initADC();
}

void __interrupt(high_priority) FNC()
{
    if (PIR1bits.ADIF)
    {
        changeSpeedLevel();
    }
    if (PIR1bits.TMR1IF)
    {
        if (counter == 1)
        {
            counter = 0;
        }else
        {
            timer1Interrupt();
        }
        PORTA = 0;
        LCDCmd(LCD_CLEAR);
        setLCD();
        timer1Interrupt();
        startTimer2();
        PIR1bits.TMR2IF = 0;
    }
    if (PIR1bits.TMR2IF == 1)
    {
        set7Display();
        PIR1bits.TMR2IF = 0;
        startTimer2();
        return;
    }
    if(INTCONbits.INT0IF)
    {
        throwFrisbee();
        //CONVERT = 1;
        INTCONbits.INT0IF = 0;
        startTimer0();
    }
    if (INTCON3bits.INT1F)
    {
        rb1Interrupt();
    }
    if(INTCONbits.RBIF) // RB interrupt
    {
        movement();
    }
    if (PIR1bits.TMR2IF)
    {
        set7Display();
        PIR1bits.TMR2IF = 0;
        startTimer2();
    }
    
}

/*char isAllocated(unsigned short x, unsigned short y)
{
    for(unsigned char i = 0; i < 4; i++)
    {
        if(locations[i][0] == x && locations[i][1] == y) return 0;
    }
    return 1;
}*/

void setLCD()
{
    if (isActive && seeTarget)
    {
        seeTarget = 0;
        //LCDGoto(1,1);
        LCDGoto(frisbee_steps[totalStep - 1][0], frisbee_steps[totalStep - 1][1]);
        LCDDat(7);
    }
    else
    {
        seeTarget = 1;
    }
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

void startTimer2(void)
{
    PIE1bits.TMR2IE = 1;
    T2CON = 0b00000100;
    TMR2 = 50;
}

void startTimer0() // 
{
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 0;
    T0CONbits.T08BIT = 0; // 16 bit
    T0CONbits.T0CS = 0; // instruction clock
    T0CONbits.PSA = 0; // 0 enables prescaler
    if(speedLevel == 1) // 400ms
    {
        T0CONbits.T0PS = 0b011; 
        TMR0L = 0b11011100;
        TMR0H = 0b00001011;
    }
    else if (speedLevel == 2) // 800ms
    {
        T0CONbits.T0PS = 0b100;
        TMR0L = 0b11011100;
        TMR0H = 0b00001011;
    } 
    else if (speedLevel == 3) // 1200ms
    {
        T0CONbits.T0PS = 0b101; 
        TMR0L = 0b11100101;
        TMR0H = 0b01001000;
    }
    else // 1600 ms
    {
        T0CONbits.T0PS = 0b101;
        TMR0L = 0b11011100;
        TMR0H = 0b00001011;
    } 
    
    T0CONbits.TMR0ON = 1; // enable
}

void setTimer3(void)
{
    T3CON = 0b10000001;
    
    //T3CON.TMR3ON = 1;
    //T3CON.RD16 = 1;
}

void iterateGame(void)
{
    if (isActive)
    {
        if (currentStep < totalStep)
        {
            locations[4][0] = frisbee_steps[currentStep][0];
            locations[4][1] = frisbee_steps[currentStep][1];
            for (unsigned char i = 0; i < 4; i++)
            {
                if (i == selectedChar)
                {
                    continue;
                }
                unsigned short movement = random_generator(8);
                movePlayer(i, movement);
            }
            currentStep++;
        }
        if (currentStep == totalStep)
        {
            isActive = 0;
            if (locations[selectedChar][0] == locations[4][0] && locations[selectedChar][1] == locations[4][1])
            {
                if (selectedChar < 2)
                {
                    teamAScore++;
                }
                else
                {
                    teamBScore++;
                }
            }
            else
            {
                if (isThrownByA)
                {
                    teamBScore++;
                }
                else
                {
                    teamAScore++;
                }
            }
            set7Display();
            PIR1bits.TMR2IF = 0;
            startTimer2();
        }
    }
}

void main(void) { 

    setTimer3();
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
        
    
    
    unsigned short convertion = 0;
    
    TRISBbits.RB0 = 1;
    INTCONbits.INT0IE = 1; //Enable INT0 pin interrupt
    INTCONbits.INT0IF = 0;  
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    INTCON3bits.INT1E = 1;
    INTCON3bits.INT1IP = 1;
    
    // to control the up and down
    PORTBbits.RB4 = 0; 
    PORTBbits.RB5 = 0;
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 0;
    PORTBbits.RB1 = 0;
    TRISBbits.RB1 = 1;
    TRISBbits.RB4 = 1; 
    TRISBbits.RB5 = 1;
    TRISBbits.RB6 = 1;
    TRISBbits.RB7 = 1;
    TRISA = 0;
    
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
    INTCON2bits.RBIP = 1;
    INTCON2bits.RBPU = 0;
    
    
    
    
    /*TRISBbits.RB1 = 1;
    TRISBbits.TRISB1 = 1;
    INTCON2bits.RBIP = 0; 
    INTCONbits.RBIE = 1;*/
    startTimer0();
    LCDCmd(LCD_CLEAR);
    setLCD();
    startTimer2();
    startTimer1();
    while(1)
    {
        if (INTCONbits.TMR0IF)
        {
            iterateGame();
            INTCONbits.TMR0IF = 0;
            startTimer0();
        }
    }

    return;
}