/* 
 * File:   game.c
 * Author: ceng33620232
 *
 * Created on June 22, 2023, 11:42 AM
 */
#include "common.h"
#include "rcv.h"
#include "string.h"



typedef enum {
    NONE = 0, MEAT = 1, POTATO = 2, BREAD = 3, COOKING = 4, SLOWCOOKING = 5
}ingredient_t;

typedef struct customer
{
    unsigned char customerId;
    ingredient_t requestedIngredients[2];
    unsigned char isFJ;
    unsigned char patience;
    unsigned char isOrderBeingCooked;
};

char started = 0;

char val = 0;
char flag = 0;

char responseString[21];
char responseHead = 0;
unsigned char isResponse = 0;

struct customer customers[3];
//unsigned char customerStatus[3][4];
ingredient_t ingredients[4];

unsigned short money = 0;

extern char string_pool[2][16];
char isLCDChanged = 1;

void convertToString() // TODO
{
    
}

/*void sendMessage(char *message, unsigned char length)
{
    unsigned char i = 0; 
    for (i = 0; i < length; i++)
    {
        buf_push(message[i], OUTBUF);
    }
}*/

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
    buf_push('$', OUTBUF);
    buf_push('W', OUTBUF);
    buf_push(':', OUTBUF);
    //buf_push('\0', OUTBUF);
    return;
}

void endGame(void)
{
    
}

void tossIngredient(ingredient_t ingredientToToss)
{
    unsigned char i;
    buf_push('$', OUTBUF);
    buf_push('T', OUTBUF);
    for (i = 0; i < 4; i++)
    {
        if(ingredients[i] == ingredientToToss)
        {
            buf_push(i, OUTBUF);
            break;
        }
    }
    buf_push(':', OUTBUF);
}

void cookOrder(unsigned char customerIndex)
{
    unsigned char i;
    if (customers[customerIndex].isOrderBeingCooked)
    {
        return;
    }
    else
    {
        customers[customerIndex].isOrderBeingCooked = 1;
    }
    buf_push('$', OUTBUF);
    buf_push('C', OUTBUF);
    buf_push(customers[customerIndex].customerId, OUTBUF);
    if (customers[customerIndex].requestedIngredients[0] == NONE)
    {
        buf_push('N', OUTBUF);
    }
    else
    {   
        for (i = 0; i < 4; i++)
        {
            if (ingredients[i] == customers[customerIndex].requestedIngredients[0])
            {
                ingredients[i] = COOKING;
                buf_push(i, OUTBUF);
                break;
            }
        }
    }
    if (customers[customerIndex].requestedIngredients[1] == NONE)
    {
        buf_push('N', OUTBUF);
    }
    else
    {   
        for (i = 0; i < 4; i++)
        {
            if (ingredients[i] == customers[customerIndex].requestedIngredients[1])
            {
                ingredients[i] = COOKING;
                buf_push(i, OUTBUF);
                break;
            }
        }
    }
    buf_push(':', OUTBUF);
    
}

void iterateGame(void)
{
    unsigned char i,j;
    unsigned char customerWithLowestPatience = 4;
    unsigned char lowestPatience = 255;
    for (i = 0; i < 3; i++)
    {
        unsigned char ing1State = 0, ing2State = 0;
        if (customers[i].customerId == 0)
        {
            continue;
        }
        for (j = 0; j < 4; j++)
        {
            if (customers[i].requestedIngredients[0] == NONE || (ingredients[j] == customers[i].requestedIngredients[0]))
            {
                ing1State = 1;
            }
            if (customers[i].requestedIngredients[1] == NONE || (ingredients[j] == customers[i].requestedIngredients[1]))
            {
                ing2State = 1;
            }
        }
        if (ing1State && ing2State)
        {
            if (customers[i].patience <= lowestPatience)
            {
                lowestPatience = customers[i].patience;
                customerWithLowestPatience = i;
            }
        }
    }
    if (customerWithLowestPatience == 4)
    {
        ingredient_t ingredientToToss = NONE;
        unsigned char countOfIngredientToToss = 0;
        unsigned char breadC = 0, meatC = 0, potatoC = 0;
        unsigned char i;
        for (i = 0; i < 4; i++)
        {
            if(ingredients[i] == NONE) return;
            if(ingredients[i] == BREAD)
            {
                breadC++;
                if (breadC > countOfIngredientToToss)
                {
                    ingredientToToss = BREAD;
                    countOfIngredientToToss = breadC;
                }
            }
            if(ingredients[i] == MEAT)
            {
                meatC++;
                if (meatC > countOfIngredientToToss)
                {
                    ingredientToToss = MEAT;
                    countOfIngredientToToss = meatC;
                }
            }
            if(ingredients[i] == POTATO)
            {
                potatoC++;
                if (potatoC > countOfIngredientToToss)
                {
                    ingredientToToss = POTATO;
                    countOfIngredientToToss = potatoC;
                }
            }
        }
        tossIngredient(ingredientToToss);
        return;
    }
    cookOrder(customerWithLowestPatience);
    
}

