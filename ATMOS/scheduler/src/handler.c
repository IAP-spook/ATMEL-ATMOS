/*
 * handler.c
 *
 * Created: 2016/4/15 0:42:37
 *  Author: AB
 */ 

#include "scheduler/inc/handler.h"

/* 
 * sensor_handler function
 * return 1 if it has some borrow time 
 */
int sensor_handler( struct event *p )
{
	int retNum;
	int data = 0;
	BaseSensor *sp;

	/* sanity check */
	if( p == EV_NULL )
	{
		#ifdef DEBUG
		printf("Null Event Error !!!");
		#endif
		return 0;
	}
	sp = ( BaseSensor * )( p->sp );

	#ifdef DEBUG
	printf("cur_state = %d\n", p->cur_state);
	#endif
	switch( p->cur_state )
	{
		/* New : init first, if retNum is not 0 goto Oops state */
		case New :
		// the way to call init() is different than most other functions, since it is virtual function in BaseDevice .
		retNum = sp->abstract.device_vt->init( &(sp->abstract) );
		if(retNum != 0)
		{
			p->cur_state = Oops;
		}
		p->cur_state = Ready;
		break;
		
		
		
		
		/* Ready : ready to activate collection request, if retNum is not 0 goto Oops state */
		case Ready :
		retNum = sp->vmt->Request(p->sp);
		if ( retNum == -1 )
		{
			p->cur_state = Oops;
		}
		else if( retNum == 0 )
		{
			p->cur_state = Ready;
			data = sp->vmt->Collect(p->sp);
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
		break;
		
		
		
		/* Running : ready to pull data right away */
		case Running :
		//      data = p->sp->vmt->Collect();
		p->cur_state = Ready;
		p->sp->vmt->Collect(p->sp);
		break;
		
		
		
		/* Terminated : nop ( do nothing ) for now */
		case Terminated:
		break;
		
		
		
		/* Oops : when some errors occur */
		case Oops :
		/* TODO : ( we may want to do something in reset function and ) re-init by setting the state to new */
#ifdef DEBUG
		      printf("Something really wrong happens\n");
#endif
		p->cur_state = New;
		break;
	}
	return 0;
	// printf(" run :\n\t");
	// p->sp->vmt->MyPrint(p->sp);

}

/* 
 * TODO : device_handler function
 */
int device_handler( struct event *p )
{
	return 0;
}

/*
 * write back handler 
 * TODO : we need a struct to hold all possible data type and we need store them into flash I guess
 */
int storeData_handler( struct event *p )
{
	return 0;
}