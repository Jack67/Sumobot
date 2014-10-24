/*
 * Platform.h
 *
 *  Created on: 25.09.2014
 *      Author: Elia
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "PE_Types.h" //bool -> unsigned char
#include <stddef.h> //includes NULL

#ifdef PL_BOARD_IS_FRDM
	#define PL_HAS_EVENTS 	1
	#define PL_HAS_TIMER 	1
	#define PL_HAS_KEYS 	1
	#define PL_NOF_KEYS 	7
	#define PL_HAS_TRIGGER 	1

	#define PL_HAS_LED_RED	1
	#define PL_HAS_LED_BLUE	1
	#define PL_HAS_LED_GREEN	1

	#define PL_HAS_DEBOUNCE 1

	#define PL_HAS_RTOS 1
#endif

#ifdef PL_BOARD_IS_ROBOT
	#define PL_HAS_EVENTS 	1
	#define PL_HAS_TIMER 	1
	#define PL_HAS_KEYS 	1
	#define PL_NOF_KEYS 	1
	#define PL_HAS_TRIGGER 	1

	#define PL_HAS_LED_RED	0
	#define PL_HAS_LED_BLUE	0
	#define PL_HAS_LED_GREEN	1

	#define PL_HAS_BUZZER 1

	#define PL_HAS_DEBOUNCE 1

	#define PL_HAS_RTOS 1
#endif




void platform_Init(void);
void platform_Deinit(void);

#endif /* PLATFORM_H_ */
