 /**
 * \file
 * \brief Drive interface.
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module implements a simple 'drive' task which is responsible to move the robot around.
 */
#include "Platform.h"
#if PL_HAS_DRIVE
#include "Drive.h"
#include "FRTOS1.h"
#include "Pid.h"
#include "Tacho.h"
#include "Motor.h"
#if PL_HAS_RTOS_TRACE
  #include "RTOSTRC1.h"
#endif
#include "Q4CLeft.h"
#include "Q4CRight.h"

static volatile bool DRV_SpeedOn = FALSE;
static volatile bool PosDriveOn = FALSE;
static int32_t DRV_SpeedLeft, DRV_SpeedRight;
static int32_t DRV_PosLeft, DRV_PosRight;

void DRV_EnableDisable(bool enable) {
  DRV_SpeedOn = enable;
}

void PosDriveEnable(bool enable)
{
		DRV_SpeedOn=enable;//Speed Drive disable
		PosDriveOn=enable;
		PID_Start();
}

void DRV_SetPos(int32_t left, int32_t right) {
  DRV_PosLeft = left;
  DRV_PosRight = right;
}

void DRV_SetSpeed(int32_t left, int32_t right) {
  DRV_SpeedLeft = left;
  DRV_SpeedRight = right;
}

static portTASK_FUNCTION(DriveTask, pvParameters) {
  (void)pvParameters; /* parameter not used */
  bool prevOn;

  prevOn = DRV_SpeedOn;
#if PL_HAS_RTOS_TRACE
  //usrEvent = xTraceOpenLabel("drive");
#endif
  for(;;) {
#if PL_HAS_RTOS_TRACE
    //RTOSTRC1_vTraceUserEvent(usrEvent);
#endif
	  if(PosDriveOn)
	  {
		  PID_Pos(Q4CLeft_GetPos(), DRV_PosLeft, TRUE);//Left
		  PID_Pos(Q4CRight_GetPos(), DRV_PosRight, FALSE);//Right
	  }
	  else
	  {
		/*! \todo extend this for your own needs and with a position PID */
		TACHO_CalcSpeed();
		if (prevOn && !DRV_SpeedOn) { /* turned off */
		  MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), 0);
		  MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), 0);
		  PID_Start(); /* reset values */
		} else if (DRV_SpeedOn) {
		  PID_Speed(TACHO_GetSpeed(TRUE), DRV_SpeedLeft, TRUE); /* left */
		  PID_Speed(TACHO_GetSpeed(FALSE), DRV_SpeedRight, FALSE); /* right */
		}
		prevOn = DRV_SpeedOn;
	  }
    FRTOS1_vTaskDelay(2/portTICK_RATE_MS);
  } /* for */
}

#if PL_HAS_SHELL
static void DRV_PrintStatus(const CLS1_StdIOType *io) {
  uint8_t buf[32];
  
  CLS1_SendStatusStr((unsigned char*)"drive", (unsigned char*)"\r\n", io->stdOut);
  CLS1_SendStatusStr((unsigned char*)"  speed", DRV_SpeedOn?(unsigned char*)"on\r\n":(unsigned char*)"off\r\n", io->stdOut);
  CLS1_SendStatusStr((unsigned char*)"  pos", PosDriveOn?(unsigned char*)"on\r\n":(unsigned char*)"off\r\n", io->stdOut);
  buf[0]='\0';
  UTIL1_strcatNum32s(buf, sizeof(buf), DRV_SpeedLeft);
  UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
  CLS1_SendStatusStr((unsigned char*)"  speed L", buf, io->stdOut);
  buf[0]='\0';
  UTIL1_strcatNum32s(buf, sizeof(buf), DRV_SpeedRight);
  UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
  CLS1_SendStatusStr((unsigned char*)"  speed R", buf, io->stdOut);
  buf[0]='\0';
  UTIL1_strcatNum32s(buf, sizeof(buf), DRV_PosLeft);
  UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
  CLS1_SendStatusStr((unsigned char*)"  pos L", buf, io->stdOut);
  buf[0]='\0';
  UTIL1_strcatNum32s(buf, sizeof(buf), DRV_PosRight);
  UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
  CLS1_SendStatusStr((unsigned char*)"  pos R", buf, io->stdOut);
}

