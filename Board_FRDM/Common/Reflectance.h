/**
 * \file
 * \brief Reflectance sensor driver interface.
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module implements a driver for the reflectance sensor array.
 */

#ifndef REFLECTANCE_H_
#define REFLECTANCE_H_

#include "Platform.h"
#if PL_HAS_LINE_SENSOR

#if PL_HAS_SHELL
  #include "CLS1.h"
  
  /*!
   * \brief Shell parser routine.
   * \param cmd Pointer to command line string.
   * \param handled Pointer to status if command has been handled. Set to TRUE if command was understood.
   * \param io Pointer to stdio handle
   * \return Error code, ERR_OK if everything was ok.
   */
  uint8_t REF_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);

  typedef enum {
    REF_STATE_INIT,
    REF_STATE_NOT_CALIBRATED,
    REF_STATE_START_CALIBRATION,
    REF_STATE_CALIBRATING,
    REF_STATE_STOP_CALIBRATION,
    REF_STATE_READY
  } RefStateType;

  #define REF_PARSE_COMMAND_ENABLED 1
#else
  #define REF_PARSE_COMMAND_ENABLED 0
#endif

  char isColorWhite(void);
  char isRobotFlipped(void);
  RefStateType getRefState(void);
/*!
 * \brief Driver Deinitialization.
 */
void REF_Deinit(void);

/*!
 * \brief Driver Initialization.
 */
void REF_Init(void);

#endif /* PL_HAS_LINE_SENSOR */

#endif /* REFLECTANCE_H_ */
