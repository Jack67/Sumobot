/*
 * AppFrdm.c
 *
 *  Created on: 04.12.2014
 *      Author: Elia
 */

#include "Platform.h"
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






    	FRTOS1_vTaskDelay(1000);
    	status = (status+1) % 3;
	}
}



void APPFRDM_Init(void)
{
	 if (FRTOS1_xTaskCreate(T3, (signed portCHAR *)"T3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS)
	 {
	     for(;;){}
	 }

}
