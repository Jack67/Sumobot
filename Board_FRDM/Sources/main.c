/* ###################################################################
**     Filename    : main.c
**     Project     : INTRO_FRDM
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-09-19, 09:03, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "LED_Red.h"
#include "BitIoLdd1.h"
#include "LED_Green.h"
#include "BitIoLdd2.h"
#include "LED_Blue.h"
#include "BitIoLdd3.h"
#include "WAIT.h"
#include "HF1.h"
#include "CS1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "SW1.h"
#include "BitIoLdd4.h"
#include "SW2.h"
#include "BitIoLdd5.h"
#include "SW3.h"
#include "BitIoLdd6.h"
#include "SW4.h"
#include "BitIoLdd7.h"
#include "SW5.h"
#include "BitIoLdd8.h"
#include "SW6.h"
#include "BitIoLdd9.h"
#include "SW7.h"
#include "BitIoLdd10.h"
#include "UTIL1.h"
#include "CLS1.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "FRTOS1.h"
#include "USB1.h"
#include "USB0.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "../Common/Platform.h"
#include "../Common/Event.h"
#include "../Common/Keys.h"
#include "../Common/LED.h"
#include "../Common/Timer.h"
#include "../Common/Trigger.h"
#include "../Common/RTOS.h"
#include "../Common/Shell.h"

void OnEvent(EVNT_Handle);
void OnTriggerBlink(void*);
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  platform_Init();
  EVNT_Init();
  KEY_Init();
  LED_Blue_Init();
  LED_Green_Init();
  LED_Red_Init();
  TMR_Init();
  CLS1_Init();
  SHELL_Init();
  TRG_Init();
  RTOS_Init();

  RTOS_Run();
  for(;;){}
  //TRG_SetTrigger(TRG_LED_BLINK,100,&OnTriggerBlink,0);

/*
  for(;;){
	  //CLS1_SendStr("I rule the world" , CLS1_GetStdio()->stdOut);
	  char c;
	  if(CLS1_KeyPressed()) //simple echo
	  {
		  CLS1_ReadChar(&c);
		  CLS1_SendChar(c);
	  }

	  KEY_Scan();
	  EVNT_HandleEvent(&OnEvent);
  }*/
}

void OnTriggerBlink(void* v)
{
	LED_Red_Neg();
	TRG_SetTrigger(TRG_LED_BLINK,100,&OnTriggerBlink,0);

}

void OnEvent(EVNT_Handle ev)
{
	switch(ev)
	{
	case EVNT_SW1_PRESSED:
		LED_Red_On();
		break;
	case EVNT_SW2_PRESSED:
		LED_Blue_On();
		break;
	case EVNT_SW3_PRESSED:
		LED_Green_On();
		break;
	case EVNT_SW1_LPRESSED:
		LED_Red_Off();
		break;
	case EVNT_SW2_LPRESSED:
		LED_Blue_Off();
		break;
	case EVNT_SW3_LPRESSED:
		LED_Green_Off();
		break;
	case EVNT_SW7_PRESSED:
		LED_Red_Neg();
		break;
	}
}

/*EnterCritical();

LED_Red_On();
WAIT_Waitms(100);
LED_Red_Off();
LED_Blue_On();
WAIT_Waitms(100);
LED_Blue_Off();
LED_Green_On();
WAIT_Waitms(100);
LED_Green_Off();

ExitCritical();
*/

void blabla()
{
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.10]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
