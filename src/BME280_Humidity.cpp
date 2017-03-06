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
 * BME280 humidity class
 *
 */

#define	TRACE
#include	"config.h"
#include	"debug.h"
#include	"utility.h"

#include	"SenseBuffer.h"
#include	"BME280.h"
#include	"BME280_Humidity.h"

void
BME280_Humidity::get(
	SenseBuffer buff)
{
	int		adc_T
		,	adc_H;

	int32_t	t_fine
		,	ivar1
		,	ivar2;
	int32_t	v_x1;
	uint8_t	tbuff[3]
		,	hbuff[2];
	float	mHumidity;

ENTER_FUNC;
	mI2C.read(VAL_TEMPERATURE, (byte *)tbuff, 3);
	mI2C.read(VAL_HUMIDITY, (byte *)hbuff, 2);

	adc_T = ( tbuff[0] * 4096 ) + ( tbuff[1] * 16 ) + ( ( tbuff[2] & 0xF0 ) >> 4 );
	adc_H = ( hbuff[0] * 256 ) + ( hbuff[1] );

	ivar1 = (((( adc_T >> 3 ) - ((int32_t)dig_T1 << 1 ))) * (int32_t)dig_T2) >> 11;
	ivar2 = (((((adc_T >> 4) - (int32_t)dig_T1) * ((adc_T >> 4) - (int32_t)dig_T1)) >> 12) *
			 (int32_t)dig_T3) >> 14;
	t_fine = ivar1 + ivar2;

	v_x1 = (t_fine - ((signed long int)76800));
	v_x1 = (((((adc_H << 14) -(((signed long int)dig_H4) << 20) - (((signed long int)dig_H5) * v_x1)) + 
              ((signed long int)16384)) >> 15) * (((((((v_x1 * ((signed long int)dig_H6)) >> 10) * 
													  (((v_x1 * ((signed long int)dig_H3)) >> 11) + ((signed long int) 32768))) >> 10) + (( signed long int)2097152)) * ((signed long int) dig_H2) + 8192) >> 14));
	v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * ((signed long int)dig_H1)) >> 4));
	v_x1 = (v_x1 < 0 ? 0 : v_x1);
	v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);

	mHumidity = ( v_x1 >> 12 ) / 1024.0;

	buff.put(&mHumidity, sizeof(float));

dbgprintf("humidity = %s", ftos(mHumidity, 0, 1));
LEAVE_FUNC;
}

size_t
BME280_Humidity::build(
	char		*p,
	SenseBuffer	buff)
{
	float	data;

ENTER_FUNC;
	buff.get(&data, sizeof(float));
LEAVE_FUNC;
	return	sprintf(p, "\"value\":\"%s\",\"unit\":\"%s\"", ftos(data, 0, precision), unit());
}

const	char	*
BME280_Humidity::name(void)
{
	return	"BME-280_Hum";
}

const	char	*
BME280_Humidity::unit(void)
{
	return	"%RH";
}
const	char	*
BME280_Humidity::data_class_name(void)
{
	return	"AirHygrometer";
}

