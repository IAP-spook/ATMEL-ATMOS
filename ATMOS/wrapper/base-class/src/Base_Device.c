/*
 * Base_Device.c
 *
 * Created: 2016/4/12 18:55:15
 *  Author: AB
 */ 
#include "wrapper/base-class/inc/Base_Device.h"
#include <stdio.h>
#include <stdlib.h>


Base_Device_FctnTable BDT =
{
	BaseDevice_VTinit,
	BaseDevice_init,
	BaseDevice_reset,
	BaseDevice_getType
};


void BaseDevice_VTinit( BaseDevice* this)
{
	this->device_vt = &BDT;
}

int BaseDevice_init( BaseDevice* this)
{
	// virtual function
	return 0;
}

int BaseDevice_reset( BaseDevice* this)
{
	// virtual function
	return 0;
}

int BaseDevice_getType( BaseDevice* this)
{
	return TYPE_BASE_DEVICE;
}

BaseDevice* New_Base_Device()
{
	BaseDevice *p = malloc( sizeof( BaseDevice ) );
	BaseDevice_VTinit(p);
	return p;
}