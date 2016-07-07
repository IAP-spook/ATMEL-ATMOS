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

/** @name Declaration and Typedef of the base struct
 *
 */
///@{
struct _Base_Device;
typedef struct _Base_Device BaseDevice;
///@}

/** @name 'Virtual functions' implementation of the struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and you shouldn't worry about modifying this for now
 */
///@{
typedef struct _Base_Device_vt
{
	void (*VTinit)( BaseDevice* );
	int (*init)( BaseDevice* );
	int (*reset)( BaseDevice* );
	int (*getType)( BaseDevice* );
} Base_Device_FctnTable;
///@}


/** @name Definition of the _Base_Device struct
 *  _Base_Device acts like a base class in C++ and only contains a struct that wraps bunch of 'virtual functions' in it.
 */
///@{
struct _Base_Device
{
	Base_Device_FctnTable *device_vt;
};
///@}

/** @name Declaration of the 'virtual functions'
 *  Declaration of the 'virtual functions' or said interface within current struct
 */
///@{
void BaseDevice_VTinit( BaseDevice* );/**<@brief VirtualTable Init, which loads all following interfaces into the _Base_Device struct. TRY NOT MODIFY THIS. */
int BaseDevice_init( BaseDevice* );/**<@brief Interface that is used for initialization for any general device( sensors included ).  */
int BaseDevice_reset( BaseDevice* );/**<@brief Interface that is used for reset ( most likely re-initialization ) when getting some unexpected error for any general device( sensors included ).  */
int BaseDevice_getType( BaseDevice* );/**<@brief Interface that is used for getting type for current device( sensors included ), and it is not so useful for now(06/27/2016).  */
///@}

/** @name Declaration of the new API for this struct
 *  Not useful for base struct except for testing but it just demonstrates the way to do it with other derivative struct.
 */
///@{
BaseDevice* New_Base_Device();
///@}
#endif