/*
 * \file LoadData_Device.c
 * \brief Implementation of LoadDataDevice struct.
 * Created: 2016/4/21 23:55:49
 *  Author: Anxin Bai
 *
 *
 * The LoadData_Device.h along with LoadData_Device.c serve as virtual device that simulated by the mcu( or cpu ).
 * It is responsible to access some limited memory ( currently 07/07/2016 managed by DemoStorage_Device.h/.c and defined in /utilities/inc/data_unit.h ) and mark the memory slot that should be used for current period.
 * At the beginning of each collecting period the execute function, it accesses the limited memory and move to the next available slot and let cur_data pointer points to it.
 * So during collecting, all sensors use cur_data pointer to load new data but don't need to worry about where in memory to save it and how to manage the memory.
 */

#include "devices/other-device/inc/LoadData_Device.h"
#include <stdlib.h>
#include <stdio.h>

extern long TimeStamp ;

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
	if( this -> store_device == NULL)
		printf("NULL store device\n");
	cur_data = this->store_device->vmt->get_CurDataUnit(this->store_device);
	/*TODO : could init -999 here */
	
	
	if( cur_data != NULL)
	{
		updateTime((uint32_t)TimeStamp,&cur_time);
		cur_data->data_tm.unix_time = cur_time.unix_time;
		calcDate( &(cur_data->data_tm));
		printf("time debug3 %ld\n",cur_data->data_tm.unix_time);
		TimeStamp = 0;
	}
	return 0;
}

int LoadData_Device_Configure(LoadDataDevice *this)
{
	return 0;
}
