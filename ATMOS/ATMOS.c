/**
 * \file ATMOS.c
 *
 * \brief The entry function
 *
 */

// something new here

#include "utilities/inc/config.h"
#include "utilities/inc/common.h"
#include "utilities/inc/time.h"
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
#include "devices/sensor/inc/My_Sensor.h"
#include "devices/sensor/inc/Si7020_Sensor.h"
#include "devices/sensor/inc/Temperature_ADC_Sensor.h"
#include "devices/sensor/inc/BMP280_Sensor.h"
#include "devices/sensor/inc/K30_Sensor.h"
#include "devices/other-device/inc/DemoStorage_Device.h"
#include "devices/other-device/inc/LoadData_Device.h"
#include "devices/other-device/inc/FlashStorage_Device.h"
#include "devices/other-device/inc/N25Q.h"
#include "devices/other-device/inc/N25Q.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "utilities/inc/data_unit.h"
#include "parameters.h"
#include "inc/util.h"

int SensorDataCount;
DataUnit *cur_data = NULL;
tm cur_time;
GlobalParameter GP;

/* TODO : clean up these messy init */
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
	spi_init_master(); // initialize SPI interface
	//SPI_SlaveInit();
	SensorDataCount = 0;
}

static void DEVICE_Init(void){

	init_parameter(&GP);
	delay_us(100);

	init_timeoutq();
	delay_us(100);

	init_timestamp(&cur_time);
	delay_us(100);

	BMP280Sensor *BMP280_ptr = New_BMP280_Sensor( 0,2 /* GP.SensorList[i].NumOfData */ );
	printf("init success");
	delay_us(100);

	/*Si7020Sensor *Si_ptr = New_Si7020_Sensor(0,2); */
	K30Sensor *K30_ptr = New_K30_Sensor(0,1);

	FlashStorageDevice *Strg_ptr = New_FlashStorage_Device(0);
	//DemoStorageDevice *Strg_ptr = New_DemoStorage_Device(0);
	//LoadDataDevice *Load_ptr = New_LoadData_Device(0,Strg_ptr);

	init_Event_Timer();
	printf("init done!\n");

	/* load all sensors */
	load_new_sensor( GP.SensorList[BMP280].StartTime, GP.SensorList[BMP280].ExecutePeriod, (BaseSensor *)BMP280_ptr, 0 );
	/* load_new_sensor( GP.SensorList[Si7020].StartTime, GP.SensorList[Si7020].ExecutePeriod, (BaseSensor *)Si_ptr, 0 ); */
	load_new_sensor( GP.SensorList[K30].StartTime, GP.SensorList[K30].ExecutePeriod, (BaseSensor *)K30_ptr, 0 );


	//printf("%d\t%d\n",GP.DeviceList[DemoStorage].StartTime,GP.DeviceList[DemoStorage].ExecutePeriod);
	/* load all ( non-sensor ) devices */
	load_new_device( GP.DeviceList[FlashStorage].StartTime, GP.DeviceList[FlashStorage].ExecutePeriod, (BaseDevice *)Strg_ptr, 0 );
	//load_new_device( GP.DeviceList[DemoLoadData].StartTime, GP.DeviceList[DemoLoadData].ExecutePeriod, (BaseDevice *)Load_ptr, 0 );
	//load_new_device( GP.DeviceList[DemoStorage].StartTime, GP.DeviceList[DemoStorage].ExecutePeriod, (BaseDevice *)Strg_ptr, 0 );
	// load_new_sensor( 4, 4, (BaseSensor *)Si7020_ptr, 0 );
}

int main(void)
{
	SYS_Init();
	APP_Init();
	init_set_timer( get_next_interval() );

	// Enable global interrupt //
	sei();

	for (;;){}

	return 0;
}

/* TODO list
 * 1. Add a My_Device.h/.c at wrappe/other-device which features a Exec() and a Configure() function DONE!!!
 * 2. Derive a DemoFlashDevice.h/.c for demonstration usage , it should hold some DataUnit queue DONE!!!
 * 3. Add two members one called Start_data, the other End_data in BaseSensor.h/.c DONE!!!
 * 4. Add getStartNum() and getEndNum() in BaseSensor.h/.c correspondingly	DONE!!!
 * 5. Add LoadData_Device to get a valid empty DataUnit DONE !!!
 * 6. Add Handler of Devices
 * 7. LoadData_Device should init all data to -9999 as default
 * 8. configurable parameter Done!!!
 * 9. configurable parameter handler Interface Done!!!
 * 10. Readable parameter Done!!!
 * 11. Data unit's timestamp issue
 * 12. Integragte common.h's type def. with devicelist.h's.
 * 13. Let Sensor's data collect() procedure be more automatical ( no assigning number )
 * 14. Adjust all int to int32_t, since int is a int16_t, the timer may overflow. Need test
 */
