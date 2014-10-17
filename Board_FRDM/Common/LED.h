/*
 * LED.h
 *
 *  Created on: 26.09.2014
 *      Author: Elia
 */

#ifndef LED_H_
#define LED_H_

#include "Platform.h"

#if PL_HAS_LED_RED == 1
void LED_Red_On(void);
void LED_Red_Off(void);
void LED_Red_Neg(void);
char LED_Red_Get(void);
void LED_Red_Put(char);
void LED_Red_Init(void);
void LED_Red_DeInit(void);
#endif

#if PL_HAS_LED_BLUE == 1
void LED_Blue_On(void);
void LED_Blue_Off(void);
void LED_Blue_Neg(void);
char LED_Blue_Get(void);
void LED_Blue_Put(char);
void LED_Blue_Init(void);
void LED_Blue_DeInit(void);
#endif

#if PL_HAS_LED_GREEN == 1
void LED_Green_On(void);
void LED_Green_Off(void);
void LED_Green_Neg(void);
char LED_Green_Get(void);
void LED_Green_Put(char);
void LED_Green_Init(void);
void LED_Green_DeInit(void);
#endif


#endif /* LED_H_ */
