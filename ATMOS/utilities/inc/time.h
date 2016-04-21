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
	uint32_t unix_time;
	uint32_t unix_msec;
	uint32_t tm_sec;
	uint32_t tm_min;
	uint32_t tm_hour;
	uint32_t tm_mday;
	uint32_t tm_mon;
	uint32_t tm_year;
	uint32_t tm_wday;
	uint32_t tm_yday;
} typedef tm;

void updateTime(uint32_t increment_msec, tm *tm_ptr);

void synchTime(uint32_t calibrate_sec, tm *tm_ptr);

void calcDate(tm *tm_ptr);




#endif /* TIME_H_ */