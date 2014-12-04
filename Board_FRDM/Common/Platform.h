/*
 * Platform.h
 *
 *  Created on: 25.09.2014
 *      Author: Elia
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "PE_Types.h" //bool -> unsigned char
#include <stddef.h> //includes NULL

#ifdef PL_BOARD_IS_FRDM
	#define PL_HAS_EVENTS 	1
	#define PL_HAS_TIMER 	1
	#define PL_HAS_KEYS 	1
	#define PL_NOF_KEYS 	7
	#define PL_HAS_TRIGGER 	1

	#define PL_HAS_LED_RED	1
	#define PL_HAS_LED_BLUE	0
	#define PL_HAS_LED_GREEN	1

	#define PL_HAS_DEBOUNCE 1

	#define PL_HAS_RTOS 1

	#define PL_HAS_SHELL 1
	#define PL_HAS_BLUETOOTH 0
	#define PL_HAS_USB_CDC 1
	#define CLS1_DEFAULT_SERIAL 1

	#define PL_HAS_SHELL_QUEUE 1
	#define PL_HAS_SEMAPHORE 1

	#define PL_HAS_LINE_SENSOR 0

	#define PL_HAS_MOTOR 0

	#define PL_HAS_CONFIG_NVM 1
	#define PL_IS_FRDM 1

	#define PL_HAS_QUAD_CALIBRATION 0
	#define PL_HAS_MPC4728	0

	#define PL_HAS_MOTOR_TACHO 0
	#define PL_HAS_MOTOR_QUAD 0
	#define PL_HAS_DRIVE 0
	#define PL_HAS_PID 0

	#define PL_HAS_ULTRASONIC 0

	#define PL_HAS_ACCEL 1
	#define PL_HAS_RADIO 1

	#define PL_HAS_REMOTE 1
	#define PL_APP_ACCEL_CONTROL_SENDER 1
#endif

#ifdef PL_BOARD_IS_ROBOT
	#define PL_HAS_EVENTS 	1
	#define PL_HAS_TIMER 	1
	#define PL_HAS_KEYS 	1
	#define PL_NOF_KEYS 	1
	#define PL_HAS_TRIGGER 	1

	#define PL_HAS_LED_RED	1
	#define PL_HAS_LED_BLUE	0
	#define PL_HAS_LED_GREEN	1

	#define PL_HAS_BUZZER 1

	#define PL_HAS_DEBOUNCE 1

	#define PL_HAS_RTOS 1

	#define PL_HAS_SHELL 1
	#define PL_HAS_BLUETOOTH 1
	#define PL_HAS_USB_CDC 1

	#define PL_HAS_SHELL_QUEUE 1

	#define PL_HAS_SEMAPHORE 1

	#define PL_HAS_LINE_SENSOR 1

	#define PL_HAS_MOTOR 1

	#define PL_HAS_CONFIG_NVM 0
	#define PL_IS_ROBO 1

	#define PL_HAS_QUAD_CALIBRATION 1
	#define PL_HAS_MPC4728	1

	#define PL_HAS_MOTOR_TACHO 1
	#define PL_HAS_MOTOR_QUAD 1
	#define PL_HAS_DRIVE 1
	#define PL_HAS_PID 1

	#define PL_HAS_ULTRASONIC 1

	#define PL_HAS_ACCEL 1

	#define PL_HAS_RADIO 1

	#define PL_HAS_REMOTE 0
#endif




void platform_Init(void);
void platform_Deinit(void);

#endif /* PLATFORM_H_ */
