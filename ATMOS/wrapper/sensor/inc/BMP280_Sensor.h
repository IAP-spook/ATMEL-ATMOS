/**
 * \file BMP280_Sensor.h
 *
 * \brief Declaration of BMP280_Sensor struct, which derived from BaseSensor.
 * 
 * Created: 2016/4/6 17:40:39
 *  Author: Anxin Bai
 */ 

#ifndef _BMP280_SENSOR_H_
#define _BMP280_SENSOR_H_

#include "wrapper/base-class/inc/Base_Sensor.h"

struct _BMP280_Sensor;
typedef struct _BMP280_Sensor BMP280Sensor;
typedef struct _BMP280_vmt
{
	void (*FctnInit)(BMP280Sensor*);
	int (*Configure)(BMP280Sensor*);
	int (*Request)(BMP280Sensor*);
	int (*Collect)(BMP280Sensor*);
	int (*Error)(BMP280Sensor*);
} BMP280_FctnTable;

typedef struct _BMP280_Abstract_vmt
{
    void (*VTinit)( BMP280Sensor* );
    int (*init)( BMP280Sensor* );
    int (*reset)( BMP280Sensor* );
    int (*getType)( BMP280Sensor* );
} BMP280_Abstract_FctnTable;


struct _BMP280_Sensor
{
	BaseSensor inherited;
};


void BMP280_FctnInit( BMP280Sensor *this);
int BMP280_Configure( BMP280Sensor *this );
int BMP280_Request( BMP280Sensor *this );
int BMP280_Collect( BMP280Sensor *this );
int BMP280_Error( BMP280Sensor *this );

void BMP280_VTinit( BMP280Sensor* );
int BMP280_init( BMP280Sensor* );
int BMP280_reset( BMP280Sensor* );
int BMP280_getType( BMP280Sensor* );

BMP280Sensor* New_BMP280_Sensor( int num, int NumData );


#endif