/**
 * \file My_Sensor.h
 *
 * \brief Declaration of My_Sensor struct, which derived from BaseSensor.
 * 
 *  Author: Anxin Bai
 */ 

#ifndef _My_SENSOR_H_
#define _My_SENSOR_H_

#include "wrapper/base-class/inc/Base_Sensor.h"

struct _My_Sensor;
typedef struct _My_Sensor MySensor;
typedef struct _My_vmt
{
	void (*FctnInit)(MySensor*);
	int (*Configure)(MySensor*);
	int (*Request)(MySensor*);
	int (*Collect)(MySensor*);
	int (*Error)(MySensor*);
} My_FctnTable;

typedef struct _My_Abstract_vmt
{
	void (*VTinit)( MySensor* );
	int (*init)( MySensor* );
	int (*reset)( MySensor* );
	int (*getType)( MySensor* );
} My_Abstract_FctnTable;


struct _My_Sensor
{
	BaseSensor inherited;
};


void My_FctnInit( MySensor *this);
int My_Configure( MySensor *this );
int My_Request( MySensor *this );
int My_Collect( MySensor *this );
int My_Error( MySensor *this );

void My_VTinit( MySensor* );
int My_init( MySensor* );
int My_reset( MySensor* );
int My_getType( MySensor* );

MySensor* New_My_Sensor( int num, int NumData );



#endif