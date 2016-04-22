/**
 * \file time.h
 *
 * \brief Declaration of various timestamp function
 * 
 * I admit the time struct is not perfectly designed as regard to _tm struct, but it is convenient anyway.
 *  calcData function modified from
 * http://stackoverflow.com/questions/21593692/convert-unix-timestamp-to-date-without-system-libs
 *
 * Created: 2016/4/20 0:23:31
 *  Author: Anxin Bai
 */ 
#include "utilities/inc/common.h"
#include <math.h>
#include <stdbool.h>

#ifndef TIME_H_
#define TIME_H_


struct _tm
{
	long unix_time;
	long unix_msec;
	long tm_sec;
	long tm_min;
	long tm_hour;
	long tm_mday;
	long tm_mon;
	long tm_year;
	long tm_wday;
	long tm_yday;
} typedef tm;

void updateTime(long increment_msec, tm tm_eg);

void synchTime(long calibrate_sec, tm *tm_ptr);

void calcDate(tm *tm_ptr);




#endif /* TIME_H_ */