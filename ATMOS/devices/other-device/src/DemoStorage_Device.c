/*
 * \file DemoStorage_Device.c
 * \brief Implementation of DemoStorageDevice struct.
 * Created: 2016/4/21 23:55:49
 *  Author: Anxin Bai
 *
 * The DemoStorage_Device.h along with DemoStorage_Device.c serve as a virtual storage or memory device that simulated by the mcu( or cpu ).
 * It is only a demo part of the software but in the future when we got real flash api to access flash storage we can transform this class/struct into two.
 * One is MemoryDevice, a pure virtual device, and it is responsible for taking advantage of memory.
 * It is literally the same as this one, except the renaming issue ( replace all DemoStorage to Memory to make it clearer and rename the file to Memory_Device.h/.c ).
 * The other one should be the FlashStorage_Device, and it is responsible for flushing ( storing any valid stuff from the Memory_Device into flash disk ).
 * This one should be kind of different, and I would like to discuss some APIs here.
 * The constructor should be like New_FlashStorage_Device( int num, MemoryDevice * ptr ), which takes a MemoryDevice struct pointer just like LoadData_Device since we want to access the memory.
 * The main function api, aka. FlashStorage_Device_Execute(DemoStorageDevice *this), should take the MemoryDevice pointer and get access to its DataUnit array by accessing the DataUnit pointer and the current position within MemoryDevice struct.
 * After that, we get the data, we just want to transfer the DataUnit to the flash storage, one by one aligned in block. Please notice that we should think twice to decide how many bytes each data should occupy, and we want to record that since we definitely want to read it back while communicating. In read-back procedure, we want know each block size and it is a reverse procedure of writing DataUnit to disk.
 * So in short, you need to access the memory data via MemoryDevice pointer first, then using some flash storage api to help write the data along with the timestamp one by one in disk.
 * BTW, we'd better debug somewhere else and make sure the flash storage api works before writing the FlashStorage_Device.h/.c to increase efficiency.
 */

#include "devices/other-device/inc/DemoStorage_Device.h"
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
	DemoStorage_Device_Configure,
	DemoStorageget_CurDataUnit
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
	DemoStorage_Device_Tinit(p);
	p->StoragesData = ( DataUnit * ) malloc( sizeof(DataUnit) * MAX_HOLD_DATA);
	
	for( int i=0;i<MAX_HOLD_DATA;i++)
		for( int j=0;j<MAX_NUM_DATA;j++)
		{
				p->StoragesData[i].data[j] = -9999; 
				p->StoragesData[i].data_tm.unix_msec = 0;
				p->StoragesData[i].data_tm.unix_time = 0;
		}
	p->cur_pos = 0;

	return p;
}




void DemoStorage_Device_Tinit(DemoStorageDevice *this)
{
	this->vmt = &DemoStorageDevice_vmt;
}

int DemoStorage_Device_Execute(DemoStorageDevice *this)
{
	printf("Executre Storage Device\n");
	for( int i=0; i<this->cur_pos; i++)
	{
		DataUnit temp = this->StoragesData[i];
		printf("%04ld %02ld %02ld %02ld %02ld %02ld\t",temp.data_tm.tm_year,temp.data_tm.tm_mon,temp.data_tm.tm_mday,temp.data_tm.tm_hour,temp.data_tm.tm_min,temp.data_tm.tm_sec);
		for( int j = 0; j < MAX_NUM_DATA; ++j )
			printf("%3.3f ",temp.data[j]);
		printf("\n");
	}
	this->cur_pos = 0;
	return 0;
}

int DemoStorage_Device_Configure(DemoStorageDevice *this)
{
	return 0;
}

DataUnit * DemoStorageget_CurDataUnit(DemoStorageDevice *this)
{
	int t = this->cur_pos;
	printf("Executre get store Device : %d\n",t);
	this->cur_pos = this->cur_pos+1;
	DataUnit * data_t = this -> StoragesData;
	return (data_t + t);
}