/**
 * \file
 * \brief Non-Volatile memory configuration handling.
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module implements the infrastructure to store configuration data
 * into non-volatile FLASH memory on the microcontroller.
 */

#ifndef CONFIGNVM_H_
#define CONFIGNVM_H_

#include "Platform.h"

#if PL_IS_FRDM
  #define NVMC_FLASH_START_ADDR    0x1FC00 
  /*!< NVRM_Config, start address of configuration data in flash */
#elif PL_IS_ROBO
  #define NVMC_FLASH_START_ADDR    0x7F000 /* NVRM_Config, start address of configuration data in flash */
//  #define NVMC_FLASH_START_ADDR    0x0 /*! \todo Define your address */
  /*!< NVRM_Config, start address of configuration data in flash */
#else
  #error "unknown target?"
#endif

/* macros for identify erased flash memory */
#define NVMC_FLASH_ERASED_UINT8  0xFF
  /*!< erased byte in flash */
#define NVMC_FLASH_ERASED_UINT16 0xFFFF
  /*!< erased word in flash */

#define NVMC_REFLECTANCE_DATA_START_ADDR  (NVMC_FLASH_START_ADDR)
#define NVMC_REFLECTANCE_DATA_SIZE        (8*2*2) /* maximum of 8 sensors (min and max) values with 16 bits */
#define NVMC_REFLECTANCE_END_ADDR         (NVMC_REFLECTANCE_DATA_START_ADDR+NVMC_REFLECTANCE_DATA_SIZE)

#define NVMC_STATUS_DATA_START_ADDR  (NVMC_REFLECTANCE_END_ADDR)
#define NVMC_STATUS_DATA_SIZE        (1)
#define NVMC_STATUS_END_ADDR         (NVMC_STATUS_DATA_START_ADDR+NVMC_STATUS_DATA_SIZE)

/*!
 * \brief Saves the reflectance calibration data
 * \param data Pointer to the data
 * \param dataSize Size of data in bytes
 * \return Error code, ERR_OK if everything is fine
 */
uint8_t NVMC_SaveReflectanceData(void *data, uint16_t dataSize);

/*!
 * \brief Returns the reflectance calibration data
 * \return Pointer to data, or NULL for failure
 */
void *NVMC_GetReflectanceData(void);


uint8_t NVMC_SaveStatus(void *data, uint16_t dataSize);
void *NVMC_GetStatus(void);


/*! \brief Driver initialization  */
void NVMC_Init(void);

/*! \brief Driver de-initialization  */
void NVMC_Deinit(void);

#endif /* CONFIGNVM_H_ */
