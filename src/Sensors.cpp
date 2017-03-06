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

#define	TRACE
#include	"config.h"
#include	"debug.h"
#include	"SensorInfo.h"
#include	"BME280.h"
#include	"BME280_Temp.h"
#include	"BME280_Humidity.h"
#include	"BME280_Barometric.h"
#include	"DS18B20.h"
#include	"Sensors.h"
#include	"GPIO.h"

uint8_t		Sensors::nSensors;
SensorInfo	*Sensors::_Sensors[NUMBER_OF_SENSORS];

void
Sensors::init(void)
{
	int		i;

	nSensors = 0;
#if	0
	Sensors::add((SensorInfo*)new BMP280_Temp());
	Sensors::add((SensorInfo*)new BMP280_Barometric());
#else
	Sensors::add((SensorInfo*)new BME280_Temp());
	Sensors::add((SensorInfo*)new BME280_Humidity());
	Sensors::add((SensorInfo*)new BME280_Barometric());
#endif
	Sensors::add((SensorInfo*)new DS18B20());
	for( i = 0; i < nSensors; i++ )	{
		DEBUG_PRINTLN(Sensors::item(i)->name());
		//Sensors[i]->initialize();
	}
}

void
Sensors::stop(
	int		sec)
{
	int		i;

ENTER_FUNC;
	for	( i = 0 ; i < nSensors ; i ++ )	{
		if		( Sensors::item(i)->fValid )	{
			Sensors::item(i)->stop(sec);
		}
	}
LEAVE_FUNC;
}

void
Sensors::start(void)
{
	int		i;

ENTER_FUNC;

	GPIO_reset_sensors();

	for	( i = 0 ; i < nSensors ; i ++ )	{
		if		( Sensors::item(i)->fValid )	{
			Sensors::item(i)->start();
		}
	}
LEAVE_FUNC;
}

void
Sensors::collect(
	time_t	n,
	SenseBuffer	buff)
{
	SensorInfo	*info;
	uint8_t	i;

ENTER_FUNC;

 buff.check_space();
	buff.put(&n, sizeof(time_t));
	for( i = 0; i< Sensors::count(); i++ ){
		info = Sensors::item(i);
		dbgprintf("%s is %s", info->name(), info->fValid ? "valid" : "invalid" );
		if	( info->fValid )	{
			buff.set_value(i);
			info->get(buff);
		}
	}
	buff.set_value(0xFF);
LEAVE_FUNC;
}

