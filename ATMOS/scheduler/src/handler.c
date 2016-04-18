/*
 * handler.c
 *
 * Created: 2016/4/15 0:42:37
 *  Author: AB
 */ 

#include "scheduler/inc/handler.h"

/* run-next function */
int run_next( struct event *p )
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
		retNum = p->sp->vmt->Request(p->sp);
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

