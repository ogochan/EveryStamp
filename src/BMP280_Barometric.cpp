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

//#define	TRACE
#include	"config.h"
#include	"debug.h"
#include	"utility.h"
#include	"os_time.h"
#include	"SenseBuffer.h"
#include	"BMP280.h"
#include	"BMP280_Barometric.h"

void
BMP280_Barometric::get(
	SenseBuffer buff)
{
	int		adc_T
		,	adc_P;

	int32_t	t_fine
		,	ivar1
		,	ivar2;
	int32_t	var1
		,	var2;
	uint32_t	p;
	uint8_t	tbuff[3]
		,	pbuff[3];
	float	mTemp
		,	mPressure;

ENTER_FUNC;
	mI2C.writeByte(REG_CONTROL, REG_WORD_RESULT);
	delay(CONVERSION_TIME);

	mI2C.read(VAL_TEMPERATURE, (byte *)tbuff, 3);

	mI2C.writeByte(REG_CONTROL, REG_WORD_RESULT);
	delay(CONVERSION_TIME);

	mI2C.read(VAL_PRESSURE, (byte *)pbuff, 3);

	adc_T = ( tbuff[0] * 4096 ) + ( tbuff[1] * 16 ) + ( ( tbuff[2] & 0xF0 ) >> 4 );
	adc_P = ( pbuff[0] * 4096 ) + ( pbuff[1] * 16 ) + ( ( pbuff[2] & 0xF0 ) >> 4 );
	ivar1 = (((( adc_T >> 3 ) - ((int32_t)dig_T1 << 1 ))) * (int32_t)dig_T2) >> 11;
	ivar2 = (((((adc_T >> 4) - (int32_t)dig_T1) * ((adc_T >> 4) - (int32_t)dig_T1)) >> 12) *
			 (int32_t)dig_T3) >> 14;
	t_fine = ivar1 + ivar2;

	var1 = ( (int32_t)t_fine >> 1 ) - (int32_t)64000;
	var2 = ((( var1 >> 2 ) * ( var1 >> 2 )) >> 11 ) * (int32_t)dig_P6;
	var2 = var2 + ((var1 * (int32_t)dig_P5) << 1 );
	var2 = ( var2 >> 2 ) + ( (int32_t)dig_P4 << 16 );
	var1 = (((dig_P3 * (((var1 >> 2 ) * ( var1 >> 2)) >> 13 )) >> 3) + (((int32_t)dig_P2 * var1) >>1 )) >> 18;
	var1 = (((32768 + var1)) * (int32_t)dig_P1) >> 15;
	if (var1 == 0)	{
		mPressure = 0.0;
	} else {
		p = ((uint32_t)((1048576 - adc_P ) - ( var2 >> 12))) * 3125;
		if (p < 0x80000000)	{
			p = (p << 1) / (uint32_t)var1;
		} else {
			p = (p / (uint32_t)var1) * 2;
		}
		var1 = ((int32_t)dig_P9 * ((int32_t)(( p >> 3 ) * ( p >> 3 )) >> 13)) >> 12;
		var2 = (( (int32_t)p >> 2 ) * (int32_t)dig_P8) >> 13;
		p = (uint32_t)((int32_t)p + ((var1 + var2 + dig_P7) >> 4));
		mPressure = p / 100.0f;
	}
	buff.put(&mPressure, sizeof(float));

LEAVE_FUNC;
}

size_t
BMP280_Barometric::build(
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
BMP280_Barometric::name(void)
{
	return	"BMP-280_Barometric";
}

const	char	*
BMP280_Barometric::unit(void)
{
	return	"hPa";
}
const	char	*
BMP280_Barometric::data_class_name(void)
{
	return	"BarometricPressure";
}

