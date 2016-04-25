/*
 * \file LoadData_Device.h
 * \brief Declaration of LoadDataDevice struct.
 * Created: 2016/4/21 23:55:29
 *  Author: Anxin Bai
 */ 


#ifndef LoadData_DEVICE_H
#define LoadData_DEVICE_H

#include "wrapper/base-class/inc/Base_Device.h"
#include "wrapper/other-device/inc/DemoStorage_Device.h"
#include "utilities/inc/data_unit.h"

extern DataUnit * cur_data;
extern tm cur_time;

struct _LoadData_Device;
typedef struct _LoadData_Device LoadDataDevice;


typedef struct _LoadData_Device_vmt
{
	void (*Device_Tinit)(LoadDataDevice*);
	int (*Execute)(LoadDataDevice*);
	int (*Configure)(LoadDataDevice*);
} LoadData_Device_FctnTable;

typedef struct _LoadData_Device_Abstract_vmt
{
	void (*VTinit)( LoadDataDevice* );
	int (*init)( LoadDataDevice* );
	int (*reset)( LoadDataDevice* );
	int (*getType)( LoadDataDevice* );
} LoadData_Device_Abstract_FctnTable;

struct _LoadData_Device
{
	BaseDevice abstract;
	LoadData_Device_FctnTable *vmt;
	int info;
	DemoStorageDevice *store_device;
};


void LoadData_Device_VTinit( LoadDataDevice* );
int LoadData_Device_init( LoadDataDevice* );
int LoadData_Device_reset( LoadDataDevice* );
int LoadData_Device_getType( LoadDataDevice* );

LoadDataDevice* New_LoadData_Device( int num, DemoStorageDevice * ptr );

void LoadData_Device_Tinit(LoadDataDevice *this);
int LoadData_Device_Execute(LoadDataDevice *this);
int LoadData_Device_Configure(LoadDataDevice *this);
#endif