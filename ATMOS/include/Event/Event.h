/*
 * Event.h
 *
 * Created: 2016/4/4 13:32:45
 *  Author: AB
 */ 


#ifndef _EVENT_H_
#define _EVENT_H_
#include <stdio.h>
#include "utilities/llist.h"
// #include "llist.h"
#include "sensors/Base_Sensor.h"

/* #define DEBGU */
#define DEBUG
#define MAX_EVENTS	16
#define MAX_SLEEP_INTERVAL 0x04 // That is a second currently
#define EV_NULL ( struct event * ) (LL_NULL)

enum DeviceState { New, Ready, Running, Terminated, Oops };

struct event 
{
	LL_PTRS;
	int timeout;
	int repeat_interval;
    int borrow_timeout;
	BaseSensor * sp;
	int info;
    enum DeviceState cur_state;
    void (* run)(struct event *);
};

struct event queue[ MAX_EVENTS ];

/* list anchors -- important, but ignore them; they are never used directly */
llobject_t TQ;
llobject_t FL;
struct event *timeoutq;
struct event *freelist;

/* run-next function */
static void run_next( struct event *p );



/* API of the Round-Robin Scheduler */
int load_new_sensor( int timeout, int repeat, BaseSensor *sensor_ptr, int otherinfo );
void init_timeoutq();
int get_next_interval();
void insert_timeoutq_event( struct event * event_pointer);
int handle_timeoutq_event( );

#ifdef DEBUG

void dump_timeoutq();
void foo();

#endif


#endif /* EVENT_H_ */