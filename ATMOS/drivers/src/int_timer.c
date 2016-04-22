/**
 * \file int_timer.c
 * 
 * \brief Implementation of an asynchronous interrupt timer.
 *
 *  Author: Anxin Bai
 */ 

#include "drivers/inc/int_timer.h"
#include "utilities/inc/time.h"
#ifdef ATMEL

/* TODO : need to adjust and let it won't return to zero */
static int timer_usage = 0;
/* TODO : timestamp struct maybe */
static long TimeStamp = 0.0;

/*************************************************************************//**
  @brief Timer Interrupt Service Routine
*****************************************************************************/
ISR(TIMER2_OVF_vect)
{
	timer_usage ++;
	printf("count1 = %d\n",timer_usage);
	printf("Timestamp : %ld\n",(TimeStamp));
	handle_timeoutq_event();
}


/* Initialization Routine Example 3 : Timer 2 Async operation */
/* Clock for Timer 2 is taken from crystal connected to TOSC pins */
void init_Event_Timer(void)
{
	TimeStamp = 0;
	timer_usage = 0;
	/* Select clock source as crystal on TOSCn pins */
	ASSR |= 1 << AS2;
	
	/* Wait till registers are ready
	 * Refer ATmega328PB datasheet section
	 * 'Asynchronous Operation of Timer/Counter2' */
	while ((ASSR & ((1 << OCR2AUB) | (1 << OCR2BUB) | (1 << TCR2AUB) | (1 << TCR2BUB) | (1<< TCN2UB))));
	
	/* Stop timer 2 */
	TCCR2B = 0x00;
		
	/* Clear Timer on compare match. Toggle OC2A on Compare Match */
	// TCCR2A = (1<<COM2A0) | (1<<WGM21);
	/* Timer Clock = 32768 Hz / 1024 */
	// TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);
}


void general_set_timer( int period_number )
{
	// printf("debug %d\n",period_number);
	/* Wait till registers are ready
	 * Refer ATmega328PB datasheet section
	 * 'Asynchronous Operation of Timer/Counter2' */
	while ((ASSR & ((1 << OCR2AUB) | (1 << OCR2BUB) | (1 << TCR2AUB) | (1 << TCR2BUB) | (1<< TCN2UB))));
	/* Stop timer 2 */
	TCCR2B = 0x00;
	
	
	/* Wait till registers are ready
	 * Refer ATmega328PB datasheet section
	 * 'Asynchronous Operation of Timer/Counter2' */
	while ((ASSR & ((1 << OCR2AUB) | (1 << OCR2BUB) | (1 << TCR2AUB) | (1 << TCR2BUB) | (1<< TCN2UB))));
	
	int temp,t;

	/* 
	 * Timer clock = I/O clock / 256 
	 * CS22	CS21 CS20 Scale		resolution
	 *  0    0    1     1		0.0078125s/256
	 *  0    1    0     8		0.0625s/256
	 *  0    1    1     32		0.25s/256
	 *  1    0    0     64		0.5s/256
	 *  1    0    1     128		1s/256
	 *  1    1    0     256		2s/256
	 *  1    1    1    1024		8s/256
	 */
	// Resolution
	// 0.25s/256
	if( period_number <= 250 )
	{
		temp = 256 - (int) ( 256.0/250.0 * period_number);
		temp = temp > 255 ? 255 : temp ;
		TCNT2 = temp;
		TCCR2B = (1<<CS21)|(1<<CS20);
	}
	// Resolution
	// 0.5s/256
	else if ( period_number <= 500 )
	{
		temp = 256 - (int) ( 256.0/500.0 * period_number);
		temp = temp > 255 ? 255 : temp ;
		TCNT2 = temp;
		TCCR2B = (1<<CS22);
	}
	// Resolution
	// 1s/256
	else if ( period_number <= 1000 )
	{
		temp = 256 - (int) ( 256.0/1000.0 * period_number);
		temp = temp > 255 ? 255 : temp ;
		TCNT2 = temp;
		TCCR2B = (1<<CS22)|(1<<CS20);	
	}
	// Resolution
	// 2s/256
	else if ( period_number <= 2000 )
	{
		temp = 256 - (int) ( 256.0/2000.0 * period_number);
		temp = temp > 255 ? 255 : temp ;
		TCNT2 = temp;
		TCCR2B = (1<<CS22)|(1<<CS21);	
	}
	// Resolution
	// 8s/256
	else if ( period_number <= 8000 )
	{
		t = (int) ( 256.0/8000.0 * period_number);
		temp = 256 - t;
		temp = temp > 255 ? 255 : temp ;

		TCNT2 = temp;
		TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);
	}
	else
	{
		printf("Timer exceeds max\n");
	}

	/* start timer 2 */
	TCCR2A = 0x00;
	TimeStamp += period_number;
}

void init_set_timer( int period_number )
{
    general_set_timer( period_number );
	/* Clear pending interrupts */
	TIFR2  = (1 << TOV2) | (1 << OCF2A) | (1 << OCF2B);
	/* Enable Overflow Interrupt */
	TIMSK2 = 1<<TOIE2;

}

void set_timer( int period_number )
{
	printf("set timer gap = %d\n\n", period_number);
	if( period_number == 0)
	{
		/* we won't every set a zero timeout, will we ? */
		return;
	}
    if( timer_usage == 0 )
    {
        init_set_timer( period_number );
    }
    else
    {
        general_set_timer( period_number );
    }
}	


#endif

/**
 * \file
 *
 * \brief Example for Timers on AVR Devices
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */