/*
 * BMP280_Sensor.c
 *
 * Created: 2016/4/6 17:43:53
 *  Author: AB
 */ 

#include "wrapper/sensor/inc/BMP280_Sensor.h"
#include "devices/inc/BMP280.h"
#include <stdio.h>
#include <stdlib.h>

BMP280_FctnTable BMP280_vmt = { 
    BMP280_FctnInit, 
    BMP280_Configure,
    BMP280_Request,
    BMP280_Collect, 
    BMP280_Error 
};

BMP280_Abstract_FctnTable BMP280_abstract_vmt = {
    BMP280_VTinit,
    BMP280_init,
    BMP280_reset,
    BMP280_getType
};




void BMP280_FctnInit(BMP280Sensor *this)
{
	this->inherited.vmt = &BMP280_vmt;
}


int BMP280_Configure(BMP280Sensor *this )
{
	// virtual function
	return 0;
}

int BMP280_Request(BMP280Sensor *this )
{
	printf("BMP280 Request\n");
	int Delay = BMP280_StartMeasurment();
	if( Delay == 0 )
		return 0;
	else
		return 2;
}

int BMP280_Collect(BMP280Sensor *this )
{
	printf("BMP280 Collect\n");
	double T, P;
	BMP280_GetTemperatureAndPressure(&T,&P);
	printf("Temperature = %.3f\nPressure = %.3f\n",T,P);
	return 0;
}

int BMP280_Error(BMP280Sensor *this )
{
	printf("BMP280_Error\n");
	return 0;
}
void BMP280_VTinit( BMP280Sensor* this)
{
    this->inherited.abstract.device_vt = &BMP280_abstract_vmt;
}

int BMP280_init( BMP280Sensor* this)
{
    printf("BMP280 Sensor Init\n");
    return 0;
}

int BMP280_reset( BMP280Sensor* this)
{
    // virtual function
    return 0;
}

int BMP280_getType(BMP280Sensor* this)
{
    return TYPE_SENSOR;
}


BMP280Sensor* New_BMP280_Sensor( int num)
{
	BMP280Sensor *p = malloc(sizeof(BMP280Sensor));
    BMP280_VTinit( p );
    BMP280_FctnInit( p );
	p->inherited.test_num = num;
	return p;
}