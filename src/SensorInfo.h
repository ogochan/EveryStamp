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
 * Sensor base class
 *
 */

#ifndef	_INC_SENSOR_INFO_H_
#define	_INC_SENSOR_INFO_H_

#include	"SenseBuffer.h"

class SensorInfo
{
  public:
	virtual	void	get(SenseBuffer buff) {};
	virtual	size_t	build(char *p, SenseBuffer buff) { return 0; };
	virtual	void	stop(int sec) {};
	virtual	void	start(void) {};
	virtual	const	char	*name(void) { return NULL; };
	virtual	const	char	*unit(void) { return NULL; };
	virtual	const	char	*data_class_name(void) { return NULL; };
	virtual	int		dimension(void) { return 1; };
	bool	fValid;

	static	const	uint8_t	precision;
	SensorInfo();
	~SensorInfo();
  protected:
};

#endif
