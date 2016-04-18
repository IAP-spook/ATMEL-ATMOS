/*
 * scheduler.c
 *
 * Created: 2016/4/14 23:05:53
 *  Author: AB
 */ 
#include "scheduler//inc/scheduler.h"


/* 
 * like other many stuff, we got an init function
 * fill the freelist with all entries and left the timeoutq empty
 */
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


/* 
 * load a brand new sensor activity in the scheduler
 */
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

/* 
 * return the numbers of period we should launch next
 */
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
	wait_time = ( wait_time > ( ev->timeout ) ) ? ( ev->timeout ) : wait_time ;
	return wait_time;
}


/* 
 * handle the next one in timeout queue
 * the return value is reserved and now we just return 0
 */
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



void foo()
{
	printf("Top of loop !\n");

}

#endif