/*
 * \file My_Device.h
 * \brief Declaration of MyDevice struct.
 * Created: 2016/4/21 23:55:29
 *  Author: Anxin Bai
 */ 


#ifndef My_DEVICE_H
#define My_DEVICE_H

#include "devices/base-class/inc/Base_Device.h"

struct _My_Device;
typedef struct _My_Device MyDevice;


typedef struct _My_Device_vmt
{
	void (*Device_Tinit)(MyDevice*);
	int (*Execute)(MyDevice*);
	int (*Configure)(MyDevice*);
} My_Device_FctnTable;

typedef struct _My_Device_Abstract_vmt
{
	void (*VTinit)( MyDevice* );
	int (*init)( MyDevice* );
	int (*reset)( MyDevice* );
	int (*getType)( MyDevice* );
} My_Device_Abstract_FctnTable;

struct _My_Device
{
	BaseDevice abstract;
	My_Device_FctnTable *vmt;
	int info;
};


void My_Device_VTinit( MyDevice* );
int My_Device_init( MyDevice* );
int My_Device_reset( MyDevice* );
int My_Device_getType( MyDevice* );

MyDevice* New_My_Device( int num );

void My_Device_Tinit(MyDevice *this);
int My_Device_Execute(MyDevice *this);
int My_Device_Configure(MyDevice *this);
#endif