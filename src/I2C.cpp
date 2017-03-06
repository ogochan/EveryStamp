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
 * I2C support class
 *
 */

#define	TRACE
#include	"config.h"
#ifdef	USE_ESP_OPEN_RTOS
#include	<i2c/i2c.h>
#endif
#include	<string.h>
#include	"debug.h"
#include	"I2C.h"

I2C::I2C(
	int	slaveAddress)
{
	address = slaveAddress & 0x7F;
}

I2C::~I2C()
{
}

void
I2C::init(void)
{
#ifdef	USE_ARDUINO
	Wire.begin(GPIO_SDA,GPIO_SCL);
#endif
#ifdef	USE_ESP_OPEN_RTOS
	i2c_init(GPIO_SCL, GPIO_SDA);
#endif
}

void
I2C::write(
	byte	command,
	byte	*data,
	int		size)
{
#ifdef	USE_ARDUINO
	Wire.beginTransmission(address);
	Wire.write(command);
	for	( ; size > 0 ; size -- )	{
		Wire.write(*data);
		data ++;
	}
	Wire.endTransmission(true);
#endif
#ifdef	USE_ESP_OPEN_RTOS
	uint8_t	buf[size+1];

	buf[0] = command;
	memcpy(&buf[1], data, size);
	i2c_slave_write(address, buf, size+1);
#endif
}

void
I2C::read(
	byte	command,
	byte	*data,
	int		size)
{
	memset(data, 0, size);
#ifdef	USE_ARDUINO
	Wire.beginTransmission(address);
	Wire.write(command);
	Wire.endTransmission(false);
	Wire.requestFrom((uint8_t)address, (size_t)size, (bool)false);
	for	( ; ( size > 0 && Wire.available() ) ; size --)	{
		*data = Wire.read();
		data ++;
	}
	Wire.endTransmission(true);
#endif
#ifdef	USE_ESP_OPEN_RTOS
	i2c_slave_read(address, command, data, size);
#endif
}

void
I2C::writeByte(
	byte	command,
	byte	data)
{
	write(command, &data, 1);
}

byte
I2C::readByte(
	byte	command)
{
	byte ret = 0;
	read(command, &ret, 1);

	return ret;
}

void
I2C::writeWord(
	byte	command,
	uint16_t	data)
{
	write(command, (byte*)(&data), 2);
}

uint16_t
I2C::readWord(
	byte	command)
{
	uint16_t	hi
		,		lo;

	lo = readByte(command);
	hi = readByte(command+1);

	return(( hi << 8 ) | lo);
}

int16_t
I2C::readInt16(
	byte	command)
{
	int	ret;
	uint16_t	hi
		,		lo;

	lo = readByte(command);
	hi = readByte(command+1);
	ret = ( hi << 8 ) | lo;
	if		( ret > 32767 ) ret -= 65536;

	return ret;
}

