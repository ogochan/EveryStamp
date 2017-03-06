/*
 *  EveryStamp for ESP8266 - a sensor data collector
 *
 *  Copyright (C) 2017  Masami Ogoshi (ogochan@wasp.co.jp)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * This code is made available on the understanding that it will not be
 * used in safety-critical situations without a full and competent review.
 *
 * Time functions(for compatibilities)
 *
 */

#define	TRACE

#include	"config.h"
#ifdef	USE_ESP_OPEN_RTOS
#include	<time.h>
#include	<espressif/esp_common.h>
#include	<FreeRTOS.h>
#include	<timers.h>
#include	"Time.h"
#include	"debug.h"

static	uint32_t
total_days(
	int		year,
	int		month,
	int		day)
{
	if		(	(  month == 1  )
			||	(  month == 2  ) ) {
		month += 12;
		year --;
	}
	return	( 365 * year + ( year / 4 ) - ( year / 100 ) + ( year / 400 )
			  + ( 306 * ( month + 1 ) / 10 ) - 428 + day );
}


static	void
__sec_to_date(
	struct	tm	*tm_v,
	int		days,
	int		dsec)
{
	int		iyear
		,	imonth
		,	iday
		,	ihour;

	iyear = ( days + 305 ) / 146097 * 400
		+ ( days + 305 ) % 146097 / 36524 * 100
		+ ( days + 305 ) % 146097 % 36524 / 1461 * 4
		+ ( days + 305 ) % 146097 % 36524 % 1461 / 365;
	iday = days - ( 365 * ( (iyear) - 1 ) + (iyear) / 4 - (iyear) / 100 + (iyear) / 400 + 31 + 28 + 1);
	if		( iday  ==  -1  )	{
		(iyear) --;
		iday = 365;
	}
	for ( imonth = 3 ; imonth < 15 ; imonth ++ )	{
		if		(	(  306 * ( imonth + 1 ) / 10 - 122 <= iday )
				&&	(  306 * ( imonth + 2 ) / 10 - 122 >  iday ) )
			break;
	}
	iday -= 306 * ( imonth + 1 ) / 10 - 122 - 1;
	if		(  imonth  >  12  )	{
		iyear ++;
		imonth -= 12;
	}
	ihour = dsec / 3600;
	tm_v->tm_hour = dsec / 3600;
	tm_v->tm_min = ( dsec - ihour * 3600 ) / 60;
	tm_v->tm_sec = ( dsec - ihour * 3600 ) % 60;
	tm_v->tm_year = iyear;
	tm_v->tm_mon = imonth;
	tm_v->tm_mday = iday;
	tm_v->tm_wday = ( ( days + 4 ) % 7 ) + 1;
}


static	void
breakTime(
	time_t		inp,
	struct	tm	*tm_v)
{
	int		days
		,	dsec;

	days = (uint32_t)inp / ( 3600 * 24 ) + total_days(1970, 1, 1);
	dsec = (uint32_t)inp % ( 3600 * 24 );

	__sec_to_date(tm_v, days, dsec);
}

struct	tm	tm_v;
time_t	cacheTime;
time_t	sysTime;

static	void
refreshCache(
	time_t	t)
{

	if (t != cacheTime) {
		breakTime(t, &tm_v);
		cacheTime = t; 
	}
}

extern	int
hour(void)
{
	return	hour(now()); 
}

extern	int
hour(
	time_t	t)
{
	refreshCache(t);
	return(tm_v.tm_hour);
}

extern	int
minute(void)
{
	return	minute(now()); 
}

extern	int
minute(
	time_t	t)
{
	refreshCache(t);
	return (tm_v.tm_min);
}

extern	int
second(void)
{
	return	second(now()); 
}

extern	int
second(
	time_t	t)
{
	refreshCache(t);
	return (tm_v.tm_sec);
}

extern	int
day(void)
{
	return	(day(now())); 
}

extern	int
day(
	time_t	t)
{
	refreshCache(t);
	return	(tm_v.tm_mday);
}

extern	int
weekday(void)
{
	return  weekday(now()); 
}

extern	int
weekday(
	time_t	t)
{
	refreshCache(t);
	return	(tm_v.tm_wday);
}
   
extern	int
month(void)
{
	return	month(now()); 
}

extern	int
month(
	time_t	t)
{
	refreshCache(t);
	return tm_v.tm_mon;
}

extern	int
year(void)
{
	return year(now()); 
}

extern	int
year(
	time_t	t)
{
	refreshCache(t);
	return (tm_v.tm_year);
}

extern	time_t
now(void)
{
	return	(sysTime);
}

extern	void
setTime(
	time_t	t)
{ 
	sysTime = (uint32_t)t;  
} 

TimerHandle_t	handle;

static	void
time_func(
	TimerHandle_t	pxTimer)
{
	sysTime ++;
}

extern	void
start_time(void)
{
	handle = xTimerCreate("RTC", ( 1000 / portTICK_PERIOD_MS), pdTRUE, NULL, time_func);
	if	( handle != NULL )	{
		xTimerStart(handle, 0);
	}
}

#endif
