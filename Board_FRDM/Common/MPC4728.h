/*
 * MPC4728.h
 *
 *  Created on: Jan 30, 2014
 *      Author: tastyger
 */
#ifndef _MPC4728_H__
#define _MPC4728_H__

#include "Platform.h"

#if PL_HAS_SHELL
  #include "CLS1.h"
  uint8_t MPC4728_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
#endif

#define MPC4728_MAX_DAC_VAL  0xfff  /* 12bit */


/* Single Write DAC Input Register (EEPROM not updated) */
uint8_t MPC4728_FastWriteDAC(uint8_t channel, uint16_t val);

/*!
 * \brief Fast Mode Write: updates all 4 channels, but does not affect EEPROM
 */
uint8_t MPC4728_FastWriteAllDAC(uint16_t dac[4], size_t dacSize, uint8_t pd[4], size_t pdSize);

/* writes DAC register and changes EEPROM */
uint8_t MPC4728_WriteDACandEE(uint8_t channel, uint16_t val);

/*!
 * \brief Returns TRUE if device is busy (writing EEPROM). Uses RDY pin if available, otherwise always returns FALSE
 * \return TRUE of busy, FALSE otherwise.
 */
bool MPC4728_IsBusy(void);

/*!
 * \brief Driver de-intitialization.
 */
void MPC4728_Deinit(void);

/*!
 * \brief Driver intitialization.
 */
void MPC4728_Init(void);


#endif /* _MPC4728_H__ */
