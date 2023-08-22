/*
 * bh1750.h
 *
 *	The MIT License.
 *  Created on: 02.12.2018
 *      Author: Mateusz Salamon
 *		www.msalamon.pl
 *		mateusz@msalamon.pl
 *
 *	https://msalamon.pl/pomiar-natezenia-swiatla-z-wykorzystaniem-stm32/
 *	https://github.com/lamik/Light_Sensors_STM32
 */

#ifndef BH1750_H_
#define BH1750_H_

#include <Arduino.h>


//#define BH1750_ADDRESS			(0x23<<1)
#define BH1750_ADDRESS			(0x23)

#define	BH1750_POWER_DOWN		0x00
#define	BH1750_POWER_ON			0x01
#define	BH1750_RESET			0x07
#define	BH1750_DEFAULT_MTREG	69

#define BH1750_CONVERSION_FACTOR	1.2

typedef enum {
	BH1750_OK		= 0,
	BH1750_ERROR	= 1
} BH1750_STATUS;

typedef enum
{
    CONTINUOUS_HIGH_RES_MODE  	= 0x10,
    CONTINUOUS_HIGH_RES_MODE_2 	= 0x11,
    CONTINUOUS_LOW_RES_MODE 	= 0x13,
    ONETIME_HIGH_RES_MODE 		= 0x20,
    ONETIME_HIGH_RES_MODE_2 	= 0x21,
    ONETIME_LOW_RES_MODE 		= 0x23
}bh1750_mode;

BH1750_STATUS BH1750_Init(void);
BH1750_STATUS BH1750_Reset(void);
BH1750_STATUS BH1750_PowerState(uint8_t PowerOn);
BH1750_STATUS BH1750_SetMtreg(uint8_t Mtreg);
BH1750_STATUS BH1750_SetMode(bh1750_mode Mode);
BH1750_STATUS BH1750_TriggerManualConversion(void);
float BH1750_ReadLight(void);

#endif /* BH1750_H_ */
