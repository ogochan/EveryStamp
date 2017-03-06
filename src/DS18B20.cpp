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
 * DS18B20 temperature class
 *
 */

#define	TRACE
#include	"config.h"
#include	"debug.h"
#include	"utility.h"

#include	"SenseBuffer.h"
#include	"DS18B20.h"

#ifdef	USE_ARDUINO
#include	<OneWire.h>
#include	<DallasTemperature.h>
OneWire	oneWire(PIN_1WIRE);
DallasTemperature	sensors(&oneWire);
#endif
#ifdef	USE_ESP_OPEN_RTOS
extern "C" {
#include	<ds18b20/ds18b20.h>
}
#define MAX_SENSORS 8
#endif

const	uint8_t	DS18B20::precision = 1;

DS18B20::DS18B20()
{
	initialize();
}

DS18B20::~DS18B20()
{
}

void
DS18B20::initialize(void)
{
ENTER_FUNC;
#ifdef	USE_ARDUINO
	sensors.begin();
	sensors.setResolution(10);
	fValid = true;
#endif
#ifdef	USE_ESP_OPEN_RTOS
    ds18b20_addr_t addrs[MAX_SENSORS];

	if	( ds18b20_scan_devices(PIN_1WIRE, addrs, MAX_SENSORS) > 0 )	{
		fValid = true;
	} else {
		fValid = false;
	}
#endif
LEAVE_FUNC;
}

void
DS18B20::get(
	SenseBuffer	buff)
{
	float	mTemp;

ENTER_FUNC;
#ifdef	USE_ARDUINO
	sensors.requestTemperatures();
	mTemp = sensors.getTempCByIndex(0);
#endif
#ifdef	USE_ESP_OPEN_RTOS
	mTemp = ds18b20_read_single(PIN_1WIRE);
#endif
	buff.put(&mTemp, sizeof(float));
dbgprintf("temp = %s", ftos(mTemp, 0, 1));
LEAVE_FUNC;
}

size_t
DS18B20::build(
	char	*p,
	SenseBuffer	buff)
{
	float	data;

ENTER_FUNC;
	buff.get(&data, sizeof(float));
LEAVE_FUNC;
	return	sprintf(p, "\"value\":\"%s\",\"unit\":\"%s\"", ftos(data, 0, precision), unit());
}

const	char	*
DS18B20::name(void)
{
	return	"DS18B20_Temp";
}

const	char	*
DS18B20::unit(void)
{
	return	"degree Celsius";
}

const	char	*
DS18B20::data_class_name(void)
{
	return	"AirTemperature";
}

