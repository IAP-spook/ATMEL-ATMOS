/*
 * \file FlashStorage_Device.h
 * \brief Declaration of FlashStorageDevice struct.
 * Created: 8/10/2016
 *  Author: C. Martin
 *
 */


#ifndef FlashStorage_DEVICE_H
#define FlashStorage_DEVICE_H

#include "devices/base-class/inc/Base_Device.h"
#include "devices/other-device/inc/N25Q.h"

FLASH_DEVICE_OBJECT N25Qfdo;
ParameterType N25Qpara;
ReturnType N25Qret;

/** @name Declaration and Typedef of the derived struct
 *
 */
///@{
struct _FlashStorage_Device;
typedef struct _FlashStorage_Device FlashStorageDevice;
///@}

/** @name 'Virtual functions' implementation of the struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and you shouldn't worry about modifying this for now
 */
///@{
typedef struct _FlashStorage_vmt
{
	void (*Device_Tinit)(FlashStorageDevice*);
	int (*Execute)(FlashStorageDevice*);
	int (*Configure)(FlashStorageDevice*);
} FlashStorage_FctnTable;
///@}

/** @name 'Virtual functions' implementation of the base struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and you shouldn't worry about modify this for now
 */
///@{
typedef struct _FlashStorage_Abstract_vmt
{
	void (*VTinit)( FlashStorageDevice* );
	int (*init)( FlashStorageDevice* );
	int (*reset)( FlashStorageDevice* );
	int (*getType)( FlashStorageDevice* );
} FlashStorage_Abstract_FctnTable;
///@}

/** @name Definition of the _FlashStorage_Device struct
 *  _FlashStorage_Device acts like a derivative class in C++ which derives something from BaseDevice and have some properties of the non-sensor device. Notice it has a FlashStorage_FctnTable struct pointer wrapping bunch of 'virtual functions' for derivative devices.
 */
///@{
struct _FlashStorage_Device
{
	BaseDevice abstract;
	FlashStorage_FctnTable *vmt;
	int info;
};
///@}

/** @name Declaration of the 'virtual functions' offered in base device struct
 *  Declaration of the 'virtual functions' or said interface within base device struct
 */
///@{
void FlashStorage_VTinit( FlashStorageDevice* );/**<@brief VirtualTable Init, which loads all following interfaces into the BaseDevice struct. TRY NOT MODIFY THIS. */
int FlashStorage_init( FlashStorageDevice* );/**<@brief Interface that is used for initialization for any general devices.  */
int FlashStorage_reset( FlashStorageDevice* );/**<@brief Interface that is used for reset ( most likely re-initialization ) when getting some unexpected error for any general devices.  */
int FlashStorage_getType( FlashStorageDevice* );/**<@brief Interface that is used for getting type for current devices, and it is not so useful for now(06/27/2016).  */
///@}


/** @name Declaration of the new API for this struct
 *  It is used for instantiate a new device in the manage system. You can adjust the input parameter as long as you use it the same way as you define it.
 */
///@{
FlashStorageDevice* New_FlashStorage_Device( int num );
///@}

/** @name Declaration of the virtual APIs only used for device ( not for sensors )
 *  It is used for instantiate a new device in the system. You can adjust the input parameter as long as you use it the same way as you define it.
 */
///@{
void FlashStorage_Tinit(FlashStorageDevice *this);/**<@brief VirtualTable Init, which loads all following interfaces into the FlashStorage_FctnTable struct. TRY NOT MODIFY THIS. */
int FlashStorage_Execute(FlashStorageDevice *this);/**<@brief The main logic function of the FlashStorage_Device, it should be automatically executed as long as you configure it well in ATMOS.c, devicelist.h and parameter.h  */
int FlashStorage_Configure(FlashStorageDevice *this);/**<@brief The configuration function of the FlashStorage_Device, it should be automatically executed as long as you configure it well in ATMOS.c, devicelist.h and parameter.h  */
///@}

#endif
