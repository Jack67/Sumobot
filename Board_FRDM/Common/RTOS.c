/**
 * \file
 * \brief Real Time Operating System (RTOS) main program.
 * \author Erich Styger, erich.styger@hslu.ch
 */

#include "Platform.h"
#if PL_HAS_RTOS
#include "RTOS.h"
#include "FRTOS1.h"
#include "LED.h"
#include "Shell.h"
#include "Reflectance.h"
#include "Event.h"
#include "Keys.h"

static portTASK_FUNCTION(T1, pvParameters) {
  for(;;) {
	  KEY_Scan();
	  if (EVNT_EventIsSet(EVNT_SW1_PRESSED)) {
	     EVNT_ClearEvent(EVNT_SW1_PRESSED);
	     EVNT_SetEvent(EVNT_REF_START_STOP_CALIBRATION);
	  }
	  FRTOS1_vTaskDelay(200);
  }
}

static portTASK_FUNCTION(T2, pvParameters) {
  for(;;) {
    LED_Green_Neg();
    SHELL_SendString("500ms\r\n");
    FRTOS1_vTaskDelay(500);
  }
}

void RTOS_Run(void)
{
  FRTOS1_vTaskStartScheduler();
}

void RTOS_Init(void)
{
  /*! \todo Add tasks here */
 if (FRTOS1_xTaskCreate(T1, (signed portCHAR *)"T1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
  {
    for(;;){} /* error */
  }
 /*
  if (FRTOS1_xTaskCreate(T2, (signed portCHAR *)"T2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
  {
      for(;;){} /* error */
 //  }

}

void RTOS_Deinit(void) {
}

#endif /* PL_HAS_RTOS */
