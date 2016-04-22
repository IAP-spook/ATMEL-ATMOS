/*
 * \file DemoStorage_Device.h
 * \brief Declaration of DemoStorageDevice struct.
 * Created: 2016/4/21 23:55:29
 *  Author: Anxin Bai
 */ 


#ifndef DemoStorage_DEVICE_H
#define DemoStorage_DEVICE_H

#include "wrapper/base-class/inc/Base_Device.h"
#include "utilities/inc/data_unit.h"

#define MAX_HOLD_DATA 15

struct _DemoStorage_Device;
typedef struct _DemoStorage_Device DemoStorageDevice;


typedef struct _DemoStorage_Device_vmt
{
	void (*Device_Tinit)(DemoStorageDevice*);
	int (*Execute)(DemoStorageDevice*);
	int (*Configure)(DemoStorageDevice*);
} DemoStorage_Device_FctnTable;

typedef struct _DemoStorage_Device_Abstract_vmt
{
	void (*VTinit)( DemoStorageDevice* );
	int (*init)( DemoStorageDevice* );
	int (*reset)( DemoStorageDevice* );
	int (*getType)( DemoStorageDevice* );
} DemoStorage_Device_Abstract_FctnTable;

struct _DemoStorage_Device
{
	BaseDevice abstract;
	DemoStorage_Device_FctnTable *vmt;
	int info;
	int cur_pos;
	DataUnit * StoragesData;
	DataUnit * (*get_CurDataUnit)(DemoStorageDevice *);
};


void DemoStorage_Device_VTinit( DemoStorageDevice* );
int DemoStorage_Device_init( DemoStorageDevice* );
int DemoStorage_Device_reset( DemoStorageDevice* );
int DemoStorage_Device_getType( DemoStorageDevice* );

DemoStorageDevice* New_DemoStorage_Device( int num );

void DemoStorage_Device_Tinit(DemoStorageDevice *this);
int DemoStorage_Device_Execute(DemoStorageDevice *this);
int DemoStorage_Device_Configure(DemoStorageDevice *this);
#endif