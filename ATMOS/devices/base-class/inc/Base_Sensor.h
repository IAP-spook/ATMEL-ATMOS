/**
 * \file Base_Sensor.h
 *
 * \brief Declaration of BaseSensor struct, which derived from BaseDevice
 * 
 *  Author: Anxin Bai
 */ 

#ifndef _BASE_SENSOR_H_
#define _BASE_SENSOR_H_
#include "devices/base-class/inc/Base_Device.h"
#include "utilities/inc/data_unit.h"

extern DataUnit * cur_data;

struct _Base_Sensor;
typedef struct _Base_Sensor BaseSensor;
typedef struct _Base_vmt
{
	void (*FctnInit)(BaseSensor*);
	int (*Configure)(BaseSensor*);
	int (*Request)(BaseSensor*);
	int (*Collect)(BaseSensor*);
	int (*Error)(BaseSensor*);

} Base_FctnTable;

typedef struct _Abstract_vmt
{
	void (*VTinit)( BaseSensor* );
	int (*init)( BaseSensor* );
	int (*reset)( BaseSensor* );
	int (*getType)( BaseSensor* );
} Abstract_FctnTable;

struct _Base_Sensor
{
	BaseDevice abstract;
	int test_num;
	int StartNum;
	int NumOfData;
	int (*getStartNum)(BaseSensor*);
	int (*getEndNum)(BaseSensor*);
	Base_FctnTable *vmt;
};


void Base_FctnInit( BaseSensor *this);
int Base_Configure( BaseSensor *this );
int Base_Request( BaseSensor *this );
int Base_Collect( BaseSensor *this );
int Base_Error( BaseSensor *this );

void Base_VTinit( BaseSensor* );
int Base_init( BaseSensor* );
int Base_reset( BaseSensor* );
int Base_getType( BaseSensor* );

BaseSensor* New_Base_Sensor( int num, int NumData  );



#endif