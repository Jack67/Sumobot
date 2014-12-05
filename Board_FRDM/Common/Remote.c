/**
 * \file
 * \brief Radio Remote Module
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module to handle accelerometer values passed over the Radio.
 */

#include "Platform.h" /* interface to the platform */
#if PL_HAS_REMOTE
#include "Remote.h"
#include "FRTOS1.h"
#include "CLS1.h"
#include "UTIL1.h"
#if PL_HAS_ACCEL
  #include "Accel.h"
#endif
#if PL_HAS_PID
  #include "PID.h"
#endif
#if PL_HAS_MOTOR
  #include "Motor.h"
#endif
#if PL_HAS_RADIO
  #include "RNet_App.h"
  #include "RNet_AppConfig.h"
#endif
#if PL_HAS_DRIVE
  #include "Drive.h"
#endif
#if PL_HAS_LED
  #include "LED.h"
#endif
#if PL_HAS_JOYSTICK
  #include "AD1.h"
#endif

#include "Keys.h"
#include "Event.h"

static bool REMOTE_isOn = FALSE;
static bool REMOTE_isVerbose = FALSE;

#if PL_APP_ACCEL_CONTROL_SENDER
static int8_t ToSigned8Bit(uint16_t val) {
  int tmp;

  tmp = ((int)((val>>8)&0xFF))-127;
  if (tmp<-128) {
    tmp = -128;
  } else if (tmp>127) {
    tmp = 127;
  }
  return (int8_t)tmp;
}

static uint8_t APP_GetXY(uint16_t *x, uint16_t *y, int8_t *x8, int8_t *y8) {
  uint8_t res;
  uint16_t values[2];
/*
  res = AD1_Measure(TRUE);
  if (res!=ERR_OK) {
    return res;
  }
  res = AD1_GetValue16(&values[0]);
  if (res!=ERR_OK) {
    return res;
  }
  *x = values[0];
  *y = values[1];
  // transform into -128...127 with zero as mid position
  *x8 = ToSigned8Bit(values[0]);
  *y8 = ToSigned8Bit(values[1]);*/
  values[0] = 0;
  values[1] = 0;
  return ERR_OK;
}

