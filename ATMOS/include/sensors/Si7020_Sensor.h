#ifndef _Si7020_SENSOR_H_
#define _Si7020_SENSOR_H_

#include "sensors/Base_Sensor.h"

struct _Si7020_Sensor;
typedef struct _Si7020_Sensor Si7020Sensor;
typedef struct _Si7020_vmt
{
	void (*FctnInit)(Si7020Sensor*);
	int (*Init)(Si7020Sensor*);
	int (*Configure)(Si7020Sensor*);
	int (*PreProcessing)(Si7020Sensor*);
	int (*Collect)(Si7020Sensor*);
	int (*Error)(Si7020Sensor*);
} Si7020_FctnTable;

struct _Si7020_Sensor
{
	BaseSensor inherited;
	int dummy;
	unsigned char temperature_data[3];
	unsigned char humility_data[3];
};


void Si7020_FctnInit( Si7020Sensor *this);
int Si7020_Init( Si7020Sensor *this);
int Si7020_Configure( Si7020Sensor *this );
int Si7020_PreProcessing( Si7020Sensor *this );
int Si7020_Collect( Si7020Sensor *this );
int Si7020_Error( Si7020Sensor *this );
Si7020Sensor* New_Si7020_Sensor( int num );



#endif