void getStatus(void)
{
    char i, j;
    for(i = 0; i < 3; i++)
    {
        char ingredient1, ingredient2;
        if (customers[i].customerId == responseString[2+4*i]) continue; // assumed that customer order is not changing between messages.
        customers[i].customerId = responseString[2+4*i];
        ingredient1 = responseString[2+4*i+1];
        ingredient2 = responseString[2+4*i+2];
        customers[i].patience = responseString[2+4*i+3];
        customers[i].isOrderBeingCooked = 0;
        if (ingredient1 == 'F' && ingredient2 == 'J')
        {
            customers[i].isFJ = 1;
        }
        else
        {
            customers[i].isFJ = 2;
            /*typedef enum {
                NONE = 0, MEAT = 1, POTATO = 2, BREAD = 3, COOKING = 4, SLOWCOOKING = 5
            }ingredient_t;*/
            if (ingredient1 == 'N') customers[i].requestedIngredients[0] = NONE;
            if (ingredient1 == 'M') customers[i].requestedIngredients[0] = MEAT;
            if (ingredient1 == 'P') customers[i].requestedIngredients[0] = POTATO;
            if (ingredient1 == 'B') customers[i].requestedIngredients[0] = BREAD;
            if (ingredient1 == 'C') customers[i].requestedIngredients[0] = COOKING;
            if (ingredient1 == 'S') customers[i].requestedIngredients[0] = SLOWCOOKING;
            
            if (ingredient2 == 'N') customers[i].requestedIngredients[1] = NONE;
            if (ingredient2 == 'M') customers[i].requestedIngredients[1] = MEAT;
            if (ingredient2 == 'P') customers[i].requestedIngredients[1] = POTATO;
            if (ingredient2 == 'B') customers[i].requestedIngredients[1] = BREAD;
            if (ingredient2 == 'C') customers[i].requestedIngredients[1] = COOKING;
            if (ingredient2 == 'S') customers[i].requestedIngredients[1] = SLOWCOOKING;
        }
    }
    
    for(i = 0; i < 4; i++)
    {
        char ingredient;
        ingredient = responseString[14 + i];
        if (ingredient == 'N') ingredients[i] = NONE;
        if (ingredient == 'M') ingredients[i] = MEAT;
        if (ingredient == 'P') ingredients[i] = POTATO;
        if (ingredient == 'B') ingredients[i] = BREAD;
        if (ingredient == 'C') ingredients[i] = COOKING;
        if (ingredient == 'S') ingredients[i] = SLOWCOOKING;
    }
    
    iterateGame();
}

void getHash(void)
{
    
}

void playGame()
{
    // Halihaz?rda runl?yorsa runlamamas? için bir event at?lmas? gerekiyor.
    if (isResponse)
    {   
        if (responseString[1] == 'G' && responseString[2] == 'O')
        {
            go();
            started = 1;
            clearResponse();
            isResponse = 0;
            return;
        }
        else if (responseString[1] == 'E' && responseString[2] == 'N' && responseString[3] == 'D')
        {
            endGame();
            clearResponse();
            isResponse = 0;
            return;
        }
        else if (responseString[1] == 'R')
        {
            getStatus();
            clearResponse();
            isResponse = 0;
            return;
        }
        else if (responseString[1] == 'H')
        {
            getHash();
            clearResponse();
            isResponse = 0;
            return;
        }  
    }
    else if(!buf_isempty(INBUF)) // if there is char in buf
    {
        while(!buf_isempty(INBUF))
        {
            val = buf_pop(INBUF);
            string_pool[0][responseHead] = val;
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
                break;
            }
        }
    }
    if(started)
    {
        go();
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
