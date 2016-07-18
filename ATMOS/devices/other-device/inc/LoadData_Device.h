/*
 * \file LoadData_Device.h
 * \brief Declaration of LoadDataDevice struct.
 * Created: 2016/4/21 23:55:29
 *  Author: Anxin Bai
 *
 *
 * The LoadData_Device.h along with LoadData_Device.c serve as virtual device that simulated by the mcu( or cpu ). 
 * It is responsible to access some limited memory ( currently 07/07/2016 managed by DemoStorage_Device.h/.c and defined in /utilities/inc/data_unit.h ) and mark the memory slot that should be used for current period.
 * At the beginning of each collecting period the execute function, it accesses the limited memory and move to the next available slot and let cur_data pointer points to it.
 * So during collecting, all sensors use cur_data pointer to load new data but don't need to worry about where in memory to save it and how to manage the memory.
 * NOTE at 07/07/2016 : You definitely still need this virtual device in the future. But DemoStorageDevice should be modified to act as Memory_Device ( a virtual device to help manage memory ).
 */ 


#ifndef LoadData_DEVICE_H
#define LoadData_DEVICE_H

#include "devices/base-class/inc/Base_Device.h"
#include "devices/other-device/inc/DemoStorage_Device.h"
#include "utilities/inc/data_unit.h"

/** @name Declaration of the current data unit and the current time stamp
 *
 */
///@{
extern DataUnit * cur_data;/**<@brief DataUnit that used to hold various sensor collections in this collecting period.  */
extern tm cur_time;/**<@brief Time stamp that used to mark the timestamp of the start time of this collecting period  */
///@}

/** @name Declaration and Typedef of this derived struct
 *
 */
///@{
struct _LoadData_Device;
typedef struct _LoadData_Device LoadDataDevice;
///@}


/** @name 'Virtual functions' implementation of the struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and you shouldn't worry about modifying this for now
 */
///@{
typedef struct _LoadData_Device_vmt
{
	void (*Device_Tinit)(LoadDataDevice*);
	int (*Execute)(LoadDataDevice*);
	int (*Configure)(LoadDataDevice*);
} LoadData_Device_FctnTable;
///@}

/** @name 'Virtual functions' implementation of the base struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and you shouldn't worry about modify this for now.
 */
///@{
typedef struct _LoadData_Device_Abstract_vmt
{
	void (*VTinit)( LoadDataDevice* );
	int (*init)( LoadDataDevice* );
	int (*reset)( LoadDataDevice* );
	int (*getType)( LoadDataDevice* );
} LoadData_Device_Abstract_FctnTable;
///@}

/** @name Definition of the _LoadData_Device struct
 *  _LoadData_Device acts like a derivative class in C++ which derives something from BaseDevice and have some properties of the non-sensor device. Notice it has a My_Device_FctnTable struct pointer wrapping bunch of 'virtual functions' for derivative devices.
 */
///@{
struct _LoadData_Device
{
	BaseDevice abstract;
	LoadData_Device_FctnTable *vmt;
	int info;
	DemoStorageDevice *store_device;/**<@brief This DemoStorageDevice ( or called Memory_Device later is needed since this class is only responsible for accessing certain memory and get a proper one to load data but not for managing the memory.)  */
};
///@}


/** @name Declaration of the 'virtual functions' offered in base sensor struct 
 *  Declaration of the 'virtual functions' or said interface within base device struct
 */
///@{
void LoadData_Device_VTinit( LoadDataDevice* );
int LoadData_Device_init( LoadDataDevice* );
int LoadData_Device_reset( LoadDataDevice* );
int LoadData_Device_getType( LoadDataDevice* );
///@}

/** @name Declaration of the new API for this struct
 *  It is used for instantiate the device in the system. You can adjust the input parameter as long as you use it the same way as you define it.
 *  Note you need to pass in a pointer of DemoStorageDevice ( or Memory_Device in the future ) since this class is only responsible for accessing certain memory and get a proper one to load data but not for managing the memory.
 */
///@{
LoadDataDevice* New_LoadData_Device( int num, DemoStorageDevice * ptr );
///@}

/** @name Declaration of the virtual APIs only used for device ( not for sensors )
 *  It is used for instantiate the device in the system. You can adjust the input parameter as long as you use it the same way as you define it.
 */
///@{
void LoadData_Device_Tinit(LoadDataDevice *this);
int LoadData_Device_Execute(LoadDataDevice *this);
int LoadData_Device_Configure(LoadDataDevice *this);
///@}
#endif