/**
 * \file K30_Sensor.c
 *
 * \brief Implementation of K30_Sensor struct, which derived from BaseSensor.
 * 
 *  Author: Cory Martin
 */ 

#include "devices/sensor/inc/K30_Sensor.h"
// #include "devices/old-sensor/K30.h"
#include <stdlib.h>
#include <stdio.h>
#include "drivers/inc/TWI.h"
#include <util/delay.h>
#include "drivers/inc/usart0.h"
#include <math.h>


K30_FctnTable K30_vmt = {
	K30_FctnInit,
	K30_Configure,
	K30_Request,
	K30_Collect,
	K30_Error
};

K30_Abstract_FctnTable K30_abstract_vmt = {
	K30_VTinit,
	K30_init,
	K30_reset,
	K30_getType
};

void K30_FctnInit(K30Sensor *this)
{
	this->inherited.vmt = &K30_vmt;
}


int K30_Configure(K30Sensor *this )
{
	// virtual function
	return 0;
}

int K30_Request(K30Sensor *this )
{
	return K30_delaytime();
	/*
	int val=0;
	int sum=0;
	
	char status;
	//printf("K30_readCO2");
	unsigned char buffer[4]={0,0,0,0};
	status=TWI_BeginWrite(0x68);
	if(status!=TWI_SLAW_ACK) return 0;
	status=TWI_Write(&readcmd[0],4);
	if(status!=TWI_SENT_ACK) return 0;
	TWI_Stop();
	_delay_ms(10);
	return 0;
	*/
}

int K30_Collect(K30Sensor *this )
{
	double K30Data;
	K30Data = K30_readCO2();
	printf("\tK30 data is %lf\n",K30Data);
/*
	int val=0;
	int sum=0;
	
	char status;
	//printf("K30_readCO2");
	unsigned char buffer[4]={0,0,0,0};
	status=TWI_BeginRead(0x68);
	if(status!=TWI_SLAR_ACK) return 0;
	status=TWI_Read(&buffer[0],4,true);
	if(status!=TWI_REC_ACK) return 0;
	TWI_Stop();
	
	val = 0;
	val |= buffer[1] & 0xFF;
	val = val << 8;
	val |= buffer[2] & 0xFF;
	sum = buffer[0] + buffer[1] + buffer[2];
	if(sum==buffer[3]){return val;}
		printf_P()
	else{return 0;}
*/
}

int K30_Error(K30Sensor *this )
{
	// virtual function
	return 0;
}
void K30_VTinit( K30Sensor* this)
{
	this->inherited.abstract.device_vt = &K30_abstract_vmt;
}

int K30_init( K30Sensor* this)
{
	// TWI_Init(200000);
	printf("K30 Sensor Init\n");
	return 0;
}

int K30_reset( K30Sensor* this)
{
	// virtual function
	return 0;
}

int K30_getType(K30Sensor* this)
{
	return TYPE_SENSOR;
}


K30Sensor* New_K30_Sensor( int num, int NumData)
{
	K30Sensor *p = malloc(sizeof(K30Sensor));
	K30_VTinit( p );
	K30_FctnInit( p );
	p->inherited.getStartNum = fctn_getStartNum;
	p->inherited.getEndNum = fctn_getEndNum;
	
	p->inherited.test_num = num;
	p->inherited.NumOfData = NumData;
	p->inherited.StartNum = SensorDataCount;
	SensorDataCount += NumData;
	printf("SensorDataCount %d\n",SensorDataCount);
	
	return p;
}

