/*
 * devicelist.h
 *
 * \brief Device and Sensor List
 * Created: 2016/5/12 0:40:41
 *  Author: AB
 */ 


#ifndef DEVICELIST_H_
#define DEVICELIST_H_

#include "devices/sensor/inc/My_Sensor.h"
#include "devices/sensor/inc/Si7020_Sensor.h"
#include "devices/sensor/inc/BMP280_Sensor.h"
#include "devices/other-device/inc/DemoStorage_Device.h"
#include "devices/other-device/inc/LoadData_Device.h"

enum 
{
	SensorType,
	DeviceType
};


/* sensor device list */
enum SensorList
{
	//SensorListBegin,
	BMP280,
	Si7020,
	SensorListEnd	
};

/* unit in ms */
#define SENSOR_INIT_PERIOD  12000

int SensorInitStartTimeList[] =
{
	//0,
	1000,
	3000
};

int SensorCheckCodeList[] =
{
	1425,
	1023
};

int SensorNumDataList[] =
{
	2,	/* BMP280 has 2 data output */
	2	/* Si7020 has 2 data output */
};



/* non-sensor device list */
enum DeviceList
{
	//DeviceListBegin,
	DemoLoadData,
	DemoStorage,
	DeviceListEnd
};

/* unit in ms */
int DeviceInitPeriodList[] =
{
	SENSOR_INIT_PERIOD,
	3 * SENSOR_INIT_PERIOD
};

int DeviceInitStartTimeList[] =
{
	//0,
	100,
	3300
};

int DeviceCheckCodeList[] =
{
	1425,
	1023
};


#endif /* DEVICELIST_H_ */