static void DRV_PrintHelp(const CLS1_StdIOType *io) {
  CLS1_SendHelpStr((unsigned char*)"drive", (unsigned char*)"Group of drive commands\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows help or status\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  speed (on|off)", (unsigned char*)"Turns speed pid on or ff\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  speed (L|R) <value>", (unsigned char*)"Sets speed value\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  pos (on|off)", (unsigned char*)"Turns pos pid on or ff\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  pos (L|R) <value>", (unsigned char*)"Sets pos value\r\n", io->stdOut);
}

uint8_t DRV_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
  uint8_t res = ERR_OK;
  const uint8_t *p;
  int32_t val32;

  if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"drive help")==0) {
    DRV_PrintHelp(io);
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"drive status")==0) {
    DRV_PrintStatus(io);
    *handled = TRUE;
  } else if (UTIL1_strncmp((char*)cmd, (char*)"drive speed L", sizeof("drive speed L")-1)==0) {
    p = cmd+sizeof("drive speed L");
    if (UTIL1_ScanDecimal32sNumber(&p, &val32)==ERR_OK) {
      DRV_SpeedLeft = val32;
      *handled = TRUE;
    } else {
      res = ERR_FAILED;
    }
  } else if (UTIL1_strncmp((char*)cmd, (char*)"drive speed R", sizeof("drive speed R")-1)==0) {
    p = cmd+sizeof("drive speed R");
    if (UTIL1_ScanDecimal32sNumber(&p, &val32)==ERR_OK) {
      DRV_SpeedRight = val32;
      *handled = TRUE;
    } else {
      res = ERR_FAILED;
    }
  }
    else if (UTIL1_strncmp((char*)cmd, (char*)"drive pos L", sizeof("drive pos L")-1)==0) {
        p = cmd+sizeof("drive pos L");
        if (UTIL1_ScanDecimal32sNumber(&p, &val32)==ERR_OK) {
          DRV_PosLeft = val32;
          *handled = TRUE;
        } else {
          res = ERR_FAILED;
        }
  }
     else if (UTIL1_strncmp((char*)cmd, (char*)"drive pos R", sizeof("drive pos R")-1)==0) {
          p = cmd+sizeof("drive pos R");
          if (UTIL1_ScanDecimal32sNumber(&p, &val32)==ERR_OK) {
          DRV_PosRight = val32;
             *handled = TRUE;
            } else {
                  res = ERR_FAILED;
            }
     }
  else if (UTIL1_strcmp((char*)cmd, (char*)"drive speed on")==0)
  {
    DRV_EnableDisable(TRUE);
    *handled = TRUE;
  }
  else if (UTIL1_strcmp((char*)cmd, (char*)"drive speed off")==0)
  {
    DRV_EnableDisable(FALSE);
    *handled = TRUE;
  }
  else if (UTIL1_strcmp((char*)cmd, (char*)"drive pos on")==0)
    {
	  PosDriveEnable(TRUE);
      *handled = TRUE;
    }
  else if (UTIL1_strcmp((char*)cmd, (char*)"drive pos off")==0)
     {
 	  PosDriveEnable(FALSE);
       *handled = TRUE;
     }
  return res;
}
#endif /* PL_HAS_SHELL */

void DRV_Deinit(void) {
   /* nothing needed */
}

void DRV_Init(void) {
  DRV_EnableDisable(TRUE);
  DRV_SpeedLeft = 0;
  DRV_SpeedRight = 0;
  if (FRTOS1_xTaskCreate(
        DriveTask,  /* pointer to the task */
        "Drive", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS) {
    /*lint -e527 */
    for(;;){} /* error! probably out of memory */
    /*lint +e527 */
  }
}

#endif /* PL_HAS_DRIVE */
