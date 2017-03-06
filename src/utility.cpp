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
 * misc functions
 *
 */

#define	TRACE

#include "config.h"
#include "debug.h"

#define	SIZE_OF_FLOAT		10

extern	char	*
ftos(
	float	val,
	int		length,
	int		prec)
{
	static	char	buff[SIZE_OF_FLOAT+1];
	char	sign
		,	d;
	char	*p;
	float	pp;
	int		ip;
	bool	zero;
	

	if	( val < 0 )	{
		sign = 1;
		val = - val;
	} else {
		sign = 0;
	}
	if	(	( length == 0 )
		||	( length > SIZE_OF_FLOAT )
		||	( ( prec + 1 ) > length  ))	{
		p = buff;
		if		( sign > 0 )	{
			*p ++ = '-';
		}
		p += sprintf(p, "%d",(int)val);
		length = SIZE_OF_FLOAT;
	} else {
		p = buff + length - prec - 2;
		ip = (int)val;
		zero = false;
		while	( p >= buff )	{
			if		( zero )	{
				if		( sign == 1 )	{
					*p -- = '-';
					sign = 0;
				} else {
					*p -- = ' ';
				}
			} else {
				d = ip % 10;
				*p -- = d + '0';
				ip = ip / 10;
			}
			if	( ip == 0 )	{
				zero = true;
			}
		}
		p = buff + length - prec - 1;
	}
	if		( prec != 0 )	{
		*p ++ = '.';
	}
	pp = (int)val;
	pp = val - (float)pp;
	while	( ( p - buff ) < length )	{
		if	( prec == 0 )	break;
		pp *= 10;
		d = (int)pp;
		*p ++ = d + '0';
		pp -= d;
		prec --;
	}
	*p = 0;
	return	buff;
}

