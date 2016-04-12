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

BMP280_FctnTable BMP280_vmt = { BMP280_FctnInit, BMP280_init, BMP280_Configure, BMP280_PreProcessing, BMP280_Collect, BMP280_Error };

void BMP280_FctnInit(BMP280Sensor *this)
{
	this->inherited.vmt = &BMP280_vmt;
}

int BMP280_init(BMP280Sensor *this )
{
	this->inherited.test_num = 10;
	return 0;
}

int BMP280_Configure(BMP280Sensor *this )
{
	printf("BMP280_Configure\n");
	// virtual function
	return 0;
}

int BMP280_PreProcessing(BMP280Sensor *this )
{
	printf("BMP280_PreProcessing\n");
	int Delay = BMP280_StartMeasurment();
	if( Delay == 0 )
		return 0;
	else
		return 2;
}

int BMP280_Collect(BMP280Sensor *this )
{
	printf("BMP280_Collect\n");
	double T, P;
	BMP280_GetTemperatureAndPressure(&T,&P);
	printf("Temperature = %.3f\nHumility = %.3f\n",T,P);
	return 0;
}

int BMP280_Error(BMP280Sensor *this )
{
	printf("BMP280_Error\n");
	return 0;
}

BMP280Sensor* New_BMP280_Sensor( int num)
{
	BMP280Sensor *p = malloc(sizeof(BMP280Sensor));
	p->inherited.test_num = num;
	return p;
}