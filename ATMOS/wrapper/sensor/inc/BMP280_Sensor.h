/*
 * BMP280_Sensor.h
 *
 * Created: 2016/4/6 17:40:39
 *  Author: AB
 */ 


#ifndef _BMP280_SENSOR_H_
#define _BMP280_SENSOR_H_

#include "wrapper/base-class/inc/Base_Sensor.h"

struct _BMP280_Sensor;
typedef struct _BMP280_Sensor BMP280Sensor;
typedef struct _BMP280_vmt
{
	void (*FctnInit)(BMP280Sensor*);
	int (*Init)(BMP280Sensor*);
	int (*Configure)(BMP280Sensor*);
	int (*request)(BMP280Sensor*);
	int (*Collect)(BMP280Sensor*);
	int (*Error)(BMP280Sensor*);
} BMP280_FctnTable;

struct _BMP280_Sensor
{
	BaseSensor inherited;
};


void BMP280_FctnInit( BMP280Sensor *this);
int BMP280_init( BMP280Sensor *this);
int BMP280_Configure( BMP280Sensor *this );
int BMP280_request( BMP280Sensor *this );
int BMP280_Collect( BMP280Sensor *this );
int BMP280_Error( BMP280Sensor *this );
BMP280Sensor* New_BMP280_Sensor( int num );





#endif /* BMP280_SENSOR_H_ */