/**
 * \file Base_Device.h
 *
 * \brief Declaration of BaseDevice struct.
 *
 * Created: 2016/4/12 18:54:36
 *  Author: Anxin Bai
 */ 
#ifndef BASE_DEVICE_H_
#define BASE_DEVICE_H_
#include "utilities/inc/common.h"

struct _Base_Device;
typedef struct _Base_Device BaseDevice;
typedef struct _Base_Device_vt
{
	void (*VTinit)( BaseDevice* );
	int (*init)( BaseDevice* );
	int (*reset)( BaseDevice* );
	int (*getType)( BaseDevice* );
} Base_Device_FctnTable;

struct _Base_Device
{
	Base_Device_FctnTable *device_vt;
};

void BaseDevice_VTinit( BaseDevice* );
int BaseDevice_init( BaseDevice* );
int BaseDevice_reset( BaseDevice* );
int BaseDevice_getType( BaseDevice* );
BaseDevice* New_Base_Device();

#endif