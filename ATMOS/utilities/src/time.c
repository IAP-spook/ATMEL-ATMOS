/**
 * \file time.c
 *
 * \brief Implementation of various timestamp function
 * 
 * Created: 2016/4/20 2:00:06
 *  Author: Anxin Bai
 */ 
#include "utilities/inc/time.h"
#include <stdio.h>


void updateTime(long increment_msec, tm * tm_eg)
{
	tm_eg->unix_msec += increment_msec;
	tm_eg->unix_time += ( tm_eg->unix_msec / 1000 );
	tm_eg->unix_msec = tm_eg->unix_msec % 1000;
	calcDate(tm_eg);
}

void synchTime(long calibrate_sec, tm *tm_ptr)
{
	tm_ptr->unix_time = calibrate_sec;
	tm_ptr->unix_msec = 500; // to minimize error
}

void calcDate(tm *tm_ptr)
{
	long seconds, minutes, hours, days, year, month;
	long dayOfWeek;
	seconds = tm_ptr->unix_time;

	/* calculate minutes */
	minutes  = seconds / 60;
	seconds -= minutes * 60;
	/* calculate hours */
	hours    = minutes / 60;
	minutes -= hours   * 60;
	/* calculate days */
	days     = hours   / 24;
	hours   -= days    * 24;

	/* Unix time starts in 1970 on a Thursday */
	year      = 1970;
	dayOfWeek = 4;

	while(1)
	{
		bool     leapYear   = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
		uint16_t daysInYear = leapYear ? 366 : 365;
		if (days >= daysInYear)
		{
			dayOfWeek += leapYear ? 2 : 1;
			days      -= daysInYear;
			if (dayOfWeek >= 7)
			dayOfWeek -= 7;
			++year;
		}
		else
		{
			tm_ptr->tm_yday = days;
			dayOfWeek  += days;
			dayOfWeek  %= 7;

			/* calculate the month and day */
			static const uint8_t daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
			for(month = 0; month < 12; ++month)
			{
				uint8_t dim = daysInMonth[month];

				/* add a day to feburary if this is a leap year */
				if (month == 1 && leapYear)
				++dim;

				if (days >= dim)
				days -= dim;
				else
				break;
			}
			break;
		}
	}

	tm_ptr->tm_sec  = seconds;
	tm_ptr->tm_min  = minutes;
	tm_ptr->tm_hour = hours;
	tm_ptr->tm_mday = days + 1;
	tm_ptr->tm_mon  = month + 1;
	tm_ptr->tm_year = year;
	tm_ptr->tm_wday = dayOfWeek;
}



void init_timestamp(tm *tm_ptr)
{
	tm_ptr->unix_msec=0;
	tm_ptr->unix_time=0;
}