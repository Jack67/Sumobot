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
	#define PL_HAS_EVENTS 1
	#define PL_HAS_TIMER 1
	#define PL_HAS_KEYS 1
	#define PL_NOF_KEYS 7
	#define PL_HAS_TRIGGER 1
#elif PL_BOARD_IS_FRDM

#endif




void platform_Init(void);
void platform_Deinit(void);

#endif /* PLATFORM_H_ */
