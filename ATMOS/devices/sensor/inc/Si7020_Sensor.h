/**
 * \file Si7020_Sensor.h
 *
 * \brief Declaration of Si7020_Sensor struct, which derived from BaseSensor.
 * 
 *  Author: Anxin Bai
 */ 

#ifndef _Si7020_SENSOR_H_
#define _Si7020_SENSOR_H_

#include "devices/base-class/inc/Base_Sensor.h"

struct _Si7020_Sensor;
typedef struct _Si7020_Sensor Si7020Sensor;
typedef struct _Si7020_vmt
{
	void (*FctnInit)(Si7020Sensor*);
	int (*Configure)(Si7020Sensor*);
	int (*Request)(Si7020Sensor*);
	int (*Collect)(Si7020Sensor*);
	int (*Error)(Si7020Sensor*);
} Si7020_FctnTable;

typedef struct _Si7020_Abstract_vmt
{
	void (*VTinit)( Si7020Sensor* );
	int (*init)( Si7020Sensor* );
	int (*reset)( Si7020Sensor* );
	int (*getType)( Si7020Sensor* );
} Si7020_Abstract_FctnTable;


struct _Si7020_Sensor
{
	BaseSensor inherited;
	unsigned char temperature_data[3];
	unsigned char humility_data[3];
};


void Si7020_FctnInit( Si7020Sensor *this);
int Si7020_Configure( Si7020Sensor *this );
int Si7020_Request( Si7020Sensor *this );
int Si7020_Collect( Si7020Sensor *this );
int Si7020_Error( Si7020Sensor *this );

void Si7020_VTinit( Si7020Sensor* );
int Si7020_Init( Si7020Sensor* );
int Si7020_reset( Si7020Sensor* );
int Si7020_getType( Si7020Sensor* );

Si7020Sensor* New_Si7020_Sensor( int num, int NumData );



#endif