/*
 * \file DemoStorage_Device.c
 * \brief Implementation of DemoStorageDevice struct.
 * Created: 2016/4/21 23:55:49
 *  Author: Anxin Bai
 */ 

#include "wrapper/other-device/inc/DemoStorage_Device.h"
#include <stdlib.h>
#include <stdio.h>

DemoStorage_Device_Abstract_FctnTable DemoStorageDevice_abstract_vmt = {
	DemoStorage_Device_VTinit,
	DemoStorage_Device_init,
	DemoStorage_Device_reset,
	DemoStorage_Device_getType
};

DemoStorage_Device_FctnTable DemoStorageDevice_vmt = {
	DemoStorage_Device_Tinit,
	DemoStorage_Device_Execute,
	DemoStorage_Device_Configure
};


void DemoStorage_Device_VTinit( DemoStorageDevice *this )
{
	this->abstract.device_vt = &DemoStorageDevice_abstract_vmt;
	return;
}

int DemoStorage_Device_init( DemoStorageDevice *this )
{
	return 0;
}

int DemoStorage_Device_reset( DemoStorageDevice *this )
{
	return 0;
}

int DemoStorage_Device_getType( DemoStorageDevice *this )
{
	return TYPE_STORAGE_DEVICE;
}

DemoStorageDevice* New_DemoStorage_Device( int infonum )
{
	DemoStorageDevice *p = malloc(sizeof(DemoStorageDevice));
	p->info = infonum;
	DemoStorage_Device_VTinit(p);
	p->StoragesData = ( DataUnit * ) malloc( sizeof(DataUnit) * MAX_HOLD_DATA);
	
	for( int i=0;i<MAX_HOLD_DATA;i++)
		for( int j=0;j<MAX_NUM_DATA;j++)
			p->StoragesData[i].data[j] = -9999; 
	p->cur_pos = 0;

	return p;
}




void DemoStorage_Device_Tinit(DemoStorageDevice *this)
{
	this->vmt = &DemoStorageDevice_vmt;
}

int DemoStorage_Device_Execute(DemoStorageDevice *this)
{
	printf("Storage Execution\n");
	for( int i=0; i<this->cur_pos; i++)
	{
		DataUnit temp = this->StoragesData[i];
		printf("%d%d%d %d%d%d\t",temp.data_tm.tm_year,temp.data_tm.tm_mon,temp.data_tm.tm_mday,temp.data_tm.tm_hour,temp.data_tm.tm_min,temp.data_tm.tm_sec);
		for( int j = 0; j < MAX_NUM_DATA; ++j )
		printf("%3.3f ",temp.data[j]);
	}
	this->cur_pos = 0;
	return 0;
}

int DemoStorage_Device_Configure(DemoStorageDevice *this)
{
	return 0;
}

DataUnit get_CurDataUnit(DemoStorageDevice *this)
{
	int t = this->cur_pos;
	this->cur_pos = this->cur_pos+1;
	return this->StoragesData[t];
}