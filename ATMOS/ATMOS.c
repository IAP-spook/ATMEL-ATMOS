
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

#include "avr/io.h"
#include "avr/interrupt.h"

unsigned char data[3];
char dummy;
float humidity, temp2;
float temp3, resistance;
int co2;
double T,P;
char buff[3] = {0,0,0};

char ref1[5]={'T','P','H','C',0};
char refT[4]={'1','2','3',0};
char refP[2]={'1',0};
char refH[2]={'1',0};
char refC[3]={'2','4',0};
	
char sendbuff[100];

void my_delay_ms(int n) {
	while(n--) {
		_delay_ms(1);
	}
}

char checkChar(char *ref, char test){
	int j=0;
	while((ref[j]!=test) && (ref[j]!=0)) j++;
	if(ref[j]==0) return 100;
	return j;
}

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

static void APP_TaskHandler(void)
{

	printf("Top of loop !\n");
	//SPI Test
	int i;
	for(i=0;i<=1;i++){
		if((i==1) && (checkChar(&ref1[0],buff[0])==100))i=0; 
		buff[i]=SPI_SlaveReceive();
	}
	PORTB |= 0b00010000; //LED on
	printf("Received %s\n",buff);
	char result;
	switch(buff[0]){
		case 'T':
			//switch(checkChar(&refT[0], buff[1])){
			switch(buff[1]){
				case '1':
					result = BMP280_StartMeasurment();
					if(result!=0){
						my_delay_ms(result);
						result = BMP280_GetTemperatureAndPressure(&T,&P);
					}
					sprintf(&sendbuff[0],"%.3F\r",T);
					break;
				case '2':
					dummy = Si7020_readTemperature(&data[0],3);
					sprintf(&sendbuff[0],"%.3F\r",Si7020_calTemperature(&data[0]));
					break;
				case '3':
					sprintf(&sendbuff[0],"%.3F\r",ADC_DieTemp());
					break;
				default:
					sendbuff[0]='\r';
					break;
			}
			break;
		case 'P':
			//switch(checkChar(&refP[0],buff[1])){
			switch(buff[1]){
				case '1':
					result = BMP280_StartMeasurment();
					if(result!=0){
						my_delay_ms(result);
						result = BMP280_GetTemperatureAndPressure(&T,&P);
					}
					sprintf(&sendbuff[0],"%.3F\r",P);
					break;
				default:
					sendbuff[0]='\r';
					break;
			}
			break;
		case 'H':
			//switch(checkChar(&refH[0],buff[1])){
			switch(buff[1]){
				case '1':
					dummy = Si7020_readHumidity(&data[0],3);
					sprintf(&sendbuff[0],"%.3F\r",Si7020_calHumidity(&data[0]));
					break;
				default:
					sendbuff[0]='\r';
					break;
			}
			break;
		case 'C':
			//switch(checkChar(&refC[0],buff[1])){
			switch(buff[1]){
				case '2':
					sprintf(&sendbuff[0],"%i\r",K30_readCO2());
					break;
				case '4':
					sprintf(&sendbuff[0],"%.3F\r",TGS2600_GetResistance());		
					break;			
				default:
					sendbuff[0]='\r';
					break;
			}
			break;
		default:
			break;
	}
	printf("Sending SPI: \n%s\n",sendbuff);
	i=0;
	PORTE&=0b11110111;
	while(sendbuff[i]!='\r'){
		SPI_SlaveTransmit(sendbuff[i]);
		i++;
	}
	SPI_SlaveTransmit('\r');
	PORTE|=0b00001000;
	PORTB &= 0b11101111; //LED off
	
	/* Busy loop all-sensor test //
	PORTB |= 0b00010000; //LED on
	_delay_ms(1000);
	PORTB &= 0b11101111; //LED off
  _delay_ms(1000);
  
  char result = BMP280_StartMeasurment();
  //printf("Measure result: %i",result);
  if(result!=0){
	  my_delay_ms(result);
	  result = BMP280_GetTemperatureAndPressure(&T,&P);
	  if(result!=0){ 
		  printf("Pres:%.3F : Temp1:%.3F",P,T);
	  }
  }
  dummy = Si7020_readHumidity(&data[0],3);
  humidity = Si7020_calHumidity(&data[0]);
  dummy = Si7020_readTemperature(&data[0],3);
  temp2 = Si7020_calTemperature(&data[0]);
  temp3 = ADC_DieTemp();
  resistance=TGS2600_GetResistance();
  co2=K30_readCO2();
  //printf("Co2: %i\n",co2);
  printf(" : Hum %.3F : Temp2 %.3F : Temp3 %.3F : Res %.3F : C02 %i\n",humidity,temp2,temp3,resistance,co2);
  */
}


/* Initialization Routine Example 3 : Timer 2 Async operation */
/* Clock for Timer 2 is taken from crystal connected to TOSC pins */
void init_Ex3(void)
{
	/* Select clock source as crystal on TOSCn pins */
	ASSR |= 1 << AS2;
	/* Clear Timer on compare match. Toggle OC2A on Compare Match */
	TCCR2A = (1<<COM2A0) | (1<<WGM21);
	/* Timer Clock = 32768 Hz / 1024 */
	TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);
	/* Set Output Compare Value to 32. Output pin will toggle every second */
	OCR2A  = 32;
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

/* Initialization Routine Example 2 : Input Capture - Timer 1*/
/* Capture pin PB0 (ICP1). By default performs capture on falling edge */
void init_Ex2(void)
{
	/* Timer clock = I/O clock / 64 */
	TCCR1B = (1<<CS11)|(1<<CS10);
	/* Clear ICF1. Clear pending interrupts */
	TIFR1   = 1<<ICF1;
	/* Enable Timer 1 Capture Event Interrupt */
	TIMSK1  = 1<<ICIE1;
}

static int count = 0;

ISR(TIMER2_COMPA_vect)
{
	count++;
	if( count & 0x01 )
		OCR2A = 32;
	else
		OCR2A = 32;
	printf("count1 = %d\n",count);
	foo();	
}
	
/* Example 2 - ISR Input Capture Interrupt Timer 1 */
ISR (TIMER1_CAPT_vect)
{
	count++;
	printf("count = %d\n",count);
	foo();
	/* Clear counter to restart counting */
	TCNT1 = 0;
}

int main(void)
{
  //SYS_Init(); //Commented out until wireless hardware is tuned
  /*
  APP_Init();
  printf("\n======================\n");
  while (1)
  {
    //SYS_TaskHandler(); //Commented out until wireless hardware is tuned
    APP_TaskHandler();
  }
  */
	SYS_Init();
	APP_Init();
	printf("init done!\n");
  	count = 0;
  	init_Ex3();
	/* Enable global interrupt */
	sei();

	for (;;){}
	return 0;
}
