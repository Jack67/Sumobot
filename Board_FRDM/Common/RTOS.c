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
#include "NVM_Config.h"
#include "motor.h"
#include "Accel.h"
#include "Drive.h"

#define MOTORSPEED			4000
#define MOTORSEARCHSPEED	1000
#define MOTOR_PAUSE			250

#ifdef PL_BOARD_IS_ROBOT
xSemaphoreHandle WhiteArea = NULL;

static portTASK_FUNCTION(Reflectance, pvParameters) {
	char fight = 0;

	for(;;) {
		if(fight)
		{
			if(isColorWhite())
			{
				xSemaphoreGive(WhiteArea);
				if (xSemaphoreTake(WhiteArea, portMAX_DELAY)==pdTRUE)
				{
					DRV_SetSpeed(-MOTORSPEED,-MOTORSPEED);
					FRTOS1_vTaskDelay(MOTOR_PAUSE);
					DRV_SetSpeed(MOTORSPEED,-MOTORSPEED);
					FRTOS1_vTaskDelay(MOTOR_PAUSE);
				}
				xSemaphoreGive(WhiteArea);
			}
		}
		else
		{
			KEY_Scan();
			if (EVNT_EventIsSet(EVNT_SW1_LPRESSED))
			{
				LED_Red_Neg();
				EVNT_ClearEvent(EVNT_SW1_LPRESSED);
				#if PL_HAS_LINE_SENSOR == 1
					EVNT_SetEvent(EVNT_REF_START_STOP_CALIBRATION);
				#endif
			}
			if (EVNT_EventIsSet(EVNT_SW1_PRESSED))
			{
				EVNT_ClearEvent(EVNT_SW1_PRESSED);
				BUZ_Beep(100, 1000);
				fight = fight % 1;
				FRTOS1_vTaskDelay(5000);
				BUZ_Beep(500, 500);
			}
			FRTOS1_vTaskDelay(1);
		}
	}
}
#endif

#ifdef PL_BOARD_IS_ROBOT
static portTASK_FUNCTION(Ultrasonic, pvParameters) {
	char search = 1;
	uint16_t value;
	ACCEL_LowLevelInit();
	for(;;)
	{
		if (xSemaphoreTake(WhiteArea, portMAX_DELAY)==pdTRUE)
		{
			if(search)
			{
				if(US_GetLastCentimeterValue() < 5)
				{
					search = 0;
					DRV_SetSpeed(MOTORSPEED,MOTORSPEED);
				}
				else
				{
					DRV_SetSpeed(MOTORSEARCHSPEED,-MOTORSEARCHSPEED);
				}
			}
			else
			{
				if(US_GetLastCentimeterValue() > 5)
				{
					search = 1;
					DRV_SetSpeed(MOTORSEARCHSPEED,-MOTORSEARCHSPEED);
				}
				else
				{
					DRV_SetSpeed(MOTORSPEED,MOTORSPEED);
				}
			}
		}
		FRTOS1_vTaskDelay(10);
	}
}
#endif



#ifdef PL_BOARD_IS_FRDM
char status = 0;
static portTASK_FUNCTION(T3, pvParameters)
{

	char* pstatus = (char*)NVMC_GetStatus();
	ACCEL_LowLevelInit();
	if(pstatus != NULL)
	{
		status = *pstatus;
	}

	for(;;)
	{
		switch(status)
		{
		case 0:
			LED_Red_On();
			//LED_Blue_Off();
			LED_Green_Off();
			break;
		case 1:
			LED_Red_Off();
			//LED_Blue_On();
			LED_Green_Off();
			break;
		case 2:
			LED_Red_Off();
			//LED_Blue_Off();
			LED_Green_On();
			break;
		}

    	FRTOS1_vTaskDelay(2500);
    	status = (status+1) % 3;
	}
}

static portTASK_FUNCTION(T4, pvParameters)
{
	for(;;)
	{
		KEY_Scan();
		if (EVNT_EventIsSet(EVNT_SW1_PRESSED))
		{
		     EVNT_ClearEvent(EVNT_SW1_PRESSED);
		     NVMC_SaveStatus(&status,1);
		}

		FRTOS1_vTaskDelay(200);
	}

}

#endif


void RTOS_Run(void)
{
  FRTOS1_vTaskStartScheduler();
}

void RTOS_Init(void)
{
#ifdef PL_BOARD_IS_ROBOT
	vSemaphoreCreateBinary(WhiteArea);
 if (FRTOS1_xTaskCreate(Reflectance, (signed portCHAR *)"Reflectance", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
  {
    for(;;){}
  }

  if (FRTOS1_xTaskCreate(Ultrasonic, (signed portCHAR *)"Ultrasonic", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
  {
      for(;;){}
  }
#endif

#ifdef PL_BOARD_IS_FRDM
 if (FRTOS1_xTaskCreate(T3, (signed portCHAR *)"T3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
 {
     for(;;){}
 }
 if (FRTOS1_xTaskCreate(T4, (signed portCHAR *)"T4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
 {
     for(;;){}
 }
#endif

}

void RTOS_Deinit(void) {
}

#endif /* PL_HAS_RTOS */
