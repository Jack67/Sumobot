/**
 * \file
 * \brief Timer driver
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module implements the driver for all our timers.
  */

#include "Platform.h"
#if PL_HAS_TIMER
#include "Timer.h"

int counter = 0;

void TMR_OnInterrupt(void) {
  /* this one gets called from an interrupt!!!! */
	counter++;
	if(counter > 1000/TMR_TICK_MS)
	{
		counter = 0;
		LED_Red_Neg();
	}

}

void TMR_Init(void) {
  /* nothing needed right now */
}

void TMR_Deinit(void) {
  /* nothing needed right now */
}

#endif /*PL_HAS_TIMER*/
