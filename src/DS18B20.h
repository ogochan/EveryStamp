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

#ifndef	__INC_DS18B20_H__
#define	__INC_DS18B20_H__

#include	"SenseBuffer.h"
#include	"SensorInfo.h"

class DS18B20:public SensorInfo
{
  public:
    void	initialize(void);
	virtual	void	get(SenseBuffer buff);
	virtual	size_t	build(char *p, SenseBuffer buff);
	virtual	const	char	*name(void);
	virtual	const	char	*unit(void);
	virtual	const	char	*data_class_name(void);
	virtual	int		dimension(void) { return 1; };

	static	const	uint8_t	precision;
	DS18B20();
  protected:
	~DS18B20();
};

#endif
