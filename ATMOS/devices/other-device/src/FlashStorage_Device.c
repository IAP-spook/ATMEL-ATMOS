/*
 * \file FlashStorage_Device.c
 * \brief Implementation of FlashStorageDevice struct.
 * Created: 8/10/2016
 *  Author: C. Martin
 *
 */
 #include "devices/other-device/inc/FlashStorage_Device.h"
 #include "devices/other-device/inc/N25Q.h"
 #include <stdlib.h>
 #include <stdio.h>

FlashStorage_Abstract_FctnTable FlashStorageDevice_abstract_vmt = {
 	FlashStorage_VTinit,
 	FlashStorage_init,
 	FlashStorage_reset,
 	FlashStorage_getType
 };

 FlashStorage_FctnTable FlashStorageDevice_vmt = {
 	FlashStorage_Tinit,
 	FlashStorage_Execute,
 	FlashStorage_Configure
 };


 void FlashStorage_VTinit( FlashStorageDevice *this )
 {
 	this->abstract.device_vt = &FlashStorageDevice_abstract_vmt;
 	return;
 }

 int FlashStorage_init( FlashStorageDevice *this )
 {
 	return 0;
 }

 int FlashStorage_reset( FlashStorageDevice *this )
 {
 	return 0;
 }

 int FlashStorage_getType( FlashStorageDevice *this )
 {
 	return TYPE_STORAGE_DEVICE;
 }

 FlashStorageDevice* New_FlashStorage_Device( int infonum )
 {
 	FlashStorageDevice *p = malloc(sizeof(FlashStorageDevice));
 	p->info = infonum;
 	FlashStorage_VTinit(p);
  FlashStorage_Tinit(p);
  N25Qret = N25Q_Init(&N25Qfdo);
  if (Flash_WrongType == N25Qret)
	{
		printf("Sorry, no device detected.\n");
		return -1;
	}
  printf("Flash Storage Initialized!\n");
 	return p;
 }




 void FlashStorage_Tinit(FlashStorageDevice *this)
 {
 	this->vmt = &FlashStorageDevice_vmt;
 }

 int FlashStorage_Execute(FlashStorageDevice *this)
 {
 	return 0;
 }

 int FlashStorage_Configure(FlashStorageDevice *this)
 {
 	return 0;
 }
