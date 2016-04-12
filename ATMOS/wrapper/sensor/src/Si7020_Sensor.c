#include "wrapper/sensor/inc/Si7020_Sensor.h"
#include "devices/inc/Si7020.h"
#include <stdio.h>
#include <stdlib.h>

Si7020_FctnTable Si7020_vmt = { Si7020_FctnInit, Si7020_Init, Si7020_Configure, Si7020_PreProcessing, Si7020_Collect, Si7020_Error };

void Si7020_FctnInit(Si7020Sensor *this)
{
	this->inherited.vmt = &Si7020_vmt;
}

int Si7020_Init(Si7020Sensor *this )
{
	this->inherited.test_num = 10;
	return 0;
}

int Si7020_Configure(Si7020Sensor *this )
{
	printf("Si7020_Configure\n");
	// virtual function
	return 0;
}

int Si7020_PreProcessing(Si7020Sensor *this )
{
	printf("Si7020_PreProcessing\n");
	this->dummy = Si7020_readTemperature(this->temperature_data,3);
	this->dummy = Si7020_readHumidity(this->humility_data,3); 
	return 0;
}

int Si7020_Collect(Si7020Sensor *this )
{
	printf("Si7020_Collect\n");
	float T = Si7020_calTemperature(this->temperature_data);
	float H = Si7020_calHumidity(this->humility_data);
	printf("Temperature = %.3f\nHumility = %.3f\n",T,H);
	return 0;
}

int Si7020_Error(Si7020Sensor *this )
{
	printf("Si7020_Error\n");
	return 0;
}

Si7020Sensor* New_Si7020_Sensor( int num)
{
	Si7020Sensor *p = malloc(sizeof(Si7020Sensor));
	p->inherited.test_num = num;
	for(int i = 0; i < 3; ++i )
	{
		p->temperature_data[i] = 0;
		p->humility_data[i] = 0;
	}
	return p;
}