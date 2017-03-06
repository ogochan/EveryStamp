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
 * Sensor data buffer class
 *
 */

#define	TRACE
#include	"config.h"
#include	"debug.h"
#include	"SensorInfo.h"
#include	"SenseBuffer.h"
#include	"Sensors.h"

uint8_t	SenseBuffer::_buff[SIZE_OF_SENSOR_BUFF];
uint8_t	*SenseBuffer::_w_pointer;
uint8_t	*SenseBuffer::_r_pointer;

void
SenseBuffer::check_space(void)
{
	SensorInfo	*info;
	size_t	size
		,	shift;
	int		i;

	size = sizeof(time_t);
	for( i = 0; i< Sensors::count(); i++ ){
		info = Sensors::item(i);
		if	( info->fValid )	{
			size += info->dimension() * sizeof(float) + 1;
		}
	}
	size ++;
	dbgprintf("use %d bytes", size);
	while	( ( SIZE_OF_SENSOR_BUFF - ( _w_pointer - _buff ) )  < size )	{
		_r_pointer = _buff + sizeof(time_t);
		while	( *_r_pointer != 0xFF )	{
			info = Sensors::item((int)*_r_pointer);
			_r_pointer ++;
			_r_pointer += info->dimension() * sizeof(float);
		}
		_r_pointer ++;
		shift = _r_pointer - _buff;
		dbgprintf("shift %d bytes", shift);
		memmove(_r_pointer, _buff, shift);
		_w_pointer -= shift;
	}
}