static portTASK_FUNCTION(RemoteTask, pvParameters)
{
  (void)pvParameters;
  for(;;) {
    if (REMOTE_isOn)
    {
#if PL_HAS_ACCEL
      uint8_t buf[7];
      int16_t x, y, z;

      /* send periodically accelerometer messages */
      ACCEL_GetValues(&x, &y, &z);
      buf[0] = (uint8_t)(x&0xFF);
      buf[1] = (uint8_t)(x>>8);
      buf[2] = (uint8_t)(y&0xFF);
      buf[3] = (uint8_t)(y>>8);
      buf[4] = (uint8_t)(z&0xFF);
      buf[5] = (uint8_t)(z>>8);
      buf[6] = 0;


      KEY_Scan();
      if (EVNT_EventIsSet(EVNT_SW1_PRESSED))
      {
    	  EVNT_ClearEvent(EVNT_SW1_PRESSED);
    	  buf[6] |= 0x01;
      }
      if (EVNT_EventIsSet(EVNT_SW2_PRESSED))
      {
          EVNT_ClearEvent(EVNT_SW2_PRESSED);
          buf[6] |= 0x02;
      }
      if (EVNT_EventIsSet(EVNT_SW3_PRESSED))
      {
          EVNT_ClearEvent(EVNT_SW3_PRESSED);
          buf[6] |= 0x04;
      }
      if (EVNT_EventIsSet(EVNT_SW4_PRESSED))
      {
          EVNT_ClearEvent(EVNT_SW4_PRESSED);
          buf[6] |= 0x08;
      }
      if (EVNT_EventIsSet(EVNT_SW5_PRESSED))
      {
          EVNT_ClearEvent(EVNT_SW5_PRESSED);
          buf[6] |= 0x10;
      }
      if (EVNT_EventIsSet(EVNT_SW6_PRESSED))
      {
          EVNT_ClearEvent(EVNT_SW6_PRESSED);
          buf[6] |= 0x20;
      }
      if (EVNT_EventIsSet(EVNT_SW7_PRESSED))
      {
          EVNT_ClearEvent(EVNT_SW7_PRESSED);
          buf[6] |= 0x40;
      }



      if (REMOTE_isVerbose)
      {
        uint8_t txtBuf[16];
        CLS1_ConstStdIOTypePtr io = CLS1_GetStdio();

        CLS1_SendStr((unsigned char*)"TX: x: ", io->stdOut);
        CLS1_SendNum16s(x, io->stdOut);
        CLS1_SendStr((unsigned char*)" y: ", io->stdOut);
        CLS1_SendNum16s(y, io->stdOut);
        CLS1_SendStr((unsigned char*)" z: ", io->stdOut);
        CLS1_SendNum16s(z, io->stdOut);
        CLS1_SendStr((unsigned char*)" t: ", io->stdOut);
        CLS1_SendNum8s(buf[6], io->stdOut);
        CLS1_SendStr((unsigned char*)" to addr 0x", io->stdOut);
        txtBuf[0] = '\0';
  #if RNWK_SHORT_ADDR_SIZE==1
        UTIL1_strcatNum8Hex(txtBuf, sizeof(txtBuf), RNETA_GetDestAddr());
  #else
        UTIL1_strcatNum16Hex(txtBuf, sizeof(txtBuf), RNETA_GetDestAddr());
  #endif
        UTIL1_strcat(txtBuf, sizeof(txtBuf), (unsigned char*)"\r\n");
        CLS1_SendStr(txtBuf, io->stdOut);
      }







      (void)RAPP_SendPayloadDataBlock(buf, sizeof(buf), RAPP_MSG_TYPE_ACCEL, RNETA_GetDestAddr(), RPHY_PACKET_FLAGS_REQ_ACK);
      //LED1_Neg();
#endif
#if PL_HAS_WATCHDOG
      WDT_IncTaskCntr(WDT_TASK_ID_REMOTE, 200);
#endif
      FRTOS1_vTaskDelay(200/portTICK_RATE_MS);
    }
    else
    {
#if PL_HAS_WATCHDOG
      WDT_IncTaskCntr(WDT_TASK_ID_REMOTE, 1000);
#endif
      FRTOS1_vTaskDelay(200/portTICK_RATE_MS);
    }
  } /* for */
}
#endif

#if PL_HAS_MOTOR
static void REMOTE_HandleMsg(int16_t x, int16_t y, int16_t z) {
  #define SCALE_DOWN 30
  #define MIN_VALUE  250 /* values below this value are ignored */
  #define DRIVE_DOWN 1

  if (!REMOTE_isOn) {
    return;
  }
  if (y>950 || y<-950) { /* have a way to stop motor: turn SRB USB port side up or down */
#if PL_HAS_DRIVE
    DRV_SetSpeed(0, 0);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), 0);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), 0);
#endif
  } else if ((y>MIN_VALUE || y<-MIN_VALUE) && (x>MIN_VALUE || x<-MIN_VALUE)) { /* x: speed, y: direction */
    int16_t speed, speedL, speedR;
    
    speed = x/SCALE_DOWN;
    if (y<0) {
      if (speed<0) {
        speedR = speed+(y/SCALE_DOWN);
      } else {
        speedR = speed-(y/SCALE_DOWN);
      }
      speedL = speed;
    } else {
      speedR = speed;
      if (speed<0) {
        speedL = speed-(y/SCALE_DOWN);
      } else {
        speedL = speed+(y/SCALE_DOWN);
      }
    }
#if PL_HAS_DRIVE
    DRV_SetSpeed(speedL*SCALE_DOWN/DRIVE_DOWN, speedR*SCALE_DOWN/DRIVE_DOWN);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), speedL);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), speedR);
#endif
  } else if (x>100 || x<-100) { /* speed */
#if PL_HAS_DRIVE
    DRV_SetSpeed(-x, -x);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), -x/SCALE_DOWN);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), -x/SCALE_DOWN);
#endif
  } else if (y>100 || y<-100) { /* direction */
#if PL_HAS_DRIVE
    DRV_SetSpeed(-y/DRIVE_DOWN, y/DRIVE_DOWN);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), -y/SCALE_DOWN);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), (y/SCALE_DOWN));
#endif
  } else { /* device flat on the table? */
#if PL_HAS_DRIVE
    DRV_SetSpeed(0, 0);
#else
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), 0);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), 0);
#endif
  }
}
#endif

