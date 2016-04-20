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
#define uint32_t unsigned int


/*TODO: better description than this : define max amount of data within a data_unit, if we get 2 sensors and each of them has 2 data to collect, it should be 4 */
/* It is vital to make this number correct for future coders in real implementation */
#define MAX_NUM_DATA 4

/* used for device/sensor management and data_unit organizing*/
enum
{
	TYPE_BMP280,
	TYPE_OTHERSENSOR,
	TYPE_BASE_DEVICE,
	TYPE_BASE_SENSOR,
	TYPE_SENSOR
};

//prototypes//
unsigned int check_crc(uint16_t, uint8_t);

#endif /* COMMON_H_ */