#include "wrapper/base-class/inc/Base_Sensor.h"
#include <stdio.h>
#include <stdlib.h>

Base_FctnTable Base_vmt = { 
	Base_FctnInit, 
	Base_Configure, 
	Base_Request, 
	Base_Collect, 
	Base_Error 
};

Abstract_FctnTable Abstract_vmt = {
	Base_VTinit,
	Base_Init,
	Base_Reset,
	Base_GetType
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
	p->abstract = New_Base_Device();
	p->test_num = num;
	Base_FctnInit(p);
	Base_VTinit(p);
	return p;
}

void Base_VTinit( BaseSensor* this)
{
	this->abstract->device_vt = &Abstract_vmt;
}


int Base_Init(BaseSensor *this )
{
	this->test_num = 10;
	return 0;
}


int Base_Reset( BaseSensor* this)
{
	// virtual function
	return 0;
}

int Base_GetType( BaseSensor* this)
{
	return TYPE_BASE_SENSOR;
}