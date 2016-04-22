/**
 * \file common.h
 *
 * \brief Declaration of miscellaneous definitions, enumerators, and functions.
 *
 * Created: 10/10/2014 11:47:44 AM
 *  Author: Camden and Anxin Bai
 */ 
#pragma once


#include <stdint.h>
#include "avr/io.h"
#include <util/delay.h>

#define SHIFTED_DIVISOR	0x988000
#define uint32_t unsigned int


/*TODO: better description than this : define max amount of data within a data_unit, if we get 2 sensors and each of them has 2 data to collect, it should be 4 */
/* It is vital to make this number correct for future coders in real implementation */
#define MAX_NUM_DATA 4

extern int SensorDataCount;

enum DeviceState { New, Ready, Running, Terminated, Oops };

/* used for device/sensor management and data_unit organizing*/
enum
{
	TYPE_BMP280,
	TYPE_BASE_DEVICE,
	TYPE_BASE_SENSOR,
	TYPE_SENSOR,
	TYPE_STORAGE_DEVICE,
	TYPE_DEVICE
};

//prototypes//
unsigned int check_crc(uint16_t, uint8_t);
