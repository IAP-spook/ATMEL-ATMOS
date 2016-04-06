/*
 * Event.c
 *
 * Created: 2016/4/4 13:35:16
 *  Author: AB
 */ 
#include "Event/Event.h"
#include "utilities/llist.h"
// #include "Event.h"

/* run-next function */
static int run_next( struct event *p )
{
    int retNum;
    int data = 0;

    /* sanity check */
    if( p == EV_NULL )
    {
#ifdef DEBUG
        printf("Null Event Error !!!");
#endif
        return 0;
    }


#ifdef DEBUG
    printf("cur_state = %d\n", p->cur_state);
#endif
    switch( p->cur_state )
    {
        /* New : not init yet */
        case New :
            // TODO : if retNum not valid go to Oops state
            // retNum = p->sp->vmt->Init();
            p->cur_state = Ready;
            break;
        /* Ready : ready to activate collection preprocess */
        case Ready :
			retNum = p->sp->vmt->PreProcessing(p->sp);
			if ( retNum == -1 )
			{
				p->cur_state = Oops;
			}
			else if( retNum == 0 )
			{
				p->cur_state = Ready;
				data = p->sp->vmt->Collect(p->sp);
#ifdef DEBUG
				printf("data : %d\n", data);
#endif
			}
			else
			{
				p->cur_state = Running;
				p->timeout = retNum;
				p->borrow_timeout = retNum;
				LL_POP( timeoutq );
				insert_timeoutq_event( p );
				return 1;
			}
            // retNum = p->sp->vmt->PreProcess();
            // TODO : retNum = waiting time
            // if ( retNum == -1 )
            // {
            //      p->cur_state = Oops;
            // }
            // else if ( retNum == 0 )
            // {
            //      p->cur_state = Ready;
            //      TODO : can wrap callback as a macro define 
            //      data = p->sp->vmt->Collect();
#ifdef DEBUG
            //      printf("data : %d\n", data);
#endif
            // }
            // else
            // {
            //      p->cur_state = Running;
            //      p->timeout = retNum;
            //      TODO : time arrangement should be re-think
			//      p->borrow_timeout = retNum;
            //      update borrow_timeout;
            //      insert_timeoutq_event( p );
            // }
            //
            //
            break;
        /* Running : ready to pull data right away */
        case Running :
            //      data = p->sp->vmt->Collect();
            p->cur_state = Ready;
			p->sp->vmt->Collect(p->sp);
#ifdef DEBUG
            //      printf("data : %d\n", data);
#endif
            break;
        /* Terminated : hang for now */
		case Terminated:
			break;
		/* Oops : when bus error occurs */
        case Oops :
            // TODO : call some handler fctns
            // reset the cur_state to new maybe
#ifdef DEBUG
            //      printf("Something really wrong happens"\n);
#endif
            break;
    }
	return 0;
    // printf(" run :\n\t");
    // p->sp->vmt->MyPrint(p->sp);
    
    // TODO : determine which event run next
    // may need to add a state machine in event
    // run function like:
    // p->sp->vmt->Init();
}

/* 
 * load a brand new sensor activity in the scheduler
 */
int load_new_sensor( int timeout, int repeat, BaseSensor *sensor_ptr, int otherinfo )
{

    /* assume we have available event in freelist */
    struct event *ep = ( struct event * ) LL_POP( freelist );
    /* if not return a -1 as an error code */
    if( ep == EV_NULL )
        return -1;
    ep->timeout = timeout;
    ep->repeat_interval = repeat;
	ep->borrow_timeout = 0;
    ep->sp = sensor_ptr;
    ep->info = otherinfo;
    ep->cur_state = Ready;
    ep->run = run_next;
    insert_timeoutq_event( ep );
    return 0;
}


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
 * return the numbers of period we should launch next
 */
int get_next_interval()
{
	int wait_time = MAX_SLEEP_INTERVAL;
	struct event * ev = ( struct event * ) LL_TOP( timeoutq );

    /* no event in queue */
    // TODO : to optimize the sleep time or let the CPU sleep or set up some other interrupts
	if( EV_NULL == ev )
	    return wait_time;
	wait_time = ( wait_time > ( ev->timeout ) ) ? ( ev->timeout ) : wait_time ;
	return wait_time;
}

/* 
 * handle the next one in timeout queue
 */
int handle_timeoutq_event( )
{
	/* assume we get a valid one */
	struct event * ev = (struct event * )LL_TOP( timeoutq );
	if( EV_NULL == ev )
        return -1;

    // TODO : add a last_interval var in event_timer.h
    // ev->timeout -= get_later_inerval();
	// if( ev->timeout <= 0 )
    
    /* sanity check */
    if( ev->sp == NULL )
        return -1;

	/* retNum may need to be designed in other ways */
    int retNum = ev->run( ev );
	if( retNum == 1 )
	{
		set_timer(get_next_interval());
		return 0;
	}
	
    // printf("running some function\n");
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
	set_timer(get_next_interval());
    /* TODO : update next event by calling set_timer(int) in Event_timer.h 
     * set_timer( get_next_interval() );
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