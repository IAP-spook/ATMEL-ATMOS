/**
 * \file Event.h
 *
 * \brief A container struct to hold device/sensor pointer and corresponding timeout info. 
 *
 * Created: 2016/4/4 13:32:45
 *  Author: Anxin Bai
 */ 


#ifndef _EVENT_H_
#define _EVENT_H_

#include <stdio.h>
#include "utilities/inc/common.h"
#include "utilities/inc/llist.h"
#include "devices/base-class/inc/Base_Sensor.h"
#include "devices/other-device/inc/DemoStorage_Device.h"
#include "devices/other-device/inc/LoadData_Device.h"

/*************************************************************************//**
  @brief A container struct to hold device/sensor pointer and corresponding timeout info. 
*****************************************************************************/
struct event 
{
	LL_PTRS;
	//! next timeout value in ms.
	int timeout;
	//! the repeat period of the event in ms. 0 means no repeatence.
	int repeat_interval;
    int borrow_timeout;
	BaseSensor * sp;
	LoadDataDevice *load_p;
	DemoStorageDevice *store_p;
	int info;
    enum DeviceState cur_state;
    int (* run)(struct event *);
};










#endif /* EVENT_H_ */