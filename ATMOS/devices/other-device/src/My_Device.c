/*
 * \file My_Device.c
 * \brief Implementation of MyDevice struct.
 * Created: 2016/4/21 23:55:49
 *  Author: Anxin Bai
 *
 *
 *	This file along with My_Device.h offer a sample implementation of adding a general device in software's view.
 *	So if you want to add any new device into the system, follow the steps listed below :
 *  1. CREATE *NEW DEVICE NAME*_Device.h in the same folder of My_Device.h and COPY all codes ( maybe except the top comments ) from My_Device.h to the new one.
 *  2. Within *NEW DEVICE NAME*_Device.h, REPLACE all "My_Device" by "*NEW DEVICE NAME*" ( like "FlashStorage" for e.g. ).
 *	   You can use Ctrl+Shift+F shortcut and click "Replace in Files" to do this work,
 *     but NOTE that below "LOOK IN" column, choose 'current project'( the newly created .h ) since you don't want to ruin My_Device.h/.c, do you?
 *  3. CREATE *NEW DEVICE NAME*_Device.c in the same folder of My_Device.c and COPY all codes ( maybe except the top comments ) from My_Device.h to the new one.
 *  4. Within *NEW DEVICE NAME*_Device.c, REPLACE all "My_Device" by "*NEW DEVICE NAME*" ( like "FlashStorage" for e.g. ).
 *	   You can use Ctrl+Shift+F shortcut and click "Replace in Files" to do this work,
 *     but NOTE that below "LOOK IN" column, choose 'current project'( the newly created .c ) since you don't want to ruin My_Device.h/.c, do you?
 *  5. Within *NEW DEVICE NAME*_Device.c, only add contents within these functions ( the add-in contents should be decided by you since this is only a rough guide about a general device )
 *			New_*NEW DEVICE NAME*(),
 *			*NEW DEVICE NAME*_init(),
 *			*NEW DEVICE NAME*_reset(),
 *			*NEW DEVICE NAME*_Execute(),
 *			*NEW DEVICE NAME*_Configure()
 *  6. If you want to create some private function to help execute some logics, feel free to add.
 */

#include "devices/other-device/inc/My_Device.h"
#include <stdlib.h>
#include <stdio.h>

My_Device_Abstract_FctnTable MyDevice_abstract_vmt = {
	My_Device_VTinit,
	My_Device_init,
	My_Device_reset,
	My_Device_getType
};

My_Device_FctnTable MyDevice_vmt = {
	My_Device_Tinit,
	My_Device_Execute,
	My_Device_Configure
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
	return TYPE_DEVICE;
}

MyDevice* New_My_Device( int infonum )
{
	MyDevice *p = malloc(sizeof(MyDevice));
	p->info = infonum;
	My_Device_VTinit(p);
	return p;
}




void My_Device_Tinit(MyDevice *this)
{
	this->vmt = &MyDevice_vmt;
}

int My_Device_Execute(MyDevice *this)
{
	return 0;
}

int My_Device_Configure(MyDevice *this)
{
	return 0;
}