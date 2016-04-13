#ifndef _BASE_SENSOR_H_
#define _BASE_SENSOR_H_
#include "wrapper/base-class/inc/Base_Device.h"

struct _Base_Sensor;
typedef struct _Base_Sensor BaseSensor;
typedef struct _Base_vmt
{
	void (*FctnInit)(BaseSensor*);
	int (*configure)(BaseSensor*);
	int (*preProcessing)(BaseSensor*);
	int (*collect)(BaseSensor*);
	int (*error)(BaseSensor*);
} Base_FctnTable;

typedef struct _Abstract_vmt
{
	void (*VTinit)( BaseSensor* );
	int (*init)( BaseSensor* );
	int (*reset)( BaseSensor* );
	int (*getType)( BaseSensor* );
} Abstract_FctnTable;

struct _Base_Sensor
{
	int test_num;
	Base_FctnTable *vmt;
	BaseDevice *abstract;
};


void Base_FctnInit( BaseSensor *this);
int Base_Init( BaseSensor *this);
int Base_Configure( BaseSensor *this );
int Base_PreProcessing( BaseSensor *this );
int Base_Collect( BaseSensor *this );
int Base_Error( BaseSensor *this );

void Base_VTinit( BaseSensor* );
int Base_init( BaseSensor* );
int Base_reset( BaseSensor* );
int Base_getType( BaseSensor* );

BaseSensor* New_Base_Sensor( int num );



#endif