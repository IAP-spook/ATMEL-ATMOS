/**
 * \file Base_Sensor.h
 *
 * \brief Declaration of BaseSensor struct, which derived from BaseDevice
 * 
 *  Author: Anxin Bai
 */ 

#ifndef _BASE_SENSOR_H_
#define _BASE_SENSOR_H_
#include "devices/base-class/inc/Base_Device.h"
#include "utilities/inc/data_unit.h"

/** @name Extern declaration of the pointer of the data struct 
 *  The pointer of the data struct which holds all varieties of sensors' data of current collecting time period, is used in Collect() interface. Notice that the pointer is periodically updated in LoadData_Device_Execute() within LoadData_Device.c every collecting period.
 */
///@{
extern DataUnit * cur_data;
///@}

/** @name Declaration and Typedef of the base sensor struct
 *
 */
///@{
struct _Base_Sensor;
typedef struct _Base_Sensor BaseSensor;
///@}

/** @name 'Virtual functions' implementation of the base sensor struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and feel free to add other interfaces if you think some sensors need them
 */
///@{
typedef struct _Base_vmt
{
	void (*FctnInit)(BaseSensor*);
	int (*Configure)(BaseSensor*);
	int (*Request)(BaseSensor*);
	int (*Collect)(BaseSensor*);
	int (*Error)(BaseSensor*);

} Base_FctnTable;
///@}

/** @name 'Virtual functions' implementation of the base struct
 *  c's implmentation of virtual function ( interface in Java or C# ) and you shouldn't worry about modify this for now
 */
///@{
typedef struct _Abstract_vmt
{
	void (*VTinit)( BaseSensor* );
	int (*init)( BaseSensor* );
	int (*reset)( BaseSensor* );
	int (*getType)( BaseSensor* );
} Abstract_FctnTable;
///@}

/** @name Definition of the _Base_Sensor struct
 *  _Base_Sensor acts like a derivative class in C++ which derive something from BaseDevice and have some properties of the sensor. Notice it has a Base_FctnTable struct pointer wrapping bunch of 'virtual functions' for derivative sensors.
 */
///@{
struct _Base_Sensor
{
	BaseDevice abstract;	/**<@brief Stuff derivated from the base device struct */
	int test_num;			/**<@brief A number property currently no use. */
	int StartNum;			/**<@brief StartNum marks the start order of the first start data among all sensors' data. No need to worry about it, some automation tricks. */
	int NumOfData;			/**<@brief NumOfData marks the number of data within this sensors. No need to worry about it, some automation tricks. */
	int (*getStartNum)(BaseSensor*);	/**<@brief API to get the right StartNum. No need to worry about it, some automation tricks. */
	int (*getEndNum)(BaseSensor*);		/**<@brief API to get the order of the last data of this sensor. No need to worry about it, some automation tricks. */
	Base_FctnTable *vmt;				/**<@brief A struct pointer that acts like virtual function table in c++ and is used for derivation consistency. */
};
///@}

/** @name Declaration of the 'virtual functions' offered in base sensor struct 
 *  Declaration of the 'virtual functions' or said interface within base sensor struct
 */
///@{
void Base_FctnInit( BaseSensor *this);/**<@brief VirtualTable Init, which loads all following interfaces into the _Base_Device struct. TRY NOT MODIFY THIS. */
int Base_Configure( BaseSensor *this );/**<@brief Interface that is used for configuration for sensors. */
int Base_Request( BaseSensor *this );/**<@brief Interface that is used for requesting data and get waiting time from sensors. If waiting time is 0, collect data immediately. */
int Base_Collect( BaseSensor *this );/**<@brief Interface that is used for collecting data right now. */
int Base_Error( BaseSensor *this );/**<@brief Interface that is used for dealing with errors. A feasible way is to re-initialization though but I prefer leave an interface here */
///@}

/** @name Declaration of the 'virtual functions' offered in base device struct 
 *  Declaration of the 'virtual functions' or said interface within base device struct
 */
///@{
void Base_VTinit( BaseSensor* );/**<@brief VirtualTable Init, which loads all following interfaces into the _Base_Device struct. TRY NOT MODIFY THIS. */
int Base_init( BaseSensor* );/**<@brief Interface that is used for initialization for any general sensors.  */
int Base_reset( BaseSensor* );/**<@brief Interface that is used for reset ( most likely re-initialization ) when getting some unexpected error for any general sensors.  */
int Base_getType( BaseSensor* );/**<@brief Interface that is used for getting type for current sensors, and it is not so useful for now(06/27/2016).  */
///@}

/** @name Declaration of the internal API
 *  Declaration of the internal API that used for automatically collecting data to the cur_data in the right order. It is strongly related to the StartNum and the NumOfData. Once again, no need to worry about it, some automation tricks.
 */
///@{
int fctn_getStartNum( BaseSensor* );
int fctn_getEndNum( BaseSensor* );
///@}

/** @name Declaration of the new API for this struct
 *  Not useful for base sensor struct except for testing but it just demonstrates the way to do it with other derivative struct.
 */
///@{
BaseSensor* New_Base_Sensor( int num, int NumData  );
///@}


#endif