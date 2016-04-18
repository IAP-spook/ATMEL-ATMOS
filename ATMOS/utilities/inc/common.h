/*
 * common.h
 *
 * Created: 10/10/2014 11:47:44 AM
 *  Author: Camden
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include "avr/io.h"
#include <util/delay.h>

#define SHIFTED_DIVISOR	0x988000

/* used for device/sensor management */
enum
{
	TYPE_BASE_DEVICE,
	TYPE_BASE_SENSOR,
	TYPE_SENSOR
};

//prototypes//
unsigned int check_crc(uint16_t, uint8_t);

#endif /* COMMON_H_ */