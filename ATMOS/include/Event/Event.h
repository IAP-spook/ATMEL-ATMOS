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

#define MAX_EVENTS	8
#define MAX_SLEEP_INTERVAL 0x1425 // not decided yet
#define EV_NULL ( struct event * ) (LL_NULL)
typedef void (* pfv_t)();


struct event 
{
	LL_PTRS;
	int timeout;
	int repeat_interval;
	pfv_t go;
	unsigned int data;
};

struct event queue[ MAX_EVENTS ];


// list anchors -- important, but ignore them; they are never used directly
llobject_t TQ;
llobject_t FL;

struct event *timeoutq;
struct event *freelist;

void insert_timeoutq_event( struct event * ep);
void init_timeoutq();
int bring_timeoutq_current();
void create_timeoutq_event(int timeout, int repeat, pfv_t function, unsigned int data);
int handle_timeoutq_event( );
void insert_timeoutq_event( struct event * ep);

void dump_timeoutq();
void foo();


#endif /* EVENT_H_ */