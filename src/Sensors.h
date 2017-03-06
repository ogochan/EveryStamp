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
 * Sensor control class
 *
 */

#ifndef	__INC_SENSORS_H__
#define	__INC_SENSORS_H__
#include	"SensorInfo.h"
class	Sensors
{
  public:
	static	void	init(void);
	static	void	start(void);
	static	void	stop(int sec);
	static	void	collect(time_t n, SenseBuffer buff);
	static	int		count(void)	{
		return	(nSensors);
	};
	static	SensorInfo	*item(uint8_t i)	{
		return	(_Sensors[i]);
	};
	static	void	add(SensorInfo *sensor)	{
		_Sensors[nSensors ++] = sensor;
	};
  protected:
	static	uint8_t		nSensors;
	static	SensorInfo	*_Sensors[NUMBER_OF_SENSORS];
};

#endif
