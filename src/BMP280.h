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

#ifndef __BMP280_H__
#define __BMP280_H__

#include "I2C.h"

#include	"SenseBuffer.h"
#include	"SensorInfo.h"

class BMP280:public SensorInfo
{
  public:
	virtual	void	get(SenseBuffer buff) {};
	virtual	size_t	build(char *p, SenseBuffer buff) { return 0;};
	virtual	void	stop(int sec) {};
	virtual	void	start(void) {};
	virtual	const	char	*name(void) { return NULL; };
	virtual	const	char	*unit(void) { return NULL; };
	virtual	const	char	*data_class_name(void) { return NULL; };
	virtual	int		dimension(void) { return 1; };
	static	const	uint8_t	precision;
  protected:
  
	BMP280();
    ~BMP280();
	static	bool	exist;
    static	void	cleanUp(void);

    static	const	int SLA_BMP280 = 0x76;
    I2C		mI2C = I2C(SLA_BMP280);

    static	const	int	CONVERSION_TIME = 10;

    static	const	byte	REG_CONTROL = 0xF4;
    static	const	byte	REG_WORD_RESULT = 0b00100101;

    static	const	byte	VAL_TEMPERATURE = 0xFA;
    static	const	byte	VAL_PRESSURE = 0xF7;

    static	int 	dig_T1, dig_T2, dig_T3;
    static	int		dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

    static	bool 	fInit;
};

#endif // __BMP280_H__
