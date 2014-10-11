/**
 * \file
 * \brief Key/Switch driver implementation.
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module implements a generic keyboard driver for up to 7 Keys.
 * It is using macros for maximum flexibility with minimal code overhead.
 */

#include "Platform.h"
#if PL_HAS_KEYS
  #include "Keys.h"
#if PL_HAS_EVENTS
  #include "Event.h"
#endif

void KEY_Scan(void) {
#if PL_NOF_KEYS >= 1
	if (KEY1_Get())
	{
		EVNT_SetEvent(EVNT_SW1_PRESSED);
	}
#endif

#if PL_NOF_KEYS >=2
	if (KEY2_Get())
	{
		EVNT_SetEvent(EVNT_SW2_PRESSED);
	}
#endif

#if PL_NOF_KEYS >=3
	if (KEY3_Get())
	{
		EVNT_SetEvent(EVNT_SW3_PRESSED);
	}
#endif

#if PL_NOF_KEYS >=4
	if (KEY4_Get())
	{
		EVNT_SetEvent(EVNT_SW4_PRESSED);
	}
#endif

#if PL_NOF_KEYS >=5
	if (KEY5_Get())
	{
		EVNT_SetEvent(EVNT_SW5_PRESSED);
	}
#endif

#if PL_NOF_KEYS >=6
	if (KEY6_Get())
	{
		EVNT_SetEvent(EVNT_SW6_PRESSED);
	}
#endif

#if PL_NOF_KEYS >=7
	if (KEY7_Get())
	{
		EVNT_SetEvent(EVNT_SW7_PRESSED);
	}
#endif

  /*! \todo Implement handling of all the other keys */
}

#if PL_HAS_KBI
void KEY_OnInterrupt(KEY_Buttons button) {
  /*! \todo will need to implement functionality for interrupts */
}
#endif

/*! \brief Key driver initialization */
void KEY_Init(void) {
  /* nothing needed for now */
}

/*! \brief Key driver de-initialization */
void KEY_Deinit(void) {
  /* nothing needed for now */
}
#endif /* PL_HAS_KEYS */
