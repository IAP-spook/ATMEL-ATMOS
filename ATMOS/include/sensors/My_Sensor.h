#ifndef _My_SENSOR_H_
#define _My_SENSOR_H_

#include "sensors/Base_Sensor.h"

struct _My_Sensor;
typedef struct _My_Sensor MySensor;
typedef struct _My_vmt
{
	void (*FctnInit)(MySensor*);
	int (*Init)(MySensor*);
	int (*Configure)(MySensor*);
	int (*PreProcessing)(MySensor*);
	int (*Collect)(MySensor*);
	int (*Error)(MySensor*);
} My_FctnTable;

struct _My_Sensor
{
	BaseSensor inherited;
};


void My_FctnInit( MySensor *this);
int My_Init( MySensor *this);
int My_Configure( MySensor *this );
int My_PreProcessing( MySensor *this );
int My_Collect( MySensor *this );
int My_Error( MySensor *this );
MySensor* New_My_Sensor( int num );



#endif