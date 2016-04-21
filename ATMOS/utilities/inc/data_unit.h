/**
 * \file data_unit.h
 *
 * \brief Decalration of data struct, which is used to store timestamp and all sensors' data within a period .
 * 
 * Created: 2016/4/20 1:12:29
 *  Author: Anxin Bai
 */ 
#ifndef DATA_UNIT_H_
#define DATA_UNIT_H_

#include "utilities/inc/common.h"
#include "utilities/inc/time.h"

struct _data_unit
{
	/* check MAX_NUM_DATA description in common.h */
	double data[MAX_NUM_DATA];
	tm data_tm;
} typedef DataUnit;


#endif /* DATA_UNIT_H_ */