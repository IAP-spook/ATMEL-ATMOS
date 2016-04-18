#include "wrapper/base-class/inc/Base_Sensor.h"
#include <stdlib.h>
#include <stdio.h>

Base_FctnTable Base_vmt = {
	Base_FctnInit,
	Base_Configure,
	Base_Request,
	Base_Collect,
	Base_Error
};

Abstract_FctnTable Abstract_vmt = {
	Base_VTinit,
	Base_init,
	Base_reset,
	Base_getType
};


void Base_FctnInit(BaseSensor *this)
{
	this->vmt = &Base_vmt;
}

int Base_Configure(BaseSensor *this )
{
	// virtual function
	return 0;
}

int Base_Request(BaseSensor *this )
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
	Base_FctnInit(p);
	Base_VTinit(p);
	return p;
}

void Base_VTinit( BaseSensor* this)
{
	this->abstract.device_vt = &Abstract_vmt;
}


int Base_Init(BaseSensor *this )
{
	return 0;
}


int Base_Reset( BaseSensor* this)
{
	// virtual function
	return 0;
}

int Base_getType( BaseSensor* this)
{
	return TYPE_BASE_SENSOR;
}