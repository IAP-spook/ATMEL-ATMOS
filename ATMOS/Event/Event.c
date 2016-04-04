/*
 * Event.c
 *
 * Created: 2016/4/4 13:35:16
 *  Author: AB
 */ 
#include "Event/Event.h"

//
// sets up concept of local time
// initializes the timeoutq and freelist
// fills the freelist with entries
// timeoutq is empty
//
void init_timeoutq()
{
	int i;

	//TODO
	//then_usec = now_usec();

	timeoutq = (struct event *)&TQ;
	LL_INIT(timeoutq);
	freelist = (struct event *)&FL;
	LL_INIT(freelist);

	for (i=0; i<MAX_EVENTS; i++) {
		struct event *ep = &queue[i];
		LL_PUSH(freelist, ep);
	}

	return;
}

//
// account for however much time has elapsed since last update
// return timeout or MAX
//
int bring_timeoutq_current()
{
	int wait_time = MAX_SLEEP_INTERVAL;
	struct event * ev = ( struct event * ) LL_TOP( timeoutq );
	if( EV_NULL == ev )
	return wait_time;

	wait_time = ( wait_time > ( ev->timeout ) ) ? ( ev->timeout ) : wait_time ;

	return wait_time;
}

//
// dump timeout queue
//
void dump_timeoutq()
{
	printf("\n\n\n=================\nenter function\n================\n");
	struct event * it;
	LL_EACH(timeoutq,it,struct event )
	{
		if( it != NULL )
		printf("timeout %d\n",it->timeout);
		else
		printf("NULL!\n");
	}
	printf("=================\nexit function\n================\n\n\n");
}

//
// get a new event structure from the freelist,
// initialize it, and insert it into the timeoutq
//
void create_timeoutq_event(int timeout, int repeat, pfv_t function, unsigned int data)
{
	// Firstly, assert if we have available event
	struct event *ep = ( struct event * ) LL_POP( freelist );
	if( ep == EV_NULL )
	return ;
	// If so, fill the event
	ep->timeout = timeout;
	ep->repeat_interval = repeat;
	ep->go = function;
	ep->data = data;

	insert_timeoutq_event( ep );
}

// No longer useful, it should be wrapped in ISR
int handle_timeoutq_event( )
{
	/*
	struct event * ev = (struct event * )LL_TOP( timeoutq );
	if( EV_NULL == ev )
	return 0;
	unsigned int now = now_usec();
	ev->timeout -= ( now - then_usec );
	then_usec = now;

	if( ev->timeout <= 0 )
	{
		ev->go( ev->data );
		printf("running some function\n");
		LL_POP( timeoutq );
		if( ev->repeat_interval != 0 )
		{
			ev->timeout = ev->repeat_interval ;
			insert_timeoutq_event( ev );
		}
		else
		{
			LL_PUSH( freelist, ev );
		}
		return 1;
	}
	*/


	return 0;
}

void insert_timeoutq_event( struct event * ep)
{
	// Try to insert it according to timeout and timeoutq
	struct event * it;
	short is_pushed = 0;

	// Judge if we gonna insert it before a current event in timeque
	LL_EACH(timeoutq,it,struct event )
	{
		if( it != EV_NULL )
		{
			if( ( it->timeout ) > ( ep->timeout ) )
			{
				it->timeout -= ep->timeout;
				LL_L_INSERT( it, ep );
				is_pushed = 1;
				printf("left insert \n");
				break;
			}
			else
			{
				ep->timeout -= it->timeout;
			}
		}
	}
	
	// If not insert it left to the header
	if( is_pushed == 0 )
	{
		LL_APPEND(timeoutq, ep);
		printf("tail insert\n");
	}

	dump_timeoutq();

}

void foo()
{
	printf("Top of loop !\n");

}
