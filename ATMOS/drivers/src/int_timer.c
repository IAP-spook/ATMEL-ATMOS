#include "drivers/inc/int_timer.h"

#ifdef ATMEL

ISR(TIMER2_COMPA_vect)
{
	timer_usage ++;
	printf("count1 = %d\n",timer_usage);
	handle_timeoutq_event();
}



/* Initialization Routine Example 3 : Timer 2 Async operation */
/* Clock for Timer 2 is taken from crystal connected to TOSC pins */
void init_Event_Timer(void)
{
	/* Select clock source as crystal on TOSCn pins */
	ASSR |= 1 << AS2;
	/* Clear Timer on compare match. Toggle OC2A on Compare Match */
	TCCR2A = (1<<COM2A0) | (1<<WGM21);
	/* Timer Clock = 32768 Hz / 1024 */
	TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);
}


void general_set_timer( int period_number )
{
    OCR2A = period_number * APERIOD ;
}

void init_set_timer( int period_number )
{
    general_set_timer( period_number );
    /* Wait till registers are ready
	 * Refer ATmega328PB datasheet section
	 * 'Asynchronous Operation of Timer/Counter2' */
	while ((ASSR & ((1 << OCR2AUB) | (1 << OCR2BUB) | (1 << TCR2AUB) 
		| (1 << TCR2BUB) | (1<< TCN2UB))));
	/* Clear pending interrupts */
	TIFR2  = (1 << TOV2) | (1 << OCF2A) | (1 << OCF2B);
	/* Enable Timer 2 Output Compare Match Interrupt */
	TIMSK2 = (1 << OCIE2A);
}

void set_timer( int period_number )
{
	printf("set timer gap = %d\n\n", period_number);
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