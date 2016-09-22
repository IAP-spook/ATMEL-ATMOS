/*
 * \file DemoStorage_Device.h
 * \brief Declaration of DemoStorageDevice struct.
 * Created: 2016/4/21 23:55:29
 *  Author: Anxin Bai
 *
 * The DemoStorage_Device.h along with DemoStorage_Device.c serve as a virtual storage or memory device that simulated by the mcu( or cpu ).
 * It is only a demo part of the software but in the future when we got real flash api to access flash storage we can transform this class/struct into two.
 * One is MemoryDevice, a pure virtual device, and it is responsible for taking advantage of memory. 
 * It is literally the same as this one, except the renaming issue ( replace all DemoStorage to Memory to make it clearer and rename the file to Memory_Device.h/.c ).
 * The other one should be the FlashStorage_Device, and it is responsible for flushing ( storing any valid stuff from the Memory_Device into flash disk ).
 * This one should be kind of different, and I would like to discuss some APIs here.
 * The constructor should be like New_FlashStorage_Device( int num, MemoryDevice * ptr ), which takes a MemoryDevice struct pointer just like LoadData_Device since we want to access the memory.
 * The main function api, aka. FlashStorage_Device_Execute(DemoStorageDevice *this), should take the MemoryDevice pointer and get access to its DataUnit array by accessing the DataUnit pointer and the current position within MemoryDevice struct.
 * After that, we get the data, we just want to transfer the DataUnit to the flash storage, one by one aligned in block. Please notice that we should think twice to decide how many bytes each data should occupy, and we want to record that since we definitely want to read it back while communicating. In read-back procedure, we want know each block size and it is a reverse procedure of writing DataUnit to disk.
 * So in short, you need to access the memory data via MemoryDevice pointer first, then using some flash storage api to help write the data along with the timestamp one by one in disk.
 */


#ifndef DemoStorage_DEVICE_H
#define DemoStorage_DEVICE_H

#include "devices/base-class/inc/Base_Device.h"
#include "utilities/inc/data_unit.h"

#define MAX_HOLD_DATA 15	/**<@brief Max DataUnit size we want to take use for storing in memory. */

/** @name Declaration and Typedef of this derived struct
 *
 */
///@{
struct _DemoStorage_Device;
typedef struct _DemoStorage_Device DemoStorageDevice;
///@}

/** @name 'Virtual functions' implementation of the struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and you shouldn't worry about modifying this for now
 */
///@{
typedef struct _DemoStorage_Device_vmt
{
	void (*Device_Tinit)(DemoStorageDevice*);
	int (*Execute)(DemoStorageDevice*);
	int (*Configure)(DemoStorageDevice*);
	DataUnit * (*get_CurDataUnit)(DemoStorageDevice *);
} DemoStorage_Device_FctnTable;
///@}

/** @name 'Virtual functions' implementation of the base struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and you shouldn't worry about modify this for now.
 */
///@{
typedef struct _DemoStorage_Device_Abstract_vmt
{
	void (*VTinit)( DemoStorageDevice* );
	int (*init)( DemoStorageDevice* );
	int (*reset)( DemoStorageDevice* );
	int (*getType)( DemoStorageDevice* );
} DemoStorage_Device_Abstract_FctnTable;
///@}

/** @name Definition of the _DemoStorage_Device struct
 *  _DemoStorage_Device acts like a derivative class in C++ which derives something from BaseDevice and have some properties of the non-sensor device. Notice it has a My_Device_FctnTable struct pointer wrapping bunch of 'virtual functions' for derivative devices.
 */
///@{
struct _DemoStorage_Device
{
	BaseDevice abstract;
	DemoStorage_Device_FctnTable *vmt;
	int info;
	int cur_pos;	/**<@brief cur_pos stands for the current valid index. numbers less than it are valid data indexes, and numbers greater than it and less than MAX_HOLD_DATA are slots have not been used */
	DataUnit * StoragesData;
	
};
///@}

/** @name Declaration of the 'virtual functions' offered in base sensor struct 
 *  Declaration of the 'virtual functions' or said interface within base device struct
 */
///@{
void DemoStorage_Device_VTinit( DemoStorageDevice* );
int DemoStorage_Device_init( DemoStorageDevice* );
int DemoStorage_Device_reset( DemoStorageDevice* );
int DemoStorage_Device_getType( DemoStorageDevice* );
///@}

/** @name Declaration of the new API for this struct
 *  It is used for instantiate the device in the system. You can adjust the input parameter as long as you use it the same way as you define it.
 */
///@{
DemoStorageDevice* New_DemoStorage_Device( int num );
///@}

/** @name Declaration of the virtual APIs only used for device ( not for sensors )
 *  It is used for instantiate the device in the system. You can adjust the input parameter as long as you use it the same way as you define it.
 */
///@{
void DemoStorage_Device_Tinit(DemoStorageDevice *this);
int DemoStorage_Device_Execute(DemoStorageDevice *this);
int DemoStorage_Device_Configure(DemoStorageDevice *this);
DataUnit * DemoStorageget_CurDataUnit(DemoStorageDevice *this);
///@}
#endif