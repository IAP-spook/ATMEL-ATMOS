/*
 * handler.h
 *
 * Created: 2016/4/15 0:42:24
 *  Author: AB
 */ 


#ifndef HANDLER_H_
#define HANDLER_H_

#include "scheduler/inc/event.h"
#include "scheduler/inc/handler.h"
#include "utilities/inc/llist.h"
#include "drivers/inc/int_timer.h"

/* sensor_handler function */
int sensor_handler( struct event *p );
int device_handler( struct event *p );
int storeData_handler( struct event *p );



#endif /* HANDLER_H_ */