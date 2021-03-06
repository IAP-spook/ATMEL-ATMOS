/**
 * \file scheduler.c
 *
 * \brief Implementation of the resources that hold all events and manage useful events in-order within a linked list. 
 *
 * Created: 2016/4/14 23:05:53
 *  Author: Anxin Bai
 */ 
#include "scheduler//inc/scheduler.h"



/*************************************************************************//**
  @brief Init function of scheduler, which initialize all events and put them into freelist
*****************************************************************************/
void init_timeoutq()
{
	int i;

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


/*************************************************************************//**
  @brief Load a brand new sensor activity into the scheduler's timeoutQ
*****************************************************************************/
int load_new_sensor( int timeout, int repeat, BaseSensor *device_ptr, int otherinfo )
{

    /* assume we have available event in freelist */
    struct event *ep = ( struct event * ) LL_POP( freelist );
    /* if not return a -1 as an error code */
    if( ep == EV_NULL )
        return -1;
    ep->timeout = timeout;
    ep->repeat_interval = repeat;
	ep->borrow_timeout = 0;
    ep->sp = device_ptr;
    ep->info = otherinfo;
    ep->cur_state = Ready;
    ep->run = sensor_handler;
    insert_timeoutq_event( ep );
    return 0;
}


/*************************************************************************//**
  @brief Load a brand new device activity into the scheduler's timeoutQ
*****************************************************************************/
int load_new_device( int timeout, int repeat, BaseDevice *device_ptr, int otherinfo )
{

    /* assume we have available event in freelist */
    struct event *ep = ( struct event * ) LL_POP( freelist );
    /* if not return a -1 as an error code */
    if( ep == EV_NULL )
        return -1;
    ep->timeout = timeout;
    ep->repeat_interval = repeat;
	ep->borrow_timeout = 0;
    ep->sp = device_ptr;
	ep->load_p = device_ptr;
	ep->store_p = device_ptr;
    ep->info = otherinfo;
    ep->cur_state = Ready;
    ep->run = device_handler;
    insert_timeoutq_event( ep );
    return 0;
}


/*************************************************************************//**
  @brief Return the the time ( ms ) of the next event
*****************************************************************************/
int get_next_interval()
{

	int wait_time = MAX_SLEEP_INTERVAL;
	struct event * ev = ( struct event * ) LL_TOP( timeoutq );

	/* sanity check */
    /* no event in queue */
    /* TODO : to optimize the sleep time or let the CPU sleep or set up some other interrupts */
	if( EV_NULL == ev )
	{
#ifdef DEBUG
		printf("we may get some memory leakage issue, or something really bad occurs\n");
		printf("so we are going to take some really sleep, since no event in scheduler\n");
#endif
	    return wait_time;
	}
	
	/* if the next event is nearer */
	if( wait_time > ev->timeout )
	{
		wait_time = ev->timeout;
	}
	/* if the next event time is longer, we need to wait it time by time, since currently, the longest waiting time support is 8s */
	else
	{
		ev->timeout = ev->timeout - wait_time;
	}
	return wait_time;
}


/*************************************************************************//**
  @brief Handle the next event in timeoutQ: execute the event's handler function according to the device type, re-insert it according to the period, and set a timer of next event.
*****************************************************************************/
int handle_timeoutq_event( )
{
	/* assume we get a valid one */
	struct event * ev = (struct event * )LL_TOP( timeoutq );
	if( EV_NULL == ev )
        return -1;
    
    /* sanity check */
    if( ev->sp == NULL )
        return -1;

	/* retNum may need to be designed in other ways */
    int retNum = ev->run( ev );
	
	/* 
	 * return 1 if it has to with some borrow time( have a request-time!=0 case ) 
	 * which means the event has already entered the scheduler, 
	 * and won't need to do anything else
	 */
	if( retNum == 1 )
	{
		/* judge if the next top in queue has a timeout <= 0, which we may want to handle it right now */
		if( get_next_interval() == 0 )
			handle_timeoutq_event();
			
		set_timer(get_next_interval());
		return 0;
	}
	
    /*
	 * return 0 means we just fired a case, 
	 * and may need to judge it over the repeat time to re-insert into the scheduler,
	 * since we definitely want to collect data periodically
	 */
    LL_POP( timeoutq );
    if( ev->repeat_interval != 0 )
    {
		ev->timeout = ev->repeat_interval - ev->borrow_timeout ;
		while( ev->timeout < 0 )
		{
			ev->timeout += ev->repeat_interval;
		}
		ev->borrow_timeout = 0;
        insert_timeoutq_event( ev );
    }
    else
    {
        LL_PUSH( freelist, ev );
    }

	
	/* judge if the next top in queue has a timeout <= 0, which we may want to handle it right now */
	if( get_next_interval() == 0 )
		handle_timeoutq_event();
		
	set_timer(get_next_interval());
    return 0;
}


/*************************************************************************//**
  @brief Insert a new event into the scheduler according to timeout order
*****************************************************************************/
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
				// printf("left insert \n");
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
		// printf("tail insert\n");
	}

	#ifdef DEBUG
	// dump_timeoutq();
	#endif

}

#ifdef DEBUG

/*************************************************************************//**
  @brief Debug usage: Print out all events in line in order with regard to timeout
*****************************************************************************/
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

#endif