uint8_t REMOTE_HandleRemoteRxMessage(RAPP_MSG_Type type, uint8_t size, uint8_t *data, RNWK_ShortAddrType srcAddr, bool *handled, RPHY_PacketDesc *packet) {
#if PL_HAS_SHELL
  uint8_t buf[16];
  CLS1_ConstStdIOTypePtr io = CLS1_GetStdio();
#endif
  int16_t x, y, z;
  
  (void)size;
  (void)packet;
  switch(type) {
    case RAPP_MSG_TYPE_ACCEL: /* Rx of message, <type><size><data */
      *handled = TRUE;
      /* get data value */
      x = (data[0])|(data[1]<<8);
      y = (data[2])|(data[3]<<8);
      z = (data[4])|(data[5]<<8);
      /*
      if (REMOTE_isVerbose) {
        CLS1_SendStr((unsigned char*)"RX: x: ", io->stdOut);
        CLS1_SendNum16s(x, io->stdOut);
        CLS1_SendStr((unsigned char*)" y: ", io->stdOut);
        CLS1_SendNum16s(y, io->stdOut);
        CLS1_SendStr((unsigned char*)" z: ", io->stdOut);
        CLS1_SendNum16s(z, io->stdOut);
        CLS1_SendStr((unsigned char*)" from addr 0x", io->stdOut);
        buf[0] = '\0';
  #if RNWK_SHORT_ADDR_SIZE==1
        UTIL1_strcatNum8Hex(buf, sizeof(buf), srcAddr);
  #else
        UTIL1_strcatNum16Hex(buf, sizeof(buf), srcAddr);
  #endif
        UTIL1_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
        CLS1_SendStr(buf, io->stdOut);
      }
      */
#if PL_HAS_MOTOR
      REMOTE_HandleMsg(x, y, z);
#endif
      return ERR_OK;
    default:
      break;
  } /* switch */
  return ERR_OK;
}

static void REMOTE_PrintHelp(const CLS1_StdIOType *io) {
  CLS1_SendHelpStr((unsigned char*)"remote", (unsigned char*)"Group of remote commands\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows remote help or status\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  verbose on|off", (unsigned char*)"Turns the verbose mode on or off\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  on|off", (unsigned char*)"Turns the remote on or off\r\n", io->stdOut);
}

static void REMOTE_PrintStatus(const CLS1_StdIOType *io) {
  CLS1_SendStatusStr((unsigned char*)"Remote", (unsigned char*)"\r\n", io->stdOut);
  CLS1_SendStatusStr((unsigned char*)"  remote", REMOTE_isOn?(unsigned char*)"on\r\n":(unsigned char*)"off\r\n", io->stdOut);
  CLS1_SendStatusStr((unsigned char*)"  verbose", REMOTE_isVerbose?(unsigned char*)"on\r\n":(unsigned char*)"off\r\n", io->stdOut);
}

uint8_t REMOTE_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
  uint8_t res = ERR_OK;

  if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"remote help")==0) {
    REMOTE_PrintHelp(io);
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"remote status")==0) {
    REMOTE_PrintStatus(io);
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote on")==0) {
    REMOTE_isOn = TRUE;
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote off")==0) {
#if PL_HAS_MOTOR
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_LEFT), 0);
    MOT_SetSpeedPercent(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), 0);
#endif
    REMOTE_isOn = FALSE;
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote verbose on")==0) {
    REMOTE_isVerbose = TRUE;
    *handled = TRUE;
  } else if (UTIL1_strcmp((char*)cmd, (char*)"remote verbose off")==0) {
    REMOTE_isVerbose = FALSE;
    *handled = TRUE;
  }
  return res;
}

bool REMOTE_GetOnOff(void) {
  return REMOTE_isOn;
}

void REMOTE_SetOnOff(bool on) {
  REMOTE_isOn = on;
}

void REMOTE_Deinit(void) {
  /* nothing to do */
}

/*! \brief Initializes module */
void REMOTE_Init(void) {
  REMOTE_isOn = TRUE;
  REMOTE_isVerbose = TRUE;
#if PL_APP_ACCEL_CONTROL_SENDER
  if (FRTOS1_xTaskCreate(RemoteTask, "Remote", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error */
  }
#endif
}
#endif /* PL_HAS_REMOTE */

