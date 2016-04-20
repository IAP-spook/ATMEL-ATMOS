/*
 * scheduler.h
 *
 * Created: 2016/4/14 23:05:30
 *  Author: AB
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

#define MAX_EVENTS	16
#define MAX_SLEEP_INTERVAL 8000 // current max time
#define EV_NULL ( struct event * ) (LL_NULL)

	
/* list anchors -- important, but ignore them; they are never used directly */
llobject_t TQ;
llobject_t FL;
struct event *timeoutq;
struct event *freelist;

struct event queue[ MAX_EVENTS ];


/* API of the Round-Robin Scheduler */
int load_new_sensor( int timeout, int repeat, BaseSensor *device_ptr, int otherinfo );
void init_timeoutq();
int get_next_interval();
void insert_timeoutq_event( struct event * event_pointer);
int handle_timeoutq_event( );

#ifdef DEBUG

void dump_timeoutq();
void foo();

#endif


#endif /* SCHEDULER_H_ */