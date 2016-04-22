/*
 * \file LoadData_Device.c
 * \brief Implementation of LoadDataDevice struct.
 * Created: 2016/4/21 23:55:49
 *  Author: Anxin Bai
 */ 

#include "wrapper/other-device/inc/LoadData_Device.h"
#include <stdlib.h>
#include <stdio.h>

LoadData_Device_Abstract_FctnTable LoadDataDevice_abstract_vmt = {
	LoadData_Device_VTinit,
	LoadData_Device_init,
	LoadData_Device_reset,
	LoadData_Device_getType
};

LoadData_Device_FctnTable LoadDataDevice_vmt = {
	LoadData_Device_Tinit,
	LoadData_Device_Execute,
	LoadData_Device_Configure
};


void LoadData_Device_VTinit( LoadDataDevice *this )
{
	this->abstract.device_vt = &LoadDataDevice_abstract_vmt;
	return;
}

int LoadData_Device_init( LoadDataDevice *this )
{
	return 0;
}

int LoadData_Device_reset( LoadDataDevice *this )
{
	return 0;
}

int LoadData_Device_getType( LoadDataDevice *this )
{
	return TYPE_DEVICE;
}

LoadDataDevice* New_LoadData_Device( int infonum, DemoStorageDevice * ptr )
{
	LoadDataDevice *p = malloc(sizeof(LoadDataDevice));
	p->info = infonum;
	p->store_device = ptr;
	LoadData_Device_VTinit(p);
	LoadData_Device_Tinit(p);
	return p;
}




void LoadData_Device_Tinit(LoadDataDevice *this)
{
	this->vmt = &LoadDataDevice_vmt;
}

int LoadData_Device_Execute(LoadDataDevice *this)
{
	printf("Executre LoadData Device\n");
	cur_data = this->store_device->get_CurDataUnit(this->store_device);
	return 0;
}

int LoadData_Device_Configure(LoadDataDevice *this)
{
	return 0;
}