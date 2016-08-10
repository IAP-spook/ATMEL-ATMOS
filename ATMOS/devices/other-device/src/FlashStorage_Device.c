/*
 * \file FlashStorage_Device.c
 * \brief Implementation of FlashStorageDevice struct.
 * Created: 8/10/2016
 *  Author: C. Martin
 *
 */
 #include "devices/other-device/inc/FlashStorage_Device.h"
 #include "devices/other-devices/inc/N25Q.h"
 #include <stdlib.h>
 #include <stdio.h>

FlashStorage_Abstract_FctnTable MyDevice_abstract_vmt = {
 	FlashStorage_VTinit,
 	FlashStorage_init,
 	FlashStorage_reset,
 	FlashStorage_getType
 };

 FlashStorage_FctnTable MyDevice_vmt = {
 	FlashStorage_Tinit,
 	FlashStorage_Execute,
 	FlashStorage_Configure
 };


 void FlashStorage_VTinit( MyDevice *this )
 {
 	this->abstract.device_vt = &MyDevice_abstract_vmt;
 	return;
 }

 int FlashStorage_init( MyDevice *this )
 {
 	return 0;
 }

 int FlashStorage_reset( MyDevice *this )
 {
 	return 0;
 }

 int FlashStorage_getType( MyDevice *this )
 {
 	return TYPE_DEVICE;
 }

 MyDevice* New_FlashStorage( int infonum )
 {
 	MyDevice *p = malloc(sizeof(MyDevice));
 	p->info = infonum;
 	FlashStorage_VTinit(p);
 	return p;
 }




 void FlashStorage_Tinit(MyDevice *this)
 {
 	this->vmt = &MyDevice_vmt;
 }

 int FlashStorage_Execute(MyDevice *this)
 {
 	return 0;
 }

 int FlashStorage_Configure(MyDevice *this)
 {
 	return 0;
 }
