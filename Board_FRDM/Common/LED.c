/*
 * LED.c
 *
 *  Created on: 26.09.2014
 *      Author: Elia
 */
#include "LED.h"

#if PL_HAS_LED_RED == 1
#include "LED_Red.h"
#endif
#if PL_HAS_LED_BLUE == 1
#include "LED_Blue.h"
#endif
#if PL_HAS_LED_GREEN == 1
#include "LED_Green.h"
#endif


//led red-----------------------------------------------------------
#if PL_HAS_LED_RED == 1
void LED_Red_On(void)
{
	LED_Red_ClrVal();
}

void LED_Red_Off(void)
{
	LED_Red_SetVal();
}

void LED_Red_Neg(void)
{
	LED_Red_NegVal();
}

char LED_Red_Get(void)
{
	return !LED_Red_GetVal();
}

void LED_Red_Put(char b)
{
	LED_Red_PutVal(!b);
}

void LED_Red_Init(void)
{
	LED_Red_Off();
}
void LED_Red_DeInit(void)
{

}
#endif

//led blue-----------------------------------------------------------
#if PL_HAS_LED_BLUE == 1
void LED_Blue_On(void)
{
	LED_Blue_ClrVal();
}

void LED_Blue_Off(void)
{
	LED_Blue_SetVal();
}

void LED_Blue_Neg(void)
{
	LED_Blue_NegVal();
}

char LED_Blue_Get(void)
{
	return !LED_Blue_GetVal();
}

void LED_Blue_Put(char b)
{
	LED_Blue_PutVal(!b);
}

void LED_Blue_Init(void)
{
	LED_Blue_Off();
}

void LED_Blue_DeInit(void)
{

}
#endif


//led green-----------------------------------------------------------
#if PL_HAS_LED_GREEN == 1
void LED_Green_On(void)
{
	LED_Green_ClrVal();
}

void LED_Green_Off(void)
{
	LED_Green_SetVal();
}

void LED_Green_Neg(void)
{
	LED_Green_NegVal();
}

char LED_Green_Get(void)
{
	return !LED_Green_GetVal();
}

void LED_Green_Put(char b)
{
	LED_Green_PutVal(!b);
}

void LED_Green_Init(void)
{
	LED_Green_Off();
}
void LED_Green_DeInit(void)
{

}
#endif
