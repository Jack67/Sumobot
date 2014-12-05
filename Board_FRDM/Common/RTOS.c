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
#include "Buzzer.h"
#include "Ultrasonic.h"
#include "RNet_App.h"
#include "Radio.h"
#include "RStack.h"
#include "RApp.h"
#include "RPHY.h"
#include "../Generated_Code/MMA1.h"

#define MOTORSPEED			3000
#define MOTORSEARCHSPEED	1000
#define MOTOR_PAUSE			500

#define Z_VALUE				1500

#ifdef PL_BOARD_IS_ROBOT
static portTASK_FUNCTION(Fight, pvParameters)
{
	char fight = 0;
	char search = 1;
	char val = 12;
	uint16_t i;
	uint16_t cm, us;

	ACCEL_Enable();

	//(void)RAPP_SendPayloadDataBlock(&val, 1, RAPP_MSG_TYPE_DATA, 0xFF, 0);

	for(;;) {
		if(fight)
		{
			if(isColorWhite())
			{
				DRV_SetSpeed(-MOTORSPEED,-MOTORSPEED);
				FRTOS1_vTaskDelay(MOTOR_PAUSE);
				DRV_SetSpeed(MOTORSPEED,-MOTORSPEED);
				FRTOS1_vTaskDelay(MOTOR_PAUSE);
				search = 1;
			}
			else if(search)
			{
				DRV_SetSpeed(MOTORSEARCHSPEED,-MOTORSEARCHSPEED);
				us = US_Measure_us();
				cm = US_usToCentimeters(us, 22);
				if(cm < 20)
				{
					DRV_SetSpeed(MOTORSPEED,MOTORSPEED);
					search = 0;
				}
				for(i = 0; i < 30; i++)
				{
					if(isColorWhite())
						break;
					FRTOS1_vTaskDelay(10);
				}
			}
			else
			{
				if(MMA1_GetZ() < Z_VALUE)
				{
					fight = 0;
					DRV_SetSpeed(0,0);
				}
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
				BUZ_Beep(100, 200);
				fight = 1;
				FRTOS1_vTaskDelay(4500);
				BUZ_Beep(500, 500);
				DRV_SetSpeed(MOTORSPEED,-MOTORSPEED);
				FRTOS1_vTaskDelay(MOTOR_PAUSE);
				DRV_SetSpeed(-MOTORSPEED,-MOTORSPEED);
				FRTOS1_vTaskDelay(MOTOR_PAUSE);
			}
			FRTOS1_vTaskDelay(1);
		}
	}
}

#endif
/*
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
*/

void RTOS_Run(void)
{
  FRTOS1_vTaskStartScheduler();
}

void RTOS_Init(void)
{
#ifdef PL_BOARD_IS_ROBOT
 if (FRTOS1_xTaskCreate(Fight, (signed portCHAR *)"Fight", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
  {
    for(;;){}
  }
#endif

#ifdef PL_BOARD_IS_FRDM
 /*
 if (FRTOS1_xTaskCreate(T3, (signed portCHAR *)"T3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
 {
     for(;;){}
 }
 if (FRTOS1_xTaskCreate(T4, (signed portCHAR *)"T4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
 {
     for(;;){}
 }
 */
#endif

}

void RTOS_Deinit(void) {
}

#endif /* PL_HAS_RTOS */
