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
 * BMP280 barometric pressure class
 *
 */

#ifndef __BMP280_BAROMETRIC_H__
#define __BMP280_BAROMETRIC_H__

#include	"I2C.h"
#include	"BMP280.h"
#include	"SenseBuffer.h"

class BMP280_Barometric:public BMP280
{
  public:
	virtual	void	get(SenseBuffer buff);
	virtual	size_t	build(char *p, SenseBuffer buff);
	virtual	void	stop(int sec) {};
	virtual	void	start(void) {};
	virtual	const	char	*name(void);
	virtual	const	char	*unit(void);
	virtual	const	char	*data_class_name(void);

	static	const	uint8_t	precision = 0;
};

#endif // __BMP280_H__
