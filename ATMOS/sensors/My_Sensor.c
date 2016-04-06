#include "sensors/My_Sensor.h"
#include <stdio.h>
#include <stdlib.h>

My_FctnTable My_vmt = { My_FctnInit, My_Init, My_Configure, My_PreProcessing, My_Collect, My_Error };

void My_FctnInit(MySensor *this)
{
	this->inherited.vmt = &My_vmt;
}

int My_Init(MySensor *this )
{
	this->inherited.test_num = 10;
	return 0;
}

int My_Configure(MySensor *this )
{
	// virtual function
	return 0;
}

int My_PreProcessing(MySensor *this )
{
	// virtual function
	return 0;
}

int My_Collect(MySensor *this )
{
	// virtual function
	return 0;
}

int My_Error(MySensor *this )
{
	// virtual function
	return 0;
}

MySensor* New_My_Sensor( int num)
{
	MySensor *p = malloc(sizeof(MySensor));
	p->inherited.test_num = num;
	return p;
}