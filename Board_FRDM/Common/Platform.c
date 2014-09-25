/*
 * Platform.c
 *
 *  Created on: 25.09.2014
 *      Author: Elia
 */

#include "Platform.h"

void platform_Init(void)
{
	LED_Blue_Init();
	LED_Green_Init();
	LED_Red_Init();

}

void platform_Deinit(void)
{
	LED_Blue_DeInit();
	LED_Green_DeInit();
	LED_Red_DeInit();
}
