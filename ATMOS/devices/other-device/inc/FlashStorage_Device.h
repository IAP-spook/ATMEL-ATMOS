/*
 * \file FlashStorage_Device.h
 * \brief Declaration of FlashStorageDevice struct.
 * Created: 2016/4/21 23:55:29
 *  Author: Anxin Bai
 *
 *
 *	This file along with FlashStorage_Device.c offer a sample implementation of adding a general device in software's view. 
 *	So if you want to add any new device into the system, follow the steps listed below :
 *  1. CREATE *NEW DEVICE NAME*_Device.h in the same folder of FlashStorage_Device.h and COPY all codes ( maybe except the top comments ) from FlashStorage_Device.h to the new one.
 *  2. Within *NEW DEVICE NAME*_Device.h, REPLACE all "FlashStorage_Device" by "*NEW DEVICE NAME*" ( like "FlashStorage" for e.g. ). 
 *	   You can use Ctrl+Shift+F shortcut and click "Replace in Files" to do this work,
 *     but NOTE that below "LOOK IN" column, choose 'current project'( the newly created .h ) since you don't want to ruin FlashStorage_Device.h/.c, do you?
 *  3. CREATE *NEW DEVICE NAME*_Device.c in the same folder of FlashStorage_Device.c and COPY all codes ( maybe except the top comments ) from FlashStorage_Device.h to the new one.
 *  4. Within *NEW DEVICE NAME*_Device.c, REPLACE all "FlashStorage_Device" by "*NEW DEVICE NAME*" ( like "FlashStorage" for e.g. ). 
 *	   You can use Ctrl+Shift+F shortcut and click "Replace in Files" to do this work,
 *     but NOTE that below "LOOK IN" column, choose 'current project'( the newly created .c ) since you don't want to ruin FlashStorage_Device.h/.c, do you?
 *  5. Within *NEW DEVICE NAME*_Device.c, only add contents within these functions ( the add-in contents should be decided by you since this is only a rough guide about a general device )
 *			New_*NEW DEVICE NAME*(),
 *			*NEW DEVICE NAME*_init(),
 *			*NEW DEVICE NAME*_reset(),
 *			*NEW DEVICE NAME*_Execute(),
 *			*NEW DEVICE NAME*_Configure()
 *  6. If you want to create some private function to help execute some logics, feel free to add.
 */


#ifndef FlashStorage_DEVICE_H
#define FlashStorage_DEVICE_H

#include "devices/base-class/inc/Base_Device.h"

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
typedef struct _FlashStorage_Device_vmt
{
	void (*Device_Tinit)(FlashStorageDevice*);
	int (*Execute)(FlashStorageDevice*);
	int (*Configure)(FlashStorageDevice*);
} FlashStorage_Device_FctnTable;
///@}

/** @name 'Virtual functions' implementation of the base struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and you shouldn't worry about modify this for now
 */
///@{
typedef struct _FlashStorage_Device_Abstract_vmt
{
	void (*VTinit)( FlashStorageDevice* );
	int (*init)( FlashStorageDevice* );
	int (*reset)( FlashStorageDevice* );
	int (*getType)( FlashStorageDevice* );
} FlashStorage_Device_Abstract_FctnTable;
///@}

/** @name Definition of the _FlashStorage_Device struct
 *  _FlashStorage_Device acts like a derivative class in C++ which derives something from BaseDevice and have some properties of the non-sensor device. Notice it has a FlashStorage_Device_FctnTable struct pointer wrapping bunch of 'virtual functions' for derivative devices.
 */
///@{
struct _FlashStorage_Device
{
	BaseDevice abstract;
	FlashStorage_Device_FctnTable *vmt;
	int info;
};
///@}

/** @name Declaration of the 'virtual functions' offered in base device struct
 *  Declaration of the 'virtual functions' or said interface within base device struct
 */
///@{
void FlashStorage_Device_VTinit( FlashStorageDevice* );/**<@brief VirtualTable Init, which loads all following interfaces into the BaseDevice struct. TRY NOT MODIFY THIS. */
int FlashStorage_Device_init( FlashStorageDevice* );/**<@brief Interface that is used for initialization for any general devices.  */
int FlashStorage_Device_reset( FlashStorageDevice* );/**<@brief Interface that is used for reset ( most likely re-initialization ) when getting some unexpected error for any general devices.  */
int FlashStorage_Device_getType( FlashStorageDevice* );/**<@brief Interface that is used for getting type for current devices, and it is not so useful for now(06/27/2016).  */
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
void FlashStorage_Device_Tinit(FlashStorageDevice *this);/**<@brief VirtualTable Init, which loads all following interfaces into the FlashStorage_Device_FctnTable struct. TRY NOT MODIFY THIS. */
int FlashStorage_Device_Execute(FlashStorageDevice *this);/**<@brief The main logic function of the FlashStorage_Device, it should be automatically executed as long as you configure it well in ATMOS.c, devicelist.h and parameter.h  */
int FlashStorage_Device_Configure(FlashStorageDevice *this);/**<@brief The configuration function of the FlashStorage_Device, it should be automatically executed as long as you configure it well in ATMOS.c, devicelist.h and parameter.h  */
///@}

#endif
