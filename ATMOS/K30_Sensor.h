/**
 * \file K30_Sensor.h
 *
 * \brief Declaration of K30_Sensor struct, which derived from BaseSensor.
 * 
 *  Author: Cory Martin
 */ 

#ifndef _K30_SENSOR_H_
#define _K30_SENSOR_H_

#include "devices/base-class/inc/Base_Sensor.h"
#include <stdint.h>

#define K30_ADDR			0x68 // 7-bit address
#define K30_I2_RAM_ADDR		0x20
#define K30_I2_EEPROM_ADDR	0x00

#define SHIFTED_DIVISOR	0x988000
 
/** @nam Status Codes
 */
#define K30_WRITE			0xD0
#define K30_READ_RAM		0x22
#define K30_RAM_HI_BYTE		0x08
#define K30_RAM_LO_BYTE		0x09

struct _K30_Sensor;
typedef struct _K30_Sensor K30Sensor;
typedef struct _K30_vmt
{
	void (*FctnInit)(K30Sensor*);
	int (*Configure)(K30Sensor*);
	int (*Request)(K30Sensor*);
	int (*Collect)(K30Sensor*);
	int (*Error)(K30Sensor*);
} K30_FctnTable;

typedef struct _K30_Abstract_vmt
{
	void (*VTinit)( K30Sensor* );
	int (*init)( K30Sensor* );
	int (*reset)( K30Sensor* );
	int (*getType)( K30Sensor* );
} K30_Abstract_FctnTable;


struct _K30_Sensor
{
	BaseSensor inherited;
};


void K30_FctnInit( K30Sensor *this);
int K30_Configure( K30Sensor *this );
int K30_Request( K30Sensor *this );
int K30_Collect( K30Sensor *this );
int K30_Error( K30Sensor *this );

void K30_VTinit( K30Sensor* );
int K30_init( K30Sensor* );
int K30_reset( K30Sensor* );
int K30_getType( K30Sensor* );

K30Sensor* New_K30_Sensor( int num, int NumData );



#endif