#include "utilities/inc/config.h"
#include "utilities/inc/common.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "drivers/inc/usart0.h"
#include "drivers/inc/TWI.h"
#include "drivers/inc/PWR.h"
#include "drivers/inc/ADC.h"
#include "drivers/inc/SPI.h"
#include "drivers/inc/int_timer.h"
#include "scheduler/inc/scheduler.h"
#include "wrapper/sensor/inc/My_Sensor.h"
#include "wrapper/sensor/inc/Si7020_Sensor.h"
#include "wrapper/sensor/inc/Temperature_ADC_Sensor.h"
#include "wrapper/sensor/inc/BMP280_Sensor.h"
#include "avr/io.h"
#include "avr/interrupt.h"

/* Initialization Routine Example 1 : Timer Overflow - TImer 0*/
void init_Ex1(void)
{
	/* Stop timer 0 */
	TCCR0B = 0x00;

	/* load value to timer */
	TCNT0 = 0x00;

	/* start timer 0 */
	TCCR0A = 0x00;

	/* Timer clock = I/O clock / 1024 */
	TCCR0B = (1<<CS02)|(1<<CS00);
	/* Clear overflow flag */
	TIFR0  = 1<<TOV0;
	/* Enable Overflow Interrupt */
	TIMSK0 = 1<<TOIE0;
}

void init_Ex2(void)
{
	/* Stop timer 1 */
	TCCR1B = 0x00;

	/* load value to timer */
	TCNT1H = 0x50;
	TCNT1L = 0x00;

	/* start timer 1 */
	TCCR1A = 0x00;

	/* Timer clock = I/O clock / 1024 */
	TCCR1B = (1<<CS12)|(1<<CS10);
	/* Clear overflow flag */
	TIFR1  = 1<<TOV1;
	/* Enable Overflow Interrupt */
	TIMSK1 = 1<<TOIE1;

}

static int count = 0;

/* Example 1 - ISR Overflow Interrupt Timer 0 */
ISR (TIMER0_OVF_vect)
{
	/* load value to timer */
	TCNT0 = 0x00;
	printf("timer 0: count = %d\n",count++);
}

/* Example 2 - ISR Overflow Interrupt Timer 1 */
ISR (TIMER1_OVF_vect)
{
	/* load value to timer */
	TCNT1H = 0x50;
	TCNT1L = 0x00;
	printf("timer 1: count = %d\n",count++);
}

int main (void)
{
	init_Ex1();
	// init_Ex2();
	sei();
	for (;;){}
	
	return 0;
}
/*
 *
#include "utilities/inc/config.h"
#include "utilities/inc/common.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "drivers/inc/usart0.h"
#include "drivers/inc/TWI.h"
#include "drivers/inc/PWR.h"
#include "drivers/inc/ADC.h"
#include "drivers/inc/SPI.h"
#include "drivers/inc/int_timer.h"
#include "scheduler/inc/scheduler.h"
#include "wrapper/sensor/inc/My_Sensor.h"
#include "wrapper/sensor/inc/Si7020_Sensor.h"
#include "wrapper/sensor/inc/Temperature_ADC_Sensor.h"
#include "wrapper/sensor/inc/BMP280_Sensor.h"
#include "avr/io.h"
#include "avr/interrupt.h"


static void APP_Init(void){
	PWR_Init();
	PWR_TurnOn5V();
	USART0_Init(9600);
	DDRB |= 0b00010000;
	DDRE |= 0b00001000;
	PORTE|= 0b00001000;
	TWI_Init(10000);
	ADC_Init();
	printf("BMP280 Status %i\n", BMP280_Init());
	BMP280_SetOversampling(4);
	SPI_SlaveInit();
}
*/

/*
int main(void)
{
	SYS_Init();
	APP_Init();
	
	// could have sealed following in a function //
	init_timeoutq();
	BMP280Sensor *BMP280_ptr = New_BMP280_Sensor( 0 );
	BMP280_FctnInit(BMP280_ptr);
	init_Event_Timer();
	printf("init done!\n");
	
	// could have sealed following in a function //
	load_new_sensor( 1, 4, (BaseSensor *)BMP280_ptr, 0 );
	// load_new_sensor( 4, 4, (BaseSensor *)Si7020_ptr, 0 );
	init_set_timer( get_next_interval() );
	
	// Enable global interrupt //
	sei();

	for (;;){}
	return 0;
}
*/
