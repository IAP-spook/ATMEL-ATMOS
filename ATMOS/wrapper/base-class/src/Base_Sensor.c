#include "wrapper/base-class/inc/Base_Sensor.h"
#include <stdio.h>
#include <stdlib.h>

Base_FctnTable Base_vmt = { Base_FctnInit, Base_Init, Base_Configure, Base_PreProcessing, Base_Collect, Base_Error };

void Base_FctnInit(BaseSensor *this)
{
	this->vmt = &Base_vmt;
}

int Base_Init(BaseSensor *this )
{
	this->test_num = 10;
	return 0;
}

int Base_Configure(BaseSensor *this )
{
	// virtual function
	return 0;
}

int Base_PreProcessing(BaseSensor *this )
{
	// virtual function
	return 0;
}

int Base_Collect(BaseSensor *this )
{
	// virtual function
	return 0;
}

int Base_Error(BaseSensor *this )
{
	// virtual function
	return 0;
}

BaseSensor* New_Base_Sensor( int num)
{
	BaseSensor *p = malloc(sizeof(BaseSensor));
	p->test_num = num;
	return p;
}
