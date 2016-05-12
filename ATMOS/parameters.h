/*
 * parameters.h
 *
 * Created: 2016/5/11 23:02:04
 *  Author: AB
 */ 


#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "devicelist.h"
#include <stdio.h>

typedef struct  
{
	int StartTime;		/* in ms */
	int ExecutePeriod;
	int NumOfData;
	int CheckCode;
} EndDeviceParameter;

typedef struct
{
	EndDeviceParameter DeviceList[DeviceListEnd];
	EndDeviceParameter SensorList[SensorListEnd];
} GlobalParameter;

typedef struct 
{
	int SensorNum;
	int CheckCode;
	int StartTime;
	int ExecutePeriod;
} UniUpdatePacket;

typedef struct  
{	
	int NumDevicePacket;
	UniUpdatePacket * DeviceUpdateList;
	int NumSensorPacket;
	UniUpdatePacket * SensorUpdateList;
} UpdateParameterPacket;

/* check update for all devices' parameters */
void update_parameter(GlobalParameter globalPara, UpdateParameterPacket upPacket)
{
	/* check update for ( non-sensor ) devices' parameters */
	for(int i=0; i<upPacket.NumDevicePacket; i++)
	{
		if( i < DeviceListEnd && upPacket.DeviceUpdateList[i].CheckCode == globalPara.DeviceList[i].CheckCode )
		{
			globalPara.DeviceList[i].StartTime = upPacket.DeviceUpdateList[i].StartTime;
			globalPara.DeviceList[i].ExecutePeriod = upPacket.DeviceUpdateList[i].ExecutePeriod;
		}
	}
	
	/* check update for sensors' parameters */
	for(int i=0; i<upPacket.NumSensorPacket; i++)
	{
		if( i < SensorListEnd && upPacket.SensorUpdateList[i].CheckCode == globalPara.SensorList[i].CheckCode )
		{
			globalPara.SensorList[i].StartTime = upPacket.SensorUpdateList[i].StartTime;
			globalPara.SensorList[i].ExecutePeriod = upPacket.SensorUpdateList[i].ExecutePeriod;
		}	
	}
	
	/* if there are some other parameters other than devices' and sensors', update them below as well */
}

/* init all devices' parameters */
void init_parameter(GlobalParameter * globalPara)
{
	/* init all sensors' parameters */
	for( int i = 0 ; i < SensorListEnd ; ++i )
	{
		//printf("%d\t%d\t%d\t%d\n",SensorInitStartTimeList[i],SENSOR_INIT_PERIOD,SensorCheckCodeList[i],SensorNumDataList[i]);
		globalPara->SensorList[i].StartTime = SensorInitStartTimeList[i];
		globalPara->SensorList[i].ExecutePeriod = SENSOR_INIT_PERIOD;
		globalPara->SensorList[i].CheckCode = SensorCheckCodeList[i];
		globalPara->SensorList[i].NumOfData = SensorNumDataList[i];
	}
	
	/* init all devices' parameters */
	for( int i = 0 ; i < DeviceListEnd ; ++i )
	{
		//printf("%d\t%d\t%d\t%d\n",DeviceInitStartTimeList[i],DeviceInitPeriodList[i],DeviceCheckCodeList[i],0);
		globalPara->DeviceList[i].StartTime = DeviceInitStartTimeList[i];
		globalPara->DeviceList[i].ExecutePeriod = DeviceInitPeriodList[i];
		globalPara->DeviceList[i].CheckCode = DeviceCheckCodeList[i]; 
		globalPara->DeviceList[i].NumOfData = 0;
	}
}


#endif /* PARAMETERS_H_ */