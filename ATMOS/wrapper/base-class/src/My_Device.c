/*
 * \file My_Device.c
 * \brief Implementation of MyDevice struct.
 * Created: 2016/4/21 23:55:49
 *  Author: Anxin Bai
 */ 

#include "wrapper/base-class/inc/My_Device.h"
#include <stdlib.h>
#include <stdio.h>

My_Device_Abstract_FctnTable MyDevice_abstract_vmt = {
	My_Device_VTinit,
	My_Device_init,
	My_Device_reset,
	My_Device_getType
};


void My_Device_VTinit( MyDevice *this )
{
	this->abstract.device_vt = &MyDevice_abstract_vmt;
	return;
}

int My_Device_init( MyDevice *this )
{
	return 0;
}

int My_Device_reset( MyDevice *this )
{
	return 0;
}

int My_Device_getType( MyDevice *this )
{
	return 0;
}

MyDevice* New_My_Device( int infonum )
{
	MyDevice *p = malloc(sizeof(MyDevice));
	p->info = infonum;
	My_Device_VTinit(p);
	return p;	
}

int Execute( MyDevice *this )
{
	printf("MyDevice_Execute\n");
	return 0;
}

int Configure( MyDevice *this )
{
	return 0;
}
