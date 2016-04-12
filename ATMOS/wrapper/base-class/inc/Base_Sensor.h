#ifndef _Base_SENSOR_H_
#define _Base_SENSOR_H_

struct _Base_Sensor;
typedef struct _Base_Sensor BaseSensor;
typedef struct _Base_vmt
{
	void (*FctnInit)(BaseSensor*);
	int (*Init)(BaseSensor*);
	int (*Configure)(BaseSensor*);
	int (*PreProcessing)(BaseSensor*);
	int (*Collect)(BaseSensor*);
	int (*Error)(BaseSensor*);
} Base_FctnTable;

struct _Base_Sensor
{
	int test_num;
	Base_FctnTable *vmt;
};


void Base_FctnInit( BaseSensor *this);
int Base_Init( BaseSensor *this);
int Base_Configure( BaseSensor *this );
int Base_PreProcessing( BaseSensor *this );
int Base_Collect( BaseSensor *this );
int Base_Error( BaseSensor *this );
BaseSensor* New_Base_Sensor( int num );



#endif