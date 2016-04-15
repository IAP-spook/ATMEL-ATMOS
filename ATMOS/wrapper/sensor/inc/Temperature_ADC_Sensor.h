/*
 * Temperature_ADC_Sensor.h
 *
 * Created: 2016/4/6 16:45:29
 *  Author: AB
 */ 

#ifndef _Temperature_ADC_SENSOR_H_
#define _Temperature_ADC_SENSOR_H_

#include "wrapper/base-class/inc/Base_Sensor.h"

struct _Temperature_ADC_Sensor;
typedef struct _Temperature_ADC_Sensor Temperature_ADCSensor;
typedef struct _Temperature_ADC_vmt
{
	void (*FctnInit)(Temperature_ADCSensor*);
	int (*Init)(Temperature_ADCSensor*);
	int (*Configure)(Temperature_ADCSensor*);
	int (*request)(Temperature_ADCSensor*);
	int (*Collect)(Temperature_ADCSensor*);
	int (*Error)(Temperature_ADCSensor*);
} Temperature_ADC_FctnTable;

struct _Temperature_ADC_Sensor
{
	BaseSensor inherited;
};


void Temperature_ADC_FctnInit( Temperature_ADCSensor *this);
int Temperature_ADC_Init( Temperature_ADCSensor *this);
int Temperature_ADC_Configure( Temperature_ADCSensor *this );
int Temperature_ADC_request( Temperature_ADCSensor *this );
int Temperature_ADC_Collect( Temperature_ADCSensor *this );
int Temperature_ADC_Error( Temperature_ADCSensor *this );
Temperature_ADCSensor* New_Temperature_ADC_Sensor( int num );



#endif