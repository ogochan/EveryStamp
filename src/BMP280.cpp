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
 * BMP280 base class
 *
 */

#define	TRACE
#include	"config.h"
#include	"debug.h"
#include	"SenseBuffer.h"
#include	"BMP280.h"

// static members declaration with initial value

int		BMP280::dig_T1=0, BMP280::dig_T2=0, BMP280::dig_T3=0;
int		BMP280::dig_P1=0, BMP280::dig_P2=0, BMP280::dig_P3=0,
BMP280::dig_P4=0, BMP280::dig_P5=0, BMP280::dig_P6=0,
BMP280::dig_P7=0, BMP280::dig_P8=0, BMP280::dig_P9=0;
const	uint8_t	BMP280::precision = 0;

bool BMP280::fInit = false;
bool BMP280::exist = false;

BMP280::BMP280() // : mI2C(SLA_BMP280)
{
	int			rc;

	dbgmsg("initialize");
	if	( !fInit )	{
		fInit = true;
		mI2C.init();

		rc = mI2C.readByte(0xD0);
		if	( rc == 0x58 )	{
			dbgmsg("valid");
			exist = true;
			dig_T1 = (uint16_t)mI2C.readWord(0x88);
			dig_T2 = (int16_t)mI2C.readInt16(0x8A);
			dig_T3 = (int16_t)mI2C.readInt16(0x8C);

			dig_P1 = (uint16_t)mI2C.readWord(0x8E);
			dig_P2 = (int16_t)mI2C.readInt16(0x90);
			dig_P3 = (int16_t)mI2C.readInt16(0x92);
			dig_P4 = (int16_t)mI2C.readInt16(0x94);
			dig_P5 = (int16_t)mI2C.readInt16(0x96);
			dig_P6 = (int16_t)mI2C.readInt16(0x98);
			dig_P7 = (int16_t)mI2C.readInt16(0x9A);
			dig_P8 = (int16_t)mI2C.readInt16(0x9C);
			dig_P9 = (int16_t)mI2C.readInt16(0x9E);
		} else {
			exist = false;
		}
	}
	if	( exist )	{
		fValid = true;
	} else {
		fValid = false;
	}
}

BMP280::~BMP280()
{
	cleanUp();
}

void
BMP280::cleanUp(void)
{
}
