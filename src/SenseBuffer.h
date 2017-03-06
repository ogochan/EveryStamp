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

#ifndef	__INC_SENSE_BUFFER_H__
#define	__INC_SENSE_BUFFER_H__

#include	<stdlib.h>
#include	<stdint.h>
#include	<string.h>

class	SenseBuffer	{
  public:
	void	put(void *data, size_t size)	{
		memcpy(_w_pointer, data, size);
		_w_pointer += size;
	};
	void	get(void *data, size_t size)	{
		memcpy(data, _r_pointer, size);
		_r_pointer += size;
	};
	void	set_value(uint8_t data)	{
		memcpy(_w_pointer, &data, 1);
		_w_pointer ++;
	};
	uint8_t	current_value(void)	{
		return	(*_r_pointer);
	};
	uint8_t	get_value(void)	{
		uint8_t	ret = *_r_pointer;
		_r_pointer ++;
		return	(ret);
	};
	int		used_size(void)	{
		return	(int)(_w_pointer - _buff);
	};
	void	rewind_read(void)	{
		_r_pointer = _buff;
	}
	void	rewind_write(void)	{
		_w_pointer = _buff;
	}
	void	rewind_read(uint8_t *p)	{
		_r_pointer = p;
	}
	uint8_t	*mark_read(void)	{
		return	(_r_pointer);
	}
	bool	is_end(void)	{
		return	(_r_pointer < _w_pointer) ? false : true;
	}
	uint8_t	*buff(void)	{
		return	(_buff);
	}
	uint8_t	*pointer(void)	{
		return	(_w_pointer);
	}
	void	set_pointer(uint32_t pointer)	{
		_w_pointer = (uint8_t *)pointer;
	}
	void	check_space(void);

  protected:
	static	uint8_t	_buff[SIZE_OF_SENSOR_BUFF];
	static	uint8_t	*_w_pointer;
	static	uint8_t	*_r_pointer;
};

#endif
