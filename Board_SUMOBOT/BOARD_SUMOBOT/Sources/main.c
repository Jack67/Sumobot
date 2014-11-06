/* ###################################################################
**     Filename    : main.c
**     Project     : BOARD_SUMOBOT
**     Processor   : MK22FX512VLQ12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-10-16, 13:50, # CodeGen: 0
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
#include "CS1.h"
#include "WAIT1.h"
#include "HF1.h"
#include "LED_Green.h"
#include "BitIoLdd1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "SW1.h"
#include "BitIoLdd2.h"
#include "BUZ1.h"
#include "BitIoLdd3.h"
#include "FRTOS1.h"
#include "UTIL1.h"
#include "USB1.h"
#include "USB0.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "CLS1.h"
#include "LED_Red.h"
#include "BitIoLdd4.h"
#include "BT1.h"
#include "Serial1.h"
#include "ASerialLdd1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "../../../Board_FRDM/Common/Platform.h"
#include "../../../Board_FRDM/Common/Event.h"
#include "../../../Board_FRDM/Common/Keys.h"
#include "../../../Board_FRDM/Common/LED.h"
#include "../../../Board_FRDM/Common/Timer.h"
#include "../../../Board_FRDM/Common/Trigger.h"
#include "../../../Board_FRDM/Common/Buzzer.h"
#include "../../../Board_FRDM/Common/RTOS.h"
#include "../../../Board_FRDM/Common/Shell.h"
#include "../../../Board_FRDM/Common/ShellQueue.h"

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
  //LED_Blue_Init();
  LED_Green_Init();
  LED_Red_Init();
  TMR_Init();
  CLS1_Init();
  TRG_Init();
  BUZ_Init();

  SQUEUE_Init();
  SHELL_Init();
  RTOS_Init();
  RTOS_Run();


  //BUZ_Beep(1000,1000);

  //TRG_SetTrigger(TRG_LED_BLINK,100,&OnTriggerBlink,0);

  for(;;){
	  //LED_Green_On();
	  //WAIT1_Waitms(500);
	  //LED_Green_Off();
	  //WAIT1_Waitms(500);
  }

}

void OnTriggerBlink(void* v)
{
	LED_Green_Neg();
	TRG_SetTrigger(TRG_LED_BLINK,100,&OnTriggerBlink,0);
}

void blabla(void){

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
