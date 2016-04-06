/*
 * Temperature_ADC_Sensor.c
 *
 * Created: 2016/4/6 16:44:18
 *  Author: AB
 */ 
#include "sensors/Temperature_ADC_Sensor.h"
#include "drivers/ADC.h"
#include <stdio.h>
#include <stdlib.h>

Temperature_ADC_FctnTable Temperature_ADC_vmt = { Temperature_ADC_FctnInit, Temperature_ADC_Init, Temperature_ADC_Configure, Temperature_ADC_PreProcessing, Temperature_ADC_Collect, Temperature_ADC_Error };

void Temperature_ADC_FctnInit(Temperature_ADCSensor *this)
{
	this->inherited.vmt = &Temperature_ADC_vmt;
}

int Temperature_ADC_Init(Temperature_ADCSensor *this )
{
	this->inherited.test_num = 10;
	return 0;
}

int Temperature_ADC_Configure(Temperature_ADCSensor *this )
{
	printf("Temperature_ADC_Configure\n");
	// virtual function
	return 0;
}

int Temperature_ADC_PreProcessing(Temperature_ADCSensor *this )
{
	printf("Temperature_ADC_PreProcessing\n");
	return 0;
}

int Temperature_ADC_Collect(Temperature_ADCSensor *this )
{
	printf("Temperature_ADC_Collect\n");
	float T = ADC_DieTemp();
	printf("Temperature = %.3f\n",T);
	return 0;
}

int Temperature_ADC_Error(Temperature_ADCSensor *this )
{
	printf("Temperature_ADC_Error\n");
	return 0;
}

Temperature_ADCSensor* New_Temperature_ADC_Sensor( int num)
{
	Temperature_ADCSensor *p = malloc(sizeof(Temperature_ADCSensor));
	p->inherited.test_num = num;
	return p;
}