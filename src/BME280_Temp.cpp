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
 * BME280 temperature class
 *
 */

//#define	TRACE
#include	"config.h"
#include	"debug.h"
#include	"utility.h"

#include	"SenseBuffer.h"
#include	"BME280.h"
#include	"BME280_Temp.h"

void
BME280_Temp::get(
	SenseBuffer buff)
{
	int		adc_T;

	int32_t	t_fine
		,	ivar1
		,	ivar2;
	uint8_t	tbuff[3];
	float	mTemp;

ENTER_FUNC;
	mI2C.read(VAL_TEMPERATURE, (byte *)tbuff, 3);

	adc_T = ( tbuff[0] * 4096 ) + ( tbuff[1] * 16 ) + ( ( tbuff[2] & 0xF0 ) >> 4 );


	ivar1 = (((( adc_T >> 3 ) - ((int32_t)dig_T1 << 1 ))) * (int32_t)dig_T2) >> 11;
	ivar2 = (((((adc_T >> 4) - (int32_t)dig_T1) * ((adc_T >> 4) - (int32_t)dig_T1)) >> 12) *
			 (int32_t)dig_T3) >> 14;
	t_fine = ivar1 + ivar2;

	mTemp  = ((t_fine * 5 + 128) >> 8) / 100.0f;
	buff.put(&mTemp, sizeof(float));
dbgprintf("temp = %s", ftos(mTemp, 0, 1));

LEAVE_FUNC;
}

size_t
BME280_Temp::build(
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
BME280_Temp::name(void)
{
	return	"BME-280_Temp";
}

const	char	*
BME280_Temp::unit(void)
{
	return	"degree Celsius";
}

const	char	*
BME280_Temp::data_class_name(void)
{
	return	"AirTemperature";
}

