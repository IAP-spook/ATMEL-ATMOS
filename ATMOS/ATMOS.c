
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "devices/Si7020.h"
#include "devices/BMP280.h"
#include "drivers/usart0.h"
#include "drivers/TWI.h"
#include "drivers/PWR.h"
#include "drivers/ADC.h"
#include "drivers/SPI.h"
#include "devices/TGS2600.h"
#include "devices/K30.h"
#include "common.h"
#include "utilities/llist.h"
#include "Event/Event.h"
#include "Event/Event_Timer.h"
#include "sensors/My_Sensor.h"
#include "sensors/Si7020_Sensor.h"
#include "sensors/Temperature_ADC_Sensor.h"
#include "sensors/BMP280_Sensor.h"
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
	TGS2600_Init();
	Si7020_init();
	printf("BMP280 Status %i\n", BMP280_Init());
	BMP280_SetOversampling(4);
	SPI_SlaveInit();
}

int main(void)
{
	SYS_Init();
	APP_Init();
	
	/* could have sealed following in a function */
	init_timeoutq();
	Si7020Sensor *Si7020_ptr = New_Si7020_Sensor( 0 );
	Si7020_FctnInit(Si7020_ptr);
	Temperature_ADCSensor *T_ADC_ptr = New_Temperature_ADC_Sensor( 0 );
	Temperature_ADC_FctnInit(T_ADC_ptr);
	BMP280Sensor *BMP280_ptr = New_BMP280_Sensor( 0 );
	BMP280_FctnInit(BMP280_ptr);
	init_Event_Timer();
	printf("init done!\n");
	
	/* could have sealed following in a function */
	load_new_sensor( 1, 4, (BaseSensor *)BMP280_ptr, 0 );
	// load_new_sensor( 4, 4, (BaseSensor *)Si7020_ptr, 0 );
	init_set_timer( get_next_interval() );
	
	/* Enable global interrupt */
	sei();

	for (;;){}
	return 0;
}
