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

#ifndef ___I2C_H__
#define ___I2C_H__

#ifdef	USE_ARDUINO
#include <Wire.h>
#endif
#ifdef	USE_ESP_OPEN_RTOS
#include <i2c/i2c.h>
#endif

#include <stdint.h>

typedef	uint8_t		byte;

class I2C
{
  public:
    I2C(int slaveAddress);
    ~I2C();

    void	init(void);
    
    void	write(byte command, byte* data, int nSize);
    void	read(byte command, byte* data, int nSize);

    void	writeByte(byte command, byte data);
    byte	readByte(byte command);
    void	writeWord(byte command, uint16_t data);
    uint16_t	readWord(byte command);
	int16_t		readInt16(byte command);

  protected:
    byte 	address;

    static	const	int GPIO_SDA = PIN_SDA;
    static	const	int GPIO_SCL = PIN_SCL;
};

#endif // __I2C_H__
