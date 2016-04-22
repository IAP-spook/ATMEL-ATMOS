/**
 * \file Si7020_Sensor.c
 *
 * \brief Implementation of Si7020_Sensor struct, which derived from BaseSensor.
 * 
 *  Author: Anxin Bai
 */ 

#include "devices/inc/Si7020.h"
#include "wrapper/sensor/inc/Si7020_Sensor.h"
#include <stdlib.h>
#include <stdio.h>

Si7020_FctnTable Si7020_vmt = {
	Si7020_FctnInit,
	Si7020_Configure,
	Si7020_Request,
	Si7020_Collect,
	Si7020_Error
};

Si7020_Abstract_FctnTable Si7020_abstract_vmt = {
	Si7020_VTinit,
	Si7020_Init,
	Si7020_reset,
	Si7020_getType
};




void Si7020_FctnInit(Si7020Sensor *this)
{
	this->inherited.vmt = &Si7020_vmt;
}


int Si7020_Configure(Si7020Sensor *this )
{
	// virtual function
	return 0;
}

int Si7020_Request(Si7020Sensor *this )
{
	printf("Si7020_Request\n");
	Si7020_readTemperature(this->temperature_data,3);
	Si7020_readHumidity(this->humility_data,3);
	return 0;
}

int Si7020_Collect(Si7020Sensor *this )
{
	printf("Si7020_Collect\n");
	float T = Si7020_calTemperature(this->temperature_data);
	float H = Si7020_calHumidity(this->humility_data);
	printf("\tTemperature = %.3f\n\tHumility = %.3f\n",T,H);
	cur_data->data[2] = T;
	cur_data->data[3] = H;
	return 0;
}

int Si7020_Error(Si7020Sensor *this )
{
	// virtual function
	return 0;
}
void Si7020_VTinit( Si7020Sensor* this)
{
	this->inherited.abstract.device_vt = &Si7020_abstract_vmt;
}

int Si7020_Init( Si7020Sensor* this)
{
	printf("Si7020 Sensor Init\n");
	return 0;
}

int Si7020_reset( Si7020Sensor* this)
{
	// virtual function
	return 0;
}

int Si7020_getType(Si7020Sensor* this)
{
	return TYPE_SENSOR;
}


Si7020Sensor* New_Si7020_Sensor( int num, int NumData)
{
	Si7020Sensor *p = malloc(sizeof(Si7020Sensor));
	Si7020_VTinit( p );
	Si7020_FctnInit( p );
	p->inherited.test_num = num;
	
	p->inherited.NumOfData = NumData;
	p->inherited.StartNum = SensorDataCount;
	SensorDataCount += NumData;
	printf("SensorDataCount %d\n",SensorDataCount);
	
	for(int i = 0; i < 3; ++i )
	{
		p->temperature_data[i] = 0;
		p->humility_data[i] = 0;
	}
	
	return p;
}