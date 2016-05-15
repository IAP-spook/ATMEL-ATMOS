/**
 * \file scheduler.h
 *
 * \brief The resources that hold all events and manage useful events in-order within a linked list. 
 *
 * Created: 2016/4/14 23:05:30
 *  Author: Anxin Bai
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "scheduler/inc/event.h"
#include "scheduler/inc/handler.h"
#include "utilities/inc/llist.h"
#include "drivers/inc/int_timer.h"


/* #define DEBGU */
#ifndef DEBUG
#define DEBUG
#endif

#define MAX_EVENTS	16 /**<@brief Max number of available device within a scheduler*/
#define MAX_SLEEP_INTERVAL 8000 /**<@brief The max sleep time(ms) with current timer support*/
#define EV_NULL ( struct event * ) (LL_NULL) /**<@brief Null pointer of a event */

	
/** @name list anchors
 *  Important, but ignore them; they are never used directly
 */
///@{
llobject_t TQ;/**<@brief TimeoutQueue anchor*/
llobject_t FL;/**<@brief FreeList anchor*/
///@}

/** @name Event list/queue
 *  The linked list resource, where each node is an event
 */
///@{
struct event *timeoutq;	/**<@brief TimeoutQueue, which holds the events in line */
struct event *freelist; /**<@brief FreeList, which holds the events ready to hold new device */
struct event queue[ MAX_EVENTS ]; /**<@brief Declaration of event resources, and all events are within this queue  */



/** @name API of scheduler
 *  API of the Round-Robin Scheduler
 */
///@{
int load_new_sensor( int32_t timeout, int32_t repeat, BaseSensor *device_ptr, int otherinfo );
int load_new_device( int32_t timeout, int32_t repeat, BaseDevice *device_ptr, int otherinfo );
void init_timeoutq();
void next_event_time_collapse(int32_t sleep_time);
int32_t get_next_interval();
void insert_timeoutq_event( struct event * event_pointer);
int handle_timeoutq_event( );
///@}

#ifdef DEBUG

void dump_timeoutq();

#endif


#endif /* SCHEDULER_H_ */